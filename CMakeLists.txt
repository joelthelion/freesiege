PROJECT(freesiege)

INCLUDE(${CMAKE_ROOT}/Modules/FindSDL.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/FindSDL_mixer.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/FindSDL_ttf.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/FindOpenGL.cmake)

INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${SDLMIXER_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${SDLTTF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

SET(ALL_SRC
    sprite.cpp
    anim.cpp
    spritecollection.cpp

    combinaison.cpp
    combinaisoncollection.cpp
    utils.cpp
    message.cpp
    collisionmatrix.cpp

    lifebar.cpp
    background.cpp
    foreground.cpp
    battlefield.cpp
    board.cpp
    menu.cpp
    gamescreen.cpp
    menuscreen.cpp
    combinaisonscreen.cpp

    unit.cpp
    ball.cpp
    druid.cpp
    explosion.cpp
    dragon.cpp
    plant.cpp
    golem.cpp
    knight.cpp
    flame.cpp
    soldier.cpp
    veteran.cpp
    archer.cpp
    arrow.cpp
    catapult.cpp
    stone.cpp
)

OPTION(COMPILE_FOR_PACKAGE "Assume Freesiege will be installed to /usr/bin and /usr/share" OFF)
IF(COMPILE_FOR_PACKAGE)
	ADD_DEFINITIONS(-DFREESIEGE_ISINSTALLED)
ENDIF(COMPILE_FOR_PACKAGE)
ADD_EXECUTABLE(fight fight.cpp ${ALL_SRC})
TARGET_LINK_LIBRARIES(fight  ${SDLMIXER_LIBRARY} ${SDL_LIBRARY} ${SDLTTF_LIBRARY} ${OPENGL_gl_LIBRARY})

ADD_EXECUTABLE(elements elements.cpp ${ALL_SRC})
TARGET_LINK_LIBRARIES(elements ${SDLMIXER_LIBRARY} ${SDL_LIBRARY} ${SDLTTF_LIBRARY} ${OPENGL_gl_LIBRARY})
