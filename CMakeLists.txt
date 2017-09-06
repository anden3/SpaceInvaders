cmake_minimum_required(VERSION 3.0.2)
project(SpaceInvaders)

set(SOURCE_PATH ${CMAKE_CURRENT_LIST_DIR}/src)
set(EXEC_LOCATION ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME})
set(EXEC_LIBS ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}.app/Contents/Libraries)
set(REL_LIB @executable_path/../Libraries)

set(INT_ID install_name_tool -id)
set(INT_CH install_name_tool -change)

set(SOURCES
    ${SOURCE_PATH}/Buffer.cpp
    ${SOURCE_PATH}/main.cpp
    ${SOURCE_PATH}/Shader.cpp
    ${SOURCE_PATH}/Sprite.cpp
)

set(LIBRARIES
    freeimage freeimageplus GLEW glfw3
)

if (WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "/MACHINE:X64")
	set(CMAKE_MODULE_LINKER_FLAGS "/MACHINE:X64")
	set(CMAKE_SHARED_LINKER_FLAGS "/MACHINE:X64")
	set(CMAKE_STATIC_LINKER_FLAGS "/MACHINE:X64")
    
    set(CMAKE_CXX_STANDARD 14)

    set(INCLUDE_DIRS
        ${SOURCE_PATH} F:/include
    )
    set(LIBRARY_DIRS
        F:/lib
    )

    list(APPEND LIBRARIES opengl32 winmm ws2_32)

    set(CMAKE_GENERATOR "Visual Studio 14 2015 Win64" CACHE INTERNAL "" FORCE)

elseif (APPLE)
    set(INCLUDE_DIRS
        ${SOURCE_PATH} /usr/local/include
    )
    
    set(LIBRARY_DIRS
        /usr/local/lib
    )

    find_library(OPENGL_LIB OpenGL)

    list(APPEND LIBRARIES
        ${OPENGL_LIB}
    )

elseif (UNIX)
    set(INCLUDE_DIRS
        ${SOURCE_PATH} /usr/local/include
    )

    set(LIBRARY_DIRS
        /usr/local/lib
    )

    find_library(OPENGL_LIB OpenGL)

    list(APPEND LIBRARIES
        ${OPENGL_LIB}
    )
endif()

list (APPEND LIBRARIES GLEW glfw3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_LIST_DIR})

include_directories(${INCLUDE_DIRS})
link_directories(${LIBRARY_DIRS})
add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)


if (APPLE)
    target_compile_options(${PROJECT_NAME} PUBLIC
        -O0 -g
        -arch x86_64 -std=gnu++14 -F Build -MMD -MT dependencies -Weverything
        -Wno-missing-braces -Wno-c++98-compat -Wno-c++98-compat-pedantic
        -Wno-exit-time-destructors -Wno-global-constructors -Wno-non-virtual-dtor
        -Wno-padded -Wno-float-equal -Wno-disabled-macro-expansion -Wno-reserved-id-macro
        -Wno-header-hygiene -Wno-sign-conversion -Wno-old-style-cast -Wno-deprecated
        -Wno-missing-prototypes -Wno-unused-parameter -Wno-gnu-anonymous-struct
        -Wno-nested-anon-types
    )

    set(LIBS
        freeimage freeimageplus GLEW glfw3
    )

    set(FILES_TO_COPY
        atlas.png
    )

    set(FOLDERS_TO_COPY
        Shaders
    )

    foreach (f ${LIBS})
        list(APPEND LIB_PATHS "/usr/local/lib/lib${f}.dylib")
    endforeach(f)

    add_custom_command(TARGET ${PROJECT_NAME} PRE_LINK
        COMMAND mkdir -p ${PROJECT_NAME}.app/Contents/MacOS
        COMMAND mkdir -p ${EXEC_LIBS}
        COMMAND mkdir -p ${PROJECT_NAME}.app/Contents/Resources

        COMMAND rsync -au ${FOLDERS_TO_COPY} ${PROJECT_NAME}.app/Contents/Resources
        COMMAND rsync -au ${FILES_TO_COPY} ${PROJECT_NAME}.app/Contents/Resources
        COMMAND rsync -Lr --chmod=+rw ${LIB_PATHS} ${EXEC_LIBS}

        COMMAND ${INT_ID} ${REL_LIB}/libFreeImage.dylib ${EXEC_LIBS}/libFreeImage.dylib

        COMMAND ${INT_ID} ${REL_LIB}/libglew.dylib ${EXEC_LIBS}/libglew.dylib
        COMMAND ${INT_ID} ${REL_LIB}/libglfw3.dylib ${EXEC_LIBS}/libglfw3.dylib

        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND mv ${PROJECT_NAME} ${EXEC_LOCATION}

        COMMAND install_name_tool -add_rpath @loader_path/../Libraries ${EXEC_LOCATION}

        COMMAND ${INT_CH} /usr/local/opt/freeimage/lib/libfreeimage.dylib @rpath/libfreeimage.dylib ${EXEC_LOCATION}
        COMMAND ${INT_CH} /usr/local/opt/glew/lib/libGLEW.1.13.0.dylib @rpath/libglew.dylib ${EXEC_LOCATION}
        COMMAND ${INT_CH} /usr/local/opt/glfw3/lib/libglfw3.3.dylib @rpath/libglfw3.dylib ${EXEC_LOCATION}

        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )

endif()

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
