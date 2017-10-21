#include "sprite.h"
#include "graphics.h"
#include "globals.h"

Sprite::Sprite() {}
Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY) :
				_x(posX),
				_y(posY)
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.w = width;
	this->_sourceRect.h = height;
	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (this->_spriteSheet == NULL) {
		printf("\nError: Unable to load image\n");
	}

	this->_boundingBox = Rectangle(this->_x, this->_y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
}

Sprite::~Sprite() {}

void Sprite::draw(Graphics &graphics, int x, int y) {
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE };
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::update() {
	this->_boundingBox = Rectangle(this->_x, this->_y,
			this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE);
}

const Rectangle Sprite::getBoundingBox() const {
	return this->_boundingBox;
}

// side getcollisionside
// find which side the collision happened on
const sides::Side Sprite::getCollisionSide(Rectangle &other) const {
	int amtRight, amtLeft, amtTop, amtBottom;
	amtRight = abs(this->getBoundingBox().getRight() - other.getLeft());
	amtLeft = abs(other.getRight() - this->getBoundingBox().getLeft());
	amtTop = abs(other.getBottom() - this->getBoundingBox().getTop());
	amtBottom = abs(this->getBoundingBox().getBottom() - other.getTop());

	int vals[4] = { amtRight, amtLeft, amtTop, amtBottom };
	int lowest = vals[0];
	for (int i = 1; i < 4; i++) {
		if (vals[i] < lowest) {
			lowest = vals[i];
		}
	}

	if (lowest == amtRight) {
		return sides::RIGHT;
	} else if (lowest == amtLeft) {
		return sides::LEFT;
	} else if (lowest == amtTop) {
		return sides::TOP;
	} else if (lowest == amtBottom) {
		return sides::BOTTOM;
	} else {
		return sides::NONE;
	}
}
