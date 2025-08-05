#pragma once
#include "optimizer.hh"
#include "interpolator/nearest_neighbor_interpolator.hh"

namespace tifo
{
    class RegularGradientDescentOptimizer : public Optimizer
    {
    public:
        explicit RegularGradientDescentOptimizer(float learning_rate);
        ~RegularGradientDescentOptimizer() override = default;

        void optimize(Transform* transform, const Metric* metric, const Image& fixed_img, Image& moving_img) override;
        void set_iteration_callback(IterationCallback callback) override;
        float get_learning_rate() const;
        void set_learning_rate(float learning_rate);
        float get_tolerance() const;
        void set_tolerance(float tolerance);
        int get_max_iterations() const;
        void set_max_iterations(int max_iterations);
        float get_step() const;
        void set_step(float step);

    private:
        IterationCallback iteration_callback_;
        float learning_rate_;
        float tolerance_;
        int max_iterations_;
        float step_;
        NearestNeighborInterpolator interpolator_;

        /**
         * Compute the gradient of the function f(transform) = 1 - metric(transform(moving_img), fixed_img) at an
         * iteration
         * @param transform The current transform
         * @param metric The metric to optimize
         * @param fixed_img The fixed image (will not be modified)
         * @param moving_img The moving image (will not be modified)
         * @return The gradient of transform parameters in the same order they are given in transform->get_parameters()
         */
        std::vector<float> compute_gradient(Transform* transform, const Metric* metric, const Image& fixed_img,
                                            const Image& moving_img) const;

        /**
         * Update transform parameters using the gradient and the learning rate
         * @param transform The current transform
         * @param gradient The current gradient
         */
        void update_parameters(Transform* transform, const std::vector<float>& gradient) const;
    };
} // namespace tifo
