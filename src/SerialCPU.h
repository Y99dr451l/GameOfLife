#pragma once
#include "GameOfLife.h"

class SerialCPU : public GameOfLife {
public:
	SerialCPU(size_t width, size_t height) : GameOfLife(width, height) { m_name = "Serial CPU"; }
	void update(float deltaTime) {
        //std::cout << "Serial CPU: " << m_size << std::endl;
        for (size_t y = 0; y < m_height; ++y) {
            size_t y0 = ((y + m_height - 1) % m_height) * m_width;
            size_t y1 = y * m_width;
            size_t y2 = ((y + 1) % m_height) * m_width;
            for (size_t x = 0; x < m_width; ++x) {
                size_t x0 = (x + m_width - 1) % m_width;
                size_t x2 = (x + 1) % m_width;
                unsigned char aliveCells = countAliveCells(x0, x, x2, y0, y1, y2);
                m_resultData[y1 + x] = aliveCells == 3 || (aliveCells == 2 && m_data[x + y1]) ? 1 : 0;
            }
        }
        std::swap(m_data, m_resultData);
        m_texture = std::make_unique<Texture>(m_width, m_height, m_data, GL_RED, GL_RED);
	}
};