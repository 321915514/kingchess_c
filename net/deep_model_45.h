//
// Created by philxling on 2024/3/30.
//

#ifndef KINGCHESS_DEEP_MODEL_H
#define KINGCHESS_DEEP_MODEL_H

#include <NvInfer.h>
#include <cuda_runtime_api.h>

#include <cuda_runtime.h>
#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <mutex>
#include "NvInferPlugin.h"
#include "common.h"
#include <queue>

#include <thread>
#include <functional>
#include <condition_variable>
#include <future>





using namespace det;
//class Logger : public nvinfer1::ILogger {
//public:
//    void log(Severity severity, nvinfer1::AsciiChar const* msg) noexcept {
//        // 打印日志消息
//        std::cout << msg << std::endl;
//        std::cout << "[TRT]" << std::string(msg);
//    }
//
//    nvinfer1::ILogger& getTRTLogger() {
//        return *this;
//    }
//};
//struct InferDeleter {
//    template <typename T>
//    void operator()(T* obj) const {
//        if (obj) {
//            obj->destroy();
//        }
//    }
//};


class deep_model {
public:
    int                  num_bindings;
    int                  num_inputs  = 0;
    int                  num_outputs = 0;
    std::vector<Binding> input_bindings;
    std::vector<Binding> output_bindings;
    std::vector<void*>   host_ptrs;
    std::vector<void*>   device_ptrs;
    std::mutex lock;
    //std::queue<std::array<float,81*5>> q;

    //using return_type = std::vector<std::vector<float>>;

    void InitModel(const std::string& path);

    deep_model() {
        //InitModel(modelPath);

        //for (auto &bindings: this->input_bindings) {
        //        void *d_ptr;
        //        CHECK(cudaMallocAsync(&d_ptr, bindings.size * bindings.dsize, this->stream));
        //        this->device_ptrs.push_back(d_ptr);
        //    }
        //
        //    for (auto &bindings: this->output_bindings) {
        //        void *d_ptr, *h_ptr;
        //        size_t size = bindings.size * bindings.dsize;
       //         CHECK(cudaMallocAsync(&d_ptr, size, this->stream));
        //        CHECK(cudaHostAlloc(&h_ptr, size, 0));
        //        this->device_ptrs.push_back(d_ptr);
       //         this->host_ptrs.push_back(h_ptr);
       //     }



    //TaskQueue queue(4);  // 使用 4 个工作线程

    //auto future_result = queue.enqueue([&model, &grid]() {
    //    return model.commit(grid);  // 假设 commit 现在返回值而非 future
    //});
    }

    deep_model(const std::string& modelPath){




        InitModel(modelPath);

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


    ~deep_model(){
        this->context->destroy();
        this->engine->destroy();
        this->runtime->destroy();
        cudaStreamDestroy(this->stream);
        for (auto& ptr : this->device_ptrs) {
            CHECK(cudaFree(ptr));
        }
        for (auto& ptr : this->host_ptrs) {
            CHECK(cudaFreeHost(ptr));
        }
    }
    //void push(float (&grid)[81*5]){
    //    std::array<float,81*5> temp;
    //    memcpy(temp,grid,sizeof(grid));
    //    q.push(temp);
    //}


    void infer(float (&grid)[45*32]){
//        for(auto item:grid){
//            std::cout<<item<<"\t";
//        }

        for (auto& bindings : this->input_bindings) {
            void* d_ptr;
            CHECK(cudaMalloc(&d_ptr, bindings.size * bindings.dsize));
            this->device_ptrs.push_back(d_ptr);
        }

        for (auto& bindings : this->output_bindings) {
            void * d_ptr, *h_ptr;
            size_t size = bindings.size * bindings.dsize;
            CHECK(cudaMalloc(&d_ptr, size));
            CHECK(cudaHostAlloc(&h_ptr, size, 0));
            this->device_ptrs.push_back(d_ptr);
            this->host_ptrs.push_back(h_ptr);
        }

        for (auto& bindings : this->input_bindings) {
            size_t size  = bindings.size * bindings.dsize;
//            void*  h_ptr = malloc(size);
//            std::memset(h_ptr, 0, size);
            void*  h_ptr = malloc(size);
            memcpy(h_ptr,grid,sizeof(grid));
            CHECK(cudaMemcpyAsync(this->device_ptrs[0], h_ptr, size, cudaMemcpyHostToDevice,this->stream));
            free(h_ptr);
        }

        this->context->enqueueV2(this->device_ptrs.data(), this->stream, nullptr);
//        std::cout<<num_outputs<<std::endl;
        for (int i = 0; i < this->num_outputs; i++) {
            size_t osize = this->output_bindings[i].size * this->output_bindings[i].dsize;
            CHECK(cudaMemcpyAsync(this->host_ptrs[i], this->device_ptrs[i + this->num_inputs], osize, cudaMemcpyDeviceToHost,this->stream));
        }
        cudaStreamSynchronize(this->stream);
    }


    std::vector<std::vector<float>> commit(float (&grid)[45*32]) {
//        int32_t grid[81 * 5] = {0};
//
//        encoder(game, grid);
//
        //for (auto i: grid) {
        //    std::cout << i << "\t";
        //}

        //std::cout<<std::endl;


            //std::lock_guard<std::mutex> lockGuard(this->lock);

        //std::promise<std::vector<std::vector<float>>> result_promise;
            //std::future<std::vector<std::vector<float>>> result_future = result_promise.get_future();

           // for (auto &bindings: this->input_bindings) {
           //     void *d_ptr;
           //     CHECK(cudaMallocAsync(&d_ptr, bindings.size * bindings.dsize, this->stream));
           //     this->device_ptrs.push_back(d_ptr);
           // }

           // for (auto &bindings: this->output_bindings) {
           //     void *d_ptr, *h_ptr;
           //     size_t size = bindings.size * bindings.dsize;
           //     CHECK(cudaMallocAsync(&d_ptr, size, this->stream));
           //     CHECK(cudaHostAlloc(&h_ptr, size, 0));
           //     this->device_ptrs.push_back(d_ptr);
           //     this->host_ptrs.push_back(h_ptr);
           // }

            //for (auto &bindings: this->input_bindings) {
            size_t size = this->input_bindings[0].size * this->input_bindings[0].dsize;
               // void*  h_ptr = malloc(size);
                //memcpy(h_ptr,grid,sizeof(grid));
//                std::memset(h_ptr, , size);
            //{
            // std::lock_guard<std::mutex> lockGuard(this->lock);
             CHECK(cudaMemcpyAsync(this->device_ptrs[0], grid, size, cudaMemcpyHostToDevice, this->stream));
                //free(h_ptr);
            //}

            //auto context = this->engine->createExecutionContext();
            //
            {
            std::lock_guard<std::mutex> lockGuard(this->lock);
            bool status = this->context->enqueueV2(this->device_ptrs.data(), this->stream, nullptr);
            if (!status) {
                std::cout << "ERROR: TensorRT inference failed" << std::endl;
            }
            }
//        std::cout<<num_outputs<<std::endl;
            for (int i = 0; i < this->num_outputs; i++) {
                size_t osize = this->output_bindings[i].size * this->output_bindings[i].dsize;
                CHECK(cudaMemcpyAsync(this->host_ptrs[i], this->device_ptrs[i + this->num_inputs], osize,
                                      cudaMemcpyDeviceToHost, this->stream));
            }
            cudaStreamSynchronize(this->stream);
            //}
           // std::cout<<"return result"<<std::endl;
            // get result
            std::vector<std::vector<float>> result;

            for (size_t i = 0; i < host_ptrs.size(); ++i) {
                // 假设输出数据是 int16_t 类型
                float *data_ptr = reinterpret_cast<float *>(host_ptrs[i]);
                // 输出前几个数据，你可以根据需要调整数量
//            std::cout << "Data from host_ptrs[" << i << "]: ";
                std::vector<float> probs;
                std::vector<float> value;
                if (!i) {
                    for (size_t j = 0; j < 1125; ++j) {
                        probs.push_back(data_ptr[j]);
                    }
                    result.push_back(probs);
                } else {
                    //for (size_t j = 0; j < 1; ++j) {
                    value.push_back(data_ptr[0]);
                    //}
                    result.push_back(value);
                }
//            std::cout << std::endl;
            }
            return result;
       // }

    }


    void print_infer_out(){

        for (size_t i = 0; i < host_ptrs.size(); ++i) {
            // 假设输出数据是 int16_t 类型
            float* data_ptr = reinterpret_cast<float*>(host_ptrs[i]);
            // 输出前几个数据，你可以根据需要调整数量
            std::cout << "Data from host_ptrs[" << i << "]: ";
            if(!i){
                for (size_t j = 0; j < 1125; ++j) {
                    std::cout << data_ptr[j] << " ";
                }
            }else{
                //for (size_t j = 0; j < 1; ++j) {
                 std::cout << data_ptr[0] << " ";
                //}
            }
            std::cout << std::endl;
        }
    }




private:
    nvinfer1::ICudaEngine*       engine  = nullptr;
    nvinfer1::IRuntime*          runtime = nullptr;
    nvinfer1::IExecutionContext* context = nullptr;
    cudaStream_t                 stream  = nullptr;
    Logger                       gLogger{nvinfer1::ILogger::Severity::kERROR};
};


#endif //KINGCHESS_DEEP_MODEL_H
