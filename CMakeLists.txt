cmake_minimum_required(VERSION 3.27)
project(Evidencia2)

set(CMAKE_CXX_STANDARD 17)

# Opciones de compilacion
option(ENABLE_COVERAGE "Habilitar medicion de cobertura de codigo" OFF)

if(ENABLE_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# Ejecutable principal: servidor REST API
add_executable(Evidencia2 main.cpp)

# Enlazar bibliotecas requeridas por cpp-httplib
find_package(Threads REQUIRED)
target_link_libraries(Evidencia2 Threads::Threads)

if(WIN32)
    target_link_libraries(Evidencia2 ws2_32)
endif()

# Ejecutable de pruebas unitarias
enable_testing()
add_executable(Evidencia2_tests test_algoritmos.cpp)
add_test(NAME unit COMMAND Evidencia2_tests)
