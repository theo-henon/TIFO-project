add_library(image STATIC
        image.hh image.cc
)
target_link_libraries(image PRIVATE math interpolator)