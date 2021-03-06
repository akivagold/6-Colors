#include "Square.h"

Square::Square()
	: PolygonShape()
{
	init();
}

string Square::toString() const
{
	return "Square: { " + PolygonShape::toString() + " }";
}

void Square::init()
{
	addPoint(sf::Vector2f(0.5f, 0.f));
	addPoint(sf::Vector2f(1.f, 0.5f));
	addPoint(sf::Vector2f(0.5f, 1.f));
	addPoint(sf::Vector2f(0.f, 0.5f));
}