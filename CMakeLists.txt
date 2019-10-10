cmake_minimum_required(VERSION 3.0.0)

project(tagberry-qt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} \
    -Werror \
    -Weverything")
endif()

set(SOURCES
 resources/icons.qrc
 src/main.cpp
 src/models/Record.cpp
 src/models/RecordSet.cpp
 src/models/RecordsDirectory.cpp
 src/models/Tag.cpp
 src/models/TagsDirectory.cpp
 src/presenters/MainWindow.cpp
 src/storage/LocalStorage.cpp
 src/widgets/Calendar.cpp
 src/widgets/CalendarCell.cpp
 src/widgets/CalendarSwitch.cpp
 src/widgets/FlowLayout.cpp
 src/widgets/TagCalendar.cpp
 src/widgets/TagLabel.cpp
)

set(MOC_HEADERS
 src/models/Record.hpp
 src/models/RecordSet.hpp
 src/models/RecordsDirectory.hpp
 src/models/Tag.hpp
 src/models/TagsDirectory.hpp
 src/presenters/MainWindow.hpp
 src/widgets/Calendar.hpp
 src/widgets/CalendarCell.hpp
 src/widgets/CalendarSwitch.hpp
 src/widgets/TagCalendar.hpp
 src/widgets/TagLabel.hpp
)

include_directories(src)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_executable(${PROJECT_NAME}
  ${SOURCES} ${MOC_SOURCES})

target_link_libraries(${PROJECT_NAME}
  Qt5::Core
  Qt5::Widgets)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy
     "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json"
     "${PROJECT_SOURCE_DIR}/compile_commands.json"
     COMMENT "Copying compile_commands.json to project root")
