cmake_minimum_required(VERSION 3.5)

project(openapi_app LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
	openapi_app_lib
	    src/AppComponent.hpp
		src/DTO/Client.h
		src/DTO/Response.h
		src/Controller/ClientController.hpp
		src/Controller/ClientControllerDTOs.h
		src/Repository/base/ClientRepository.h
		src/Repository/base/ClientDataException.hpp
		src/Repository/impl/JsonFileClientRepository.h
		src/Repository/impl/JsonFileClientRepository.cpp)

set(oatpp_version 1.3.0)
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(sdk_compiled_dir sdk/compiled/Debug/)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(sdk_compiled_dir sdk/compiled/Release/)
else()
	message(FATAL_ERROR "Unsupported build type: " ${CMAKE_BUILD_TYPE})
endif()

list(APPEND CMAKE_PREFIX_PATH ${sdk_compiled_dir})

find_package(oatpp ${oatpp_version} REQUIRED)
find_package(oatpp-swagger ${oatpp_version} REQUIRED)

target_compile_definitions(
	openapi_app_lib
	    PUBLIC -DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res")

target_link_libraries(
	openapi_app_lib
	    PUBLIC oatpp::oatpp
		PUBLIC oatpp::oatpp-swagger)

target_include_directories(
	openapi_app_lib
	    PUBLIC ${oatpp_INCLUDE_DIRS}
		PUBLIC ${oatpp-swagger_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/App.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE openapi_app_lib)

add_executable(
	openapi_app_test
	    test/App/ClientApiTestClient.hpp
		test/App/TestComponent.hpp
		test/ClientControllerTest.h
		test/ClientControllerTest.cpp
		test/Tests.cpp)

target_link_libraries(openapi_app_test PRIVATE openapi_app_lib)

enable_testing()
add_test(test, openapi_app_test)

