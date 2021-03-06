#include "AreaButton.h"

// init
const float AreaButton::WIN_NUM_PRECENTS = 50.f;

AreaButton::AreaButton(sf::RenderWindow& window, float areaPercent)
	: TextView(window)
{
	setAreaPercent(areaPercent);
	init();
}

float AreaButton::getAreaPercent() const
{
	return m_areaPercent;
}

void AreaButton::setAreaPercent(float area)
{
	if (area < 0.f || area > 100.f)
		throw std::out_of_range("Area percent " + std::to_string(area) + " cannot be less than zero or bigger from 100");
	m_areaPercent = area;
}

void AreaButton::updateAreaPercent(int MyShape, int numOfAllshapes)
{
	float AreaPercent = (float(MyShape) / float(numOfAllshapes)) * 100.f;
	setAreaPercent(AreaPercent);
}

string AreaButton::toString() const
{
	return "AreaButton: { " + TextView::toString() + ", areaPercent: " + std::to_string(m_areaPercent) + " }";
}

void AreaButton::init()
{
	setFont("BERNHC");
	setPreText("Area:\n" );
	setTextSize(37);
}
