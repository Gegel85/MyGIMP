//
// Created by Gegel85 on 07/04/2020.
//

#ifdef __GNUG__
#include <cxxabi.h>
#endif
#include <regex>
#include <codecvt>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <numeric>
#include <fstream>
#include "Utils.hpp"
#include "Exceptions.hpp"
#include "Enum.hpp"
#include "Network/SecuredSocket.hpp"

namespace Mimp::Utils
{
	static const std::map<std::string, std::string> _icons{
		{"folder", "icons/folder.png"     },
		{".rar",   "icons/archive.png"    },
		{".zip",   "icons/archive.png"    },
		{".7z",    "icons/archive.png"    },
		{".tgz",   "icons/archive.png"    },
		{".gz",    "icons/archive.png"    },
		{".xz",    "icons/archive.png"    },
		{".htm",   "icons/webFile.png"    },
		{".html",  "icons/webFile.png"    },
		{".css",   "icons/webFile.png"    },
		{".mimp",  "icons/mimpFile.png"   },
		{".iso",   "icons/discFile.png"   },
		{".txt",   "icons/textFile.png"   },
		{".doc",   "icons/textFile.png"   },
		{".mid",   "icons/midiFile.png"   },
		{".midi",  "icons/midiFile.png"   },
		{".class", "icons/javaFile.png"   },
		{".jar",   "icons/javaFile.png"   },
		{".ttf",   "icons/fontsFile.png"  },
		{".otf",   "icons/fontsFile.png"  },
		{".wav",   "icons/musicFile.png"  },
		{".ogg",   "icons/musicFile.png"  },
		{".mp3",   "icons/musicFile.png"  },
		{".png",   "icons/imageFile.png"  },
		{".jpg",   "icons/imageFile.png"  },
		{".jpeg",  "icons/imageFile.png"  },
		{".bmp",   "icons/imageFile.png"  },
		{".gif",   "icons/imageFile.png"  },
		{".exe",   "icons/binaryFile.png" },
		{".o",     "icons/binaryFile.png" },
		{".out",   "icons/binaryFile.png" },
		{".a",     "icons/binaryFile.png" },
		{".gch",   "icons/binaryFile.png" },
		{".dll",   "icons/binaryFile.png" },
		{".so",    "icons/binaryFile.png" },
		{".xml",   "icons/configFile.png" },
		{".ini",   "icons/configFile.png" },
		{".json",  "icons/configFile.png" },
		{".md",    "icons/configFile.png" },
		{".c",     "icons/sourceFile.png" },
		{".h",     "icons/sourceFile.png" },
		{".hpp",   "icons/sourceFile.png" },
		{".cpp",   "icons/sourceFile.png" },
		{".lua",   "icons/sourceFile.png" },
		{".java",  "icons/sourceFile.png" },
		{".py",    "icons/sourceFile.png" },
		{".asm",   "icons/sourceFile.png" },
		{".php",   "icons/sourceFile.png" },
		{".js",    "icons/sourceFile.png" },
		{".sh",    "icons/shellScript.png"},
		{".run",   "icons/shellScript.png"},
		{".bat",   "icons/shellScript.png"},
		{"",       "icons/unknownFile.png"},
	};

	std::wstring utf8ToWstring(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
		return myconv.from_bytes(str);
	}

	std::string wstringToUtf8(const std::wstring& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
		return myconv.to_bytes(str);
	}

	std::string pathToString(const std::filesystem::path &path) {
#ifdef _GLIBCXX_FILESYSTEM_IS_WINDOWS
		std::string result;
		std::wstring p = path;

		result.reserve(p.size());
		for (wchar_t c : p)
			if (c > 255)
				result.push_back('?');
			else
				result.push_back(c);
		return result;
#else
		return path;
#endif
	}

	std::string getLastExceptionName()
	{
#ifdef __GNUG__
		int status;
		char *value;
		std::string name;

		auto val = abi::__cxa_current_exception_type();

		if (!val)
			return "No exception";

		value = abi::__cxa_demangle(val->name(), nullptr, nullptr, &status);
		name = value;
		free(value);
		return name;
#else
		return "Unknown exception";
#endif
	}

	int	dispMsg(const std::string &title, const std::string &content, int variate)
	{
		auto button = tgui::Button::create("OK");
		auto text = tgui::TextBox::create();
		tgui::Gui gui;
		auto font = tgui::getGlobalFont();
		const auto startWidth = button->getSize().x + 102;
		unsigned width = startWidth;
		unsigned height = button->getSize().y + 60;
		float currentWidth = startWidth;
		auto size = text->getTextSize();

		std::cerr << title << std::endl << content << std::endl;
		for (char c : content) {
			currentWidth += font.getGlyph(c, size, false).advance;
			width = std::max(static_cast<unsigned>(currentWidth), width);
			if (c == '\n' || c == '\r')
				currentWidth = startWidth;
			if (c == '\n' || c == '\v')
				height += size;
			if (currentWidth >= 700) {
				currentWidth = startWidth;
				height += size;
			}
		}

		sf::RenderWindow win{{std::min(700U, width), std::min(220U, height)}, title, sf::Style::Titlebar | sf::Style::Close};
		auto pic = tgui::Picture::create("icons/error.png");
		sf::Event event;

		gui.setTarget(win);
		gui.add(button, "ok");
		gui.add(text);

		button->setPosition("&.w - w - 10", "&.h - h - 10");
		button->connect("Pressed", [&win]{
			win.close();
		});

		text->setText(content);
		text->setPosition(52, 10);
		text->setSize("ok.x - 62", "ok.y - 20");
		text->setReadOnly();
		text->getRenderer()->setBorderColor("transparent");
		text->getRenderer()->setBackgroundColor("transparent");

		pic->setPosition(10, 10);
		pic->setSize(32, 32);

		if (variate & MB_ICONERROR)
			gui.add(pic);

		while (win.isOpen()) {
			while (win.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					win.close();
				gui.handleEvent(event);
			}

			win.clear({230, 230, 230, 255});
			gui.draw();
			win.display();
		}

		return 0;
	}

	static void _makeFolders(
		std::filesystem::path &currentPath,
		const tgui::ScrollablePanel::Ptr &panel,
		const tgui::EditBox::Ptr &file,
		const std::function<void()> &open,
		const std::regex &pattern = std::regex(".*", std::regex_constants::icase)
	) {
		auto pos = 10;
		std::vector<std::filesystem::directory_entry> paths = {
			std::filesystem::directory_entry("."),
			std::filesystem::directory_entry("..")
		};

		for (auto &entry : std::filesystem::directory_iterator(currentPath))
			paths.push_back(entry);

		panel->removeAllWidgets();
		for (auto &entry : paths) {
			std::string pic;
			const auto &filePath = entry.path();
			auto fileStr = pathToString(filePath.filename());

			if (entry.is_directory())
				pic = _icons.at("folder");
			else if (!std::regex_search(pathToString(filePath), pattern))
				continue;
			else
				try {
					pic = _icons.at(pathToString(filePath.extension()));
				} catch (std::out_of_range &) {
					pic = _icons.at("");
				}

			auto button = tgui::Button::create("");
			auto picture = tgui::Picture::create(pic);
			auto label = tgui::Label::create(fileStr);
			auto renderer = button->getRenderer();

			button->setPosition(10, pos);
			button->setSize({"&.w - 40", 20});
			button->connect("Clicked", [button, file, fileStr, &open]{
				if (file->getText() == fileStr)
					open();
				else
					file->setText(fileStr);
			});
			renderer->setBackgroundColor("transparent");
			renderer->setBackgroundColorFocused("blue");
			renderer->setBackgroundColorHover("#00BFFF");
			renderer->setBorderColor("transparent");
			renderer->setBorderColorHover("#00BFFF");
			label->setPosition(40, pos + 2);
			label->ignoreMouseEvents();
			picture->setPosition(12, pos + 2);
			picture->ignoreMouseEvents();
			panel->add(button);
			panel->add(label);
			panel->add(picture);
			pos += 30;
		}

		auto label = tgui::Label::create();

		label->setPosition(40, pos - 11);
		label->setSize(10, 9);
		panel->add(label);

		panel->setVerticalScrollAmount(30);
		panel->setVerticalScrollbarValue(0);
	}

	std::string cleanPath(const std::string &path)
	{
		std::vector<std::string> files = {};
		std::istringstream iss{path};
		std::string item;

		while (std::getline(iss, item, static_cast<char>(std::filesystem::path::preferred_separator)))
			if (item == "..") {
				if (files.size() > 1)
					files.pop_back();
			} else if (item != "." && !item.empty())
				files.push_back(item);

		return std::accumulate(
			files.begin() + 1,
			files.end(),
#ifdef _WIN32
			files[0],
#else
			static_cast<char>(std::filesystem::path::preferred_separator) + files[0],
#endif
			[](const std::string &a, const std::string &b){
				return a + static_cast<char>(std::filesystem::path::preferred_separator) + b;
			}
		);
	}

	std::string openFileDialog(const std::string &title, const std::string &basePath, const std::vector<std::pair<std::string, std::string>> &patterns, bool overWriteWarning, bool mustExist)
	{
		sf::RenderWindow window{{500, 300}, title, sf::Style::Titlebar};
		tgui::Gui gui{window};
		std::string result;
		std::string startText;
		std::filesystem::path currentPath = basePath;
		sf::Event event;

		currentPath = std::filesystem::absolute(currentPath);
		while (!std::filesystem::is_directory(currentPath)) {
			if (!startText.empty())
				startText += std::filesystem::path::preferred_separator;
			startText += pathToString(currentPath.filename());
			currentPath = currentPath.parent_path();
		}

		gui.loadWidgetsFromFile("widgets/open_file_dialog.gui");

		auto path = gui.get<tgui::TextBox>("Path");
		auto file = gui.get<tgui::EditBox>("file");
		auto box = gui.get<tgui::ComboBox>("Patterns");
		auto panel = gui.get<tgui::ScrollablePanel>("Folders");
		std::function<void()> open = [&gui, &result, &window, path, box, file, &currentPath, panel, &open, mustExist, overWriteWarning]{
			if (file->getText().isEmpty())
				return;

			std::string ext = box->getSelectedItemId();

			if (std::filesystem::path(file->getText()).is_relative())
				result = path->getText() + std::filesystem::path::preferred_separator + file->getText();
			else
				result = file->getText();

			if (std::filesystem::is_directory(result)) {
				result = cleanPath(result);
				currentPath = result + static_cast<char>(std::filesystem::path::preferred_separator);
				path->setText(pathToString(currentPath));
				file->setText("");
				_makeFolders(currentPath, panel, file, open, std::regex(box->getSelectedItemId().toAnsiString(), std::regex_constants::icase));
				return;
			}

			if (mustExist && !std::filesystem::exists(result))
				return;

			if (overWriteWarning && std::filesystem::exists(result)) {
				auto pan = tgui::Panel::create({"100%", "100%"});
				pan->getRenderer()->setBackgroundColor({0, 0, 0, 175});
				gui.add(pan);

				auto win = tgui::ChildWindow::create();
				win->setPosition("(&.w - w) / 2", "(&.h - h) / 2");
				gui.add(win);

				win->setFocused(true);

				const bool tabUsageEnabled = gui.isTabKeyUsageEnabled();
				auto closeWindow = [&gui, win, pan, tabUsageEnabled]{
					gui.remove(win);
					gui.remove(pan);
					gui.setTabKeyUsageEnabled(tabUsageEnabled);
				};

				pan->connect("Clicked", closeWindow);
				win->connect({"Closed", "EscapeKeyPressed"}, closeWindow);
				win->loadWidgetsFromFile("widgets/overwrite_warning.gui");

				auto label = win->get<tgui::Label>("Label");

				label->setText(result + label->getText());
				win->setSize(label->getSize().x + 20, 100);
				win->get<tgui::Button>("Yes")->connect("Clicked", [&window]{
					window.close();
				});
				win->get<tgui::Button>("No")->connect("Clicked", [win]{
					win->close();
				});
				return;
			}

			window.close();
		};

		gui.get<tgui::Button>("Cancel")->connect("Clicked", [&result, &window]{
			result = "";
			window.close();
		});
		gui.get<tgui::Button>("Open")->connect("Clicked", open);

		if (overWriteWarning)
			gui.get<tgui::Button>("Open")->setText("Save");

		for (auto &pair : patterns)
			box->addItem(pair.second, pair.first);
		box->addItem("All files", ".*");
		box->setSelectedItemByIndex(0);
		box->connect("ItemSelected", [&currentPath, &panel, &file, &box, &open]{
			_makeFolders(currentPath, panel, file, open, std::regex(box->getSelectedItemId().toAnsiString(), std::regex_constants::icase));
		});

		path->setText(pathToString(currentPath));
		file->setText(startText);
		_makeFolders(currentPath, panel, file, open, std::regex(box->getSelectedItemId().toAnsiString(), std::regex_constants::icase));

		while (window.isOpen()) {
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					result = "";
					window.close();
				} else if (event.type == sf::Event::Resized) {
					window.setView(sf::View{sf::FloatRect(0, 0, event.size.width, event.size.height)});
					gui.setView(sf::View{sf::FloatRect(0, 0, event.size.width, event.size.height)});
				}
				gui.handleEvent(event);
			}
			window.clear({200, 200, 200});
			gui.draw();
			window.display();
		}
		return result;
	}

	std::string saveFileDialog(const std::string &title, const std::string &basePath, const std::vector<std::pair<std::string, std::string>> &patterns)
	{
		return openFileDialog(title, basePath, patterns, true, false);
	}

	tgui::ChildWindow::Ptr openWindowWithFocus(tgui::Gui &gui, tgui::Layout width, tgui::Layout height)
	{
		auto panel = tgui::Panel::create({"100%", "100%"});

		panel->getRenderer()->setBackgroundColor({0, 0, 0, 175});
		gui.add(panel);

		auto window = tgui::ChildWindow::create();
		window->setSize(width, height);
		window->setPosition("(&.w - w) / 2", "(&.h - h) / 2");
		gui.add(window);

		window->setFocused(true);

		const bool tabUsageEnabled = gui.isTabKeyUsageEnabled();
		auto closeWindow = [&gui, window, panel, tabUsageEnabled]{
			gui.remove(window);
			gui.remove(panel);
			gui.setTabKeyUsageEnabled(tabUsageEnabled);
		};

		panel->connect("Clicked", closeWindow);
		window->connect({"Closed", "EscapeKeyPressed"}, closeWindow);
		return window;
	}

	Color HSLtoRGB(const HSLColor &color)
	{
		struct {
			double h;
			double s;
			double l;
		} temp{
			color.h * 360 / 240.,
			color.s / 240.,
			color.l / 240.,
		};
		double C = (1 - std::abs(2 * temp.l - 1)) * temp.s;
		double Tp = temp.h / 60;
		double m = temp.l - C / 2;
		double X = C * (1 - std::abs(std::fmod(Tp, 2) - 1));
		struct {
			double r;
			double g;
			double b;
		} colorp;

		if (temp.h == 0)
			colorp = {0, 0, 0};
		else if (Tp < 1)
			colorp = {C, X, 0};
		else if (Tp < 2)
			colorp = {X, C, 0};
		else if (Tp < 3)
			colorp = {0, C, X};
		else if (Tp < 4)
			colorp = {0, X, C};
		else if (Tp < 5)
			colorp = {X, 0, C};
		else if (Tp < 6)
			colorp = {C, 0, X};

		return {
			static_cast<unsigned char>((colorp.r + m) * 255),
			static_cast<unsigned char>((colorp.g + m) * 255),
			static_cast<unsigned char>((colorp.b + m) * 255),
			255
		};
	}

	HSLColor RGBtoHSL(const Color &color)
	{
		struct {
			double r;
			double g;
			double b;
		} colorp{color.r / 255., color.g / 255., color.b / 255.};
		double M = std::max(colorp.r, std::max(colorp.g, colorp.b));
		double m = std::min(colorp.r, std::min(colorp.g, colorp.b));
		double C = M - m;
		double Tp = 0;

		if (C == 0)
			Tp = 0;
		else if (M == colorp.r)
			Tp = std::fmod((colorp.g - colorp.b) / C, 6);
		else if (M == colorp.g)
			Tp = std::fmod((colorp.b - colorp.r) / C + 2, 6);
		else if (M == colorp.b)
			Tp = std::fmod((colorp.r - colorp.g) / C + 4, 6);

		double l = (M + m) / 2;

		return HSLColor{
			static_cast<unsigned char>(60. * Tp * 240 / 360),
			static_cast<unsigned char>(l == 1 ? 0 : C / (1 - std::abs(2 * l - 1)) * 240),
			static_cast<unsigned char>(l * 240)
		};
	}

	tgui::ChildWindow::Ptr makeColorPickWindow(tgui::Gui &gui, const std::function<void(Color color)> &onFinish, Color startColor)
	{
		char buffer[8];
		auto window = openWindowWithFocus(gui, 490, 370);

		window->loadWidgetsFromFile("widgets/color.gui");
		window->setTitle("Pick color");

		auto light = window->get<tgui::Slider>("Lightness");
		auto satHuePic = window->get<tgui::Picture>("SatHue");
		auto cross = window->get<tgui::Picture>("Cross");
		auto preview = window->get<tgui::TextBox>("Preview");
		auto hue = window->get<tgui::EditBox>("Hue");
		auto saturation = window->get<tgui::EditBox>("Sat");
		auto lightness = window->get<tgui::EditBox>("Light");
		auto edit = window->get<tgui::EditBox>("Edit");
		auto buttonCallBack = [edit, satHuePic, preview, light, hue, saturation, lightness, cross](tgui::Button::Ptr button){
			char buffer[8];
			auto c = button->getRenderer()->getBackgroundColor();
			Color temp = {
				c.getRed(),
				c.getGreen(),
				c.getBlue()
			};
			HSLColor color = RGBtoHSL(temp);
			auto pos = satHuePic->getPosition();
			auto size = cross->getSize();

			sprintf(buffer, "#%02X%02X%02X", temp.r, temp.g, temp.b);
			light->setValue(color.l);
			hue->setText(std::to_string(color.h));
			saturation->setText(std::to_string(color.s));
			lightness->setText(std::to_string(color.l));
			cross->setPosition({
				(color.h * 200 / 240) + pos.x - size.x / 2,
				((240 - color.s) * 200 / 240) + pos.y - size.y / 2
			});
			edit->setText(buffer);
			preview->getRenderer()->setBackgroundColor(c);
		};
		auto sliderCallback = [cross, light, lightness, satHuePic, hue, saturation, preview, edit]{
			char buffer[8];
			unsigned char h = (cross->getSize().x / 2 + cross->getPosition().x - satHuePic->getPosition().x) * 240 / 200;
			unsigned char s = 240 - (cross->getSize().y / 2 + cross->getPosition().y - satHuePic->getPosition().y) * 240 / 200;
			HSLColor color = {
				h,
				s,
				static_cast<unsigned char>(light->getValue())
			};
			Color bufferColor = HSLtoRGB(color);

			sprintf(buffer, "#%02X%02X%02X", bufferColor.r, bufferColor.g, bufferColor.b);
			preview->getRenderer()->setBackgroundColor({bufferColor.r, bufferColor.g, bufferColor.b, 255});
			edit->setText(buffer);
			light->setValue(color.l);
			lightness->setText(std::to_string(color.l));
		};

		for (auto &id : window->getWidgetNames())
			if (id.substring(0, strlen("Button")) == "Button") {
				auto button = window->get<tgui::Button>(id);

				button->connect("Clicked", buttonCallBack, button);
			}
		light->connect("ValueChanged", sliderCallback);
		edit->onReturnKeyPress.connect( [cross, hue, saturation, light, lightness, satHuePic, edit]{
			std::string text = edit->getText();

			if (text.size() > 7) {
				edit->setText(text.substr(0, 7));
				return;
			} else if (text.size() != 7)
				return;

			tgui::Color color{edit->getText()};
			Color bufferColor{
				color.getRed(),
				color.getGreen(),
				color.getBlue()
			};
			auto tmp = RGBtoHSL(bufferColor);
			auto pos = satHuePic->getPosition();
			auto size = cross->getSize();

			light->setValue(tmp.l);
			hue->setText(std::to_string(tmp.h));
			saturation->setText(std::to_string(tmp.s));
			lightness->setText(std::to_string(tmp.l));
			cross->setPosition({
				(tmp.h * 200 / 240.f) + pos.x - size.x / 2,
				((240 - tmp.s) * 200 / 240.f) + pos.y - size.y / 2
			});
		});
		hue->onReturnKeyPress.connect([cross, satHuePic, light, hue, saturation, lightness, edit, preview]{
			unsigned char h;

			try {
				auto nbr = std::stoul(hue->getText().toAnsiString());

				if (nbr > UINT8_MAX)
					return;
				h = nbr;
			} catch (...) { return; }

			char buffer[8];
			unsigned char s = 240 - (cross->getSize().y / 2 + cross->getPosition().y - satHuePic->getPosition().y) * 240 / 200;
			HSLColor color = {
				h,
				s,
				static_cast<unsigned char>(light->getValue())
			};
			Color temp = HSLtoRGB(color);
			auto pos = satHuePic->getPosition();
			auto size = cross->getSize();

			sprintf(buffer, "#%02X%02X%02X", temp.r, temp.g, temp.b);
			cross->setPosition({
				(h * 200 / 240) + pos.x - size.x / 2,
				cross->getPosition().y
			});
			edit->setText(buffer);
			preview->getRenderer()->setBackgroundColor({buffer});
		});
		saturation->onReturnKeyPress.connect([cross, satHuePic, light, hue, saturation, lightness, edit, preview]{
			unsigned char s;

			try {
				auto nbr = std::stoul(saturation->getText().toAnsiString());

				if (nbr > UINT8_MAX)
					return;
				s = nbr;
			} catch (...) { return; }

			char buffer[8];
			unsigned char h = (cross->getSize().x / 2 + cross->getPosition().x - satHuePic->getPosition().x) * 240 / 200;
			HSLColor color = {
				h,
				s,
				static_cast<unsigned char>(light->getValue())
			};
			Color temp = HSLtoRGB(color);
			auto pos = satHuePic->getPosition();
			auto size = cross->getSize();

			sprintf(buffer, "#%02X%02X%02X", temp.r, temp.g, temp.b);
			cross->setPosition({
				(color.h * 200 / 240) + pos.x - size.x / 2,
				((240 - color.s) * 200 / 240) + pos.y - size.y / 2
			});
			edit->setText(buffer);
			preview->getRenderer()->setBackgroundColor({buffer});
		});
		lightness->onReturnKeyPress.connect([cross, satHuePic, light, hue, saturation, lightness, edit, preview]{
			unsigned char l;

			try {
				auto nbr = std::stoul(saturation->getText().toAnsiString());

				if (nbr > UINT8_MAX)
					return;
				l = nbr;
			} catch (...) { return; }

			char buffer[8];
			unsigned char h = (cross->getSize().x / 2 + cross->getPosition().x - satHuePic->getPosition().x) * 240 / 200;
			unsigned char s = 240 - (cross->getSize().y / 2 + cross->getPosition().y - satHuePic->getPosition().y) * 240 / 200;
			HSLColor color = {
				h,
				s,
				l
			};
			Color temp = HSLtoRGB(color);

			sprintf(buffer, "#%02X%02X%02X", temp.r, temp.g, temp.b);
			light->setValue(l);
			edit->setText(buffer);
			preview->getRenderer()->setBackgroundColor({buffer});
		});

		HSLColor color = RGBtoHSL(startColor);
		auto pos = satHuePic->getPosition();
		auto size = cross->getSize();

		sprintf(buffer, "#%02X%02X%02X", startColor.r, startColor.g, startColor.b);
		light->setValue(color.l);
		hue->setText(std::to_string(color.h));
		saturation->setText(std::to_string(color.s));
		lightness->setText(std::to_string(color.l));
		cross->setPosition({
			(color.h * 200 / 240) + pos.x - size.x / 2,
			((240 - color.s) * 200 / 240) + pos.y - size.y / 2
		});
		edit->setText(buffer);
		preview->getRenderer()->setBackgroundColor({buffer});

		cross->ignoreMouseEvents();

		auto colorPickHandler = [cross, satHuePic, light, hue, saturation, edit, preview](tgui::Vector2f pos){
			char buffer[8];
			unsigned char h = pos.x * 240 / 200;
			unsigned char s = 240 - pos.y * 240 / 200;
			HSLColor color = {
				h,
				s,
				static_cast<unsigned char>(light->getValue())
			};
			Color temp = HSLtoRGB(color);
			auto size = cross->getSize();

			sprintf(buffer, "#%02X%02X%02X", temp.r, temp.g, temp.b);
			hue->setText(std::to_string(color.h));
			saturation->setText(std::to_string(color.s));
			cross->setPosition({
				(color.h * 200 / 240) + satHuePic->getPosition().x - size.x / 2,
				((240 - color.s) * 200 / 240) + satHuePic->getPosition().y - size.y / 2
			});
			edit->setText(buffer);
			preview->getRenderer()->setBackgroundColor({buffer});
		};

		satHuePic->onMousePress.connect(colorPickHandler);
		satHuePic->onMouseRelease.connect(colorPickHandler);

		window->get<tgui::Button>("Cancel")->connect("Clicked", [window]{
			window->close();
		});
		window->get<tgui::Button>("OK")->connect("Clicked", [onFinish, edit, window]{
			tgui::Color color{edit->getText()};
			Color bufferColor{
				color.getRed(),
				color.getGreen(),
				color.getBlue(),
				255
			};

			if (onFinish)
				onFinish(bufferColor);
			window->close();
		});
		return window;
	}

	tgui::ChildWindow::Ptr makeSliderWindow(tgui::Gui &gui, const std::function<void(float value)> &onFinish, float defaultValue, float min, float max, float step)
	{
		auto window = openWindowWithFocus(gui, 260, 80);
		window->loadWidgetsFromFile("widgets/slider.gui");

		auto slider = window->get<tgui::Slider>("Slider");
		auto txt = window->get<tgui::Label>("Nb");
		auto sliderCallback = [slider, txt]{
			std::stringstream s;

			s << slider->getValue();
			txt->setText(s.str());
		};
		std::stringstream s;

		slider->setMaximum(min);
		slider->setMaximum(max);
		slider->setStep(step);
		slider->setValue(defaultValue);
		slider->connect("ValueChanged", sliderCallback);

		s << slider->getValue();
		txt->setText(s.str());

		window->get<tgui::Button>("Cancel")->connect("Clicked", [window]{
			window->close();
		});
		window->get<tgui::Button>("Ok")->connect("Clicked", [onFinish, slider, window]{
			if (onFinish)
				onFinish(slider->getValue());
			window->close();
		});
		return window;
	}

	std::string DrawShapeToString(DrawShape shape)
	{
		switch (shape) {
		case CIRCLE:
			return "Circle";
		case SQUARE:
			return "Square";
		case DIAMOND:
			return "Diamond";
		case NB_OF_SHAPES:
			throw InvalidDrawShapeException("NB_OF_SHAPES is not a DrawShape");
		}
		throw InvalidDrawShapeException("Invalid DrawShape");
	}

	DrawShape DrawShapeFromString(const std::string &str)
	{
		if (str == "Circle") {
			return CIRCLE;
		} else if (str == "Square") {
			return SQUARE;
		} else if (str == "Diamond") {
			return DIAMOND;
		}
		throw InvalidDrawShapeException(str + " is not a valid DrawShape");
	}

	static std::string handleHttpRequest(Socket &socket, const std::string &url, unsigned short port, unsigned recurseLimit)
	{
		auto pos = url.find_first_of('/');
		std::string host = pos == std::string::npos ? url : url.substr(0, pos);
		Socket::HttpRequest request{
			"",
			"GET",
			host,
			port,
			{},
			pos == std::string::npos ? "/" : url.substr(host.size())
		};
		auto response = socket.makeHttpRequest(request);

		if (response.returnCode / 100 == 3)
			try {
				return resolveUrl(response.header["Location"], recurseLimit - 1);
			} catch (TooMuchRecursionException &e) {
				throw TooMuchRecursionException(e, url + ": " + std::to_string(response.returnCode) + " (" + response.codeName + ")");
			}

		return response.body;
	}

	static std::string fileProtocol(const std::string &path, unsigned)
	{
		std::ifstream stream{path, std::ifstream::binary};

		if (stream.fail())
			throw FileNotFoundException(path);

		std::string content;

		stream.seekg(0, std::ios::end);
		content.reserve(stream.tellg());
		stream.seekg(0, std::ios::beg);
		content.assign(std::istreambuf_iterator<char>{stream}, {});
		return content;
	}

	static std::string httpProtocol(const std::string &path, unsigned recurseLimit)
	{
		Socket socket;

		return handleHttpRequest(socket, path, 80, recurseLimit);
	}

	static std::string httpsProtocol(const std::string &path, unsigned recurseLimit)
	{
		SecuredSocket socket;

		return handleHttpRequest(socket, path, 443, recurseLimit);
	}

	static const std::map<std::string, std::function<std::string (const std::string &path, unsigned)>> _protocols{
		{"file", fileProtocol},
		{"http", httpProtocol},
		{"https", httpsProtocol}
	};

	std::string resolveUrl(const std::string &url, unsigned recurseLimit)
	{
		std::cout << "Resolve URL " << url << std::endl;
		if (recurseLimit == 0)
			throw TooMuchRecursionException(url + ": Recurse limit is 0");

		std::string protocol;
		auto pos = url.find_first_of("://");

		if (pos == std::string::npos)
			protocol = "http";
		else
			protocol = url.substr(0, pos);

		std::function<std::string (const std::string &, unsigned)> fct;

		try {
			fct = _protocols.at(protocol);
		} catch (std::out_of_range &) {
			throw UnsupportedProtocolException(protocol + ":// is not a supported protocol");
		}
		return fct(pos == std::string::npos ? url : url.substr(pos + 3), recurseLimit);
	}

	bool isOutOfBound(Mimp::Vector2<int> pt, Mimp::Vector2<unsigned> size)
	{
		if (pt.x < 0)
			return true;
		if (pt.y < 0)
			return true;
		if (static_cast<unsigned>(pt.x) >= size.x)
			return true;
		if (static_cast<unsigned>(pt.y) >= size.y)
			return true;
		return false;
	}
}
