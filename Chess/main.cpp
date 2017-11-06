//
//  main.cpp
//  Chess
//
//  Created by Tran Hoang Duong on 8/20/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

//#include <iostream>
//#include "StockfishEngine.h"
//#include "thread.h"
//#include "NodeConnectorImplementation.h"

//#include <cpprest/http_client.h>
//#include <cpprest/filestream.h>

//using namespace utility;                    // Common utilities like string conversions
//using namespace web;                        // Common features like URIs.
//using namespace web::http;                  // Common HTTP functionality
//using namespace web::http::client;          // HTTP client features
//using namespace concurrency::streams;       // Asynchronous streams

//void playerTurn();
//void computerTurn();
//
//std::atomic<bool> isPlayerTurn (false);
//StockfishEngine* engine;

//void playerTurn() {
//    std::cout << "\nYou play: ";
//    std::string input;
//    std::cin >> input;
//    bool result = engine->move(BaseTypes::Move(input));
//    if (result) {
//        engine->log();
//        isPlayerTurn = false;
//        computerTurn();
//    } else {
//        std::cout << "\nInvalid move!!!";
//        playerTurn();
//    }
//}
//
//void computerTurn() {
//    engine->calculate([=](BaseTypes::Move move) {
//        std::cout << "\nCom play: " << move.fromPos.file << move.fromPos.rank << move.toPos.file << move.toPos.rank;
//        engine->move(move);
//        engine->log();
//        isPlayerTurn = true;
//    });
//    Threads.main()->wait(isPlayerTurn);
//    playerTurn();
//}
//
//void startGame(bool playerGoFirst) {
//    if (playerGoFirst) {
//        playerTurn();
//    } else {
//        computerTurn();
//    }
//}
//
//void OnConnected() {
//    std::cout << "On connect";
//}

//int main(int argc, const char * argv[]) {
//    auto fileStream = std::make_shared<ostream>();
//
//    pplx::task<void> requestTask = fstream::open_ostream(U("result.html"))
//    .then([=](ostream outFile) {
//        *fileStream = outFile;
//
//        http_client client(U("http://www.bing.com"));
//
//        uri_builder builder(U("/search"));
//        builder.append_query(U("q"), U("xvideos.com"));
//        return client.request(methods::GET, builder.to_string());
//    })
//    .then([=](http_response response) {
//        std::cout << "Received response status code: " << response.status_code();
//
//        return response.body().read_to_end(fileStream->streambuf());
//    })
//    .then([=](size_t) {
//        return fileStream->close();
//    });
//
//    try {
//        requestTask.wait();
//    } catch (const std::exception &e) {
//        std::cout << "Error exception: " << e.what() << std::endl;
//    }
    
//    sio::client h;
//    std::string name = "Duong H. Tran";
//    h.connect("http://localhost:8080");
//    h.set_socket_open_listener([&](std::string nsp) {
//        std::cout << "On connect: " << nsp << std::endl;
//        h.socket()->emit("hello", name);
//    });
//
//    h.socket()->on("hello", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
//        std::cout << "On hello" << std::endl;
//
//        std::string username = message->get_map()["id"]->get_string();
//
//        std::cout << "Message: " << message->get_map()["message"]->get_string() << std::endl;
//        std::cout << "User id: " << username << std::endl;
//
//        auto msg = sio::object_message::create();
//        msg->get_map()["name"] = sio::string_message::create(name);
//
//        h.socket()->emit("createGame", msg);
//    });
//
//    h.socket()->on("joinGameSucceeded", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
//        std::cout << "On joinGameSucceeded" << std::endl;
//    });
//
//    h.socket()->on("joinGameFailed", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
//        std::cout << "On joinGameFailed" << std::endl;
//        std::cout << "Message: " << message->get_map()["message"]->get_string() << std::endl;
//    });
    
//    engine = StockfishEngine::getInstance();
//    engine->start(1);
//    std::cout << "\nGame started!!!";
//    engine->log();
//
//    startGame(true);
    
    
//}

#include "MainMenuScreen.h"

int main(int argc, char* argv[]) {
    Screen::replaceScreen(MainMenuScreen::create(20, 4));
}

