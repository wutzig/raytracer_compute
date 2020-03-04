#include "App.h"
Renderer* renderer;
void framebuffer_size_callback(GLFWwindow*, int new_screen_width, int new_screen_height){
    glViewport(0, 0, new_screen_width, new_screen_height);
}

void key_press(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        switch(key){
            case GLFW_KEY_LEFT:
                renderer->AddSegment(0, -1.0);
                break;
            case GLFW_KEY_RIGHT:
                renderer->AddSegment(0, 1.0);
                break;
            case GLFW_KEY_UP:
                renderer->AddSegment(2, -1.0);
                break;
            case GLFW_KEY_DOWN:
                renderer->AddSegment(2, 1.0);
                break;
            case GLFW_KEY_LEFT_CONTROL:
                renderer->AddSegment(1, -1.0);
                break;
            case GLFW_KEY_LEFT_SHIFT:
                renderer->AddSegment(1, 1.0);
                break;
            case GLFW_KEY_G:
                renderer->grow = !renderer->grow;
                break;
        }
    }
}

int main(){
    GLFWwindow* window;
    if (!glfwInit()) std::exit(1);
    window = glfwCreateWindow(WIDTH, HEIGHT, "GameWindow", NULL, NULL);
    // glfwMakeContextCurrent(window);
    
    glewInit();
    if (!window){
        glfwTerminate();
        std::exit(1);
    }

    glfwSetKeyCallback(window, key_press);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) std::exit(1);

    ShaderProgram shaderProgram("src/glsl/vertex.glsl", "src/glsl/fragment.glsl", NULL);
    ShaderProgram computeProgram(NULL, NULL, "src/glsl/compute.glsl");
    renderer = new Renderer(computeProgram.shaderProgramID);
    renderer->UpdateUniformLocations(computeProgram.locations);
    renderer->CreateObjects();
    // renderer->loadBMP("src/green.bmp");
    
    int frames = 0;
    float delta_time;
    float delta_time_fps = 0.0f;
    int fps;
    std::clock_t then = std::clock();
    std::clock_t now;
    while (!glfwWindowShouldClose(window)){
        renderer->UseComputeShader();
        renderer->DrawScence(shaderProgram.shaderProgramID);
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glViewport(0, 0, WIDTH, HEIGHT);
        frames = frames + 1;
        now = std::clock();
        delta_time = now - then;
        then = now;
        delta_time_fps += delta_time;
        renderer->delta_time = delta_time;
        if(glm::floor(delta_time_fps) >= 1000){
            std::cout<<glm::ceil((float)frames * 1000.0f / delta_time_fps)<<" "<<"\r";
            delta_time_fps = 0.0f; 
            frames = 0;
        }
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    shaderProgram.DeleteProgram();
    computeProgram.DeleteProgram();
    renderer->DeleteBuffers();
    std::exit(0);
}