cmake_minimum_required(VERSION 3.10)
project(GameOfLife LANGUAGES C CXX)

message(STATUS "CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
if(EXISTS "${CMAKE_SOURCE_DIR}/vcpkg.json")
  message(STATUS "vcpkg.json found in ${CMAKE_SOURCE_DIR}")
else()
  message(WARNING "vcpkg.json NOT found in ${CMAKE_SOURCE_DIR}")
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

option(USE_CUDA "Enable CUDA support (set OFF if CUDA toolkit isn't installed)" ON)
if(USE_CUDA)
    find_package(CUDAToolkit QUIET)
    if(CUDAToolkit_FOUND)
        message(STATUS "Found CUDAToolkit")
    else()
        find_program(NVCC nvcc)
        if(NVCC)
            message(STATUS "Found nvcc (${NVCC})")
        else()
            message(WARNING "CUDA not found")
            set(USE_CUDA OFF)
        endif()
    endif()
endif()

set(CU_SOURCES "")
if(USE_CUDA)
    enable_language(CUDA)
    set(CMAKE_CUDA_STANDARD 14)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
    set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)
    add_compile_definitions(USE_CUDA)
    file(GLOB_RECURSE CU_SOURCES 
        "${PROJECT_SOURCE_DIR}/src/*.cu"
        "${PROJECT_SOURCE_DIR}/dep/*.cu"
    )
endif()

file(GLOB_RECURSE CPP_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/dep/*.cpp"
    "${PROJECT_SOURCE_DIR}/dep/*.c"
)

add_executable(${PROJECT_NAME}
    ${CPP_SOURCES}
    ${CU_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/dep
    ${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include
    ${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include/imgui/backends
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad::glad
    OpenGL::GL
    glm::glm
    imgui::imgui
)