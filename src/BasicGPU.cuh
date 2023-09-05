#pragma once
#include <random>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "ArrayApp.h"

void runCudaSimulation(unsigned int width, unsigned int height, unsigned char* m_data, unsigned char* d_data, unsigned char* d_resultData, size_t dataBytes);

class BasicGPU : public ArrayApp {
public:
    BasicGPU(unsigned int width, unsigned int height) : ArrayApp(width, height) {
        m_name = "GPU Game of Life";
        initData();
        initializeCuda();
    }
    ~BasicGPU() {
        cleanupCuda();
    }

    void update(float deltaTime) override {
        cudaMemcpy(d_data, m_data.data(), dataBytes, cudaMemcpyHostToDevice);
        unsigned char* m_datap = m_data.data();
        runCudaSimulation(m_width, m_height, m_datap, d_data, d_resultData, dataBytes);
        if (m_data.size() == m_size) m_texture = std::make_unique<Texture>(m_width, m_height, m_data.data(), GL_RED, GL_RED);
    }

    void initData() {
        m_data.resize(m_size); // m_resultData is unused
        for (size_t i = 0; i < m_size; ++i) m_data[i] = mt() % 2;
        m_texture = std::make_unique<Texture>(m_width, m_height, m_data.data(), GL_RED, GL_RED);
    }

    void resize(unsigned int width, unsigned int height) override {
        ArrayApp::resize(width, height);
		cleanupCuda();
		initializeCuda();
    }

private:
    unsigned char* d_data;
    unsigned char* d_resultData;
    size_t dataBytes;
    std::mt19937 mt{ (unsigned int)time(0) };

    void initializeCuda() {
        dataBytes = m_size * sizeof(unsigned char);
        if (cudaMalloc((void**)&d_data, dataBytes) != cudaSuccess) {
			std::cout << "Failed to allocate device memory" << std::endl;
			exit(EXIT_FAILURE);
		}
        if (cudaMalloc((void**)&d_resultData, dataBytes) != cudaSuccess) {
            std::cout << "Failed to allocate device memory" << std::endl;
			exit(EXIT_FAILURE);
        }
        if (cudaMemcpy(d_data, m_data.data(), dataBytes, cudaMemcpyHostToDevice) != cudaSuccess) {
            std::cout << "Failed to copy data to device" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    void cleanupCuda() {
        cudaFree(d_data);
        cudaFree(d_resultData);
    }
};