#pragma once

#include <kazmath/kazmath.h>

#include "Buffers.h"
#include "DataTypes.h"
#include "Textures.h"

typedef struct Vertex 
{
	kmVec3 pos;
	kmVec3 normal;
	kmVec2 texCoord;

} Vertex;

typedef struct Mesh 
{
	Vector vertices; //type: Vertex
	Vector indices; //type: int
	Vector diffuseMap; //type: uint
	Vector specularMap; //type: uint
	Vector emissiveMap; //type: uint
	Buffers buffer;

} Mesh;

Mesh* genMesh(Vector vertices, Vector indices, Vector textures);
