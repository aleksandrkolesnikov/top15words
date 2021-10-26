set(TARGET_NAME Top15Words)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Qml Quick Charts REQUIRED)

qt5_add_resources(QT_RESOURCES qml.qrc)

message("QT_RESOURCES = ${QT_RESOURCES}")

add_executable(${TARGET_NAME}
    Main.cpp
    Applet.h
    Applet.cpp
    domain/WordsProvider.h
    domain/WordsProvider.cpp
    domain/DataModel.h
    domain/DataModel.cpp
    ${QT_RESOURCES})

target_link_libraries(${TARGET_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::Charts)

target_compile_features(${TARGET_NAME}
    PRIVATE
        cxx_std_17)
