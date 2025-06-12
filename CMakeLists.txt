cmake_minimum_required(VERSION 3.10)
project(BouncingBall C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(BouncingBall src/main.c)

target_include_directories(BouncingBall PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.32.6/x86_64-w64-mingw32/include

    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.32.6/x86_64-w64-mingw32/include/SDL2
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2_image-2.8.3/x86_64-w64-mingw32/include


    ${CMAKE_CURRENT_SOURCE_DIR}/plug
)




#IMPORTANT NOTE:

#1) Standard C/C++ console applications have main() as their entry point and Windows GUI applications have WinMain() as their entry point

#2) SDL2 provides SDL2main, that impelements the platform-specific entry point(WinMain on Windows, as it is a cross platform library) and then calls our standard int main(int argc, char *argv[]) function.

#3) So, when we use SDL2, we write a main() function, and libSDL2main.a provides the necessary WinMain wrapper

#4) The 'mingw32' ie libmingw32.a library is a core part of the MinGW toolchain. It contains essential startup code and runtime support for compiled C/C++ programs on Windows

#5) This includes the code that initializes the C runtime environment and calls the application's entry point.

#6) When mingw32 is linked, it provides the necessary MinGW-specific runtime environment and startup files. libSDL2main.a is designed to work within this environment, providing its WinMain implementation which then calls our 'main'

#7) If we remove mingw32 from the explicit link list, the interaction between the default MinGW startup procedures and libSDL2main.a is broken and we get error: WinMain not found


#NOTE: If we don't use 'ming32', the alternative is to #define SDL_MAIN_HANDLED

target_link_libraries(BouncingBall PRIVATE
    mingw32
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.32.6/x86_64-w64-mingw32/lib/libSDL2main.a
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.32.6/x86_64-w64-mingw32/lib/libSDL2.a
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2_image-2.8.3/x86_64-w64-mingw32/lib/libSDL2_image.a
    # ${CMAKE_CURRENT_SOURCE_DIR}/build/libplug.dll.a
    setupapi
    imm32
    version
    winmm
    kernel32
    user32
    gdi32
    winspool
    shell32
    ole32
    oleaut32
    uuid
    comdlg32
    advapi32
)
