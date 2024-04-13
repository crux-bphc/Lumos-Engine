#include "camera.h"

Camera::Camera(input* Input){
    this->Input = Input; 
}

void Camera::UpdateMovementArray(){

    if(Input->isKeyPressed(GLFW_KEY_W)){
        this->movementArray[1] = 0.1;
    }
    else {
        this->movementArray[1] = 0;
    }

    if(Input->isKeyPressed(GLFW_KEY_S)){
        this->movementArray[1] = -0.1;
    }
    else {
        this->movementArray[1] = 0;
    }

    if(Input->isKeyPressed(GLFW_KEY_A)){
        this->movementArray[0] = -0.1;
    }
    else {
        this->movementArray[0] = 0;
    }

    if(Input->isKeyPressed(GLFW_KEY_D)){

       this->movementArray[0] = 0.1;
    }
    else {
        this->movementArray[0] = 0;
    }
    

}