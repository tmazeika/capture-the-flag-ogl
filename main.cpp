#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "game.h"
#include "window.h"

#define S_PER_UPDATE  1 / 60.0
#define S_PER_FPS_OUT 5

void loop(const Window, Game);

int main()
{
    Window window(800, 600, "Burgee");
    window.initialize();

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Could not initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    Shader shader;
    shader.initialize();
    shader.use();

    Game game(shader, window.get_input());
    game.initialize();

    loop(window, game);

    return EXIT_SUCCESS;
}

void loop(const Window window, Game game)
{
    double previous = glfwGetTime();
    double lag = 0;
    double current;
    double elapsed;

    // fps
    double fps_time = glfwGetTime();
    unsigned int fps_frames = 0;

    while (!window.should_close() && !game.should_close()) {
        current = glfwGetTime();
        elapsed = current - previous;
        previous = current;
        lag += elapsed;

        while (lag >= S_PER_UPDATE) {
            game.update();
            lag -= S_PER_UPDATE;
        }

        game.render(static_cast<float>(lag / S_PER_UPDATE));
        window.swap_and_poll();

        // fps
        fps_frames++;

        if (glfwGetTime() - fps_time >= S_PER_FPS_OUT) {
            std::cout << S_PER_FPS_OUT * 1000.0 / fps_frames << " ms/frame" << std::endl;

            fps_frames = 0;
            fps_time += S_PER_FPS_OUT;
        }
    }
}
