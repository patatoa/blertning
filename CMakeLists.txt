cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Blertning)

# Add the executable
add_executable(Blertning main.cpp FileInfo.cpp SqliteDataAccess.cpp)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# If there are any libraries that need to be linked, use target_link_libraries
# For example, if you need to link against the ssl, crypto, and sqlite3 libraries, you would do:
target_link_libraries(Blertning stdc++fs ssl crypto sqlite3)