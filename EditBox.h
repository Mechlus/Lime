#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Vector2D.h"
#include <string>
#include <vector>
#include "Image2D.h"

class EditBox
{
	irr::gui::IGUIEditBox* box;
	bool border;

public:
	EditBox() : EditBox("Edit Box", Vector2D(0, 0), Vector2D(256, 16)) {}

	EditBox(std::string tx) : EditBox(tx, Vector2D(0, 0), Vector2D(256, 16)) {}

	EditBox(std::string tx, const Vector2D& pos) : EditBox(tx, pos, Vector2D(256, 16)) {}

	EditBox(std::string tx, const Vector2D& pos, const Vector2D& dimensions) {
		box = guienv->addEditBox(charToWchar(tx.c_str()), irr::core::recti(irr::core::vector2di(pos.x, pos.y), irr::core::vector2di(pos.x + dimensions.x, pos.y + dimensions.y)));
		border = false;
	}

	EditBox(const EditBox& other) {
		box = other.box;
	}

	wchar_t* charToWchar(const char* str) {
		irr::core::stringw wideStr = irr::core::stringw(str);
		return const_cast<wchar_t*>(wideStr.c_str());
	}

	std::string getText() {
		if (box) {
			irr::core::stringw wideStr(box->getText());
			irr::core::stringc narrowStr(wideStr);
			return narrowStr.c_str();
		}
	}

	void setText(std::string tx) {
		if (box)
			box->setText(charToWchar(tx.c_str()));
	}

	void destroy() {
		if (box)
			box->remove();
	}

	bool getVisible() {
		return box ? box->isVisible() : false;
	}

	void setVisible(bool vis) {
		if (box)
			box->setVisible(vis);
	}

	void bringToFront() {
		if (box)
			box->bringToFront(box);
	}

	void sendToBack() {
		if (box)
			box->sendToBack(box);
	}

	Vector2D getSize() {
		if (box) {
			irr::core::recti r = box->getRelativePosition();
			return Vector2D(r.LowerRightCorner.X - r.UpperLeftCorner.X, r.LowerRightCorner.Y - r.UpperLeftCorner.Y);
		}
		return Vector2D();
	}

	void setSize(const Vector2D& size) {
		if (box) {
			irr::core::recti r = box->getRelativePosition();
			r.LowerRightCorner.X = r.UpperLeftCorner.X + size.x;
			r.LowerRightCorner.Y = r.UpperLeftCorner.Y + size.y;
			box->setRelativePosition(r);
		}
	}

	void setBorderAlignment(int a, int b, int c, int d) {
		if (box)
			box->setAlignment((irr::gui::EGUI_ALIGNMENT)a, (irr::gui::EGUI_ALIGNMENT)b, (irr::gui::EGUI_ALIGNMENT)c, (irr::gui::EGUI_ALIGNMENT)d);
	}

	void setTextAlignment(const Vector2D& align) {
		if (box)
			box->setTextAlignment((irr::gui::EGUI_ALIGNMENT)align.x, (irr::gui::EGUI_ALIGNMENT)align.y);
	}

	void setMaxSize(const Vector2D& max) {
		if (box)
			box->setMaxSize(irr::core::dimension2du(max.x, max.y));
	}

	bool getWrap() {
		return box ? box->isWordWrapEnabled() : false;
	}

	void setWrap(bool wrap) {
		if (box)
			box->setWordWrap(wrap);
	}

	Vector3D getTextColor() {
		return box ? Vector3D(box->getOverrideColor().getRed(), box->getOverrideColor().getGreen(), box->getOverrideColor().getBlue()) : Vector3D();
	}

	void setTextColor(const Vector3D& col) {
		if (box)
			box->setOverrideColor(irr::video::SColor(box->getOverrideColor().getAlpha(), col.x, col.y, col.z));
	}

	int getTextOpacity() {
		return box ? box->getOverrideColor().getAlpha() : 0;
	}

	void setTextOpacity(int op) {
		if (box)
			box->setOverrideColor(irr::video::SColor(op, box->getOverrideColor().getRed(), box->getOverrideColor().getGreen(), box->getOverrideColor().getBlue()));
	}

	bool setFont(const std::string& fontName) {
		if (!box)
			return false;
		if (fontCache[fontName]) {
			irr::gui::IGUIFont* f = guienv->getBuiltInFont();
			auto it = fontCache.find(fontName);
			if (it != fontCache.end()) {
				f = it->second;
				box->setOverrideFont(f);
				return true;
			}
			box->setOverrideFont(f);
			return false;
		}
		return false;
	}

	Vector2D getPosition() {
		if (box)
			return Vector2D(box->getRelativePosition().UpperLeftCorner.X, box->getRelativePosition().UpperLeftCorner.Y);
		return Vector2D();
	}

	void setPosition(const Vector2D& pos) {
		if (box)
			box->setRelativePosition(irr::core::position2di(pos.x, pos.y));
	}

	void setParent(const Image2D& other) {
		if (box)
			other.img->addChild(box);
	}

	bool getDrawBorder() {
		return box ? border : false;
	}

	void setDrawBorder(bool enable) {
		if (box) {
			box->setDrawBorder(enable);
			border = enable;
		}
	}

	bool getIsPassword() {
		return box ? box->isPasswordBox() : false;
	}

	void setIsPassword(bool enable) {
		if (box)
			box->setPasswordBox(enable);
	}

	bool getIsEnabled() {
		return box ? box->isEnabled() : false;
	}

	void setIsEnabled(bool enable) {
		if (box)
			box->setEnabled(enable);
	}

	bool getMultiLine() {
		return box ? box->isMultiLineEnabled() : false;
	}

	void setMultiLine(bool enable) {
		if (box)
			box->setMultiLine(enable);
	}

	bool getAutoScroll() {
		return box ? box->isAutoScrollEnabled() : false;
	}

	void setAutoScroll(bool enable) {
		if (box)
			box->setAutoScroll(enable);
	}
};

inline void bindEditBox() {
	sol::usertype<EditBox> bind_type = lua->new_usertype<EditBox>("EditBox",
		sol::constructors <EditBox(), EditBox(const EditBox & other), EditBox(std::string tx), EditBox(std::string tx, const Vector2D & pos), EditBox(std::string tx, const Vector2D & pos, const Vector2D & dimensions)>(),

		"position", sol::property(&EditBox::getPosition, &EditBox::setPosition),
		"visible", sol::property(&EditBox::getVisible, &EditBox::setVisible),
		"size", sol::property(&EditBox::getSize, &EditBox::setSize),
		"wrap", sol::property(&EditBox::getWrap, &EditBox::setWrap),
		"textColor", sol::property(&EditBox::getTextColor, &EditBox::setTextColor),
		"textOpacity", sol::property(&EditBox::getTextOpacity, &EditBox::setTextOpacity),
		"text", sol::property(&EditBox::getText, &EditBox::setText),
		"drawBorder", sol::property(&EditBox::getDrawBorder, &EditBox::setDrawBorder),
		"enabled", sol::property(&EditBox::getIsEnabled, &EditBox::setIsEnabled),
		"password", sol::property(&EditBox::getIsPassword, &EditBox::setIsPassword),
		"multiLine", sol::property(&EditBox::getMultiLine, &EditBox::setMultiLine),
		"autoScroll", sol::property(&EditBox::getAutoScroll, &EditBox::setAutoScroll)
	);

	bind_type["destroy"] = &EditBox::destroy;
	bind_type["setFont"] = &EditBox::setFont;
	bind_type["setMaxSize"] = &EditBox::setMaxSize;
	bind_type["toFront"] = &EditBox::bringToFront;
	bind_type["toBack"] = &EditBox::sendToBack;
	bind_type["setBorderAlignment"] = &EditBox::setBorderAlignment;
	bind_type["setTextAlignment"] = &EditBox::setTextAlignment;
	bind_type["setParent"] = &EditBox::setParent;
}