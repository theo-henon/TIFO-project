add_library(transform STATIC
        rigid_transform.hh rigid_transform.cc
        transform.hh
)
target_link_libraries(transform PRIVATE math interpolator)