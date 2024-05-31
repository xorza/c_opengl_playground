cmake_minimum_required(VERSION 3.28)
project(c_playground C)

set(CMAKE_C_STANDARD 23)

add_executable(c_playground main.c)

target_include_directories(c_playground PRIVATE
        libs/glew-2.2.0/include
        libs/SDL2-2.30.3/x86_64-w64-mingw32/include
        "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/um"
)
target_link_directories(c_playground PRIVATE
        libs/SDL2-2.30.3/x86_64-w64-mingw32/lib
        libs/glew-2.2.0/lib/Release/x64
        "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64"
)
target_link_libraries(c_playground SDL2 glew32s opengl32)