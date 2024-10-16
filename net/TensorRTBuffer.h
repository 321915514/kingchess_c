//
// Created by philxling on 2024/3/31.
//

#ifndef KINGCHESS_TENSORRTBUFFER_H
#define KINGCHESS_TENSORRTBUFFER_H

#pragma once
#include "NvInfer.h"
#include "common.h"
#include <cassert>
#include <cuda_runtime_api.h>
#include <iostream>
#include <iterator>
#include <memory>
#include <new>
#include <numeric>
#include <string>
#include <vector>

namespace tensor_custom
{
    inline uint32_t getElementSize(nvinfer1::DataType dt) noexcept
    {
        switch (dt)
        {
            case nvinfer1::DataType::kINT32: return 4;
            case nvinfer1::DataType::kFLOAT: return 4;
            case nvinfer1::DataType::kHALF: return 2;
            case nvinfer1::DataType::kBOOL:
            case nvinfer1::DataType::kINT8: return 1;
        }
        return 0;
    }
    inline int64_t volume(const nvinfer1::Dims& d)
    {
        return std::accumulate(d.d, d.d + d.nbDims, 1, std::multiplies<int64_t>());
    }

    template<class AllocFunc, class CopyFunc, class FreeFunc>
    class GenericBuffer
    {
    public:
        GenericBuffer(nvinfer1::DataType type)
                :_size(0)
                , _type(type)
                , _buffer(nullptr)
        {
        }

        GenericBuffer(size_t size, nvinfer1::DataType type)
                :_size(size)
                , _type(type)
                , _buffer(nullptr)
        {
            if (!_allocFun(&_buffer, nbBytes()))
            {
                throw std::bad_alloc();
            }
        }

        GenericBuffer(const GenericBuffer& generic_buf)
        {
            _size = generic_buf._size;
            _type = generic_buf._type;
            if (generic_buf._buffer != nullptr)
            {
                _copyFun(_buffer, generic_buf._buffer, nbBytes());
            }
        }

        GenericBuffer& operator=(const GenericBuffer& generic_buf)
        {
            _size = generic_buf._size;
            _type = generic_buf._type;
            if (generic_buf._buffer != nullptr)
            {
                _copyFun(_buffer, generic_buf._buffer, nbBytes());
            }
        }

        ~GenericBuffer()
        {
            _freeFun(_buffer);
        }

        size_t nbBytes()
        {
            return _size * getElementSize(_type);
        }

        size_t getSize()
        {
            return _size;
        }

        nvinfer1::DataType getType()
        {
            return _type;
        }

        void* data()
        {
            return _buffer;
        }

        const void* data() const
        {
            return _buffer;
        }

    private:
        AllocFunc _allocFun;
        CopyFunc _copyFun;
        FreeFunc _freeFun;
        size_t _size;
        nvinfer1::DataType _type;
        void* _buffer;
    };

    class HostAllocator
    {
    public:
        bool operator()(void** ptr, size_t size) const
        {
            *ptr = malloc(size);
            return *ptr != nullptr;
        }
    };

    class HostFree
    {
    public:
        void operator()(void* ptr) const
        {
            free(ptr);
        }
    };

    class HostCopy
    {
    public:
        void operator()(void* dst,void*src,size_t bytes_size) const
        {
            memcpy(dst, src, bytes_size);
        }
    };

    class DeviceAllocator
    {
    public:
        bool operator()(void** ptr,size_t size)const
        {
            return cudaMalloc(ptr, size) == cudaSuccess;
        }
    };

    class DeviceFree
    {
    public:
        void operator()(void* ptr) const
        {
            cudaFree(ptr);
        }
    };

    class DeviceCopy
    {
    public:
        void operator()(void* dst, void*src, size_t bytes_size) const
        {
            cudaMemcpy(dst, src, bytes_size, cudaMemcpyDeviceToDevice);
        }
    };

    using DeviceBuffer = GenericBuffer<DeviceAllocator, DeviceCopy, DeviceFree>;
    using HostBuffer = GenericBuffer<HostAllocator, HostCopy, HostFree>;

    class BufferManager
    {
    public:
        BufferManager(std::shared_ptr<nvinfer1::ICudaEngine> engine,
                      const int batchSize = 1,
                      const nvinfer1::IExecutionContext* context = nullptr);

        void copyInputToDevice();
        void copyInputToDeviceAsync(const cudaStream_t& stream);
        void copyOutputToHost();
        void copyOutputToHostAsync(const cudaStream_t& stream);
        void* getHostBufferData(int idx,size_t& data_size);

        std::vector<void*>& getDeviceBindngs();
        const std::vector<void*>& getDeviceBinds() const;

    private:
        void memcpyBuffers(bool hostToDevice,bool async,const cudaStream_t& stream = 0);

    private:
        int _batchSize;
        std::shared_ptr<nvinfer1::ICudaEngine> _engine;
        std::vector<std::unique_ptr<HostBuffer>> _hostBuffers;
        std::vector<std::unique_ptr<DeviceBuffer>> _deviceBuffers;
        std::vector<void*> _deviceBindngs;
    };
}








#endif //KINGCHESS_TENSORRTBUFFER_H
