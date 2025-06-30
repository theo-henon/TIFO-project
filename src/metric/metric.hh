#pragma once
#include "image/image.hh"

namespace tifo
{
    class Metric
    {
    public:
        virtual ~Metric() = default;

        /**
         *
         * @param img1 First image
         * @param img2 Second image
         * @return A number between 0 and 1 (1 => images are very similar, 0 => images are very different)
         */
        virtual float compare(const Image& img1, const Image& img2) const = 0;
    };
} // namespace tifo
