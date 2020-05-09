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

	SelectedArea::SelectedArea(Vector2<unsigned> size) :
		_size(size),
		_map(new bool[size.x * size.y])
	{
		this->clear();
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
				layer.buffer.setPixel(pt, color);
	}

	bool SelectedArea::pointInMap(Vector2<int> point)
	{
		return this->pointInMap(point.x, point.y);
	}

	bool SelectedArea::pointInMap(int x, int y)
	{
		return this->_map[x + y * this->_size.x];
	}

	SelectedArea* SelectedArea::copy()
	{
		SelectedArea *new_sa = new SelectedArea(this->_size);
		new_sa->_update_bool_array(this->_map);
		return new_sa;
	}

	void SelectedArea::_update_bool_array(const bool *bool_array)
	{
		for (unsigned i = 0; i < this->_size.x * this->_size.y; i++)
			this->_map[i] = bool_array[i];
	}

	const Vector2<unsigned int> &SelectedArea::getSize() const
	{
		return _size;
	}
}
