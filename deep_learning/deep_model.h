#pragma once


#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <NvInfer.h>
#include <cuda_runtime_api.h>
#

class Logger : public nvinfer1::ILogger {
public:
    void log(Severity severity, nvinfer1::AsciiChar const* msg) noexcept {
        std::cout<< "[TRT]" << std::string(msg);
    }

    nvinfer1::ILogger& getTRTLogger() {
        return *this;
    }
};

class IOBuffer {
public:
    IOBuffer(size_t size, nvinfer1::DataType type, bool is_host)
            : m_size(size),
              m_type(type),
              m_is_host(is_host) {
        bool success;
        if (m_is_host) {
            m_buffer = malloc(GetBytes());
            success = (m_buffer != nullptr);
        } else {
            success = (cudaMalloc(&m_buffer, GetBytes()) == cudaSuccess);
        }
        if (!success) {
            std::cout << "Cannot alloc the memory!";
        }
    }

    ~IOBuffer() {
        if (m_is_host) {
            free(m_buffer);
        } else {
            cudaFree(m_buffer);
        }
    }

    size_t GetBytes() const {
        if (m_type == nvinfer1::DataType::kFLOAT) {
            return m_size * 4;
        } else {
            return m_size;
        }
    }

    void* Data() {
        return m_buffer;
    }

private:
    void* m_buffer;
    size_t m_size;
    nvinfer1::DataType m_type;
    bool m_is_host;
};

class BufferHandler {
public:
    BufferHandler(std::shared_ptr<nvinfer1::ICudaEngine> engine);
    ~BufferHandler();

    std::vector<void*>& GetDeviceBindings() {
        return m_device_bindings;
    }
    int GetIndex(const std::string tensor_name) const;
    void* GetBuffer(bool is_host, const std::string tensor_name) const;
    void CopyInputToDevice();
    void CopyInputToDevice(int start, int num);
    void CopyOutputToHost();
protected:
    std::shared_ptr<nvinfer1::ICudaEngine> m_engine;
    std::vector<std::unique_ptr<IOBuffer> > m_host_buffers;
    std::vector<std::unique_ptr<IOBuffer> > m_device_buffers;
    std::vector<void*> m_device_bindings;
    int m_batch_size;
};

class DeepModel {
public:
    DeepModel() :
            m_engine(nullptr) {}

    ~DeepModel() {
    }

    int InitModel(const std::string& path);

    BufferHandler* GetNewBuffer();
    void FillSingleInput(BufferHandler* buffers, Feature* feature, int k);
    bool ProcessBatch(BufferHandler* buffers);
    void GetSingleOutput(BufferHandler* buffers, MixedResult* mixed_result, int k);

    void PredictOne(const Situation& situ, MixedResult* result, int code = -1);

//    const FeatureHandler& GetFeatureHandler() const {
//        return m_feature_handler;
//    }

    int BatchSize() const {
        return m_batch_size;
    }

    int BranchFlag() const {
        return m_branch_flag;
    }


protected:
    void GetOutputs();
    void GetResult(const std::vector<float*> buffer,
                   std::vector<MixedResult*>& results);

protected:
//    FeatureHandler m_feature_handler;
    std::shared_ptr<nvinfer1::ICudaEngine> m_engine;
    std::unique_ptr<nvinfer1::IExecutionContext> m_context;
    Logger m_logger;
    std::mutex m_gpu_mutex;

    int m_conv_begin, m_conv_size;
    int m_vector_begin, m_vector_size;
    int m_batch_size = 0;
    int m_branch_flag = 0;
};


