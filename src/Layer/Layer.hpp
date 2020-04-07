//
// Created by Gegel85 on 06/04/2020.
//

#ifndef MYGIMP_LAYER_HPP
#define MYGIMP_LAYER_HPP

#include <string>
#include "../Data/Vector2.hpp"
#include "../Data/Color.hpp"
#include "FrameBuffer.hpp"

namespace Mimp
{
	struct Layer {
		FrameBuffer buffer;
		bool locked = false;
		bool visible = true;
		Vector2<int> pos = {0, 0};

		Layer(Vector2<unsigned int> size, const Color &defaultColor = Color::Transparent);

		Vector2<unsigned int> getSize() const noexcept;
		bool isLocked() const noexcept;
	};
}


#endif //MYGIMP_LAYER_HPP
