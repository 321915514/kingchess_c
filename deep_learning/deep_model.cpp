#include <fstream>
#include <vector>

#include "deep_model.h"
#include "thirdparty/gflags/gflags.h"
#include "thirdparty/glog/logging.h"
#include "abacus/utility/message.h"

BufferHandler::BufferHandler(std::shared_ptr<nvinfer1::ICudaEngine> engine) {
    m_engine = engine;
    m_batch_size = m_engine->getTensorShape("conv_inputs").d[0];
    for (int i = 0; i < m_engine->getNbIOTensors(); ++i) {
        std::string name = m_engine->getIOTensorName(i);
        auto dims = m_engine->getTensorShape(name.c_str());
        auto type = m_engine->getTensorDataType(name.c_str());
        size_t vol = 1;
        for (int j = 0; j < dims.nbDims; ++j) {
            vol *= dims.d[j];
        }
        std::unique_ptr<IOBuffer> host_buffer{new IOBuffer(vol, type, true)};
        std::unique_ptr<IOBuffer> device_buffer{new IOBuffer(vol, type, false)};
        m_device_bindings.emplace_back(device_buffer->Data());
        m_host_buffers.emplace_back(std::move(host_buffer));
        m_device_buffers.emplace_back(std::move(device_buffer));
    }
}

BufferHandler::~BufferHandler() {
    for (int i = 0; i < m_host_buffers.size(); ++i) {
        m_host_buffers[i].reset();
    }
    for (int i = 0; i < m_device_buffers.size(); ++i) {
        m_device_buffers[i].reset();
    }
}

int BufferHandler::GetIndex(const std::string tensor_name) const {
    for (int i = 0; i < m_engine->getNbIOTensors(); ++i) {
        std::string name = m_engine->getIOTensorName(i);
        if (name == tensor_name) {
            return i;
        }
    }
    return -1;
}

void* BufferHandler::GetBuffer(bool is_host, const std::string tensor_name) const {
    int index = GetIndex(tensor_name);
    if (index == -1) {
        return nullptr;
    }
    return (is_host ? m_host_buffers[index]->Data() : m_device_buffers[index]->Data());
}

void BufferHandler::CopyInputToDevice() {
    for (int i = 0; i < m_engine->getNbIOTensors(); ++i) {
        std::string name = m_engine->getIOTensorName(i);
        if (m_engine->getTensorIOMode(name.c_str()) == nvinfer1::TensorIOMode::kINPUT) {
            const void* src_ptr = m_host_buffers[i]->Data();
            void* dst_ptr = m_device_buffers[i]->Data();
            const size_t byte_size = m_host_buffers[i]->GetBytes();
            const cudaMemcpyKind memcpy_type = cudaMemcpyHostToDevice;
            cudaMemcpy(dst_ptr, src_ptr, byte_size, memcpy_type);
        }
    }
}

void BufferHandler::CopyInputToDevice(int start, int num) {
    for (int i = 0; i < m_engine->getNbIOTensors(); ++i) {
        std::string name = m_engine->getIOTensorName(i);
        if (m_engine->getTensorIOMode(name.c_str()) == nvinfer1::TensorIOMode::kINPUT) {
            const size_t byte_size = m_host_buffers[i]->GetBytes() / m_batch_size;
            const void* src_ptr = m_host_buffers[i]->Data() + start * byte_size;
            void* dst_ptr = m_device_buffers[i]->Data() + start * byte_size;
            const cudaMemcpyKind memcpy_type = cudaMemcpyHostToDevice;
            cudaMemcpy(dst_ptr, src_ptr, num * byte_size, memcpy_type);
        }
    }
}

void BufferHandler::CopyOutputToHost() {
    for (int i = 0; i < m_engine->getNbIOTensors(); ++i) {
        std::string name = m_engine->getIOTensorName(i);
        if (m_engine->getTensorIOMode(name.c_str()) == nvinfer1::TensorIOMode::kOUTPUT) {
            const void* src_ptr = m_device_buffers[i]->Data();
            void* dst_ptr = m_host_buffers[i]->Data();
            const size_t byte_size = m_device_buffers[i]->GetBytes();
            const cudaMemcpyKind memcpy_type = cudaMemcpyDeviceToHost;
            cudaMemcpy(dst_ptr, src_ptr, byte_size, memcpy_type);
        }
    }
}

int DeepModel::InitModel(const std::string& path) {
    m_feature_handler.LoadConfig(path + "_config.txt");
    std::cout << "Deep model loading: " << path;

    bool square;
    m_feature_handler.GetRange("conv", &square, &m_conv_begin, &m_conv_size);
    m_feature_handler.GetRange("vector", &square, &m_vector_begin, &m_vector_size);

    std::ifstream fin(path, std::ios::binary);
    if (fin.fail()) {
        std::cout << "Cannot read model file: " << path;
        return 0;
    }
    fin.seekg(0, std::ifstream::end);
    auto f_size = fin.tellg();
    fin.seekg(0, std::ifstream::beg);
    std::vector<char> engine_data(f_size);
    fin.read(engine_data.data(), f_size);

    std::unique_ptr<nvinfer1::IRuntime> runtime{nvinfer1::createInferRuntime(m_logger.getTRTLogger())};
    m_engine = std::shared_ptr<nvinfer1::ICudaEngine>(
            runtime->deserializeCudaEngine(engine_data.data(), f_size));

    m_batch_size = m_engine->getTensorShape("conv_inputs").d[0];
    if (m_batch_size != m_feature_handler.MinibatchSize()) {
        LOG(FATAL) << "Minibatch size is not matched. Please check the config file";
        return 0;
    }

    GetOutputs();
    std::cout << ModelFlag::DebugString(m_branch_flag);

    m_context = std::unique_ptr<nvinfer1::IExecutionContext>(m_engine->createExecutionContext());
    if (!m_context) {
        std::cout << "Cannot create context for model file: " << path;
        return 0;
    }
    std::cout << "Deep model loaded: " << path;
    std::cout << "Conv number = " << m_feature_handler.SquareNum();
    std::cout << "Vector number = " << m_feature_handler.VectorNum();
    std::cout << "Minibatch size = " << m_batch_size;
    return m_branch_flag;
}

BufferHandler* DeepModel::GetNewBuffer() {
    return (new BufferHandler(m_engine));
}

void DeepModel::FillSingleInput(BufferHandler* buffers, Feature* feature, int k) {
    if (k >= m_batch_size) {
        return ;
    }
    float* conv_inputs = static_cast<float*>(buffers->GetBuffer(true, "conv_inputs"));
    memcpy(conv_inputs + k * m_conv_size,
           feature->m_buffer + m_conv_begin,
           m_conv_size * sizeof(float));
    float* vector_inputs = static_cast<float*>(buffers->GetBuffer(true, "vector_inputs"));
    memcpy(vector_inputs + k * m_vector_size,
           feature->m_buffer + m_vector_begin,
           m_vector_size * sizeof(float));
}

bool DeepModel::ProcessBatch(BufferHandler* buffers) {
    m_gpu_mutex.lock();
    bool status = m_context->executeV2(buffers->GetDeviceBindings().data());
    m_gpu_mutex.unlock();
    return status;
}

void DeepModel::GetSingleOutput(BufferHandler* buffers, MixedResult* mixed_result, int k) {
    if (k >= m_batch_size) {
        return ;
    }
    int square = Coordinate::m_square;
    mixed_result->branch_flag = m_branch_flag;
    if (m_branch_flag & ModelFlag::kPolicy) {
        float* policy = static_cast<float*>(buffers->GetBuffer(true, "policy"));
        memcpy(mixed_result->policy_result.m_prob,
               policy + k * (square + 1),
               (square + 1) * sizeof(float));
    }
    if (m_branch_flag & ModelFlag::kValue) {
        float* value = static_cast<float*>(buffers->GetBuffer(true, "value"));
        mixed_result->value_result.m_value = *(value + k);
    }
    if (m_branch_flag & ModelFlag::kArea) {
        float* area = static_cast<float*>(buffers->GetBuffer(true, "area"));
        memcpy(mixed_result->area_result.m_area,
               area + k * (2 * square + 1),
               square * sizeof(float));
        memcpy(mixed_result->area_result.m_share,
               area + k * (2 * square + 1) + square,
               square * sizeof(float));
        mixed_result->area_result.m_first_pass = *(area + k * (2 * square + 1) + 2 * square);
    }
    if (m_branch_flag & ModelFlag::kTerritory) {
        float* territory = static_cast<float*>(buffers->GetBuffer(true, "territory"));
        memcpy(mixed_result->area_result.m_territory,
               territory + k * 2 * square,
               square * sizeof(float));
        memcpy(mixed_result->area_result.m_capture,
               territory + k * 2 * square + square,
               square * sizeof(float));
        mixed_result->area_result.m_has_territory = true;
    }
    if (m_branch_flag & ModelFlag::kScoreReg) {
        float* score_reg = static_cast<float*>(buffers->GetBuffer(true, "score_reg"));
        mixed_result->score_result.m_score = *(score_reg + k);
    }
    if (m_branch_flag & ModelFlag::kScoreLogit) {
        float* score_logit = static_cast<float*>(buffers->GetBuffer(true, "score_logit"));
        memcpy(mixed_result->score_logit_result.m_score_logit,
               score_logit + k * (square + 1) * 2,
               (square + 1) * 2 * sizeof(float));
        mixed_result->score_logit_result.m_logit_size = (square + 1) * 2;
        mixed_result->score_logit_result.m_zero_pos = square;
    }
}

void DeepModel::GetOutputs() {
    m_branch_flag = 0;
    for (int i = 0; i < m_engine->getNbIOTensors(); ++i) {
        std::string name = m_engine->getIOTensorName(i);
        // Outputs
        if (name == "policy") {
            m_branch_flag |= ModelFlag::kPolicy;
        }
        if (name == "value") {
            m_branch_flag |= ModelFlag::kValue;
        }
        if (name == "area") {
            m_branch_flag |= ModelFlag::kArea;
        }
        if (name == "territory") {
            m_branch_flag |= ModelFlag::kTerritory;
        }
        if (name == "score_reg") {
            m_branch_flag |= ModelFlag::kScoreReg;
        }
        if (name == "score_logit") {
            m_branch_flag |= ModelFlag::kScoreLogit;
        }
    }
}

void DeepModel::PredictOne(const Situation& situ, MixedResult* result, int code) {
    if (code < 0) {
        code = rand() % 8;
    }
    BufferHandler buffers(m_engine);
    Feature feature;
    m_feature_handler.GetFeature(situ, &feature, code);
    FillSingleInput(&buffers, &feature, 0);
    buffers.CopyInputToDevice();
    ProcessBatch(&buffers);
    buffers.CopyOutputToHost();
    GetSingleOutput(&buffers, result, 0);
    result->Symmetry(code);
}

