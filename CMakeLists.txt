cmake_minimum_required(VERSION 3.10)
project(Inheritance_Client_Attempt_1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

include_directories(C:\\OpenSSL102-Win32\\include)
include_directories(C:\\OpenSSL102-Win32\\lib)
include_directories(C:\\OpenSSL102-Win32)
link_directories(C:\\OpenSSL-Win32\\lib\\MinGW)

set(OPENSSL_ROOT_DIR=C:\\OpenSSL102-Win32)
set(OPENSSL_INCLUDE_DIR=C:\\OpenSSL102-Win32\\include)
set(OPENSSL_LIBRARIES=C:\\OpenSSL102-Win32\\lib)

link_libraries(ws2_32 wsock32)

add_executable(Inheritance_Client_Attempt_1 includes/includes.h includes/Data.h
        includes/mingw.thread.h includes/mingw.mutex.h includes/mingw.shared_mutex.h
        Sources/SSLSender.cpp Headers/SSLSender.h Sources/SSLReceiver.cpp Headers/SSLReceiver.h
        Sources/IReceiver.cpp Headers/IReceiver.h Sources/ISender.cpp Headers/ISender.h
        Sources/Client.cpp Headers/Client.h Sources/IConnection.cpp Headers/IConnection.h
        Sources/IClientHandler.cpp
        Headers/IClientHandler.h Sources/ClientHandlerEcho.cpp Headers/ClientHandlerEcho.h
        Sources/SSLConnection.cpp Headers/SSLConnection.h Sources/main.cpp Sources/ClientHandlerMulti.cpp Headers/ClientHandlerMulti.h)

find_package(OpenSSL REQUIRED)
target_link_libraries(Inheritance_Client_Attempt_1 ${OPENSSL_LIBRARIES} ws2_32)
