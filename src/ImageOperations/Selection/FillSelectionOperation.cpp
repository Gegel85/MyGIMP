//
// Created by Gegel85 on 25/04/2020.
//

#include <TGUI/TGUI.hpp>
#include "FillSelectionOperation.hpp"
#include "../../Utils.hpp"

namespace Mimp
{
	FillSelectionOperation::FillSelectionOperation() :
		ImageOperation({"Selection", "Fill area"}/*, {KEY_A, true, false, false}*/)
	{
	}

	void FillSelectionOperation::click(tgui::Gui &gui, Image &image) const
	{
		Utils::makeColorPickWindow(gui, [&image](Color color){
			FillSelectionOperation::_fill(image.getSelectedLayer(), image.selectedArea, color);
		}, Color::Black);
	}

	void FillSelectionOperation::_fill(Layer &layer, const SelectedArea &area, const Color &color)
	{
		if (area.isAnAreaSelected())
			for (const auto &pt : area.getPoints())
				layer.getFrameBuffer().setPixel(pt, color);
	}
}