cmake_minimum_required(VERSION 3.5)

project(GUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        cache.cpp
        memory.cpp
        cache.h
        memory.h
        register_file.cpp
        register_file.h
        program_counter.cpp
        program_counter.h
        Write_Back.cpp
        Write_Back.h
        Set_Assoc_Cache.h
        Set_Assoc_Cache.cpp
        Mem_Access.h
        Mem_Access.cpp
        Instr_Fetch.h
        Instr_Fetch.cpp
        Instr_Decode.h
        Instr_Decode.cpp
        Execution.h
        Execution.cpp
        Direct_Mapped_Cache.cpp
        Direct_Mapped_Cache.h

        clock.cpp
        clock.h
        B.cpp
        HALT.cpp I_Basic.cpp I_JALR.cpp I_Load.cpp Instruction.cpp JAL.cpp NO-OP.cpp R_or_M.cpp S.cpp STALL.cpp U_AUIPC.cpp U_LUI.cpp Instr_Fetch.cpp
        register_file.cpp B.h HALT.h I_Basic.h I_JALR.h I_Load.cpp
        I_Load.h JAL.h NO-OP.h R_or_M.h S.h STALL.h U_AUIPC.h U_LUI.h
        Instruction.cpp Instruction.h



    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(GUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GUI
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GUI)
endif()
set_target_properties(GUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GUI)
endif()

# Create a library for cache and memory
#add_library(CacheAndMemory STATIC Set_Assoc_Cache.cpp Direct_Mapped_Cache.cpp Cache.cpp memory.cpp)
#target_include_directories(CacheAndMemory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Create a library for program_counter
#add_library(ProgramCounter STATIC program_counter.cpp)
#target_include_directories(ProgramCounter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Create a library for register_file
#add_library(RegisterFile STATIC register_file.cpp)
#target_include_directories(RegisterFile PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Get all .cpp files in the source directory
#file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Remove the files already added to libraries from the list
#list(REMOVE_ITEM SOURCE_FILES   "${CMAKE_CURRENT_SOURCE_DIR}/Set_Assoc_Cache.cpp"
#                                "${CMAKE_CURRENT_SOURCE_DIR}/Direct_Mapped_Cache.cpp"
#                                "${CMAKE_CURRENT_SOURCE_DIR}/Cache.cpp"
#                                "${CMAKE_CURRENT_SOURCE_DIR}/memory.cpp"
#                                "${CMAKE_CURRENT_SOURCE_DIR}/program_counter.cpp"
#                                "${CMAKE_CURRENT_SOURCE_DIR}/register_file.cpp")

# Create the MYSIMULATOR executable
#add_executable(MYSIMULATOR ${SOURCE_FILES})

# Link the libraries to the MYSIMULATOR target
#target_link_libraries(MYSIMULATOR CacheAndMemory ProgramCounter RegisterFile Instructions)

#message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
