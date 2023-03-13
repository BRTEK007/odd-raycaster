cmake_minimum_required(VERSION 3.7)

project(SDL_RAYCASTER)

include_directories(include)

add_compile_options(-O0)

add_executable(raycaster)

file(GLOB raycaster_SRC "src/*.c")

target_sources(raycaster PRIVATE ${raycaster_SRC})

target_link_libraries(raycaster SDL2)
target_link_libraries(raycaster SDL2_ttf)

target_link_libraries(raycaster m)
target_link_libraries(raycaster pthread)

#set_target_properties(raycaster PROPERTIES LINK_FLAGS "-s USE_SDL=2 -s USE_SDL_TTF=2 --preload-file assets")#emscripten