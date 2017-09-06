#pragma once

#include <glm/glm.hpp>

// Forward declaring objects.
class Shader;
class UniformBuffer;

struct GLFWwindow;

// The color that the screen gets filled with when the color buffer is cleared.
const glm::vec3 CLEAR_COLOR = glm::vec3(0);

// Resolution of the actual game.
const int GAME_RES_X = 224;
const int GAME_RES_Y = 256;

// Declaring shaders.
extern Shader* shader;

// External object references.
extern GLFWwindow* Window;
extern UniformBuffer UBO;

// External options.
extern float SCALE;
