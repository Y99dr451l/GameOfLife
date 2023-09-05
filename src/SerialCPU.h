#pragma once
#include <ctime>
#include <random>
#include "ArrayApp.h"

class SerialCPU : public ArrayApp {
public:
	SerialCPU(unsigned int width, unsigned int height) : ArrayApp(width, height) {
        m_name = "Serial CPU";
        initData();
    }
	void update(float deltaTime) {
        for (unsigned int y = 0; y < m_height; ++y) {
            unsigned int y0 = ((y + m_height - 1) % m_height) * m_width;
            unsigned int y1 = y * m_width;
            unsigned int y2 = ((y + 1) % m_height) * m_width;
            for (unsigned int x = 0; x < m_width; ++x) {
                unsigned int x0 = (x + m_width - 1) % m_width;
                unsigned int x2 = (x + 1) % m_width;
                unsigned char aliveCells = countAliveCells(x0, x, x2, y0, y1, y2);
                m_resultData[y1 + x] = aliveCells == 3 || (aliveCells == 2 && m_data[x + y1]) ? 1 : 0;
            }
        }
        std::swap(m_data, m_resultData);
        if (m_data.size() == m_size) m_texture = std::make_unique<Texture>(m_width, m_height, m_data.data(), GL_RED, GL_RED);
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
    std::mt19937 mt{ (unsigned int)time(0) };
};