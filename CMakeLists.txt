cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(pdf-embedded-font-sampler LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/vendor/extra-cmake-modules/find-modules")
find_package(poppler REQUIRED COMPONENTS Cpp)


add_executable(pdf-embedded-font-sampler src/main.cpp)
target_include_directories(pdf-embedded-font-sampler PRIVATE ${Poppler_INCLUDE_DIRS})
target_link_libraries(pdf-embedded-font-sampler ${Poppler_LIBRARIES})

if(MSVC)
    set(WARN_FLAGS ${WARN_FLAGS} /permissive- /W4
        /WX
        /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 /w14311
        /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826
        /w14905 /w14906 /w14928)
else()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # GCC >= 4.3
        if (NOT(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.3))
            set(WARN_FLAGS ${WARN_FLAGS} -Wsign-conversion)
        # GCC >= 4.6
        elseif (NOT(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.6))
            set(WARN_FLAGS ${WARN_FLAGS} -Wdouble-promotion)
        # GCC >= 4.8
        elseif (NOT(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8))
            set(WARN_FLAGS ${WARN_FLAGS} -Wuseless-cast)
        # GCC >= 6.0
        elseif (NOT(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 6.0))
            set(WARN_FLAGS ${WARN_FLAGS}
                -Wmisleading-indentation
                -Wduplicated-cond
                -Wnull-dereference
            )
        # GCC >= 7.0
        elseif (NOT(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 7.0))
            set(WARN_FLAGS ${WARN_FLAGS} -Wduplicated-branches)
        endif()
        # GCC, all versions
        set(WARN_FLAGS ${WARN_FLAGS}
            -Wlogical-op
            -Wpedantic)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # CLANG >= 3.2
        if (NOT(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.2))
            set(WARN_FLAGS ${WARN_FLAGS} -Wpedantic)
        # CLANG >= 3.8
        elseif (NOT(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.8))
            set(WARN_FLAGS ${WARN_FLAGS} -Wdouble-promotion)
        # CLANG >= 8.0
        elseif (NOT(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 8.0))
            # As of now (Nov 2018) -Wlifetime is only present in the
            # special branch of clang (not even in trunk!).
            # So, assume future release 8 (but we don't really know).
            set(WARN_FLAGS ${WARN_FLAGS} -Wlifetime)
        endif()
        # Clang, all versions
        set(WARN_FLAGS ${WARN_FLAGS} -Wsign-conversion)
    endif()
endif()
