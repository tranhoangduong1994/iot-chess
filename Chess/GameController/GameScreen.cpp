//
//  GameScreen.cpp
//  Chess
//
//  Created by macbook on 11/2/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "GameScreen.h"
#include "OfflineGame.h"
#include <iostream>

GameScreen* GameScreen::create(OfflineGame* game, int width, int height) {
    GameScreen* gameScreen = new GameScreen();
    
    gameScreen->width = width;
    gameScreen->height = height;
    
    gameScreen->screenBuffer.resize(height);
    
    gameScreen->game = game;
    
    game->setDelegate(gameScreen);
    game->start(BaseTypes::WHITE, 1);
    
    return gameScreen;
}

void GameScreen::clearScreen() {
    for (int i = 0; i < 30; i++) {
        std::cout << std::endl;
    }
}

void GameScreen::display(int lineNumber, std::string string) {
    if (lineNumber > screenBuffer.size()) {
        return;
    }
    screenBuffer[lineNumber - 1] = string;
    
    clearScreen();
    
    for (int i = 0; i < screenBuffer.size(); i++) {
        if (screenBuffer.at(i) != "") {
            std::cout << "#" << (i + 1) << " " << screenBuffer.at(i) << std::endl;
        } else {
//            std::cout << "#" << (i + 1) << " " << std::endl;
        }
    }
}

std::string GameScreen::getInput() {
    std::string result;
    std::cin >> result;
    return result;
}

void GameScreen::onGameStarted(const EventData& data) {
    display(1, "Game started!!");
}

void GameScreen::onTurnBegan(const EventData& data) {
    if (data.find("opponent_move") != data.end()) {
        std::string opponentMove = data.at("opponent_move");
        display(1, "Opponent move: " + opponentMove);
    }
    
    display(2, "It's your turn.");
    display(3, "You play: ");
    display(4, "");
    
    std::string move = getInput();
    std::cout << "test move: " + move;
    game->move(BaseTypes::Move(move));
}

void GameScreen::onTurnEnded(const EventData& data) {
    display(3, "You play: " + data.at("your_move"));
    display(4, "Please wait for opponent move...");
}

void GameScreen::onInvalidMove(const EventData& data) {
    display(3, "You play: " + data.at("your_move"));
    display(4, "Invalid move, please try again: ");
    std::string move = getInput();
    game->move(BaseTypes::Move(move));
}

void GameScreen::onWinGame(const EventData& data) {
    
}

void GameScreen::onLoseGame(const EventData& data) {
    
}

void GameScreen::onDrawGame(const EventData& data) {
    
}
