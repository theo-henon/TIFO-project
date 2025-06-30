#pragma once
#include "optimizer.hh"

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
    };
} // namespace tifo
