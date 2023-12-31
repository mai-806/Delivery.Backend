cmake_minimum_required(VERSION 3.12)
project(deli_auth CXX)

set(CMAKE_CXX_STANDARD 20)

include(third_party/userver/cmake/SetupEnvironment.cmake)
include(GNUInstallDirs)

add_subdirectory(third_party/userver)

include_directories(src)

set(SQL_REQUESTS
		src/models/sql/all.hpp
		src/models/sql/check_user_exist.hpp
		src/models/sql/update_user_login.hpp
		src/models/sql/update_user_type.hpp
		src/models/sql/select_access_token_where_user_id.hpp
		src/models/sql/select_password_where_login.hpp
		src/models/sql/select_user_id.hpp
		src/models/sql/update_user_password.hpp
		src/models/sql/select_user_id_by_token.hpp
    	src/models/sql/insert_user.hpp
		src/models/sql/get_user.hpp
		)

set(VIEWS
		src/views/objects/objects.hpp
		src/views/objects/parsers.cpp

		src/views/objects/parsers.hpp
    
    src/views/v1/user/patch/handler.cpp
    src/views/v1/user/patch/handler.hpp

		src/views/v1/auth/login/post/handler.cpp
		src/views/v1/auth/login/post/handler.hpp

		src/views/v1/auth/logout/post/handler.cpp
		src/views/v1/auth/logout/post/handler.hpp

		src/views/v1/auth/user/reset/post/handler.cpp
		src/views/v1/auth/user/reset/post/handler.hpp

		src/views/v1/auth/user/post/handler.cpp
		src/views/v1/auth/user/post/handler.hpp

		src/views/v1/user/get/handler.cpp
		src/views/v1/user/get/handler.hpp
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
