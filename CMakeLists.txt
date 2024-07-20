cmake_minimum_required(VERSION 3.10)

project(CountersProject LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

enable_testing()

# Установите путь к Google Test, если необходимо
set(GTEST_ROOT "/usr/src/gtest")
find_package(GTest REQUIRED)
find_package(Qt5Core REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

# Создаем библиотеку
add_library(Counters STATIC
    counters.cpp
    counters.h
)

target_link_libraries(Counters
    Qt5::Core
    ${CMAKE_THREAD_LIBS_INIT}
)

# Создаем основной исполняемый файл
add_executable(main
    main.cpp
)

target_link_libraries(main
    Counters
    Qt5::Core
)

# Создаем исполняемый файл для тестов
add_executable(tests
    counters_tests.cpp
)

target_link_libraries(tests
    Counters
    ${GTEST_LIBRARIES}
    Qt5::Core
    pthread
)

# Добавляем тест
add_test(NAME CountersUnitTests COMMAND tests)

