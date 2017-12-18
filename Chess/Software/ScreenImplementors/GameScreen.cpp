//
//  GameScreen.cpp
//  Chess
//
//  Created by macbook on 11/2/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "GameScreen.h"

#include <iostream>

#include "GameController.h"
#include "OptionScreen.h"
#include "OffPositionScreen.h"
#include "MainMenuScreen.h"
#include "GameSettingScreen.h"
#include "BoardServices.h"
#include "KeyboardController.h"
#include "ScreenManager.h"

GameScreen* GameScreen::create(GameController* gameController) {
    GameScreen* screen = new GameScreen();
    screen->gameController = gameController;
    screen->init();
    
    return screen;
}

void GameScreen::onEnter() {
    std::cout << "GameScreen - onEnter" << std::endl;
    BoardServices::getInstance()->setBoardServicesEventsDelegator(gameController);
    KeyboardController::getInstance()->setKeyboardEventsDelegator(gameController);
    if (!entered) {
        entered = true;
        gameController->setDelegator(this);
        gameController->start();
        return;
    }
    
    if (awaitAdjustment) {
        std::cout << "[GameScreen] back from off position screen" << std::endl;
        awaitAdjustment = false;
        gameController->handleBackFromOffPositionScreen();
    }
}

void GameScreen::onExit() {
    std::cout << "GameScreen - onExit" << std::endl;
    BoardServices::getInstance()->setBoardServicesEventsDelegator(NULL);
    KeyboardController::getInstance()->setKeyboardEventsDelegator(NULL);
    delete gameController;
}

void GameScreen::init() {
    awaitAdjustment = false;
    entered = false;
}

void GameScreen::onResetting() {
    print(1, "Setting up new game");
    print(2, "Please wait...");
    print(3, "");
    print(4, "");
}

void GameScreen::onPiecesOffPosition(BaseTypes::Bitboard currentState, BaseTypes::Bitboard expectedState) {
    awaitAdjustment = true;
    OffPositionScreen* screen = OffPositionScreen::create(currentState, expectedState);
    ScreenManager::getInstance()->pushScreen(screen);
    std::cout << "[GameScreen] onPiecesOffPosition - done" << std::endl;
}

void GameScreen::onGameStarted() {
    print(1, "Game started");
}

void GameScreen::onTurnBegan(BaseTypes::Move previousOpponentMove) {
	std::cout << "[GameScreen] onTurnBegan" << std::endl;
    if (previousOpponentMove != BaseTypes::Move()) {
        print(1, "Opponent move: " + previousOpponentMove.toString());
    }
    
    print(2, "It's your turn");
    print(3, "Make your move...");
    print(4, "");
}

void GameScreen::onTurnEnded(BaseTypes::Move playerMove) {
	std::cout << "[GameScreen] onTurnEnded" << std::endl;
    print(2, "Your move: " + playerMove.toString());
    print(3, "It's opponent's turn");
    print(4, "Please wait...");
}

void GameScreen::onInvalidMove() {
//    print(3, "You move: " + data.player_move);
    print(3, "Invalid. Try again..");
}

void GameScreen::onMultipleMovesAvailable(std::vector<BaseTypes::Move> moves, std::function<void(bool, BaseTypes::Move)> onSelected) {
    std::vector<OptionScreenEntry> entries;
    for (int i = 0; i < moves.size(); i++) {
        OptionScreenEntry entry;
        entry.name = moves.at(i).toString();
        entry.onSelected = [=](std::string content) {
            onSelected(true, BaseTypes::Move(content));
            ScreenManager::getInstance()->popScreen();
        };
        entries.push_back(entry);
    }
    
    OptionScreenEntry cancelEntry;
    cancelEntry.name = "CANCEL";
    cancelEntry.onSelected = [=](BaseTypes::Move move) {
        onSelected(false, BaseTypes::Move());
        ScreenManager::getInstance()->popScreen();
    };
    entries.push_back(cancelEntry);
    
    OptionScreen* screen = OptionScreen::create("Which is your move?", entries);
    ScreenManager::getInstance()->pushScreen(screen);
}

void GameScreen::onGameMenuRequested() {
    std::vector<OptionScreenEntry> entries;
    
    OptionScreenEntry newGameEntry;
    newGameEntry.name = "New game";
    newGameEntry.onSelected = [=](std::string content) {
        ScreenManager::getInstance()->runScreen(GameSettingScreen::create());
    };
    entries.push_back(newGameEntry);

    OptionScreenEntry mainMenuEntry;
    newGameEntry.name = "Go to Main menu";
    newGameEntry.onSelected = [=](std::string content) {
        ScreenManager::getInstance()->runScreen(MainMenuScreen::create());
    };
    entries.push_back(newGameEntry);
    
    OptionScreenEntry cancelEntry;
    cancelEntry.name = "Cancel";
    cancelEntry.onSelected = [=](std::string content) {
        ScreenManager::getInstance()->popScreen();
    };
    entries.push_back(cancelEntry);
    
    OptionScreen* gameMenuScreen = OptionScreen::create("GAME MENU", entries);
    ScreenManager::getInstance()->pushScreen(gameMenuScreen);
}

void GameScreen::onWinGame(BaseTypes::Move lastMove) {
    std::vector<OptionScreenEntry> entries;
    
    OptionScreenEntry newGameEntry;
    newGameEntry.name = "Play new game";
    newGameEntry.onSelected = [=](std::string content) {
		ScreenManager::getInstance()->runScreen(GameSettingScreen::create());
	};
	entries.push_back(newGameEntry);

    OptionScreenEntry mainMenuEntry;
    mainMenuEntry.name = "Go to Main menu";
    mainMenuEntry.onSelected = [=](std::string content) {
        ScreenManager::getInstance()->runScreen(MainMenuScreen::create());
	};
	entries.push_back(mainMenuEntry);

    OptionScreen* screen = OptionScreen::create("YOU WIN!!!", entries);
    ScreenManager::getInstance()->pushScreen(screen);
}

void GameScreen::onLoseGame(BaseTypes::Move lastMove) {
    std::vector<OptionScreenEntry> entries;
    
    OptionScreenEntry newGameEntry;
    newGameEntry.name = "Play new game";
    newGameEntry.onSelected = [=](std::string content) {
		ScreenManager::getInstance()->runScreen(GameSettingScreen::create());
	};
	entries.push_back(newGameEntry);

    OptionScreenEntry mainMenuEntry;
    mainMenuEntry.name = "Go to Main menu";
    mainMenuEntry.onSelected = [=](std::string content) {
        ScreenManager::getInstance()->runScreen(MainMenuScreen::create());
	};
	entries.push_back(mainMenuEntry);

    OptionScreen* screen = OptionScreen::create("YOU LOSE", entries);
    ScreenManager::getInstance()->pushScreen(screen);
}

void GameScreen::onDrawGame(DrawGameType type, BaseTypes::Move lastMove) {
    std::vector<OptionScreenEntry> entries;
    
    OptionScreenEntry newGameEntry;
    newGameEntry.name = "Play new game";
    newGameEntry.onSelected = [=](std::string content) {
		ScreenManager::getInstance()->runScreen(GameSettingScreen::create());
	};
	entries.push_back(newGameEntry);

    OptionScreenEntry mainMenuEntry;
    mainMenuEntry.name = "Go to Main menu";
    mainMenuEntry.onSelected = [=](std::string content) {
        ScreenManager::getInstance()->runScreen(MainMenuScreen::create());
	};
	entries.push_back(mainMenuEntry);

    OptionScreen* screen = OptionScreen::create("DRAW GAME", entries);
    ScreenManager::getInstance()->pushScreen(screen);        
}
