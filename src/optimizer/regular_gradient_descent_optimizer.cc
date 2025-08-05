#include "regular_gradient_descent_optimizer.hh"

#include <cmath>
#include <iostream>

namespace tifo
{
    RegularGradientDescentOptimizer::RegularGradientDescentOptimizer(float learning_rate)
        : iteration_callback_(nullptr)
        , learning_rate_(learning_rate)
        , tolerance_(1e-2f)
        , max_iterations_(1000)
        , step_(1e-3f)
    {}

    void RegularGradientDescentOptimizer::optimize(Transform* transform, const Metric* metric, const Image& fixed_img,
                                                   Image& moving_img)
    {
        std::cout << "iteration,transform,metric" << std::endl;

        float current_metric = 0.f;
        for (int iter = 0; iter < max_iterations_; ++iter)
        {
            // Apply initial transform and compute initial metric value
            Image current_img = transform->apply_img(moving_img, interpolator_);
            current_metric = metric->compare(fixed_img, current_img);
            const float current_cost = 1.f - current_metric;

            // Log gradient descent status
            std::cout << iter << ',';
            transform->print(std::cout);
            std::cout << ',' << current_metric << std::endl;

            // Call callback if set
            if (iteration_callback_)
            {
                iteration_callback_(iter, current_img);
            }

            // Compute gradient
            std::vector<float> parameters = transform->get_parameters();
            std::vector gradient(parameters.size(), 0.0f);
            for (size_t i = 0; i < parameters.size(); ++i)
            {
                std::vector<float> perturbed = parameters;
                perturbed[i] += step_;

                transform->set_parameters(perturbed);
                Image tmp_img = transform->apply_img(moving_img, interpolator_);

                const float perturbed_metric = metric->compare(fixed_img, tmp_img);
                const float perturbed_cost = 1.0f - perturbed_metric;

                gradient[i] = (perturbed_cost - current_cost) / step_;
            }

            // Compute gradient L2 magnitude
            float norm = 0.0f;
            for (float g : gradient)
            {
                norm += g * g;
            }
            norm = std::sqrt(norm);

            if (norm < tolerance_)
            {
                break;
            }

            // Update transform
            for (size_t i = 0; i < parameters.size(); ++i)
            {
                parameters[i] -= learning_rate_ * gradient[i];
            }
            transform->set_parameters(parameters);
        }
    }

    void RegularGradientDescentOptimizer::set_iteration_callback(IterationCallback callback)
    {
        iteration_callback_ = std::move(callback);
    }

    float RegularGradientDescentOptimizer::get_learning_rate() const
    {
        return learning_rate_;
    }

    void RegularGradientDescentOptimizer::set_learning_rate(float learning_rate)
    {
        learning_rate_ = learning_rate;
    }

    float RegularGradientDescentOptimizer::get_tolerance() const
    {
        return tolerance_;
    }

    void RegularGradientDescentOptimizer::set_tolerance(float tolerance)
    {
        tolerance_ = tolerance;
    }

    int RegularGradientDescentOptimizer::get_max_iterations() const
    {
        return max_iterations_;
    }

    void RegularGradientDescentOptimizer::set_max_iterations(int max_iterations)
    {
        max_iterations_ = max_iterations;
    }

    float RegularGradientDescentOptimizer::get_step() const
    {
        return step_;
    }

    void RegularGradientDescentOptimizer::set_step(float step)
    {
        step_ = step;
    }
} // namespace tifo