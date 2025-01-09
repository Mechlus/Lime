#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Vector2D.h"
#include "Texture.h"
#include <string>
#include <vector>

class Text2D
{
	irr::gui::IGUIStaticText* text;

public:
	Text2D() : Text2D("Text", Vector2D(0,0), Vector2D(64, 16)) {}

	Text2D(std::string tx) : Text2D(tx, Vector2D(0, 0), Vector2D(64, 16)) {}

	Text2D(std::string tx, const Vector2D& pos) : Text2D(tx, pos, Vector2D(64,16)) {}

	Text2D(std::string tx, const Vector2D& pos, const Vector2D& dimensions) {
		text = guienv->addStaticText(charToWchar(tx.c_str()), irr::core::recti(irr::core::vector2di(pos.x, pos.y), irr::core::vector2di(pos.x + dimensions.x, pos.y + dimensions.y)));
	}

	wchar_t* charToWchar(const char* str) {
		irr::core::stringw wideStr = irr::core::stringw(str);
		wchar_t* wcharBuffer = new wchar_t[wideStr.size() + 1];
		wcscpy(wcharBuffer, wideStr.c_str());
		return wcharBuffer;
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
		if (text)
			text->setBackgroundColor(irr::video::SColor(op, text->getBackgroundColor().getRed(), text->getBackgroundColor().getGreen(), text->getBackgroundColor().getBlue()));
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
};

void bindText2D() {
	sol::usertype<Text2D> bind_type = lua->new_usertype<Text2D>("Text2D",
		sol::constructors <>()


	);
}