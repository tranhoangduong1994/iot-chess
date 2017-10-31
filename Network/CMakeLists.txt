cmake_minimum_required(VERSION 2.8)
project(Network C CXX)
set(CMAKE_BUILD_TYPE Release)

include_directories(
					../Common/
					NetworkEventsProtocol/
					NodeServerEventsImplementation/
					../../socket.io-client-cpp/build/include/
					)

file(GLOB SOURCES 
				"NodeServerEventsImplementation/*.cpp" 
				"NetworkEventsProtocol/*.cpp"
	)

add_library(Network STATIC ${SOURCES})