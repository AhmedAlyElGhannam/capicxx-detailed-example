cmake_minimum_required(VERSION 3.22)
project(ivi-client)

find_package(CommonAPI REQUIRED)
find_package(CommonAPI-SomeIP REQUIRED)

file(GLOB ${PROJECT_NAME}_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src-gen/v1/bmw/ivi/*.cpp
)

add_executable(${PROJECT_NAME} 
    "${${PROJECT_NAME}_SRC}"
)

include_directories(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src-gen
    ${CAPICXX_CORE_RUNTIME_INC}
    ${CAPICXX_SOMEIP_RUNTIME_INC}
    ${VSOMEIP_INC}
)

target_link_libraries(${PROJECT_NAME}
    vsomeip3
    vsomeip3-cfg
    vsomeip3-e2e
    vsomeip3-sd
    CommonAPI
    CommonAPI-SomeIP
)
