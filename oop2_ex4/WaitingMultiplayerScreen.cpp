#include "WaitingMultiplayerScreen.h"
#include <SFML/Network.hpp>

WaitingMultiplayerScreen::WaitingMultiplayerScreen(sf::RenderWindow& window)
	: GUI::BaseScreen<GUI::RelativeLayout<GUI::View>>(window), 
	m_waitingToClientTv(std::make_shared<GUI::TextView>(window, "Waiting For The Other Player...")),
	m_myIpTv(std::make_shared<GUI::TextView>(window)),
	m_startBt(std::make_shared<GUI::Button>(window, "Start Game")),
	m_loadAv(std::make_shared<GUI::AnimationView>(window))
{
	init();
}

void WaitingMultiplayerScreen::setReadyToPlay()
{
	m_waitingToClientTv->setText("Other player connected!");
	m_loadAv->hide();
	m_startBt->enable();
	m_startBt->getBackground().setColor(sf::Color(sf::Color::Green));
	m_myIpTv->hide();	
}

string WaitingMultiplayerScreen::toString() const
{
	return "WaitingMultiplayerScreen: { " + GUI::BaseScreen<GUI::RelativeLayout<GUI::View>>::toString() + " }";
}

void WaitingMultiplayerScreen::init()
{
	// init waiting to client title
	m_waitingToClientTv->setTextColor(sf::Color::Cyan);
	m_waitingToClientTv->setTextSize(32);
	m_waitingToClientTv->setTextColor(sf::Color::Black);
	m_waitingToClientTv->setFont("BRLNSDB");
	addView(m_waitingToClientTv, sf::FloatRect(1.f / 4.f, 0.05f, 1.f / 2.f, 0.10f));

	// init load animation view
	m_loadAv->setAnimation("load");
	m_loadAv->setAnimationFrequency(30);
	addView(m_loadAv, sf::FloatRect(3.f/8.f, 0.2f, 2.f/8.f, 3.f/8.f));

	// init my ip title 
	m_myIpTv->setTextColor(sf::Color::Cyan);
	m_myIpTv->setTextSize(30);
	m_myIpTv->setTextColor(sf::Color::Black);
	m_myIpTv->setTextStyle(sf::Text::Style::Bold);
	m_myIpTv->setText("My IP: " + sf::IpAddress::getLocalAddress().toString());
	m_myIpTv->setFont("BAUHS93");
	addView(m_myIpTv, sf::FloatRect(1.f / 5.f, 0.60f, 3.f / 5.f, 0.10f));

	// init start game button
	m_startBt->getBackground().setColor(sf::Color(170, 170, 170));
	m_startBt->setTextSize(24);
	m_startBt->setTextColor(sf::Color::White);
	m_startBt->setTextStyle(sf::Text::Style::Bold);
	m_startBt->disable();
	m_startBt->addEnterListener([this](View& view) {
		view.getBackground().setColor(sf::Color(255, 255, 255, 128));
	});
	m_startBt->addLeaveListener(([this](View& view) {
		view.getBackground().setColor(sf::Color(sf::Color::Green));
	}));
	
	addView(m_startBt, sf::FloatRect(2.f / 5.f, 0.8f, 1.f / 5.f, 0.1f));
}
