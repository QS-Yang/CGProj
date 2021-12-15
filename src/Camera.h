#pragma once
#include "Player.h"
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#define Step 0.2
#define Pi 3.1415926
using namespace glm;

class Camera{
public:
    float distanceFromPlayer = 20;
    float angleAroundPlayer = 0;

    vec3 position = vec3(0,0,0);
    float pitch = 10;
    float yaw;
    float roll;

    bool firstMouse = true;
    float lastX =  800.0f / 2.0;
    float lastY =  600.0 / 2.0;
    float fov   =  45.0f;

    double *xpos=new double, *ypos = new double;
    float xoffset=0, yoffset=0;

    GLFWwindow* window;

    Player *player;

    Camera(GLFWwindow* window, Player *player){
        this->window = window;
        this->player = player;
    }

    void move(){
        // if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        //     position.z-=Step;
        // }
        // if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        //     position.z+=Step;
        // }
        // if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        //     position.x-=Step;
        // }
        // if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        //     position.x+=Step;
        // }
        flushmouse();
        // calculateZoom();
        calculatePitch();
        calculateAngleAroundPlayer();
        float horizontalDistance = calculateHorizontalDistance();
        float verticalDistance = calculateVerticalDistance();
        calculateCameraPosition(horizontalDistance, verticalDistance);
        yaw = 180 - (player->ry + angleAroundPlayer);
    }

    // void calculateZoom(){
    //     float zoomLevel = glScroll * 0.01;
    //     distanceFromPlayer -= zoomLevel;
    // }

    void calculatePitch(){
        if(glfwGetMouseButton(window, 0)==GLFW_PRESS){
            float pitchChange = yoffset * 0.1;
            pitch -= pitchChange;
        }
    }

    void calculateAngleAroundPlayer(){
        if(glfwGetMouseButton(window, 0)==GLFW_PRESS){
            float angleChange = xoffset * 0.3;
            angleAroundPlayer -= angleChange;
        }
    }

    float calculateHorizontalDistance(){
        return (float) (distanceFromPlayer * cos(pitch*2*Pi/360.0));
    }

    float calculateVerticalDistance(){
        return (float) (distanceFromPlayer * sin(pitch*2*Pi/360.0));
    }

    void calculateCameraPosition(float hDistance, float vDistance){
        float theta = player->ry + angleAroundPlayer;
        float offsetx = (float) (hDistance * sin(theta*2*Pi/360.0));
        float offsetz = (float) (hDistance * cos(theta*2*Pi/360.0));
        position.x = player->position.x - offsetx;
        position.z = player->position.z - offsetz;
        position.y = player->position.y + vDistance;
    }

    void flushmouse(){
        glfwGetCursorPos(window, xpos, ypos); 
        if(firstMouse)
        {
            lastX = *xpos;
            lastY = *ypos;
            firstMouse = false;
        }

        xoffset = *xpos - lastX;
        yoffset = lastY - *ypos; 
        lastX = *xpos;
        lastY = *ypos;
    }

};

mat4 createViewMatrix(Camera camera){
	mat4 viewMat = mat4(1.0);
	viewMat = rotate<float>(viewMat, radians(camera.pitch), vec3(1, 0, 0));
	viewMat = rotate<float>(viewMat, radians(camera.yaw), vec3(0, 1, 0));
	vec3 cameraPos = camera.position;
	vec3 negCameraPos = -cameraPos;
	viewMat = translate<float>(viewMat, negCameraPos);
	return viewMat;
}