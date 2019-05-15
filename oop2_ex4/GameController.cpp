#include "GameController.h"
#include "MainScreen.h"
#include "ChooseAIModeScreen.h"
#include "GameScreen.h"
#include "JoinGameScreen.h"
#include "WaitingMultiplayerScreen.h"
#include "RequestsClientThread.h"
#include "RequestsServerThread.h"
#include "Timer.h"
#include "Logger.h"
#include "PlayerAIStupid.h"
#include "UserPlayer.h"
#include "ColoringAlgorithm.h"
#include "AreaButton.h"
#include "winScreen.h"

GameController::GameController()
{ }

void GameController::run()
{
	// create window
	sf::RenderWindow window(sf::VideoMode(1000, 500), "6 Colors");
	// run main screen
	runMainScreen(window);
}

string GameController::toString() const
{
	return "GameController: { }";
}

void GameController::runMainScreen(sf::RenderWindow& window)
{
	MainScreen mainScreen(window);
	mainScreen.getSingleBt()->addClickListener([this](GUI::View& view) {
		runChooseModeAIScreen(view.getWindow());
	});
	mainScreen.getCreateBt()->addClickListener([this](GUI::View& view) {
		runWaitMultScreen(view.getWindow());
	});
	mainScreen.getJoinBt()->addClickListener([this](GUI::View& view) {
		runJoinScreen(view.getWindow());
	});
	mainScreen.getExitleBt()->addClickListener([this, &mainScreen](GUI::View& view) {
		mainScreen.close();
	});
	mainScreen.run();
}

void GameController::runChooseModeAIScreen(sf::RenderWindow& window)
{
	ChooseAIModeScreen chooseAIMScreen(window);
	chooseAIMScreen.addLevelClickListener([this](std::shared_ptr<LevelDifficultyButton> levelDiffBt) {

		// create players
		std::vector<std::shared_ptr<PlayerBase>> players;
		players.push_back(std::make_shared<UserPlayer>());
		std::shared_ptr<PlayerBase> aiPlayer;

		switch (levelDiffBt->getLevelDifficulty())
		{
			case LevelDifficultyButton::LevelDifficulty::STUPID: {
				aiPlayer = std::make_shared<PlayerAIStupid>();	
			} break;
			case LevelDifficultyButton::LevelDifficulty::REGULAR: {
				// TODO aiPlayer = std::make_shared<RegularAIPlayer>();
			} break;
			case LevelDifficultyButton::LevelDifficulty::SUPER: {
				// TODO aiPlayer = std::make_shared<SuperAIPlayer>();
			} break;
		}
		players.push_back(aiPlayer);
		runGameScreen(levelDiffBt->getWindow(), players);
	});
	chooseAIMScreen.run();
}

void GameController::runGameScreen(sf::RenderWindow& window, std::vector<std::shared_ptr<PlayerBase>>& players)
{
	// timer for screen updates
	Timer screenUpdatesTimer;

	// the players
	std::shared_ptr<PlayerBase> userPlayer = players[0], otherPlayer = players[1];
	
	// create game screen
	GameScreen gameScreen(window);

	// create and play game
	createGame(gameScreen, players);
	playGame(screenUpdatesTimer, gameScreen, players);
	
	gameScreen.getGameMenu()->getExitButton()->addClickListener([&gameScreen](GUI::View& view) {
		gameScreen.close();
	});
	gameScreen.getGameMenu()->getRestartButton()->addClickListener([this, &screenUpdatesTimer, &gameScreen, &players](GUI::View& view) {
		// create and play game
		createGame(gameScreen, players);
		playGame(screenUpdatesTimer, gameScreen, players);
	});
	gameScreen.run(screenUpdatesTimer);
}

void GameController::createGame(GameScreen& gameScreen, std::vector<std::shared_ptr<PlayerBase>>& players)
{
	// clean players info (shapes)
	for (auto& player : players)
		player->clean();

	// the players
	std::shared_ptr<PlayerBase>& userPlayer = players[0], &otherPlayer = players[1];

	// set board
	gameScreen.getBoard()->randomizeBoard(Board::DEFAULT_BOARD_SIZE);
	gameScreen.getGameMenu()->getTurnButton()->setText(userPlayer->getName() + " turn");
	Graph<PolygonView>& graph = gameScreen.getBoard()->getPolygonsGraph();

	// connect to game
	userPlayer->connectToGame(&gameScreen, otherPlayer);
	otherPlayer->connectToGame(&gameScreen, userPlayer);	
	otherPlayer->setStartVertex(graph.getVertex(gameScreen.getBoard()->getBoardSize().x - 1));
	userPlayer->setStartVertex(graph.getVertex(graph.getNumOfVertices() - gameScreen.getBoard()->getBoardSize().x));
}

void GameController::playGame(Timer& screenUpdatesTimer, GameScreen& gameScreen, std::vector<std::shared_ptr<PlayerBase>>& players) 
// TODO need to get who play first
{
	// the players
	std::shared_ptr<PlayerBase>& userPlayer = players[0], &otherPlayer = players[1];

	bool isFirstPlayerTurn = true;

	screenUpdatesTimer.start(30, [&userPlayer, &otherPlayer, &gameScreen, &isFirstPlayerTurn]() {
		if (isFirstPlayerTurn) {
			if (userPlayer->isReadyToPlay()) {
				sf::Color selectedColor = userPlayer->selectColor();
				ColoringAlgorithm colorAlgo;
				colorAlgo.colorGraph(userPlayer->getPlayerVertices(), userPlayer->getBorderVertices(), selectedColor);

				gameScreen.getBottomPanel()->getColorPanel()->disable();
				isFirstPlayerTurn = false;
				gameScreen.getGameMenu()->getTurnButton()->setText(otherPlayer->getName() + " turn");
				
				// update area percent
				std::shared_ptr<AreaButton>& myAreaBT = gameScreen.getBottomPanel()->getMyAreaButton();
				myAreaBT->updateAreaPercent(static_cast<int>(userPlayer->getPlayerVertices().size()), static_cast<int>(gameScreen.getBoard()->getPolygonsGraph().getNumOfVertices()));
				myAreaBT->setText(myAreaBT->getPreText() + std::to_string(myAreaBT->getAreaPercent()) + "%");
				if (myAreaBT->getAreaPercent() >= 20.f) {
					gameScreen.close();
					WinScreen winScreen(gameScreen.getWindow());
					winScreen.run();
				}
					

			}
		}
		else {
			if (otherPlayer->isReadyToPlay()) {
				if (otherPlayer->isReadyToPlay()) {
					sf::Color selectedColor = otherPlayer->selectColor();
					ColoringAlgorithm colorAlgo;
					colorAlgo.colorGraph(otherPlayer->getPlayerVertices(), otherPlayer->getBorderVertices(), selectedColor);
					gameScreen.getBottomPanel()->getColorPanel()->enable();
					isFirstPlayerTurn = true;
					gameScreen.getGameMenu()->getTurnButton()->setText(userPlayer->getName() + " turn");
					
					// update area percent
					std::shared_ptr<AreaButton>& rivalAreaBT = gameScreen.getBottomPanel()->getRivalAreaButton();
					rivalAreaBT->updateAreaPercent(static_cast<int>(otherPlayer->getPlayerVertices().size()), static_cast<int>(gameScreen.getBoard()->getPolygonsGraph().getNumOfVertices()));
					rivalAreaBT->setText(rivalAreaBT->getPreText() + std::to_string(rivalAreaBT->getAreaPercent()) + "%");
				}
			}
		}
	});
}

void GameController::runJoinScreen(sf::RenderWindow& window)
{
	// timer for screen updates
	Timer screenUpdatesTimer;

	// TODO create server player

	// create requests queues
	RequestsQueue<string> sendRequests, receiveRequests;

	// create client thread
	RequestsClientThread clientThread(sendRequests, receiveRequests);

	screenUpdatesTimer.start(30, [&clientThread]() {
		// check if client connected
		if (clientThread.isConnectedToServer()) {
			// TODO run game screen
		}
	});

	// create screen
	JoinGameScreen joinGameScreen(window);
	joinGameScreen.getConnectButton()->addClickListener([this, &clientThread, &joinGameScreen](GUI::View& view) {
		joinGameScreen.setAsConnecting();
		// connect to server
		sf::IpAddress serverIpAddress(joinGameScreen.getIpEditText()->getText());
		clientThread.start(serverIpAddress, INetworkThread::DEFAULT_PORT);
	});
	joinGameScreen.run(screenUpdatesTimer);
	
	// stop client thread
	clientThread.stop();
}

void GameController::runWaitMultScreen(sf::RenderWindow& window)
{
	bool gameIsReady = false;

	// TODO create client player

	// timer for screen updates
	Timer screenUpdatesTimer;

	// create network requests queues
	RequestsQueue<string> sendRequests, receiveRequests;

	// create server
	RequestsServerThread serverThread(sendRequests, receiveRequests);
	serverThread.start(INetworkThread::DEFAULT_PORT);

	// create screen
	WaitingMultiplayerScreen waitMultScreen(window);

	screenUpdatesTimer.start(30, [&serverThread, &waitMultScreen, &gameIsReady]() {		
		if (!gameIsReady) {
			// check if client connected
			if (serverThread.getNumOfClients() == 1) {
				// update components
				serverThread.setBlockConnections(true);
				waitMultScreen.setReadyToPlay();
				gameIsReady = true;
			}
		}		
	});

	waitMultScreen.getStartButton()->addClickListener([](GUI::View& view) {
		// TODO open game screen
	});
	waitMultScreen.run(screenUpdatesTimer);
	
	// stop server thread
	serverThread.stop();
}
