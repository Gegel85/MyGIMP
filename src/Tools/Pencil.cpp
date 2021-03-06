//
// Created by Gegel85 on 07/04/2020.
//

#include <TGUI/TGUI.hpp>
#include "Pencil.hpp"
#include "../Utils.hpp"

namespace Mimp
{
	Pencil::Pencil(ToolBox &box) :
		Tool("Pencil"),
		_box(box)
	{
		this->setKeyCombination({Keys::KEY_P, false, false, false});
	}

	void Pencil::onMouseDrag(Mimp::Vector2<float> oldPos, Mimp::Vector2<float> newPos, Mimp::MouseClick click,
				 Mimp::Image &image)
	{
		if (image.getSelectedLayer().isLocked())
			return;

		auto &layer = image.getSelectedLayer();

		layer.buffer->drawLine(
			(oldPos - layer.pos).rotate(-layer.rotation, layer.getSize() / 2).to<int>(),
			(newPos - layer.pos).rotate(-layer.rotation, layer.getSize() / 2).to<int>(),
			this->_box.getSelectedColor(click),
			this->_radius,
			this->_shape
		);
	}

	void Pencil::onClick(Mimp::Vector2<float> pos, Mimp::MouseClick click, Mimp::Image &image)
	{
		if (image.getSelectedLayer().isLocked())
			return;

		image.takeFrameBufferSnapshot();
		auto &layer = image.getSelectedLayer();

		layer.buffer->drawAt((pos - layer.pos).rotate(-layer.rotation, layer.getSize() / 2).to<int>(), this->_box.getSelectedColor(click), this->_radius, this->_shape);
	}

	tgui::ScrollablePanel::Ptr Pencil::getParametersPanel()
	{
		auto panel = tgui::ScrollablePanel::create();

		panel->loadWidgetsFromFile("widgets/tools_cfg/pencil_cfg.gui");

		auto radiusSlider = panel->get<tgui::Slider>("Radius");
		auto shapeBox = panel->get<tgui::ComboBox>("Shape");
		auto radiusPreview = panel->get<tgui::TextBox>("RadiusPreview");

		radiusPreview->setText(std::to_string(this->_radius));
		radiusSlider->setValue(this->_radius);
		shapeBox->removeAllItems();
		for (int i = 0; i < NB_OF_SHAPES; i++)
			shapeBox->addItem(Utils::DrawShapeToString(static_cast<DrawShape>(i)));
		shapeBox->setSelectedItemByIndex(this->_shape);

		radiusSlider->connect("ValueChanged", [radiusPreview, this, radiusSlider]{
			this->_radius = radiusSlider->getValue();
			radiusPreview->setText(std::to_string(this->_radius));
		});
		shapeBox->connect("ItemSelected", [this, shapeBox]{
			this->_shape = static_cast<DrawShape>(shapeBox->getSelectedItemIndex());
		});
		return panel;
	}
}