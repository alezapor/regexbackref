cmake_minimum_required(VERSION 3.10)

project(RegexMatcher
        VERSION 1.1
        DESCRIPTION "A regex matcher supporting backreferences"
        HOMEPAGE_URL "https://github.com/alezapor/regexbackref"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()
add_subdirectory(src)