#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stddef.h>
#include <string.h>

#include "Buffers.h"
#include "DataTypes.h"
#include "Mesh.h"

Mesh* genMesh(Vector vertices, Vector indices, Vector textures) /* equivalent of C++ (vector <Vertex> vertices, vector <unsigned int> indices, vector <Texture> textures) */
{
	Mesh mesh;

	mesh.vertices = vertices;
	mesh.indices = indices;
	for (int i = 0; i < textures.currentSize; i++) {

		if (strstr((char*)get(&textures, i), "DiffuseMap") != NULL)
			push_back(&mesh.diffuseMap, get(&textures, i));
		else if (strstr((char*)get(&textures, i), "SpecularMap") != NULL)
			push_back(&mesh.specularMap, get(&textures, i));
		else if (strstr((char*)get(&textures, i), "EmissiveMap") != NULL)
			push_back(&mesh.emissiveMap, get(&textures, i));

	}

	mesh.buffer = *genBuffers();
	writeToVBO(&mesh.buffer, &mesh.vertices, mesh.vertices.currentSize * sizeof(Vertex), GL_STATIC_DRAW);
	writeToEBO(&mesh.buffer, &mesh.indices, mesh.indices.currentSize * sizeof(unsigned int), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); //position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); //normal 
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord)); //texCoord

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	return &mesh;
}




