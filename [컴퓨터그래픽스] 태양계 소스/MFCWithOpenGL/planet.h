#pragma once
class Planet {
public :
	Planet(float _radius, float _position)
		: radius(_radius), position(_position) {};
	void drawPlanet();
	float radius;
	float position;
	float rot;
	float revo;
};

