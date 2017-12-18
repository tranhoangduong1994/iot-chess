//
//  MessageController.cpp
//  Chess
//
//  Created by macbook on 11/25/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "MessageController.h"
#include "BoardServices.h"
#include <thread>

#if defined(__linux) || defined(linux) || defined(__linux)
#include <wiringPi.h>
#include <wiringSerial.h>
#endif

#if defined(__APPLE__) || defined(__MACH__)
#include "wiringPi.h"
#include "wiringSerial.h"
#endif

const char* PORT_PREFIX = "/dev/ttyUSB";
const int MAX_PORT_INDEX = 254;
const int BAUD = 9600;

MessageController* MessageController::instance = NULL;

MessageController* MessageController::getInstance() {
    if (!instance) {
        instance = new MessageController();
        instance->init();
    }
    
    return instance;
}

void MessageController::send(ServiceRequestType type, std::string content) {
	std::string messageToSend = std::to_string(type) + content + "|";
	std::cout << "[MessageController] send: " << messageToSend << std::endl;
	std::cout << "[MessageController] message length = " << messageToSend.size() << std::endl;
	for (int i = 0; i < messageToSend.size(); i++) {
		std::cout << "Char " << i << ": " << messageToSend.at(i) << ", int value = " << std::to_string((int)messageToSend.at(i)) << std::endl;
	}
    serialPuts(fileDescription, messageToSend.c_str());
    serialFlush(fileDescription);
}

void MessageController::checkMessage() {
    char c = serialGetchar(fileDescription);
    if (c == '|') {
        processMessageBuffer();
        messageBuffer = "";
        return;
    }
    messageBuffer += c;
}

void MessageController::processMessageBuffer() {
	std::cout << "[MessageController] processMessageBuffer: " << messageBuffer << std::endl;
	if (messageBuffer.size() < 2) {
		return;
	}
    int messageType = messageBuffer.at(0) - 48;
    int messageHeader = messageBuffer.at(1) - 48;
    if (messageType == MessageType::ServiceResponse) {
        if (messageHeader == ServiceResponseType::MOVE_DONE) {
            if (gDelegator) {
				std::string move = messageBuffer.substr(2, 4);
				std::string newState = messageBuffer.substr(6);
                gDelegator->onOpponentFinishedMove(move, BaseTypes::Bitboard(newState));
            }
            return;
        }
        
        if (messageHeader == ServiceResponseType::SCAN_DONE) {
            if (gDelegator) {
                gDelegator->onScanDone(messageBuffer.substr(2));
            }
            return;
        }
        
        if (messageHeader == ServiceResponseType::RESET_DONE) {
            if (gDelegator) {
				std::string newState = messageBuffer.substr(2);
                gDelegator->onBoardResetted(BaseTypes::Bitboard(newState));
            }
            return;
        }
    } else if (messageType == MessageType::Event) {
        if (messageHeader == EventType::SYSTEM_READY) {
            if (sDelegator) {
                SerialPortConnectedData data(fileDescription);
                sDelegator->onSerialPortConnected(data);
            }
        }
    }
}

void MessageController::init() {
    fileDescription = -1;
    messageBuffer = "";
    
    sDelegator = NULL;
    gDelegator = NULL;
    
    initSerialPort();
}

void MessageController::initSerialPort() {
    std::thread([=]() {
        int valueToTry = 0;
        usbPortNumber = "";
        while (fileDescription < 0) {
            usbPortNumber = std::string(PORT_PREFIX) + std::to_string(valueToTry);
            valueToTry = (valueToTry + 1) % (MAX_PORT_INDEX + 1);
            std::cout << "Trying port " << usbPortNumber << std::endl;
            fileDescription = serialOpen(usbPortNumber.c_str(), BAUD);
        }
        startLoop();
    }).detach();
}

void MessageController::startLoop() {
    while (true) {
        if ((fileDescription = serialOpen(usbPortNumber.c_str(), BAUD)) < 0) {
            break;
        }
        checkMessage();
    }
    
    if (sDelegator) {
        sDelegator->onSerialPortDisconnected();
    }
}

void MessageController::setBoardSystemEventsDelegator(BoardSystemEventsProtocol* delegator) {
    sDelegator = delegator;
}

void MessageController::setBoardServicesEventsDelegator(BoardServicesEventsProtocol* delegator) {
    gDelegator = delegator;
}
