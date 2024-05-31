cmake_minimum_required(VERSION 3.28)
project(c_playground CXX)

set(CMAKE_CXX_STANDARD 23)


add_subdirectory(libs/glm/ EXCLUDE_FROM_ALL)

file(GLOB SRC_FILES "src/*.cpp")

add_executable(c_playground ${SRC_FILES})

target_include_directories(c_playground PRIVATE
        libs/include
        libs/glm
        libs/glew-2.2.0/include
        libs/SDL2-2.30.3/x86_64-w64-mingw32/include
        "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/um"
)
target_link_directories(c_playground PRIVATE
        libs/SDL2-2.30.3/x86_64-w64-mingw32/lib
        libs/glew-2.2.0/lib/Release/x64
        "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64"
)
target_link_libraries(c_playground PRIVATE
        SDL2
        glew32s
        opengl32
        glm
)

target_compile_options(c_playground PRIVATE
        -fno-exceptions
        -fno-rtti
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-unused-function
        -Wno-unused-but-set-variable
        -Wno-unused-value
        -Wno-unused-local-typedefs
#        -Wno-unused-private-field
        -Wno-unused-const-variable
)