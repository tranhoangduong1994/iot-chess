cmake_minimum_required(VERSION 2.8)
project(Network)
set(CMAKE_BUILD_TYPE Release)

include_directories(
					../Common/
					NetworkEventsProtocol/
					NodeServerEventsImplementation/
					../../socket.io-client-cpp/build/include/
					)

#link_directories(
#				../../socket.io-client-cpp/build/lib/Release/
#				)

#target_link_libraries(
#					Network
#					libsioclient.a
#					)

file(GLOB SOURCES 
				"NodeServerEventsImplementation/*.cpp" 
				"NetworkEventsProtocol/*.cpp"
	)

add_library(Network STATIC ${SOURCES})