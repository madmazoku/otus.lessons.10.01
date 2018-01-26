# global
cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(vektor VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(vektor VERSION 0.0.1)
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost COMPONENTS unit_test_framework program_options REQUIRED)
find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT madmazoku@gmail.com)

include(CPack)

enable_testing()

# local
configure_file(in.version.h version.h)

add_executable(
    vektor
        "main.cpp"
)

add_executable(
    vektor_test
        "test.cpp"
)

set_target_properties(vektor vektor_test PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

set_target_properties(vektor vektor_test PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_STATIC_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(vektor
    ${SDL2_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

target_link_libraries(vektor_test
    ${Boost_LIBRARIES}
)

install(TARGETS vektor RUNTIME DESTINATION bin)

add_test(vektor vektor_test)
