project(glinterface)

find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)

aux_source_directory(src SRC_DIR)

add_library(${PROJECT_NAME} STATIC ${SRC_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})