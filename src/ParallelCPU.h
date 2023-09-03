#pragma once
#include "GameOfLife.h"
#include "threadpool.h"

class ParallelCPU : public GameOfLife {
public:
    ParallelCPU(size_t width, size_t height) : GameOfLife(width, height), nWorkerComplete(0), nThreads(std::thread::hardware_concurrency()), pool(nThreads) { m_name = "Parallel CPU"; }
    void update(float deltaTime) {
        //std::cout << "Parallel CPU: " << m_size << std::endl;
		nWorkerComplete = 0;
        for (size_t i = 0; i < nThreads; ++i) pool.enqueue([&, i] { worker(nThreads, i); });
		while (nWorkerComplete < nThreads) {}
        std::swap(m_data, m_resultData);
		if (m_data.size() == m_size) m_texture = std::make_unique<Texture>(m_width, m_height, m_data.data(), GL_RED, GL_RED);
    }
    void worker(int nThreads, size_t id) {
		for (size_t i = id; i < m_width; i += nThreads) {
			for (size_t j = 0; j < m_height; j++) {
				size_t index = i + j * m_width;
				size_t x1 = index % m_width;
				size_t y1 = index - x1;
				size_t y0 = (y1 + m_size - m_width) % m_size;
				size_t y2 = (y1 + m_width) % m_size;
				size_t x0 = (x1 + m_width - 1) % m_width;
				size_t x2 = (x1 + 1) % m_width;
				unsigned char aliveCells = countAliveCells(x0, x1, x2, y0, y1, y2);
				m_resultData[y1 + x1] = aliveCells == 3 || (aliveCells == 2 && m_data[x1 + y1]) ? 1 : 0;
			}
		}
		nWorkerComplete++;
	}
private:
	int nThreads;
	ThreadPool pool;
	std::atomic<int> nWorkerComplete;
};