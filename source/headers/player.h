#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "globals.h"
#include "slope.h"

class Graphics;

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
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

	/*
	 * player jumps
	 */
	void jump();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleSlopeCollisions(std::vector<Slope> &others);

	const float getX() const;
	const float getY() const;
private:
	/*
	 * change in x and y
	 */
	float _dx, _dy;
	Direction _facing;

	bool _grounded;
};
#endif
