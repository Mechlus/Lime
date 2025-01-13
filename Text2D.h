#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Vector2D.h"
#include "Image2D.h"
#include <string>
#include <vector>

class Text2D
{
	irr::gui::IGUIStaticText* text;

public:
	Text2D() : Text2D("Text", Vector2D(0,0), Vector2D(256, 16)) {}

	Text2D(std::string tx) : Text2D(tx, Vector2D(0, 0), Vector2D(256, 16)) {}

	Text2D(std::string tx, const Vector2D& pos) : Text2D(tx, pos, Vector2D(256,16)) {}

	Text2D(std::string tx, const Vector2D& pos, const Vector2D& dimensions) {
		text = guienv->addStaticText(charToWchar(tx.c_str()), irr::core::recti(irr::core::vector2di(pos.x, pos.y), irr::core::vector2di(pos.x + dimensions.x, pos.y + dimensions.y)));
		text->setBackgroundColor(irr::video::SColor(0, 180, 180, 180));
	}

	Text2D(const Text2D& other) {
		text = other.text;
	}

	wchar_t* charToWchar(const char* str) {
		irr::core::stringw wideStr = irr::core::stringw(str);
		return const_cast<wchar_t*>(wideStr.c_str());
	}

	std::string getText() {
		if (text) {
			irr::core::stringw wideStr(text->getText());
			irr::core::stringc narrowStr(wideStr);
			return narrowStr.c_str();
		}
		return "";
	}

	void setText(std::string tx) {
		if (text)
			text->setText(charToWchar(tx.c_str()));
	}

	void destroy() {
		if (text)
			text->remove();
	}

	bool getVisible() {
		return text ? text->isVisible() : false;
	}

	void setVisible(bool vis) {
		if (text)
			text->setVisible(vis);
	}

	void bringToFront() {
		if (text)
			text->bringToFront(text);
	}

	void sendToBack() {
		if (text)
			text->sendToBack(text);
	}

	Vector2D getSize() {
		if (text) {
			irr::core::recti r = text->getRelativePosition();
			return Vector2D(r.LowerRightCorner.X - r.UpperLeftCorner.X, r.LowerRightCorner.Y - r.UpperLeftCorner.Y);
		}
		return Vector2D();
	}

	void setSize(const Vector2D& size) {
		if (text) {
			irr::core::recti r = text->getRelativePosition();
			r.LowerRightCorner.X = r.UpperLeftCorner.X + size.x;
			r.LowerRightCorner.Y = r.UpperLeftCorner.Y + size.y;
			text->setRelativePosition(r);
		}
	}

	void setBorderAlignment(int a, int b, int c, int d) {
		if (text)
			text->setAlignment((irr::gui::EGUI_ALIGNMENT)a, (irr::gui::EGUI_ALIGNMENT)b, (irr::gui::EGUI_ALIGNMENT)c, (irr::gui::EGUI_ALIGNMENT)d);
	}

	void setTextAlignment(const Vector2D& align) {
		if (text)
			text->setTextAlignment((irr::gui::EGUI_ALIGNMENT)align.x, (irr::gui::EGUI_ALIGNMENT)align.y);
	}

	void setMaxSize(const Vector2D& max) {
		if (text)
			text->setMaxSize(irr::core::dimension2du(max.x, max.y));
	}

	bool getWrap() {
		return text ? text->isWordWrapEnabled() : false;
	}

	void setWrap(bool wrap) {
		if (text)
			text->setWordWrap(wrap);
	}

	bool getDrawBackground() {
		return text ? text->isDrawBackgroundEnabled() : false;
	}

	void setDrawBackground(bool draw) {
		if (text)
			text->setDrawBackground(draw);
	}

	Vector3D getBackgroundColor() {
		return text ? Vector3D(text->getBackgroundColor().getRed(), text->getBackgroundColor().getGreen(), text->getBackgroundColor().getBlue()) : Vector3D();
	}

	void setBackgroundColor(const Vector3D& col) {
		if (text)
			text->setBackgroundColor(irr::video::SColor(text->getBackgroundColor().getAlpha(), col.x, col.y, col.z));
	}

	int getBackgroundOpacity() {
		return text ? text->getBackgroundColor().getAlpha() : 0;
	}

	void setBackgroundOpacity(int op) {
		if (text) {
			text->setDrawBackground(op == 0 ? false : true);
			text->setBackgroundColor(irr::video::SColor(op, text->getBackgroundColor().getRed(), text->getBackgroundColor().getGreen(), text->getBackgroundColor().getBlue()));
		}
	}

	Vector3D getTextColor() {
		return text ? Vector3D(text->getOverrideColor().getRed(), text->getOverrideColor().getGreen(), text->getOverrideColor().getBlue()) : Vector3D();
	}

	void setTextColor(const Vector3D& col) {
		if (text)
			text->setOverrideColor(irr::video::SColor(text->getOverrideColor().getAlpha(), col.x, col.y, col.z));
	}

	int getTextOpacity() {
		return text ? text->getOverrideColor().getAlpha() : 0;
	}

	void setTextOpacity(int op) {
		if (text)
			text->setOverrideColor(irr::video::SColor(op, text->getOverrideColor().getRed(), text->getOverrideColor().getGreen(), text->getOverrideColor().getBlue()));
	}

	bool setFont(const std::string& fontName) {
		if (!text)
			return false;
		if (fontCache[fontName]) {
			irr::gui::IGUIFont* f = guienv->getBuiltInFont();
			auto it = fontCache.find(fontName);
			if (it != fontCache.end()) {
				f = it->second;
				text->setOverrideFont(f);
				return true;
			}
			text->setOverrideFont(f);
			return false;
		}
		return false;
	}

	Vector2D getPosition() {
		if (text)
			return Vector2D(text->getRelativePosition().UpperLeftCorner.X, text->getRelativePosition().UpperLeftCorner.Y);
		return Vector2D();
	}

	void setPosition(const Vector2D& pos) {
		if (text)
			text->setRelativePosition(irr::core::position2di(pos.x, pos.y));
	}

	void setParent(const Image2D& other) {
		if (text) {
			other.img->addChild(text);
		}
	}

	bool getDrawBorder() {
		return text ? text->isDrawBorderEnabled() : false;
	}

	void setDrawBorder(bool enable) {
		if (text)
			text->setDrawBorder(enable);
	}
};

void bindText2D() {
	sol::usertype<Text2D> bind_type = lua->new_usertype<Text2D>("Text2D",
		sol::constructors <Text2D(), Text2D(const Text2D & other), Text2D(std::string tx), Text2D(std::string tx, const Vector2D & pos), Text2D(std::string tx, const Vector2D & pos, const Vector2D & dimensions)>(),

		"position", sol::property(&Text2D::getPosition, &Text2D::setPosition),
		"visible", sol::property(&Text2D::getVisible, &Text2D::setVisible),
		"size", sol::property(&Text2D::getSize, &Text2D::setSize),
		"wrap", sol::property(&Text2D::getWrap, &Text2D::setWrap),
		"backgroundColor", sol::property(&Text2D::getBackgroundColor, &Text2D::setBackgroundColor),
		"backgroundOpacity", sol::property(&Text2D::getBackgroundOpacity, &Text2D::setBackgroundOpacity),
		"textColor", sol::property(&Text2D::getTextColor, &Text2D::setTextColor),
		"textOpacity", sol::property(&Text2D::getTextOpacity, &Text2D::setTextOpacity),
		"text", sol::property(&Text2D::getText, &Text2D::setText),
		"drawBorder", sol::property(&Text2D::getDrawBorder, &Text2D::setDrawBorder)
	);

	bind_type["destroy"] = &Text2D::destroy;
	bind_type["setFont"] = &Text2D::setFont;
	bind_type["setMaxSize"] = &Text2D::setMaxSize;
	bind_type["toFront"] = &Text2D::bringToFront;
	bind_type["toBack"] = &Text2D::sendToBack;
	bind_type["setBorderAlignment"] = &Text2D::setBorderAlignment;
	bind_type["setTextAlignment"] = &Text2D::setTextAlignment;
	bind_type["setParent"] = &Text2D::setParent;
}