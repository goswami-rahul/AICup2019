cmake_minimum_required(VERSION 3.5)
project(aicup2019)

# OS and compiler checks.
if(WIN32)
    add_definitions(-DWIN32)
    SET(PROJECT_LIBS Ws2_32.lib)
endif()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)

# set some compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -Wno-misleading-indentation -Wno-shadow -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector")

file(GLOB HEADERS "*.hpp" "model/*.hpp" "csimplesocket/*.h")
SET_SOURCE_FILES_PROPERTIES(${HEADERS} PROPERTIES HEADER_FILE_ONLY TRUE)
file(GLOB SRC "*.cpp" "model/*.cpp" "csimplesocket/*.cpp")
add_executable(aicup2019 ${HEADERS} ${SRC})
TARGET_LINK_LIBRARIES(aicup2019 ${PROJECT_LIBS})