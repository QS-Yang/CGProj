#pragma once
#include <string>
#include "SkyboxShader.h"
#include "Model.h"
#include "Camera.h"

#define SIZE 500.0
float VERTEICES[] = {
    -SIZE, SIZE, -SIZE,
    -SIZE, -SIZE, -SIZE,
    SIZE, -SIZE, -SIZE,
    SIZE, -SIZE, -SIZE,
    SIZE, SIZE, -SIZE,
    -SIZE, SIZE, -SIZE,

    -SIZE, -SIZE, SIZE,
    -SIZE, -SIZE, -SIZE,
    -SIZE, SIZE, -SIZE,
    -SIZE, SIZE, -SIZE,
    -SIZE, SIZE, SIZE,
    -SIZE, -SIZE, SIZE,

    SIZE, -SIZE, -SIZE,
    SIZE, -SIZE, SIZE,
    SIZE, SIZE, SIZE,
    SIZE, SIZE, SIZE,
    SIZE, SIZE, -SIZE,
    SIZE, -SIZE, -SIZE,

    -SIZE, -SIZE, SIZE,
    -SIZE, SIZE, SIZE,
    SIZE, SIZE, SIZE,
    SIZE, SIZE, SIZE,
    SIZE, -SIZE, SIZE,
    -SIZE, -SIZE, SIZE,

    -SIZE, SIZE, -SIZE,
    SIZE, SIZE, -SIZE,
    SIZE, SIZE, SIZE,
    SIZE, SIZE, SIZE,
    -SIZE, SIZE, SIZE,
    -SIZE, SIZE, -SIZE,

    -SIZE, -SIZE, -SIZE,
    -SIZE, -SIZE, SIZE,
    SIZE, -SIZE, -SIZE,
    SIZE, -SIZE, -SIZE,
    -SIZE, -SIZE, SIZE,
    SIZE, -SIZE, SIZE
};

class SkyboxRender {
private:
    std::string TEXTUREFILES[6] = {
        "../skybox/right.png",
        "../skybox/left.png",
        "../skybox/top.png",
        "../skybox/bottom.png",
        "../skybox/back.png",
        "../skybox/front.png"
    };
public:
    Model cube;
    unsigned int textureID;
    SkyboxShader* shader;

    SkyboxRender(Loader loader, mat4 projectMatrix) {
        cube = loader.LoadToV(VERTEICES, (int)(sizeof(VERTEICES) / sizeof(VERTEICES[0])), 3);
        textureID = loader.loadCubeMap(TEXTUREFILES, 6);
        shader = new SkyboxShader("../sShader.vs", "../sShader.fs");
        shader->start();
        shader->loadProjectionMatrix(projectMatrix);
        shader->stop();
    }

    void render(Camera camera) {
        shader->start();
        shader->loadViewMatrix(camera);
        glBindVertexArray(cube.getVAO());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        glDrawElements(GL_TRIANGLES, cube.getVertexCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        shader->stop();
    }
};
