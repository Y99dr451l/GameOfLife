#include <cuda_runtime.h>
#include "BasicGPU.cuh"

__global__ void gameOfLifeKernel(const unsigned char* inputGrid, unsigned char* outputGrid, int width, int height) {
    unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x < width && y < height) {
        size_t size = width * height;
        unsigned int index = x + y * width;
        unsigned int x1 = index % width;
        unsigned int y1 = index - x1;
        unsigned int y0 = (y1 + size - width) % size;
        unsigned int y2 = (y1 + width) % size;
        unsigned int x0 = (x1 + width - 1) % width;
        unsigned int x2 = (x1 + 1) % width;
        unsigned char aliveCells = inputGrid[x0 + y0] + inputGrid[x1 + y0] + inputGrid[x2 + y0] + inputGrid[x0 + y1] + inputGrid[x2 + y1] + inputGrid[x0 + y2] + inputGrid[x1 + y2] + inputGrid[x2 + y2];
        outputGrid[index] = aliveCells == 3 || (aliveCells == 2 && inputGrid[index]) ? 1 : 0;
    }
}

void runCudaSimulation(unsigned int width, unsigned int height, unsigned char* m_data, unsigned char* d_data, unsigned char* d_resultData, size_t dataBytes) {
    dim3 nThreads(32, 32);
    dim3 nBlocks((width + nThreads.x - 1) / nThreads.x, (height + nThreads.y - 1) / nThreads.y);
    gameOfLifeKernel <<<nBlocks, nThreads>>> (d_data, d_resultData, width, height);
    cudaDeviceSynchronize();
    if (cudaMemcpy(m_data, d_resultData, dataBytes, cudaMemcpyDeviceToHost) != cudaSuccess) {
		std::cout << "Error copying data from device to host" << std::endl;
	}
}