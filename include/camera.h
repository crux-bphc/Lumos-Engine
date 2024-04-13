#pragma once
#include "input.h"


class Camera{
    input* Input;

    public:

        float movementArray[2] = {0};
        Camera(input* Input);
        void UpdateMovementArray();
};





