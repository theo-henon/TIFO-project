add_library(metric STATIC
        metric.hh
        mean_squared_error_metric.hh mean_squared_error_metric.cc
)
target_link_libraries(metric PRIVATE math image interpolator)