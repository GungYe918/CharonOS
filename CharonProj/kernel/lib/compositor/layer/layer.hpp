/**
 * @file layer.hpp 
 * 
 * window compositor중 화면 레이러를 구현하는 layer.hpp
*/

#pragma once

// sys header
#include <memory>
#include <map>
#include <vector>

// kernel lib
#include "../../graphics/graphics.hpp"
#include "../window/window.hpp"


class Layer {
    public:
        Layer(unsigned int id = 0);
        unsigned int ID() const;

        Layer& SetWindow(const std::shared_ptr<Window>& window);
        std::shared_ptr<Window> GetWindow() const;

        Layer& Move(Vector2D<int> pos);
        Layer& MoveRelative(Vector2D<int> pos_diff);

        void DrawTo(PixelWriter& writer) const;

    private:
        unsigned int id_;
        Vector2D<int> pos_;
        std::shared_ptr<Window> window_;
};

class LayerManager {
    public:
        void SetWriter(PixelWriter* writer);
        Layer& NewLayer();

        void Draw() const;

        void Move(unsigned int id, Vector2D<int> new_position);
        void MoveRelative(unsigned int id, Vector2D<int> pos_diff);

        void UpDown(unsigned int id, int new_height);
        void Hide(unsigned int id);

    private:
        PixelWriter* writer_{nullptr};
        std::vector<std::unique_ptr<Layer>> layers_{};
        std::vector<Layer*> layer_stack_{};
        unsigned int latest_id_{0};

        Layer* FindLayer(unsigned int id);
};

extern LayerManager* layer_manager;