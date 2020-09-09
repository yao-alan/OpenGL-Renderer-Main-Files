#pragma once

#include <kazmath/kazmath.h>

#include "Buffers.h"
#include "DataTypes.h"
#include "Mesh.h"

typedef struct Material 
{
	kmVec3 ambient;
	kmVec3 diffuse;
	kmVec3 specular;
	Vector diffuseMap; /* basically just a texture */
	Vector specularMap;
	Vector emissiveMap; /* unaffected by lighting */
	float shininess; /* sharpness of specular component */
} Material;

typedef struct Spotlight 
{
	kmVec3 lookDir;
	float innerCutoff;
	float outerCutoff;
} Spotlight;

typedef struct Attenuation 
{
	float constant;
	float linear;
	float quadratic;
} Attenuation;

typedef struct WorldSpaceData 
{
	kmVec3 pos;
	kmVec3 rotationAxis;
	kmVec3 rotatedAngle;
} WorldSpaceData;

typedef struct Object 
{
	WorldSpaceData worldSpace;
	Mesh mesh;
	Buffers buffer;
	Material material;
	Attenuation attenuation; //only if object is a light source; otherwise useless
	Spotlight spotlight; //only if object is a spotlight; otherwise useless
} Object;

void drawObject(Object obj);



