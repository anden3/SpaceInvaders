#pragma once

#include "Buffer.hpp"

class Sprite {
public:
    Sprite(std::string name, glm::vec2 position);

    void Draw();

private:
    Buffer VBO;

    glm::vec2 Size;
    glm::vec2 Offset;
    glm::vec2 Position;

    void Mesh();
};
