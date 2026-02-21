cmake_minimum_required(VERSION 3.22)
project(IVI-Service)

find_package(CommonAPI REQUIRED)
find_package(CommonAPI-SomeIP REQUIRED)

file(GLOB ${PROJECT_NAME}_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src-gen/v1/bmw/ivi/*.cpp
)

add_executable(${PROJECT_NAME} 
    "${${PROJECT_NAME}_SRC}"
)

target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)

target_include_directories(${PROJECT_NAME} PRIVATE
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

# copy init script & config file for convenience
set(SERVICE_INIT_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/../service_init.sh)
set(SERVICE_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../service-config.json)
set(SERVICE_DEFAULT_BINDING_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../commonapi.ini)

add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SERVICE_INIT_SCRIPT}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SERVICE_CONFIG_FILE}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SERVICE_DEFAULT_BINDING_FILE}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
