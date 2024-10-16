//
// Created by philxling on 2024/3/30.
//

#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include "deep_model.h"
#include <cassert>
#include <memory>

void deep_model::InitModel(const std::string &engine_file_path) {

    std::ifstream file(engine_file_path, std::ios::binary);
    assert(file.good());
    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);
    char* trtModelStream = new char[size];
    assert(trtModelStream);
    file.read(trtModelStream, size);
    file.close();
    initLibNvInferPlugins(&this->gLogger, "");
    this->runtime = nvinfer1::createInferRuntime(this->gLogger);
    assert(this->runtime != nullptr);

    this->engine = this->runtime->deserializeCudaEngine(trtModelStream, size);

    assert(this->engine != nullptr);

    delete[] trtModelStream;




    this->context = this->engine->createExecutionContext();

    assert(this->context != nullptr);
    cudaStreamCreate(&this->stream);
    this->num_bindings = this->engine->getNbBindings();

    for (int i = 0; i < this->num_bindings; ++i) {
        Binding            binding;
        nvinfer1::Dims     dims;
        nvinfer1::DataType dtype = this->engine->getBindingDataType(i);
        std::string        name  = this->engine->getBindingName(i);
        binding.name             = name;
        binding.dsize            = type_to_size(dtype);

        bool IsInput = engine->bindingIsInput(i);
        if (IsInput) {
            this->num_inputs += 1;
            dims         = this->engine->getProfileDimensions(i, 0, nvinfer1::OptProfileSelector::kMAX);
            binding.size = get_size_by_dims(dims);
            binding.dims = dims;
            this->input_bindings.push_back(binding);
            // set max opt shape
            this->context->setBindingDimensions(i, dims);
        }
        else {
            dims         = this->context->getBindingDimensions(i);
            binding.size = get_size_by_dims(dims);
            binding.dims = dims;
            this->output_bindings.push_back(binding);
            this->num_outputs += 1;
        }
    }


    for (auto &bindings: this->input_bindings) {
                void *d_ptr;
                CHECK(cudaMalloc(&d_ptr, bindings.size * bindings.dsize));
                this->device_ptrs.push_back(d_ptr);
    }

    for (auto &bindings: this->output_bindings) {
                void *d_ptr, *h_ptr;
                size_t size = bindings.size * bindings.dsize;
                CHECK(cudaMalloc(&d_ptr, size));
                CHECK(cudaHostAlloc(&h_ptr, size, 0));
                this->device_ptrs.push_back(d_ptr);
                this->host_ptrs.push_back(h_ptr);
    }

}


