/*
** EPITECH PROJECT, 2020
** MyGimp
** File description:
** FillSelection.cpp
*/
#include "FillSelection.hpp"

Mimp::FillSelection::FillSelection(Mimp::ToolBox &toolBox):
	Tool("FillSelection"),
	_toolBox(toolBox)
{}

void Mimp::FillSelection::onClick(Mimp::Vector2<int>, Mimp::MouseClick click, Mimp::Image &image)
{
	if (image.selectedArea.isAnAreaSelected())
		for (Vector2<int> pt : image.selectedArea)
			image.getSelectedLayer().buffer.drawPixel(pt, this->_toolBox.getSelectedColor(click));
}

tgui::ScrollablePanel::Ptr Mimp::FillSelection::getParametersPanel() const
{
	return tgui::ScrollablePanel::create({0, 0});
}