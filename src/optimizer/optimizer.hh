#pragma once

#include "metric/metric.hh"
#include "transform/transform.hh"

#include <functional>

namespace tifo
{
    using IterationCallback = std::function<void(int iteration, const Image& transformed_img)>;

    class Optimizer
    {
    public:
        virtual ~Optimizer() = default;

        /**
         *
         * @param transform The transform (parameter) to optimize
         * @param metric The metric (value) to optimize
         * @param fixed_img The fixed image
         * @param moving_img The moving image
         *
         * Optimize cost function f(transform) = 1 - metric
         */
        virtual void optimize(Transform* transform, const Metric* metric, const Image& fixed_img,
                              Image& moving_img) = 0;

        virtual void set_iteration_callback(IterationCallback callback) = 0;
    };
} // namespace tifo
