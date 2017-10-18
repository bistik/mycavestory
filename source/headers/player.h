#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "globals.h"

class Graphics;

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, float x, float y);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	/*
	 * move player left by -dx
	 */
	void moveLeft();

	/*
	 * move player right by +dx
	 */
	void moveRight();
	/*
	 * stop moving player, play idle animation
	 */
	void stopMoving();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();
private:
	/*
	 * change in x and y
	 */
	float _dx, _dy;
	Direction _facing;
};
#endif
