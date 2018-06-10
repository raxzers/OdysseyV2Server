cmake_minimum_required(VERSION 3.9)
project(odysseyServer2_0)

set(CMAKE_CXX_STANDARD 11)
find_package (Threads)
find_package(Boost COMPONENTS system REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

add_executable(odysseyServer2_0 main.cpp ServerHandler.cpp ServerHandler.h Server.cpp Server.h Session.cpp Session.h RequestHandler.cpp RequestHandler.h Metadata.cpp Metadata.h base64.cpp base64.h User.cpp User.h Sorter.cpp Sorter.h)
    target_link_libraries(odysseyServer2_0 ${Boost_LIBRARIES})
endif()

target_link_libraries (odysseyServer2_0 ${CMAKE_THREAD_LIBS_INIT})