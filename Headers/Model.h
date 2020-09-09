#pragma once
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdbool.h>
#include <stdio.h>

#include "Mesh.h"
#include "Object.h"

bool loadModel(Object* obj, const char* fileName);

void processNode(Object* obj, struct aiNode* node, const struct aiScene* scene);

void processMesh(Object* obj, struct aiMesh* mesh, const struct aiScene* scene);