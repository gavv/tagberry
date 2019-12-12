cmake_minimum_required(VERSION 3.0.0)

project(tagberry-qt)

include(ExternalProject)
include(ProcessorCount)

ProcessorCount(CPU_COUNT)

ExternalProject_Add(qsqlmigrator
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/3rdparty/QSqlMigrator
  BUILD_IN_SOURCE ON
  LOG_BUILD ON
  CONFIGURE_COMMAND qmake -r
    "CONFIG+=static staticlib NO_QSM_TESTS NO_QSM_MYSQL NO_QSM_POSTGRES NO_QSM_FIREBIRD"
  BUILD_COMMAND make "-j${CPU_COUNT}"
  INSTALL_COMMAND ""
)

include_directories(
  ${PROJECT_SOURCE_DIR}/3rdparty/QSqlMigrator/src
  ${PROJECT_SOURCE_DIR}/3rdparty/QSqlMigrator/plugins
)

link_directories(
  ${PROJECT_SOURCE_DIR}/3rdparty/QSqlMigrator/bin
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Weverything")
endif()

set(SOURCES
 resources/icons.qrc
 src/main.cpp
 src/models/ColorScheme.cpp
 src/models/Record.cpp
 src/models/RecordSet.cpp
 src/models/RecordsDirectory.cpp
 src/models/Root.cpp
 src/models/Tag.cpp
 src/models/TagsDirectory.cpp
 src/presenters/CalendarArea.cpp
 src/presenters/MainWindow.cpp
 src/presenters/RecordsArea.cpp
 src/storage/LocalStorage.cpp
 src/storage/Migrator.cpp
 src/storage/migrations/01_CreateTables.cpp
 src/storage/migrations/02_AddRecordState.cpp
 src/widgets/Cell.cpp
 src/widgets/Calendar.cpp
 src/widgets/CalendarCell.cpp
 src/widgets/CalendarSwitch.cpp
 src/widgets/FlowLayout.cpp
 src/widgets/RecordCell.cpp
 src/widgets/RecordList.cpp
 src/widgets/TagCalendar.cpp
 src/widgets/TagLabel.cpp
 src/widgets/TagSelector.cpp
)

set(MOC_HEADERS
 src/models/ColorScheme.hpp
 src/models/Record.hpp
 src/models/RecordSet.hpp
 src/models/RecordsDirectory.hpp
 src/models/Root.hpp
 src/models/Tag.hpp
 src/models/TagsDirectory.hpp
 src/presenters/CalendarArea.hpp
 src/presenters/MainWindow.hpp
 src/presenters/RecordsArea.hpp
 src/widgets/Cell.hpp
 src/widgets/Calendar.hpp
 src/widgets/CalendarCell.hpp
 src/widgets/CalendarSwitch.hpp
 src/widgets/RecordCell.hpp
 src/widgets/RecordList.hpp
 src/widgets/TagCalendar.hpp
 src/widgets/TagLabel.hpp
 src/widgets/TagSelector.hpp
)

include_directories(src)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)

qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_executable(tagberry-qt
  ${SOURCES} ${MOC_SOURCES})

add_dependencies(tagberry-qt qsqlmigrator)

target_link_libraries(tagberry-qt
  Qt5::Core
  Qt5::Widgets
  Qt5::Sql
  SqliteMigrator
  QSqlMigrator)

install(
  TARGETS tagberry-qt
  RUNTIME DESTINATION bin)

install(
  FILES resources/desktop/tagberry-qt.desktop
  DESTINATION share/applications)

install(
  FILES resources/desktop/icon-256.png
  RENAME tagberry-qt.png
  DESTINATION share/icons/hicolor/256x256/apps)

install(
  FILES resources/desktop/icon-128.png
  RENAME tagberry-qt.png
  DESTINATION share/icons/hicolor/128x128/apps)

install(
  FILES resources/desktop/icon-64.png
  RENAME tagberry-qt.png
  DESTINATION share/icons/hicolor/64x64/apps)

install(
  FILES resources/desktop/icon-48.png
  RENAME tagberry-qt.png
  DESTINATION share/icons/hicolor/48x48/apps)

add_custom_command(TARGET tagberry-qt POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy
     "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json"
     "${PROJECT_SOURCE_DIR}/compile_commands.json"
     COMMENT "Copying compile_commands.json to project root")
