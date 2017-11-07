cmake_minimum_required(VERSION 2.8)
project(Stockfish C CXX)
set(CMAKE_BUILD_TYPE Release)

include_directories(
	../Common/
	../Chess/ChessAI/EngineWrapper/
	../Stockfish/syzygy/ 
)

file(GLOB SOURCES 
				"*.cpp"
				"syzygy/*.cpp"
	)

add_library(Stockfish STATIC ${SOURCES})