project(Application LANGUAGES CXX)

find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(freetype-gl CONFIG REQUIRED)
aux_source_directory(src SRC_DIR)
aux_source_directory(Camera SRC_DIR)

add_executable(${PROJECT_NAME} ${SRC_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE Camera)
target_link_libraries(${PROJECT_NAME} PRIVATE glinterface)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype-gl)
