/*
** EPITECH PROJECT, 2020
** MyGimp
** File description:
** SelectedArea.cpp
*/
#include <cstring>
#include <thread>
#include "SelectedArea.hpp"
#include "Utils.hpp"

namespace Mimp
{
	SelectedArea::SelectedArea(unsigned width, unsigned height) :
		SelectedArea(Vector2<unsigned>{width, height})
	{
	}

	SelectedArea::SelectedArea(Vector2<unsigned> size, const bool *map) :
		_size(size),
		_map(new bool[size.x * size.y])
	{
		this->clear();
		if (map)
			std::memcpy(this->_map, map, size.x * size.y);
	}

	SelectedArea::SelectedArea(const SelectedArea &other) :
		SelectedArea(other.getSize(), other._map)
	{
	}

	SelectedArea::~SelectedArea()
	{
		delete[] this->_map;
	}

	void SelectedArea::clear() noexcept
	{
		std::memset(this->_map, false, this->_size.x * this->_size.y);
		this->_nbPoints = 0;
	}

	void SelectedArea::selectAll() noexcept
	{
		std::memset(this->_map, true, this->_size.x * this->_size.y);
		this->_nbPoints = this->_size.x * this->_size.y;
	}

	bool SelectedArea::isAnAreaSelected() const noexcept
	{
		return this->_nbPoints;
	}

	void SelectedArea::add(Vector2<int> point)
	{
		this->add(point.x, point.y);
	}

	void SelectedArea::add(int x, int y)
	{
		if (Utils::isOutOfBound({x, y}, this->_size))
			return;

		auto &val = this->_map[x + y * this->_size.x];

		this->_nbPoints += !val;
		val = true;
	}

	void SelectedArea::remove(Vector2<int> point)
	{
		this->remove(point.x, point.y);
	}

	void SelectedArea::remove(int x, int y)
	{
		if (Utils::isOutOfBound({x, y}, this->_size))
			return;

		auto &val = this->_map[x + y * this->_size.x];

		this->_nbPoints -= val;
		val = false;
	}

	std::vector<Vector2<int>> SelectedArea::getPoints() const noexcept
	{
		int index = 0;
		std::vector<Vector2<int>> selectedPoints;

		selectedPoints.reserve(this->_nbPoints);
		for (unsigned y = 0; y < this->_size.y; y++)
			for (unsigned x = 0; x < this->_size.x; x++)
				if (this->_map[index++])
					selectedPoints.emplace_back(x, y);
		return selectedPoints;
	}

	void SelectedArea::invert() noexcept
	{
		int index = 0;

		this->_nbPoints = this->_size.x * this->_size.y - this->_nbPoints;
		for (unsigned y = 0; y < this->_size.y; y++)
			for (unsigned x = 0; x < this->_size.x; x++) {
				this->_map[index] = !this->_map[index];
				index++;
			}
	}

	void SelectedArea::fill(Layer &layer, const Color &color)
	{
		if (this->isAnAreaSelected())
			for (const auto &pt : this->getPoints())
				layer.buffer->setPixel(pt, color);
	}

	bool SelectedArea::pointInMap(Vector2<int> point) const
	{
		return this->pointInMap(point.x, point.y);
	}

	bool SelectedArea::pointInMap(int x, int y) const
	{
		if (Utils::isOutOfBound({x, y}, this->_size))
			return false;
		return this->_map[x + y * this->_size.x];
	}

	Vector2<unsigned int> SelectedArea::getSize() const
	{
		return _size;
	}

	void SelectedArea::setSize(Vector2<unsigned> size)
	{
		if (this->_size == size)
			return;

		this->_size = size;
		delete[] this->_map;
		this->_map = new bool[size.x * size.y];
		this->clear();
	}
}
