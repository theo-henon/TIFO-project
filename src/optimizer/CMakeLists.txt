add_library(optimizer STATIC
        optimizer.hh
        regular_gradient_descent_optimizer.hh regular_gradient_descent_optimizer.cc
)
target_link_libraries(optimizer PRIVATE math image interpolator transform metric)