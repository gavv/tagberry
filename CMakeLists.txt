cmake_minimum_required(VERSION 3.0.0)

project(tagberry-qt)

include(ExternalProject)
include(ProcessorCount)

ProcessorCount(CPU_COUNT)

ExternalProject_Add(qsqlmigrator
  SOURCE_DIR "${PROJECT_SOURCE_DIR}/3rdparty/QSqlMigrator"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/qsqlmigrator-build"
  LOG_BUILD ON
  CONFIGURE_COMMAND qmake -r
    "${PROJECT_SOURCE_DIR}/3rdparty/QSqlMigrator/QSqlMigrator.pro"
    "CONFIG+=static staticlib NO_QSM_TESTS NO_QSM_MYSQL NO_QSM_POSTGRES NO_QSM_FIREBIRD"
  BUILD_COMMAND make "-j${CPU_COUNT}"
  INSTALL_COMMAND ""
)

include_directories(SYSTEM
  "${PROJECT_SOURCE_DIR}/3rdparty/QSqlMigrator/src"
  "${PROJECT_SOURCE_DIR}/3rdparty/QSqlMigrator/plugins"
)

link_directories(
  "${PROJECT_SOURCE_DIR}/3rdparty/QSqlMigrator/bin"
)

ExternalProject_Add(qmarkdowntextedit
  SOURCE_DIR "${PROJECT_SOURCE_DIR}/3rdparty/QMarkdownTextEdit"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/qmarkdowntextedit-build"
  LOG_BUILD ON
  CONFIGURE_COMMAND qmake -r
    "${PROJECT_SOURCE_DIR}/3rdparty/QMarkdownTextEdit/qmarkdowntextedit.pro"
    "CONFIG+=static staticlib"
  BUILD_COMMAND make "-j${CPU_COUNT}"
  INSTALL_COMMAND ""
)

include_directories(SYSTEM
  "${PROJECT_SOURCE_DIR}/3rdparty/QMarkdownTextEdit"
)

link_directories(
  "${CMAKE_CURRENT_BINARY_DIR}/qmarkdowntextedit-build"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} \
    -Wall \
    -Wextra")
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} \
    -Weverything \
    -Wno-system-headers \
    -Wno-padded \
    -Wno-covered-switch-default \
    -Wno-global-constructors \
    -Wno-exit-time-destructors \
    -Wno-c++98-compat \
    -Wno-c++98-compat-pedantic \
    -Wno-c++98-c++11-c++14-compat")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} \
      -Werror \
      -ggdb \
      -fno-omit-frame-pointer")
  endif()
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(SANITIZE_FLAGS
      "-fsanitize=undefined \
      -fsanitize=address")
    set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} ${SANITIZE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZE_FLAGS}")
  endif()
endif()

set(SOURCES
 3rdparty/QMarkdownTextEdit/media.qrc
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
 src/sanitizers.cpp
 src/storage/LocalStorage.cpp
 src/storage/Migrator.cpp
 src/storage/migrations/01_CreateTables.cpp
 src/storage/migrations/02_AddRecordState.cpp
 src/widgets/Calendar.cpp
 src/widgets/CalendarCell.cpp
 src/widgets/CalendarSwitch.cpp
 src/widgets/Cell.cpp
 src/widgets/FlowLayout.cpp
 src/widgets/RecordCell.cpp
 src/widgets/RecordList.cpp
 src/widgets/TagCalendar.cpp
 src/widgets/TagLabel.cpp
 src/widgets/TagListEdit.cpp
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
 src/widgets/Calendar.hpp
 src/widgets/CalendarCell.hpp
 src/widgets/CalendarSwitch.hpp
 src/widgets/Cell.hpp
 src/widgets/RecordCell.hpp
 src/widgets/RecordList.hpp
 src/widgets/TagCalendar.hpp
 src/widgets/TagLabel.hpp
 src/widgets/TagListEdit.hpp
)

include_directories(src)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)

qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU|Clang")
  set_source_files_properties(
    ${MOC_SOURCES} PROPERTIES COMPILE_FLAGS "-w")
endif()

add_executable(tagberry-qt
  ${SOURCES} ${MOC_SOURCES})

add_dependencies(tagberry-qt
  qsqlmigrator
  qmarkdowntextedit)

target_link_libraries(tagberry-qt
  Qt5::Core
  Qt5::Widgets
  Qt5::Sql
  QMarkdownTextedit
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
