#pragma once
//---- include section ------
#include <string>
#include <array>
#include <vector>
#include "HorizontalLayout.h"
#include "ColorButton.h"

//---- using section --------
using std::string;

/*
 * ColorPanel class
 */
class ColorPanel :
	public GUI::HorizontalLayout<ColorButton>
{
public:
	// array of colors
	static const std::array<sf::Color, 6> COLORS;
	// buttons background colors
	static const sf::Color BLUE_COLOR, GREEN_COLOR, RED_COLOR,
		                   YELLOW_COLOR, PURPLE_COLOR, ORANGE_COLOR;
	// constructor
	explicit ColorPanel(sf::RenderWindow& window);
	// convert to string
	virtual string toString() const override;
	// get button by color
	const std::shared_ptr<ColorButton>& getColorButton(const sf::Color& color) const;
	// add click on color listener
	void addClickColorListener(std::function<void(std::shared_ptr<ColorButton>)> onClickCB);	
private:
	// color panel
	std::vector<std::shared_ptr<ColorButton>> m_colorPanel;
	// init
	void initComponents(sf::RenderWindow& window);
};


