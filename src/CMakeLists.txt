add_subdirectory(math)
add_subdirectory(image)
add_subdirectory(interpolator)
add_subdirectory(transform)
add_subdirectory(metric)
add_subdirectory(optimizer)

add_executable(${PROJECT_NAME}
        main.cc
        transform/rigid_transform.cc
        transform/rigid_transform.hh
)
target_include_directories(${PROJECT_NAME} PRIVATE ${TIFF_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE math image interpolator transform metric optimizer ${TIFF_LIBRARY})