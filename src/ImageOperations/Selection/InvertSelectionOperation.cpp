/*
** EPITECH PROJECT, 2020
** MyGimp
** File description:
** InvertSelectionOperation.cpp
*/
#include "InvertSelectionOperation.hpp"
#include "../../Widgets/CanvasWidget.hpp"

Mimp::InvertSelectionOperation::InvertSelectionOperation():
	ImageOperation({"Selection", "Invert Selection"}, {Keys::KEY_I, true, false, false})
{}

void Mimp::InvertSelectionOperation::click(tgui::Gui &, CanvasWidget::Ptr image, tgui::ChildWindow::Ptr, Editor &) const
{
	image->takeSelectionSnapshot();
	image->selectedArea->invert();
}
