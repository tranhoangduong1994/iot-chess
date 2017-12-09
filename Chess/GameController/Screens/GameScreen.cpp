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

#include "BoardServices.h"

GameScreen* GameScreen::create(GameController* gameController) {
    GameScreen* screen = new GameScreen();
    screen->gameController = gameController;
    screen->init();
    
    return screen;
}

void GameScreen::onEnter() {
    std::cout << "GameScreen - onEnter" << std::endl;
    BoardServices::getInstance()->setBoardServicesEventsProtocol(gameController);
    BoardServices::getInstance()->setBoardKeyEventsDelegate(game);
    if (!entered) {
        entered = true;
        game->start(BaseTypes::Side::WHITE, 1);
        game->setDelegate(this);
        return;
    }
    
    if (awaitAdjustment) {
        awaitAdjustment = false;
        gameController->handleBackFromOffPositionScreen();
    }
}

void GameScreen::onExit() {
    std::cout << "GameScreen - onExit" << std::endl;
    BoardServices::getInstance()->setBoardIngameEventsDelegate(NULL);
    BoardServices::getInstance()->setBoardKeyEventsDelegate(NULL);
    delete gameController;
    delete this;
}

void GameScreen::init() {
    awaitAdjustment = false;
    entered = false;
}

void GameScreen::onPiecesOffPosition(const BaseTypes::Bitboard& currentState, const BaseTypes::Bitboard &expectedState) {
    awaitAdjustment = true;
    OffPositionScreen* screen = OffPositionScreen::create(currentState, expectedState);
    Screen::pushScreen(screen);
    std::cout << "[GameScreen] onPiecesOffPosition - done" << std::endl;
}

void GameScreen::onGameStarted(const GameStartedData& data) {
    clear();
    print(1, "Game started");
}

void GameScreen::onTurnBegan(const TurnBeganData& data) {
    if (data.opponent_move != "") {
        print(1, "Opponent move: " + data.opponent_move);
    }
    
    print(2, "It's your turn.");
    print(3, "Make your move...");
    print(4, "");
}

void GameScreen::onTurnEnded(const TurnEndedData& data) {
    print(3, "You played: " + data.player_move);
    print(4, "Please wait for opponent move...");
}

void GameScreen::onInvalidMove(const InvalidMoveData& data) {
    print(3, "You play: " + data.player_move);
    print(4, "Invalid move, please try again: ");
}

void GameScreen::onMultipleMovesAvailable(const std::vector<BaseTypes::Move>& moves, std::function<void(bool, BaseTypes::Move)> onSelected) {
    std::vector<OptionScreenEntry> entries;
    for (int i = 0; i < moves.size(); i++) {
        OptionScreenEntry entry;
        entry.name = moves.at(i).toString();
        entry.onSelected = [=](std::string content) {
            onSelected(true, BaseTypes::Move(content));
        };
        entries.push_back(entry);
    }
    
    OptionScreenEntry cancelEntry;
    cancelEntry.name = "CANCEL";
    cancelEntry.onSelected = [=](BaseTypes::Move move) {
        onSelected(false, BaseTypes::Move());
    };
    entries.push_back(cancelEntry);
    
    OptionScreen* screen = OptionScreen::create("Which is your move?", entries);
    Screen::pushScreen(screen);
}


void GameScreen::onWinGame(const WinGameData& data) {
    std::vector<OptionScreenEntry> entries;
    
    OptionScreenEntry newGameEntry;
    newGameEntry.name = "Play new game";
    newGameEntry.onSelected = [=](std::string content) {
		
	};
	entries.push_back(newGameEntry);

    OptionScreenEntry mainMenuEntry;
    newGameEntry.name = "Go to Main menu";
    newGameEntry.onSelected = [=](std::string content) {
		
	};
	entries.push_back(newGameEntry);

    OptionScreen* screen = OptionScreen::create("YOU WIN!!!", entries);
    Screen::pushScreen(screen);
}

void GameScreen::onLoseGame(const LoseGameData& data) {
    std::vector<OptionScreenEntry> entries;
    
    OptionScreenEntry newGameEntry;
    newGameEntry.name = "Play new game";
    newGameEntry.onSelected = [=](std::string content) {
		
	};
	entries.push_back(newGameEntry);

    OptionScreenEntry mainMenuEntry;
    newGameEntry.name = "Go to Main menu";
    newGameEntry.onSelected = [=](std::string content) {
		
	};
	entries.push_back(newGameEntry);

    OptionScreen* screen = OptionScreen::create("YOU LOSE", entries);
    Screen::pushScreen(screen);    
}

void GameScreen::onDrawGame(const DrawGameData& data) {
    std::vector<OptionScreenEntry> entries;
    
    OptionScreenEntry newGameEntry;
    newGameEntry.name = "Play new game";
    newGameEntry.onSelected = [=](std::string content) {
		
	};
	entries.push_back(newGameEntry);

    OptionScreenEntry mainMenuEntry;
    newGameEntry.name = "Go to Main menu";
    newGameEntry.onSelected = [=](std::string content) {
		
	};
	entries.push_back(newGameEntry);

    OptionScreen* screen = OptionScreen::create("DRAW GAME", entries);
    Screen::pushScreen(screen);        
}
