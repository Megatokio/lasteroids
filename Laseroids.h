// Copyright (c) 2021 Mathema GmbH
// SPDX-License-Identifier: BSD-3-Clause
// Author: Günter Woigk (Kio!)
// Copyright (c) 2021 kio@little-bat.de
// BSD 2-clause license

#pragma once
#include "cdefs.h"
#include "basic_geometry.h"


class Object
{
public:
	Object (){}
	Object (const Point& position, const Dist& speed, int16 orientation, int16 rotation) :
		position(position),speed(speed),orientation(orientation),rotation(rotation) {}
	Object (const Point& position) : position(position) {}
	Object (FLOAT x, FLOAT y) : position(x,y) {}
	virtual void draw() = 0;
	virtual void move();
	void wrap();

	Point position;			// x,y [m]
	Dist  speed;			// dx,dy [m/s]
	int16 orientation=0;	// deg, 0° = heading right, CW
	int16 rotation=0;		// rotational speed deg/s
};

class Star : public Object
{
public:
	virtual void draw() override;
	virtual void move() override;
	Star();
};

class Asteroid : public Object
{
public:
	Point vertices[16];
	uint  num_vertices;
	uint  size;
	FLOAT radians;
	virtual void draw() override;
	//virtual void move() override;
	Asteroid(){}
	Asteroid(uint size_id, const Point& position, const Dist& speed, int16 orientation=0, int16 rotation=0);
};

class PlayerShip : public Object
{
public:
	bool shield = false;
	uint acceleration = 0;	// 0 .. 3
	virtual void draw() override;
	virtual void move() override;
	PlayerShip();
};

class AlienShip : public Object
{
public:
	bool visible = false;
	virtual void draw() override;
	virtual void move() override;
	AlienShip(){printf("%s\n",__func__);}
	AlienShip(const Point& position);
};

class Bullet : public Object
{
public:
	static constexpr uint LIFETIME = 20;
	static constexpr FLOAT LENGTH = 100;
	static constexpr FLOAT SPEED  = 10000/20;
	Dist size;					// offset from start to end of line
	uint count_down;
	virtual void draw() override;
	virtual void move() override;
	Bullet(){}
	Bullet(const Point& position, const Dist& direction, FLOAT speed=SPEED);
};


class Laseroids
{
public:
	PlayerShip player;
	AlienShip alien;
	Asteroid asteroids[20];
	uint num_asteroids = 0;
	Bullet bullets[20];
	uint num_bullets = 0;
	Star stars[15];
	uint num_stars = NELEM(stars);

	uint lives = 4;
	uint score = 0;

	Laseroids();
	void init() { new(this) Laseroids(); }
	void draw_all();
	void move_all();
	void test_collissions();
	bool game_over() { return lives == 0; }
	void run_1_frame();
	void accelerate();
	void decelerate();
	void rotate_CW();
	void rotate_CCW();
	void activate_shield(bool=1);
};
