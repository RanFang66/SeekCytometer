cmake_minimum_required(VERSION 3.16)

project(SeekCytometer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "D:/Qt/6.8.0/llvm-mingw_64")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Sql)

set(TS_FILES SeekCytometer_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        ${TS_FILES}
)

include_directories(datamodel database widgets)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SeekCytometer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        datamodel/TreeNode.h datamodel/TreeNode.cpp
        datamodel/TreeModel.h datamodel/TreeModel.cpp
        database/DatabaseManager.h database/DatabaseManager.cpp
        database/User.h database/User.cpp
        database/Experiment.h database/Experiment.cpp
        database/Specimen.h database/Specimen.cpp
        database/Tube.h database/Tube.cpp
        widgets/ExperimentsBrowser.h widgets/ExperimentsBrowser.cpp
        widgets/CytometerSettingsWidget.h widgets/CytometerSettingsWidget.cpp
        widgets/DetectorSettingsWidget.h widgets/DetectorSettingsWidget.cpp
        database/DetectorSettings.h database/DetectorSettings.cpp
        database/Detector.h database/Detector.cpp
        database/CytometerSettings.h database/CytometerSettings.cpp
        datamodel/DetectorSettingsModel.h datamodel/DetectorSettingsModel.cpp
        datamodel/DetectorModel.h datamodel/DetectorModel.cpp
        database/BaseDAO.h database/BaseDAO.cpp
        database/DetectorsDAO.h database/DetectorsDAO.cpp
        database/DetectorSettingsDAO.h database/DetectorSettingsDAO.cpp
        database/TubesDAO.h database/TubesDAO.cpp
        database/SpecimensDAO.h database/SpecimensDAO.cpp
        database/ExperimentsDAO.h database/ExperimentsDAO.cpp
        database/UsersDAO.h database/UsersDAO.cpp
        database/CytometerSettingsDAO.h database/CytometerSettingsDAO.cpp
        database/BrowserDataDAO.h database/BrowserDataDAO.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SeekCytometer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(SeekCytometer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SeekCytometer
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(SeekCytometer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SeekCytometer)
endif()
set_target_properties(SeekCytometer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SeekCytometer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SeekCytometer)
endif()
