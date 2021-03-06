#include <TGUI/TGUI.hpp>
#include "Symmetry.hpp"

namespace Mimp {

	Symmetry::Symmetry(std::shared_ptr<tgui::ChildWindow> window) : Effect(window)
	{
		auto checkboxX = window->get<tgui::CheckBox>("SymmetryXCheck");
		auto checkboxY = window->get<tgui::CheckBox>("SymmetryYCheck");

		auto sliderX = window->get<tgui::Slider>("SymmetryXSlider");
		auto sliderY = window->get<tgui::Slider>("SymmetryYSlider");

		auto textX = window->get<tgui::TextBox>("SymmetryXPreview");
		auto textY = window->get<tgui::TextBox>("SymmetryYPreview");

		auto centralCheck =  window->get<tgui::CheckBox>("CentralSymmetryCheck");

		auto checkboxCallback = [this](tgui::CheckBox::Ptr box, tgui::Slider::Ptr slider, bool b) {
			bool isChecked = box->isChecked();

			slider->setEnabled(isChecked);
			b ? this->_symmetry.y = isChecked : this->_symmetry.x = isChecked;
			this->_image->setSymmetry(this->_symmetry);
		};

		checkboxX->connect("Changed", checkboxCallback, checkboxX, sliderX, 0);
		checkboxY->connect("Changed", checkboxCallback, checkboxY, sliderY, 1);

		centralCheck->connect("Changed", [this, sliderX, sliderY, centralCheck]() {
			bool isChecked = centralCheck->isChecked();

			sliderX->setEnabled(isChecked);
			sliderY->setEnabled(isChecked);
			this->_centralSymmetry = isChecked;
			this->_image->setCentralSymmetry(isChecked);
		});

		auto sliderCallback = [this](tgui::Slider::Ptr slider, tgui::TextBox::Ptr box, bool b) {
			slider->setMaximum(b ? this->_image->getSize().x : this->_image->getSize().y);
			box->setText(std::to_string(slider->getValue()));

			b ? this->_axis.x = slider->getValue() : this->_axis.y = slider->getValue();
			this->_image->setSymmetryAxis(this->_axis);
		};

		sliderX->connect("ValueChanged", sliderCallback, sliderX, textX, 0);
		sliderY->connect("ValueChanged", sliderCallback, sliderY, textY, 1);
	}

	void Symmetry::setImage(CanvasWidget::Ptr &image)
	{
		image->setSymmetry(this->_symmetry);
		image->setSymmetryAxis(this->_axis);
		image->setCentralSymmetry(this->_centralSymmetry);
		Effect::setImage(image);
	}
}