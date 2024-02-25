#include "camera.h"
#include <stdio.h>

//2D Camera 
Camera::Camera(glm::vec2 center, float zoom, float sensitivity)
{   
    this->Center = center;
    this->Zoom = zoom;
    this->Sensitivity = sensitivity;
}

Camera::Camera(float posX, float posY, float zoom)
{
    this->Center = glm::vec2(posX, posY);
    this->Zoom = zoom;
}

glm::mat4 Camera::GetProjectionMatrix(int SCR_WIDTH,int SCR_HEIGHT){

    float left = this->Center[0] - SCR_WIDTH/2.0f;
    float right = this->Center[0] + SCR_WIDTH/2.0f;
    float bottom = this->Center[1] + SCR_HEIGHT/2.0f;
    float top = this->Center[1] - SCR_HEIGHT/2.0f;

    glm::mat4 orthoMatrix = glm::ortho(left, right, bottom, top, 0.01f, 100.0f);
    glm::mat4 zoomMatrix =  glm::scale(glm::mat4(1.0f), glm::vec3(Zoom,Zoom,Zoom)); 
    //printf("%f %f\n", Center[0], Center[1]);
    return (orthoMatrix * zoomMatrix);
}

void Camera::ProcessKeyboard()
{
    float velocity = Sensitivity;
    if (this->direction == UP)
    {
        this->Center[1] -=  velocity;
        //printf("up\n");
    }
    if (this->direction == DOWN){
        this->Center[1] +=  velocity;
        //printf("down\n");
    }
    if (this->direction == LEFT){
        this->Center[0] -=  velocity;
        //printf("left\n");
    }
    if (this->direction == RIGHT){
        this->Center[0] +=  velocity;
        //printf("right\n");
    }
    if (this->direction == STATIC)  {
        ;}
    //printf("%f\n", Center[0]);
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::translate(glm::mat4(1.0f), glm::vec3(-Center[0], -Center[1], 0.0f));
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= static_cast<float>(yoffset);
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

/* 3D Camera

#include "camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix(Camera lol,int SCR_WIDTH,int SCR_HEIGHT){
    return glm::perspective(glm::radians(lol.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp)); 
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::ProcessKeyboard(Camera_Movement direction)
{
    float velocity = MovementSpeed;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == STATIC)  
        ;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}



void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= static_cast<float>(yoffset);
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}
*/