//
// Created by Gegel85 on 06/04/2020.
//

#include <cmath>
#include "Color.hpp"

namespace Mimp
{
	const Color Color::Black       = 0x000000FF;
	const Color Color::LightGray   = 0xA0A0A0FF;
	const Color Color::DarkGray    = 0x505050FF;
	const Color Color::White       = 0xFFFFFFFF;
	const Color Color::Red         = 0xFF0000FF;
	const Color Color::Blue        = 0x0000FFFF;
	const Color Color::Green       = 0x00FF00FF;
	const Color Color::Magenta     = 0xFF00FFFF;
	const Color Color::Yellow      = 0xFFFF00FF;
	const Color Color::Cyan        = 0x00FFFFFF;
	const Color Color::Transparent = 0x00000000;

	Color::Color(unsigned int color) noexcept :
		r((color >> 0x18U) & 0xFFU),
		g((color >> 0x10U) & 0xFFU),
		b((color >> 0x08U) & 0xFFU),
		a((color >> 0x00U) & 0xFFU)
	{
	}

	Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept :
		r(r),
		g(g),
		b(b),
		a(a)
	{
	}

	Color::operator unsigned int() const noexcept
	{
		return (this->r << 0x18U) | (this->g << 0x10U) | (this->b << 0x08U) | (this->a << 0x00U);
	}

	Color Color::operator+(const Mimp::Color &other)
	{
		if (this->a && other.a) {
			double thisA = this->a / 255.;
			double otherA = other.a / 255.;
			double resultA = 1 - (1 - otherA) * (1 - thisA);

			return {
				static_cast<unsigned char>(round((other.r * otherA / resultA) + (this->r * thisA * (1 - otherA) / resultA))),
				static_cast<unsigned char>(round((other.g * otherA / resultA) + (this->g * thisA * (1 - otherA) / resultA))),
				static_cast<unsigned char>(round((other.b * otherA / resultA) + (this->b * thisA * (1 - otherA) / resultA))),
				static_cast<unsigned char>(resultA * 255)
			};
		} else if (this->a)
			return *this;
		else
			return other;
	}

	Color::operator sf::Color() const noexcept
	{
		return sf::Color(this->r, this->g, this->b, this->a);
	}

	unsigned Color::diff(const Color &other, bool include_alpha) const noexcept
	{
		if (*this == other)  // Because the color has 70% chance of being the same, this is for optimisation.
			return 0;
		return std::abs(this->r - other.r) + std::abs(this->g - other.g) + std::abs(this->b - other.b)
			+ (include_alpha ? std::abs(this->a - other.a) : 0);
	}
}