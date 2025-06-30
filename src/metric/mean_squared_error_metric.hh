#pragma once

#include "metric.hh"

namespace tifo
{
    class MeanSquaredErrorMetric : public Metric
    {
    public:
        ~MeanSquaredErrorMetric() override = default;

        float compare(const Image& img1, const Image& img2) const override;
    };
} // namespace tifo
