//
// Created by Gegel85 on 25/04/2020.
//

#include "ImageOperation.hpp"
#include "../ShortcutManager.hpp"

namespace Mimp {
	ImageOperation::ImageOperation(const std::vector<std::string> &&hierarchy, const std::string &&name) :
			_hierarchy(hierarchy),
			_name(name)
	{
	}

	ImageOperation::ImageOperation(const std::vector<std::string> &&hierarchy, const std::string &&name, const Keys::KeyCombination &keys) :
			_hierarchy(hierarchy),
			_name(name)
	{
		this->setKeyCombination(keys);
	}

	const std::vector<std::string> &ImageOperation::getMenuHierarchy() const
	{
		return this->_hierarchy;
	}
}