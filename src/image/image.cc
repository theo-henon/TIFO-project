#include "image.hh"

namespace tifo
{
    Image::Image(uint32_t width, uint32_t height, uint32_t slices)
        : pixels_(width * height * slices)
        , width_(width)
        , height_(height)
        , slices_(slices)
    {}

    uint32_t Image::get_width() const
    {
        return width_;
    }

    uint32_t Image::get_height() const
    {
        return height_;
    }

    uint32_t Image::get_depth() const
    {
        return slices_;
    }

    uint8_t Image::get_pixel(uint32_t row, uint32_t col, uint32_t slice) const
    {
        return pixels_.at(slice * width_ * height_ + row * width_ + col);
    }

    void Image::set_pixel(uint32_t row, uint32_t col, uint32_t slice, uint8_t value)
    {
        pixels_.at(slice * width_ * height_ + row * width_ + col) = value;
    }

    size_t Image::size() const
    {
        return width_ * height_ * slices_;
    }
} // namespace tifo