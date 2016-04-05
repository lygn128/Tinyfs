cmake_minimum_required(VERSION 3.0)
project(tfnode)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp server.cpp server.h config.cpp config.h sds.cpp sds.h Global.h NimbleStore.cpp NimbleStore.h utils.cpp utils.h Event.cpp Event.h Connection.cpp Connection.h Packet.cpp Packet.h  LittleEndian.cpp LittleEndian.h Types.cpp Types.h commons/Logger.cpp commons/Logger.h)
add_executable(tfnode ${SOURCE_FILES})