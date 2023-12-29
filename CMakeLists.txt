cmake_minimum_required(VERSION 3.12)

# Set the project name
project(Blertning)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the executable
add_executable(Blertning main.cpp FileInfo.cpp SqliteDataAccess.cpp)

# If there are any libraries that need to be linked, use target_link_libraries
# For example, if you need to link against the ssl, crypto, and sqlite3 libraries, you would do:
find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)
target_link_libraries(Blertning OpenSSL::SSL OpenSSL::Crypto sqlite3)