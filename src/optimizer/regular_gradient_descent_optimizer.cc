#include "regular_gradient_descent_optimizer.hh"

#include "interpolator/nearest_neighbor_interpolator.hh"

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
        float current_metric = metric->compare(fixed_img, moving_img);
        float current_cost = 1.0f - current_metric;

        for (int iter = 0; iter < max_iterations_; ++iter)
        {
            // Get transform parameters
            std::vector<float> parameters = transform->get_parameters();

            // Log gradient descent status
            std::cout << iter << ',';
            transform->print(std::cout);
            std::cout << ',' << current_metric << std::endl;

            // Compute gradient
            std::vector gradient(parameters.size(), 0.0f);
            for (size_t i = 0; i < parameters.size(); ++i)
            {
                std::vector<float> perturbed = parameters;
                perturbed[i] += step_;

                transform->set_parameters(perturbed);
                NearestNeighborInterpolator interpolator;
                Image temp_img = transform->apply_img(moving_img, interpolator);

                const float perturbed_metric = metric->compare(fixed_img, temp_img);
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

            // Apply transform to moving image
            NearestNeighborInterpolator interpolator;
            Image transformed_img = transform->apply_img(moving_img, interpolator);

            // Update current metric/cost
            current_metric = metric->compare(fixed_img, transformed_img);
            current_cost = 1.0f - current_metric;

            // Call callback if set
            if (iteration_callback_)
            {
                iteration_callback_(iter, transformed_img);
            }
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