//
//  GameSettingScreen.cpp
//  Chess
//
//  Created by macbook on 12/11/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "GameSettingScreen.h"
#include "OptionScreen.h"
#include "GameController.h"
#include "GameScreen.h"

GameSettingScreen* GameSettingScreen::create() {
    GameSettingScreen* screen = new GameSettingScreen();
    return screen;
}

GameSettingScreen::GameSettingScreen() : OptionScreen("GAME SETTINGS", std::vector<OptionScreenEntry>()) {
    OptionScreenEntry sideEntry;
    sideEntry.name = "Side: WHITE";
    sideEntry.onSelected = [=](std::string name) {
        std::vector<OptionScreenEntry> sideEntries;
        
        OptionScreenEntry whiteEntry;
        whiteEntry.name = "WHITE";
        whiteEntry.onSelected = [=](std::string name) {
            side = BaseTypes::Side::WHITE;
            Screen::popScreen();
        };
        sideEntries.push_back(whiteEntry);
        
        OptionScreenEntry blackEntry;
        blackEntry.name = "BLACK";
        blackEntry.onSelected = [=](std::string name) {
            side = BaseTypes::Side::BLACK;
            Screen::popScreen();
        };
        sideEntries.push_back(blackEntry);
        
        Screen::pushScreen(OptionScreen::create("Select side", sideEntries));
    };
    entries.push_back(sideEntry);
    
    OptionScreenEntry difficultyEntry;
    difficultyEntry.name = "Difficulty: 10";
    difficultyEntry.onSelected = [=](std::string name) {
        std::vector<OptionScreenEntry> difficultyEntries;
        
        for (int i = 0; i < 20; i++) {
            OptionScreenEntry subEntry;
            subEntry.name = std::to_string(i + 1);
            subEntry.onSelected = [=](std::string name) {
                difficulty = atoi(name.c_str());
                Screen::popScreen();
            };
            difficultyEntries.push_back(subEntry);
        }
        
        Screen::pushScreen(OptionScreen::create("Select difficulty", difficultyEntries));
    };
    entries.push_back(difficultyEntry);
    
    OptionScreenEntry startGameEntry;
    startGameEntry.name = "START GAME";
    startGameEntry.onSelected = [=](std::string content) {
        GameController* gameController = new GameController(side, difficulty);
        GameScreen* gameScreen = GameScreen::create(gameController);
        Screen::runScreen(gameScreen);
    };
    entries.push_back(startGameEntry);
    
    init();
}

void GameSettingScreen::init() {
    OptionScreen::init();
    side = BaseTypes::Side::WHITE;
    difficulty = 10;
}

void GameSettingScreen::onEnter() {
    entries.at(0).name = (side == BaseTypes::Side::WHITE ? "Side: <WHITE>" : "Side: <BLACK>");
    entries.at(1).name = std::string("Difficulty: ") + std::to_string(difficulty);
    OptionScreen::onEnter();
}

void GameSettingScreen::onExit() {
    OptionScreen::onExit();
}
