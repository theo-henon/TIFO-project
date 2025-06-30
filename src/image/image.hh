#pragma once

#include <cstdint>
#include <vector>

namespace tifo
{
    /**
     * A 3D image class. Stored as a stack of 2D-image. That means the slice 0 is the top of the stack.
     */
    class Image
    {
    public:
        Image(uint32_t width, uint32_t height, uint32_t slices);

        uint32_t get_width() const;

        uint32_t get_height() const;

        uint32_t get_depth() const;

        uint8_t get_pixel(uint32_t row, uint32_t col, uint32_t slice) const;

        void set_pixel(uint32_t row, uint32_t col, uint32_t slice, uint8_t value);

        size_t size() const;

    private:
        std::vector<uint8_t> pixels_;
        uint32_t width_;
        uint32_t height_;
        uint32_t slices_;
    };
} // namespace tifo