cmake_minimum_required(VERSION 3.12)
project(deli_main CXX)

set(CMAKE_CXX_STANDARD 20)

include(third_party/userver/cmake/SetupEnvironment.cmake)
include(GNUInstallDirs)

add_subdirectory(third_party/userver)

include_directories(src)

set(SQL_REQUESTS
        src/models/sql/all.hpp
        src/models/sql/insert_order.hpp
        src/models/sql/get_orders.hpp
        src/models/sql/get_order_info.hpp
        src/models/sql/get_free_courier_ids.hpp
)

set(CLIENTS
        src/clients/deli-auth/client.cpp
        src/clients/deli-auth/client.hpp
)

set(VIEWS

        src/views/objects/objects.hpp
        src/views/objects/parsers.cpp
        src/views/objects/parsers.hpp

        src/views/v1/order/post/handler.cpp
        src/views/v1/order/post/handler.hpp

        src/views/v1/orders/get/handler.cpp
        src/views/v1/orders/get/handler.hpp

        src/views/testing/clients/deli-auth-v1-user-get/post/handler.cpp
        src/views/testing/clients/deli-auth-v1-user-get/post/handler.hpp

        src/views/v1/couriers/available/get/handler.cpp
        src/views/v1/couriers/available/get/handler.hpp

        src/views/v1/order/get/handler.cpp
        src/views/v1/order/get/handler.hpp
)

# Common sources
add_library(${PROJECT_NAME}_objs OBJECT

        src/common/consts.hpp
        src/common/exceptions.hpp

        src/components/requester.cpp
        src/components/requester.hpp

        src/models/models.hpp
        src/models/requests.cpp
        src/models/requests.hpp

        ${SQL_REQUESTS}
        ${VIEWS}
        ${CLIENTS}

)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver-core userver-postgresql)


# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)


# Unit Tests
add_executable(${PROJECT_NAME}_unittest

)


target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver-utest)
add_google_tests(${PROJECT_NAME}_unittest)


# Benchmarks
add_executable(${PROJECT_NAME}_benchmark

)


target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_objs userver-ubench)
add_google_benchmark_tests(${PROJECT_NAME}_benchmark)

# Functional Tests
add_subdirectory(tests)

if (DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif ()

set(CONFIG_VAR_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/config_vars.yaml)
set(CONFIG_FALLBACK_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/dynamic_config_fallback.json)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configs/static_config.yaml.in ${CMAKE_CURRENT_SOURCE_DIR}/configs/static_config.yaml)

FILE(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})

set(CMAKE_CXX_COMPILER clang++)
target_link_options(${PROJECT_NAME} PRIVATE "-lstdc++")
target_link_options(${PROJECT_NAME}_unittest PRIVATE "-lstdc++")
target_link_options(${PROJECT_NAME}_benchmark PRIVATE "-lstdc++")