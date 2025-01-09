#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "StaticMesh.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include <string>
#include <vector>

class Text3D
{
public:
	irr::scene::ITextSceneNode* text;
	irr::video::SColor color;
	int opacity;
	std::string myText;

	Text3D(const std::string& tx, const Vector3D& pos, const Vector3D& col, int op, const std::string& fontName) {
		irr::gui::IGUIFont* f = guienv->getBuiltInFont();
		if (defaultFont != "")
			f = fontCache[defaultFont];
		if (fontName != "") {
			f = fontCache[fontName];
		}

		text = smgr->addTextSceneNode(f, L"Text", color);

		myText = tx;
		setOpacity(op);
		setColor(col);
		setPosition(pos);
		text->grab();
	}

	Text3D() : Text3D("Text", Vector3D(), Vector3D(255,255,255), 255, "") {}
	Text3D(const std::string& tx) : Text3D(tx, Vector3D(), Vector3D(255, 255, 255), 255, "") {}
	Text3D(const std::string& tx, const Vector3D& pos) : Text3D(tx, pos, Vector3D(255, 255, 255), 255, "") {}
	Text3D(const std::string& tx, const Vector3D& pos, const Vector3D& col, int op) : Text3D(tx, pos, col, op, "") {}
	Text3D(const std::string& tx, const std::string& fontName) : Text3D(tx, Vector3D(), Vector3D(255, 255, 255), 255, fontName) {}

	/*
	~Text3D() {
		text->drop();
	}
	*/

	void destroy() {
		text->remove();
	}

	std::string getText() {
		if (text)
			return myText;
		return "";
	}

	void setText(const std::string& tx) {
		if (text) {
			myText = tx;
			std::wstring wideText(tx.begin(), tx.end());
			text->setText(wideText.c_str());
		}
	}

	void setParent(StaticMesh* parent) {
		if (text && parent && parent->meshNode) {
			text->setParent(parent->meshNode);
			return;
		}
		text->setParent(nullptr);
	}

	bool getVisibility() const {
		if (!text)
			return false;
		return text->isVisible();
	}

	void setVisibility(bool visible) {
		text->setVisible(visible);
	}

	Vector3D getPosition() {
		if (!text)
			return Vector3D();
		return Vector3D(text->getPosition().X, text->getPosition().Y, text->getPosition().Z);
	}

	void setPosition(const Vector3D& pos) {
		if (!text)
			return;
		text->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
	}

	Vector3D getColor() {
		if (text)
			return Vector3D(color.getRed(), color.getGreen(), color.getBlue());
		return Vector3D();
	}

	void setColor(const Vector3D& col) {
		if (text) {
			color = irr::video::SColor(opacity, col.x, col.y, col.z);
			updateColor();
		}
	}

	int getOpacity() {
		if (text)
			return opacity;
		return 0;
	}

	void setOpacity(int op) {
		opacity = irr::core::clamp<int>(op, 0, 255);
		if (text) {
			color = irr::video::SColor(opacity, color.getRed(), color.getGreen(), color.getBlue());
			updateColor();
		}
	}

	void updateColor() {
		text->setTextColor(color);
	}

	bool setFont(const std::string& fontName) {
		if (fontCache[fontName]) {
			// Remove previous text object, replace it fully
			irr::core::vector3df pos = text->getPosition();
			irr::scene::ISceneNode* par = text->getParent();
			bool visible = text->isVisible();
			std::wstring wideText(myText.begin(), myText.end());
			text->remove();

			bool found = false;
			irr::gui::IGUIFont* f = guienv->getBuiltInFont();
			auto it = fontCache.find(fontName);
			if (it != fontCache.end()) {
				f = it->second;
				found = true;
			}

			text = smgr->addTextSceneNode(f, wideText.c_str(), color, par, pos);
			text->grab();
			return found;
		}
		return false;
	}
};

void bindText3D() {
	sol::usertype<Text3D> bind_type = lua->new_usertype<Text3D>("Text3D",
		sol::constructors < Text3D(const std::string & tx, const std::string & fontName), Text3D(const std::string & tx, const Vector3D & pos, const Vector3D & col, int op), Text3D(const std::string & tx), Text3D(const std::string & tx, const Vector3D & pos), Text3D() > (),
		
		"position", sol::property(&Text3D::getPosition, &Text3D::setPosition),
		"visible", sol::property(&Text3D::getVisibility, &Text3D::setVisibility),
		"textColor", sol::property(&Text3D::getColor, &Text3D::setColor),
		"opacity", sol::property(&Text3D::getOpacity, &Text3D::setOpacity),
		"text", sol::property(&Text3D::getText, &Text3D::setText)
	);

	bind_type["destroy"] = &Text3D::destroy;
	bind_type["setParent"] = &Text3D::setParent;
	bind_type["setFont"] = &Text3D::setFont;
} 