#include "config.h"
#include "Control.h"
#include <GL/glew.h>

namespace Graphics3D {


    Control::Control(Display::Window* window, IWASD* iwasd) {
        this->window = window;
        this->iwasd = iwasd;

        window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                this->window->Close();
            }

            if (key == GLFW_KEY_W && action == GLFW_PRESS){
                this->iwasd->setWPressed(true);
            } else if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
                this->iwasd->setWPressed(false);
            }

            if (key == GLFW_KEY_A && action == GLFW_PRESS){
                this->iwasd->setAPressed(true);
            } else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
                this->iwasd->setAPressed(false);
            }

            if (key == GLFW_KEY_S && action == GLFW_PRESS){
                this->iwasd->setSPressed(true);
            } else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
                this->iwasd->setSPressed(false);
            }

            if (key == GLFW_KEY_D && action == GLFW_PRESS){
                this->iwasd->setDPressed(true);
            } else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
                this->iwasd->setDPressed(false);
            }
        });
    }


    void Control::update(float deltaTime) {
        this->iwasd->updateMove(deltaTime);
    }


    Control::~Control() {
        delete this->window;
        delete this->iwasd;
    }

}
