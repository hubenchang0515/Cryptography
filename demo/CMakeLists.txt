aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}" SRC)
add_executable(crypto ${SRC})
target_link_libraries(crypto PRIVATE ${PROJECT_NAME})

install(TARGETS crypto
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/)