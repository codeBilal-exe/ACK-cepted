cmake_minimum_required(VERSION 3.16)
project(SynFloodDetector VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

find_library(PCAP_LIB pcap REQUIRED)
find_path(PCAP_INCLUDE pcap.h)

if(NOT PCAP_LIB)
    message(FATAL_ERROR "libpcap not found. Install with: sudo apt install libpcap-dev")
endif()

set(SOURCES
    src/main.cpp
    src/packet_capture.cpp
    src/detection_engine.cpp
    src/firewall.cpp
    src/gui/main_window.cpp
    src/gui/connection_table.cpp
    src/gui/alert_panel.cpp
)

set(HEADERS
    src/packet_capture.h
    src/detection_engine.h
    src/firewall.h
    src/gui/main_window.h
    src/gui/connection_table.h
    src/gui/alert_panel.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    src/
    ${PCAP_INCLUDE}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Widgets
    Qt5::Core
    ${PCAP_LIB}
)
