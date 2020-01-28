#include "config.h"
#include "Control.h"
#include <GL/glew.h>

namespace Graphics3D {


    Control::Control(Display::Window* window, IWASD* wasd, IMouse* mouse) {
        this->window = window;
        this->wasd = wasd;
        this->mouse = mouse;

        window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                this->window->Close();
            }

            if (key == GLFW_KEY_W && action == GLFW_PRESS){
                this->wasd->setWPressed(true);
            } else if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
                this->wasd->setWPressed(false);
            }

            if (key == GLFW_KEY_A && action == GLFW_PRESS){
                this->wasd->setAPressed(true);
            } else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
                this->wasd->setAPressed(false);
            }

            if (key == GLFW_KEY_S && action == GLFW_PRESS){
                this->wasd->setSPressed(true);
            } else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
                this->wasd->setSPressed(false);
            }

            if (key == GLFW_KEY_D && action == GLFW_PRESS){
                this->wasd->setDPressed(true);
            } else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
                this->wasd->setDPressed(false);
            }

            if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
                this->wasd->toggleFly();
            }
        });

        window->SetMouseMoveFunction([this](float64 xPos, float64 yPos) {
            this->mouse->setMousePos(xPos, yPos);
        });
    }


    /**
     *  Calls the update function of the wsad and mouse objects.
    */
    void Control::update(float deltaTime) {
        this->wasd->updateMove(deltaTime);
        this->mouse->updateLook(deltaTime);
    }


    Control::~Control() {
        delete this->window;
        delete this->wasd;
        delete this->mouse;
    }

}
