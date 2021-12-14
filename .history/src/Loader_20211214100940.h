#pragma once
#include "Model.h"
#include "Texture.h"
#include "TextureData.h"
#include <list>
#include "stb_image.h"
using namespace std;

class Loader {
private:
	list<unsigned int> VAOs;
	list<unsigned int> VBOs;
	list<unsigned int> Textures;

	unsigned int CreateVAO() {
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		VAOs.push_back(VAO);
		glBindVertexArray(VAO);
		return VAO;
	}
	void storeDataIntoAttriList(float position[], int length, int an, int cs) {
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		VBOs.push_back(VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * length, position, GL_STATIC_DRAW);
		glVertexAttribPointer(an, cs, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
	}

	void UnBindVAO() {
		glBindVertexArray(0);
	}

	void BindIndicesBuffer(int indices[], int length) {
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		VBOs.push_back(VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * length, indices, GL_STATIC_DRAW);
	}

public:
	void Clean() {
		for(unsigned int VAO : VAOs) {
			glDeleteVertexArrays(1, &VAO);
		}
		for (unsigned int VBO : VBOs) {
			glDeleteVertexArrays(1, &VBO);
		}
		for (unsigned int texture : Textures) {
			glDeleteTextures(1, &texture);
		}
	}

	Model LoadToV(float position[], int length, float textureCoord[], int lenofTex, int indices[], int lengthofIdx, float normal[], int lenofNor) {
		unsigned int VAO = CreateVAO();
		BindIndicesBuffer(indices, lengthofIdx);
		storeDataIntoAttriList(position, length, 0, 3);
		storeDataIntoAttriList(textureCoord, lenofTex, 1, 2);
		storeDataIntoAttriList(normal, lenofNor, 2, 3);
		UnBindVAO();
		return Model(VAO, lengthofIdx);
	}
	
	Model LoadToV(float positions[], int length, int dimensions) {
		unsigned int VAO = CreateVAO();
		storeDataIntoAttriList(positions, length, 0, dimensions);
		UnBindVAO();
		return Model(VAO, length / dimensions);
	}

	unsigned int loadTexture(string filename) {

		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			if(nrChannels == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if(nrChannels == 4)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		// if(filename == "../texture/blendMap.png")
		// 	printf("%d\n", data[1000]);
		// else
		// 	printf("No");
		stbi_image_free(data);
		Texture texture(textureID);
		Textures.push_back(textureID);
		return textureID;
	}

	int loadCubeMap(string textureFiles[], int length) {
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		for(int i = 0; i < length; i++) {
			TextureData data = decodeTextureFile("../texture" + textureFiles[i]);
			if(data.nrChannels == 3) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, data.width, data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data);
			}
			else if(data.nrChannels == 4) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Textures.push_back(textureID);
		return textureID;
	}

	TextureData decodeTextureFile(string filename) {
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			return TextureData(width, height, nrChannels,data);
		}
		else {
			std::cout << "Failed to load skybox texture" << std::endl;
		}
		stbi_image_free(data);
		return TextureData(0, 0, 0, nullptr);
	}
};