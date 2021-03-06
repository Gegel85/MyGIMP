#include "PolygonSelection.hpp"

#include <iostream>

namespace Mimp {

	PolygonSelection::PolygonSelection(ToolBox &toolBox) : SelectionTool("Polygon Selection", toolBox)
	{
		this->setKeyCombination({Keys::KEY_P, true, false, false});
	}

	void PolygonSelection::onClick(Mimp::Vector2<float> pos, Mimp::MouseClick click, Mimp::Image &image)
	{
		if (click == MouseClick::MIMP_LEFT_CLICK) {
			image.selectedArea->clear();
			this->_polygon.reset();
		}
		if (click == MouseClick::MIMP_RIGHT_CLICK) {
			image.selectedArea->clear();
			this->_polygon.add(pos.to<int>());
			this->_polygon.update([&image](Vector2<int> pt) { image.selectedArea->add(pt); });
		}
	}

	tgui::ScrollablePanel::Ptr PolygonSelection::getParametersPanel()
	{
		auto panel = tgui::ScrollablePanel::create();

		std::string content;
		content += "Left click : Start a new selection. Must be used first\nwhen dealing with several images.\n";
		content += "Right click : Add the mouse position to the selection.\n";
		content += "Note : Undo/Redo don't work with this tool.\nUse Left click to clear the selection.";
		auto hint = tgui::Label::create(content);

		panel->add(hint);
		return panel;
	}

	void PolygonSelection::clear()
	{
		this->_polygon.reset();
	}
}
