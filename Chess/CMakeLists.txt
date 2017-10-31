cmake_minimum_required(VERSION 2.8)
project(iot-chess)
set(CMAKE_BUILD_TYPE Release)

file(GLOB SOURCES 
				"*.cpp" 
				"GameController/*.cpp" 
				"GameLogicProcessor/Engine/*.cpp"
				"GameLogicProcessor/Validator/*.cpp"
	)

#set(Boost_INCLUDE_DIR /usr/local/Cellar/boost/1.65.1)
#set(Boost_LIBRARY_DIR /usr/local/Cellar/boost/1.65.1/lib)
#set(BoostPython_INCLUDE_DIR /usr/local/Cellar/boost-python/1.65.1)
#set(BoostPython_LIBRARY_DIR /usr/local/Cellar/boost-python/1.65.1/lib)
#find_package(Boost COMPONENTS system filesystem REQUIRED)
#find_package(Boost COMPONENTS python REQUIRED)

find_package(Boost COMPONENTS filesystem system date_time python REQUIRED)
message("Include dirs of boost: " ${Boost_INCLUDE_DIRS} )
message("Libs of boost: " ${Boost_LIBRARIES} )

find_package(PythonLibs 2.7 REQUIRED)
message("Include dirs of Python: " ${PYTHON_INCLUDE_DIRS} )
message("Libs of Python: " ${PYTHON_LIBRARIES} )

add_executable(iot-chess ${SOURCES})

include_directories(
					../Common/
					GameLogicProcessor/Engine/
					GameLogicProcessor/Validator/
					../Stockfish/
					../Network/NetworkEventsProtocol/
					../Network/NodeServerEventsImplementation/
					../../socket.io-client-cpp/build/include/
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
					Network
					#sioclient
					${Boost_LIBRARIES}
					${PYTHON_LIBRARIES}
					)

install(TARGETS iot-chess DESTINATION bin)