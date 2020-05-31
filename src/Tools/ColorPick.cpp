/*
** EPITECH PROJECT, 2020
** MyGimp
** File description:
** ColorPick.cpp
*/
#include "ColorPick.hpp"

Mimp::ColorPick::ColorPick(Mimp::ToolBox &toolBox):
	Tool("ColorPick"),
	_toolBox(toolBox)
{
	this->setKeyCombination({Keys::KEY_C, false, false, false});
}

void Mimp::ColorPick::onClick(Mimp::Vector2<int> pos, Mimp::MouseClick click, Mimp::Image &image)
{
	auto &layer = image.getSelectedLayer();
	Color color = layer.buffer->getPixel(pos - layer.pos);

	this->_toolBox.setSelectedColor(click, color);
}


void Mimp::ColorPick::onMouseDrag(Mimp::Vector2<int>, Mimp::Vector2<int>, Mimp::MouseClick, Mimp::Image &)
{}

tgui::ScrollablePanel::Ptr Mimp::ColorPick::getParametersPanel()
{
	return tgui::ScrollablePanel::create({0, 0});
}
