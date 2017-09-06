#include "main.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <FreeImagePlus.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Buffer.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"

// Setting flags.
static bool WindowFocused = true;
static bool WindowMinimized = false;

// Setting default option values.
static bool VSYNC = true;

static int SCREEN_WIDTH = 0;
static int SCREEN_HEIGHT = 0;

float SCALE = 3;

// Initializing objects.
UniformBuffer UBO = UniformBuffer();

// The main window which everything is rendered in.
GLFWwindow* Window = nullptr;

// Defining shaders.
Shader* shader = nullptr;

// Initialize different objects and states.
void Init_GL();
void Init_Shaders();
unsigned int Init_Textures();

// Declare event handlers
void Key_Proxy(GLFWwindow* window, int key, int scancode, int action, int mods);

void Window_Focused(GLFWwindow* window, int focused);
void Window_Minimized(GLFWwindow* window, int iconified);

int main() {
    Init_GL();
    Init_Shaders();
    unsigned int texture = Init_Textures();

    auto sprite = Sprite("Cannon", glm::vec2(5, 5));

    // Main loop.
    while (!glfwWindowShouldClose(Window)) {
        if (WindowMinimized || !WindowFocused) {
            glfwWaitEvents();
			continue;
        }

        // Clear the screen buffer from the last frame.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Polls and processes received events.
        glfwPollEvents();

        glBindTexture(GL_TEXTURE_2D, texture);
        sprite.Draw();
        glBindTexture(GL_TEXTURE_2D, 0);

        // Swap the newly rendered frame with the old one.
        glfwSwapBuffers(Window);
    }

    // Shut down the graphics library and return.
    glfwTerminate();
	return 0;
}

void Init_GL() {
	glfwInit();

	// Set the OpenGL version.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_AUTO_ICONIFY, false);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	// Get the video mode of the monitor.
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
    
    // Set the window to be decorated, allowing users to close it.
    glfwWindowHint(GLFW_DECORATED, true);

    SCREEN_WIDTH = static_cast<int>(GAME_RES_X * SCALE);
    SCREEN_HEIGHT = static_cast<int>(GAME_RES_Y * SCALE);
    
    // Create a windowed window.
    Window = glfwCreateWindow(
        SCREEN_WIDTH, SCREEN_HEIGHT,
        "Space Invaders", nullptr, nullptr
    );

    glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

    // Set the window's position to the upper left corner.
    glfwSetWindowPos(Window, 0, 0);

    // Set the window to be used for future OpenGL calls.
    glfwMakeContextCurrent(Window);

    // Set whether to use VSync based on the value in the config file.
    glfwSwapInterval(VSYNC);

    // Set GLEW to experimental mode (doesn't work otherwise D:)
    glewExperimental = GL_TRUE;

    // Initializes GLEW, which enables vendor-specific OpenGL extensions.
	glewInit();

	// Set up callbacks for event handlers.
    glfwSetKeyCallback(Window, Key_Proxy);
    glfwSetWindowFocusCallback(Window, Window_Focused);
    glfwSetWindowIconifyCallback(Window, Window_Minimized);
	
	// Specify the size of the OpenGL view port.
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	// Set the color that the window gets filled with when the color buffer gets cleared.
	glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, 1.0f);
}

void Init_Shaders() {
    shader = new Shader("shader");

    // Default identity matrix.
    glm::mat4 model;

	glm::mat4 projection = glm::ortho(
		0.0f, static_cast<float>(SCREEN_WIDTH),		// Left - Right
		static_cast<float>(SCREEN_HEIGHT), 0.0f,	// Bottom - Top
		-1.0f, 1.0f									// Near - Far
    );
    
    shader->Upload("model", model);
    shader->Upload("projection", projection);
}

unsigned int Init_Textures() {
    // Activate a texture unit.
    glActiveTexture(GL_TEXTURE0);

    fipImage image;
    image.load("atlas.png");
    // image.flipVertical();

    int width  = static_cast<int>(image.getWidth());
    int height = static_cast<int>(image.getHeight());
    unsigned char* imageData = image.accessPixels();

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, imageData);

    glBindTexture(GL_TEXTURE_2D, 0);

    shader->Upload("sprites", 0);
    shader->Upload("spriteColor", glm::vec3(1.0f));

    return texture;
}

void Key_Proxy(GLFWwindow* window, int key, int scancode, int action, int mods) {

}

void Window_Focused(GLFWwindow* window, int focused) {
    WindowFocused = focused > 0;
}
void Window_Minimized(GLFWwindow* window, int iconified) {
    WindowMinimized = iconified > 0;
}
