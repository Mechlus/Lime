#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Vector2D.h"
#include "Texture.h"
#include <string>
#include <vector>

class Image2D
{

public:
	irr::gui::IGUIImage* img = nullptr;
	irr::gui::IGUIButton* button = nullptr;
	bool clickable = false;
	sol::function onClick;

	Image2D() {
	}

	Image2D(const Texture& tex) {
		img = guienv->addImage(tex.texture, irr::core::vector2di(0, 0));
	}

	Image2D(const Texture& tex, const Vector2D& pos) {
		img = guienv->addImage(tex.texture, irr::core::vector2di(pos.x, pos.y));
	}

	Image2D(const Texture& tex, const Vector2D& pos, const Vector2D& dimensions) : Image2D(tex, pos) {
		setSize(dimensions);
	}

	Image2D(const Image2D& other) {
		img = other.img;
	}

	bool getClickable() {
		return img ? button != nullptr : false;
	}

	bool getHovered() {
		if (!img)
			return false;

		irr::core::position2di mousePos = device->getCursorControl()->getPosition();
		irr::core::recti rect(
			img->getAbsoluteClippingRect().UpperLeftCorner,
			img->getAbsoluteClippingRect().LowerRightCorner
		);
		return rect.isPointInside(mousePos);
	}

	void setHovered(bool x) {
		// Ignore
	}

	void setClickable(sol::function f) {
		if (!img)
			return;
		if (f && !button) {
			clickable = true;
			updateButton();
			receiver->imageCallbackArray.push_back(ImageCallbackPair(button, f));
		}
		else if (!f && button) {
			receiver->removeImg(button);
			button->remove();
			clickable = false;
		}
	}

	void updateButton() {
		if (!clickable)
			return;

		irr::core::recti r = img->getRelativePosition();
		if (button)
			receiver->removeImg(button);
		button = guienv->addButton(irr::core::recti(0, 0, r.LowerRightCorner.X - r.UpperLeftCorner.X, r.LowerRightCorner.Y - r.UpperLeftCorner.Y), img);
		button->setDrawBorder(false);
		button->setUseAlphaChannel(true);
	}

	Vector3D getColor() {
		return img ? Vector3D(img->getColor().getRed(), img->getColor().getGreen(), img->getColor().getBlue()) : Vector3D();
	}

	void setColor(Vector3D& color) {
		if (img)
			img->setColor(irr::video::SColor(img->getColor().getAlpha(), color.x, color.y, color.z));
	}

	int getOpacity() {
		return img ? img->getColor().getAlpha() : 0;
	}

	void setOpacity(int o) {
		if (img)
			img->setColor(irr::video::SColor(o, img->getColor().getRed(), img->getColor().getGreen(), img->getColor().getBlue()));
	}

	bool getVisible() {
		return img ? img->isVisible() : false;
	}

	void setVisible(bool vis) {
		if (img)
			img->setVisible(vis);
	}

	Vector2D getPosition() {
		if (img)
			return Vector2D(img->getRelativePosition().UpperLeftCorner.X, img->getRelativePosition().UpperLeftCorner.Y);
		return Vector2D();
	}

	void setPosition(const Vector2D& pos) {
		if (img)
			img->setRelativePosition(irr::core::position2di(pos.x, pos.y));
	}

	Vector2D getSize() {
		if (img) {
			irr::core::recti r = img->getRelativePosition();
			return Vector2D(r.LowerRightCorner.X - r.UpperLeftCorner.X, r.LowerRightCorner.Y - r.UpperLeftCorner.Y);
		}
		return Vector2D();
	}

	void setSize(const Vector2D& size) {
		if (img) {
			irr::core::recti r = img->getRelativePosition();
			r.LowerRightCorner.X = r.UpperLeftCorner.X + size.x;
			r.LowerRightCorner.Y = r.UpperLeftCorner.Y + size.y;
			img->setRelativePosition(r);
			if (button)
				updateButton();
		}
	}

	void setImage(const Texture& tex) {
		if (img)
			img->setImage(tex.texture);
	}

	void destroy() {
		if (img)
			img->remove();
	}

	void bringToFront() {
		if (img)
			img->bringToFront(img);
	}

	void sendToBack() {
		if (img)
			img->sendToBack(img);
	}

	bool scalesToFit() {
		if (img)
			return img->isImageScaled();
		return false;
	}

	void setScalesToFit(bool scale) {
		if (img)
			img->setScaleImage(scale);
	}

	void setBorderAlignment(int a, int b, int c, int d) {
		if (img)
			img->setAlignment((irr::gui::EGUI_ALIGNMENT)a, (irr::gui::EGUI_ALIGNMENT)b, (irr::gui::EGUI_ALIGNMENT)c, (irr::gui::EGUI_ALIGNMENT)d);
	}

	void setMaxSize(const Vector2D& max) {
		if (img)
			img->setMaxSize(irr::core::dimension2du(max.x, max.y));
	}

	bool getEnabled() {
		return img ? img->isEnabled() : false;
	}

	void setEnabled(bool enable) {
		if (img)
			img->setEnabled(enable);
	}

	bool getUseAlpha() {
		return img ? img->isAlphaChannelUsed() : false;
	}

	void setUseAlpha(bool enable) {
		if (img)
			img->setUseAlphaChannel(enable);
	}

	void setParent(const Image2D& other) {
		if (img)
			other.img->addChild(img);
	}
};

inline void bindImage2D() {
	sol::usertype<Image2D> bind_type = lua->new_usertype<Image2D>("Image2D",
		sol::constructors <Image2D(const Texture& tex), Image2D(const Texture& tex, const Vector2D& pos), Image2D(const Texture& tex, const Vector2D& pos, const Vector2D& dimensions), Image2D(const Image2D& other)>(),

		"position", sol::property(&Image2D::getPosition, &Image2D::setPosition),
		"visible", sol::property(&Image2D::getVisible, &Image2D::setVisible),
		"size", sol::property(&Image2D::getSize, &Image2D::setSize),
		"enabled", sol::property(&Image2D::getEnabled, &Image2D::setEnabled),
		"useAlpha", sol::property(&Image2D::getUseAlpha, &Image2D::setUseAlpha),
		"scaleToFit", sol::property(&Image2D::scalesToFit, &Image2D::setScalesToFit),
		"hovered", sol::property(&Image2D::getHovered, &Image2D::setHovered),
		"color", sol::property(&Image2D::getColor, &Image2D::setColor),
		"opacity", sol::property(&Image2D::getOpacity, &Image2D::setOpacity)
	);

	bind_type["destroy"] = &Image2D::destroy;
	bind_type["load"] = &Image2D::setImage;
	bind_type["setMaxSize"] = &Image2D::setMaxSize;
	bind_type["toFront"] = &Image2D::bringToFront;
	bind_type["toBack"] = &Image2D::sendToBack;
	bind_type["setBorderAlignment"] = &Image2D::setBorderAlignment;
	bind_type["setParent"] = &Image2D::setParent;
	bind_type["fireOnClick"] = &Image2D::setClickable;
}