#pragma once
//---- include section ------
#include <string>
#include <vector>
#include "Shape.h"

//---- using section --------
using std::string;

/*
 * Shape class
 */
class PolygonShape :
	public Shape
{
public:
	// constructor
	PolygonShape();
	// add relative point to polygon
	void addPoint(const sf::Vector2f& relPoint);
	// get number of points
	int getNumOfPoints() const { return static_cast<int>(m_relPoints.size()); }
	// get point
	const sf::Vector2f& getPoint(int index) const;
	// get number of edges
	virtual int getNumOfEdges() const { return getNumOfPoints(); }
	// convert to string
	virtual string toString() const override;
private:
	// relative points of polygon
	std::vector<sf::Vector2f> m_relPoints;
	// check if relative point is legal
	bool isLegalRelPoint(const sf::Vector2f& relPoint) const { return (isRelativeNum(relPoint.x) && isRelativeNum(relPoint.y)); }
	// check if is number in [0, 1]
	bool isRelativeNum(float num) const { return ((num >= 0.f) && (num <= 1.f)); }
};