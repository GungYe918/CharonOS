/**
 * @file window.hpp
 * 
 * 윈도우 컴포지터를 구성하는 파일 중 window를 그리는 window.hpp
 * 
*/

/*  include  */
#pragma once

#include <vector>
#include <optional>
#include "../../graphics/graphics.hpp"


class Window {
    public:
        class WindowPainter : public PixelWriter {
            public:
                WindowPainter(Window& window) : window_{window} {}
                virtual void Write(int x, int y, const PixelColor& c) override {
                    window_.At(x, y) = c;
                }

                virtual int Width() const override {  return window_.Width();  }
                virtual int Height() const override {  return window_.Height();  }

            private:
                Window& window_;
        };

        Window(int width, int height);
        ~Window() = default;
        Window(const Window& rhs) = delete;
        Window& operator=(const Window& rhs) = delete;

        void DrawTo(PixelWriter& writer, Vector2D<int> position);
        /**
         * @brief 
         * 
         * @param writer
         * 
         * @param position
         * 
        */

       void SetTransparentColor(std::optional<PixelColor> c);
       WindowPainter* Painter();

       PixelColor& At(int x, int y);
       const PixelColor& At(int x, int y) const;

       int Width() const;
       int Height() const;

    
    private:
        int width_;
        int height_;

        std::vector<std::vector<PixelColor>> data_{};
        std::optional<PixelColor> transparent_color_{std::nullopt};
        WindowPainter painter_{*this};
}; 
