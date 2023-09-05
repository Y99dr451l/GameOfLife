#pragma once
#include <ctime>
#include <random>
#include "ArrayApp.h"
#include "threadpool.h"

class ParallelCPU : public ArrayApp {
public:
    ParallelCPU(unsigned int width, unsigned int height) : ArrayApp(width, height), nWorkerComplete(0), nThreads(std::thread::hardware_concurrency()), pool(nThreads) {
		m_name = "Parallel CPU";
		initData();
	}
    void update(float deltaTime) {
		nWorkerComplete = 0;
        for (unsigned int i = 0; i < nThreads; ++i) pool.enqueue([&, i] { worker(nThreads, i); });
		while (nWorkerComplete < nThreads) {}
        std::swap(m_data, m_resultData);
		if (m_data.size() == m_size) m_texture = std::make_unique<Texture>(m_width, m_height, m_data.data(), GL_RED, GL_RED);
    }
    void worker(int nThreads, unsigned int id) {
		for (unsigned int i = id; i < m_width; i += nThreads) {
			for (unsigned int j = 0; j < m_height; j++) {
				unsigned int index = i + j * m_width;
				unsigned int x1 = index % m_width;
				unsigned int y1 = index - x1;
				unsigned int y0 = (y1 + m_size - m_width) % m_size;
				unsigned int y2 = (y1 + m_width) % m_size;
				unsigned int x0 = (x1 + m_width - 1) % m_width;
				unsigned int x2 = (x1 + 1) % m_width;
				unsigned char aliveCells = countAliveCells(x0, x1, x2, y0, y1, y2);
				m_resultData[index] = aliveCells == 3 || (aliveCells == 2 && m_data[index]) ? 1 : 0;
			}
		}
		nWorkerComplete++;
	}
	void initData() {
		m_data.resize(m_size);
		m_resultData.resize(m_size);
		for (size_t i = 0; i < m_size; ++i) {
			m_data[i] = mt() % 2;
			m_resultData[i] = 0;
		}
		m_texture = std::make_unique<Texture>(m_width, m_height, m_data.data(), GL_RED, GL_RED);
	}
private:
	inline unsigned char countAliveCells(unsigned int x0, unsigned int x1, unsigned int x2, unsigned int y0, unsigned int y1, unsigned int y2) {
		return m_data[x0 + y0] + m_data[x1 + y0] + m_data[x2 + y0] + m_data[x0 + y1] + m_data[x2 + y1] + m_data[x0 + y2] + m_data[x1 + y2] + m_data[x2 + y2];
	}
	unsigned int nThreads;
	ThreadPool pool;
	std::atomic<int> nWorkerComplete;
	std::mt19937 mt{ (unsigned int)time(0) };
};