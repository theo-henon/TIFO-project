add_library(interpolator STATIC
        interpolator.hh
        nearest_neighbor_interpolator.hh nearest_neighbor_interpolator.cc
        interpolation_method.hh
        trilinear_interpolator.cc
        trilinear_interpolator.hh
)