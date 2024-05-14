#include "window.hpp"



Window::Window(int width, int height) : width_{width}, height_{height} {
    data_.resize(height);

    for (int y = 0; y < height; ++y) {
        data_[y].resize(width);
    }
}

void Window::DrawTo(PixelWriter& writer, Vector2D<int> position) {
    if (!transparent_color_) {
        for (int y = 0; y < Height(); ++y) {
            for (int x = 0; x < Width(); ++x) {
                writer.Write(position.x + x, position.y + y, At(x, y));
            }
        }
        return;
    }

    const auto transcolor = transparent_color_.value();

    for (int y = 0; y < Height(); ++y) {
        for (int x = 0; x < Width(); ++x) {
            const auto color = At(x, y);

            if (color != transcolor) {
                writer.Write(position.x + x, position.y + y, color);
            }
        }
    }
}

void Window::SetTransparentColor(std::optional<PixelColor> c) {
    transparent_color_ = c;
}

Window::WindowPainter* Window::Painter() {
    return &painter_;
}

PixelColor& Window::At(int x, int y) {
    return data_[y][x];
}

const PixelColor& Window::At(int x, int y) const {
    return data_[y][x];
}

int Window::Width() const {
    return width_;
}

int Window::Height() const {
    return height_;
}