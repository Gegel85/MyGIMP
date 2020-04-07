//
// Created by Gegel85 on 07/04/2020.
//

#ifndef MYGIMP_PENCIL_HPP
#define MYGIMP_PENCIL_HPP


#include "Tool.hpp"
#include "../ToolBox.hpp"

namespace Mimp
{
	class Pencil : public Tool {
	private:
		ToolBox &_box;

	public:
		Pencil(ToolBox &);
		void onMouseDrag(Vector2<int> pos, MouseClick click, Layer &layer) const override;
		void onClick(Vector2<int> pos, MouseClick click, Layer &layer) const override;
		tgui::ScrollablePanel::Ptr getParametersPanel() const override;
	};
}


#endif //MYGIMP_PENCIL_HPP