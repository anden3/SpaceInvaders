#include "Sprite.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "main.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"

const std::map<std::string, glm::vec4> SpriteIndexes = {
    {"UFO", {0, 0, 16, 7}},

    {"Large_1", {19, 0, 12, 7}},
    {"Large_2", {34, 0, 12, 7}},

    {"Medium_1", {49, 0, 11, 7}},
    {"Medium_2", {63, 0, 11, 7}},

    {"Small_1", {77, 0, 8, 7}},
    {"Small_2", {89, 0, 8, 7}},

    {"Explosion", {100, 0, 13, 7}},

    {"Cannon", {34, 14, 13, 8}}
};

// Vertex and Texture coordinates for a 2D quad.
const glm::vec2 Coordinates[6] = {
    {0.0f, 1.0f},
    {1.0f, 0.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f}
};

Sprite::Sprite(std::string name, glm::vec2 position) : Position(position) {
    glm::vec4 properties = SpriteIndexes.at(name);

    Offset = glm::vec2(properties.x, properties.y);
    Size = glm::vec2(properties.z, properties.w);

    VBO.Init(shader);
    VBO.Create(2, 2);

    Mesh();
}

void Sprite::Mesh() {
    std::vector<float> data;

    for (auto coords : Coordinates) {
        // Position of vertex.
        data.push_back(SCALE * (Position.x + coords.x * Size.x));
        data.push_back(SCALE * (Position.y + coords.y * Size.y));

        // Texcoords of vertex.
        data.push_back(Offset.x + coords.x * Size.x);
        data.push_back(Offset.y + coords.y * Size.y);
    }

    VBO.Upload(data);
}

void Sprite::Draw() {
    VBO.Draw();
}
