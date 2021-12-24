#pragma once
#include "Camera.h"
#include "Light.h"
#include "Entity.h"
#include "TexturedModel.h"
#include "ShadowShader.h"
#include "ShadowBox.h"
#include "ShadowFrameBuffer.h"
#include "ShadowEntityRender.h"
#include <vector>
#include <map>

using namespace glm;

class ShadowMasterRenderer {
private:
    int SHADOW_MAP_SIZE = 2048;
	int flag = 1;
	float nearPlane = 1.0;
    float farPlane = 7.5;


    ShadowBox shadowBox;
    
    mat4 projectionMatrix = mat4(1.0f), lightViewMatrix = mat4(1.0f), pvMatrix = mat4(1.0f);
    mat4 offset = createOffset();
    
    ShadowFrameBuffer shadowFbo;
	ShadowShader shader;
	ShadowEntityRender entityRender;
    

    mat4 createOffset() {
        mat4 offset = mat4(1.0f);
        offset = translate(offset, vec3(0.5, 0.5, 0.5));
        offset = scale(offset, vec3(0.5, 0.5, 0.5));
        return offset;
    }

    void prepare(vec3 lightDirection, ShadowBox box) {
		updateOrthoProjectionMatrix(box.getWidth(), box.getHeight(), box.getLength());
		updateLightViewMatrix(lightDirection, box.getCenter());
		shadowBox.setLightViewMatrix(lightViewMatrix);
		// lightViewMatrix = lookAt(lightDirection, vec3(0.0f), vec3(0.0, 1.0, 0.0));
		// projectionMatrix = ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
		pvMatrix = projectionMatrix * lightViewMatrix;
		entityRender.setpvMatrix(pvMatrix);
		shadowFbo.bindFrameBuffer();
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		shader.Start();
	}

    void updateLightViewMatrix(vec3 direction, vec3 center) {
		vec3 dir = normalize(direction);
		vec3 cen = center * -1.0f;
		lightViewMatrix = mat4(1.0f);
		float pitch = (float) acos(length(vec2(dir.x, dir.z)));
        lightViewMatrix = rotate(lightViewMatrix, pitch, vec3(1, 0, 0));
		float yaw = (float) degrees(((float) atan(dir.x / dir.z)));
		yaw = dir.z > 0 ? yaw - 180 : yaw;
        lightViewMatrix = rotate(lightViewMatrix, -radians(yaw), vec3(0, 1, 0));
		lightViewMatrix = translate(lightViewMatrix, cen);
	}

    void updateOrthoProjectionMatrix(float width, float height, float length) {
		projectionMatrix = mat4(1.0f);
		projectionMatrix[0][0] = 2.0f / width;
		projectionMatrix[1][1] = 2.0f / height;
		projectionMatrix[2][2] = -2.0f / length;
		projectionMatrix[3][3] = 1;
	}

    void finish() {
		shader.Stop();
		shadowFbo.unbindFrameBuffer();
	}

public:
    ShadowMasterRenderer(){}

    ShadowMasterRenderer(Camera camera) {
		shader = ShadowShader("depth.vs", "depth.fs");
        shadowBox = ShadowBox(lightViewMatrix, camera);
		shadowFbo = ShadowFrameBuffer(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
		entityRender = ShadowEntityRender(shader, pvMatrix);
		std::cout << "create a shadowMasterRender" << std::endl;
    }

    void render(std::map<TexturedModel, std::vector<Entity>> entities, Light sun) {
		shadowBox.update();
		vec3 sunPosition = sun.pos;
		vec3 lightDirection = vec3(-sunPosition.x, -sunPosition.y, -sunPosition.z);
		prepare(lightDirection, shadowBox);
		//prepare(sunPosition, shadowBox);
		entityRender.render(entities);
		finish();
	}

    mat4 getToShadowMapSpaceMatrix() {
		mat4 ret = offset * pvMatrix;
		return ret;
	}

    void cleanUp() {
		shader.Clear();
		shadowFbo.cleanup();
	}

	unsigned int getShadowMap() {
		return shadowFbo.shadowMap;
	}

	mat4 getLightSpaceTransform() {
		return lightViewMatrix;
	}

};