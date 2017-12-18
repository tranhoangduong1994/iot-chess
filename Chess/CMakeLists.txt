cmake_minimum_required(VERSION 2.8)
project(iot-chess C CXX)
set(CMAKE_BUILD_TYPE Release)

file(GLOB SOURCES 
				"*.cpp"
				"Hardware/Displayer/*.cpp"
				"Hardware/KeyboardController/*.cpp"
				"Hardware/Services/*.cpp"
				"Software/GameController/*.cpp"
				"Software/Screen/*.cpp"
				"Software/ScreenImplementors/*.cpp"
				"Software/Wrappers/Engine/*.cpp"
				"Software/Wrappers/PythonHelper/*.cpp"
				"Software/Wrappers/Validator/*.cpp"
	)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost COMPONENTS filesystem system date_time python REQUIRED)
message("Include dirs of boost: " ${Boost_INCLUDE_DIRS} )
message("Libs of boost: " ${Boost_LIBRARIES} )

find_package(PythonLibs 2.7 REQUIRED)
message("Include dirs of Python: " ${PYTHON_INCLUDE_DIRS} )
message("Libs of Python: " ${PYTHON_LIBRARIES} )

add_executable(iot-chess ${SOURCES})

include_directories(
					../Common/
					./
					Hardware/Displayer
					Hardware/KeyboardController
					Hardware/Services
					Software/GameController
					Software/Screen
					Software/ScreenImplementors
					Software/Wrappers/Engine
					Software/Wrappers/PythonHelper
					Software/Wrappers/Validator
					../Stockfish/
					#[[../Network/NetworkEventsProtocol/
					../Network/NodeServerEventsImplementation/
					../../socket.io-client-cpp/build/include/]]
				    ${Boost_INCLUDE_DIRS}
    				${PYTHON_INCLUDE_DIRS}
					)


link_directories(
				../../socket.io-client-cpp/build/lib/Release/
			    ${Boost_INCLUDE_DIRS}
			    ${PYTHON_INCLUDE_DIRS}
				)

target_link_libraries(
					iot-chess 
					Stockfish
					#[[Network
					#sioclient]]
					${Boost_LIBRARIES}
					${PYTHON_LIBRARIES}
					Threads::Threads
					)

install(TARGETS iot-chess DESTINATION bin)