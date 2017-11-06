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
    if (!displayer) {
        displayer = DisplayerImplementation::getInstance();
    }
    
    GameScreen* screen = new GameScreen();
    screen->game = game;
    screen->game->setDelegate(screen);
    screen->width = width;
    screen->height = height;
    return screen;
}

std::string GameScreen::getInput() {
    std::string result;
    std::cin >> result;
    return result;
}

void GameScreen::onGameStarted(const EventData& data) {
    print(1, "Game started!!");
}

void GameScreen::onTurnBegan(const EventData& data) {
    if (data.find("opponent_move") != data.end()) {
        std::string opponentMove = data.at("opponent_move");
        print(1, "Opponent move: " + opponentMove);
    }
    
    print(2, "It's your turn.");
    print(3, "You play: ");
    print(4, "");
    
    std::string move = getInput();
    std::cout << "test move: " + move;
    game->move(BaseTypes::Move(move));
}

void GameScreen::onTurnEnded(const EventData& data) {
    print(3, "You play: " + data.at("your_move"));
    print(4, "Please wait for opponent move...");
}

void GameScreen::onInvalidMove(const EventData& data) {
    print(3, "You play: " + data.at("your_move"));
    print(4, "Invalid move, please try again: ");
    std::string move = getInput();
    game->move(BaseTypes::Move(move));
}

void GameScreen::onWinGame(const EventData& data) {
    
}

void GameScreen::onLoseGame(const EventData& data) {
    
}

void GameScreen::onDrawGame(const EventData& data) {
    
}
