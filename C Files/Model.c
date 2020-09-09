#include "Model.h"

bool loadModel(Object* obj, const char* filePath)
{
	const struct aiScene* scene = aiImportFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene) {
		printf("Could not load the scene graph.");
		return false;
	}

	processNode(obj, scene->mRootNode, scene);

	aiReleaseImport(scene);

	return true;
}

void processNode(Object* obj, struct aiNode* node, const struct aiScene* scene)
{
	/* process node mesh */
	for (int i = 0; i < node->mNumMeshes; i++) {
		
		struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(obj, mesh, scene);

	}
}

void processMesh(Object* obj, struct aiMesh* mesh, const struct aiScene* scene) 
{
	initVector(&(obj->mesh.vertices), sizeof(Vertex));
	initVector(&(obj->mesh.indices), sizeof(int));
	initVector(&(obj->mesh.diffuseMap), sizeof(unsigned int));
	initVector(&(obj->mesh.specularMap), sizeof(unsigned int));
	initVector(&(obj->mesh.emissiveMap), sizeof(unsigned int));

	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		kmVec3 vPos;
		kmVec3Fill(&vPos, mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.pos = vPos;

		kmVec3 vNorm;
		kmVec3Fill(&vNorm, mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.normal = vNorm;

		kmVec2 vTexCoord;
		if (mesh->mTextureCoords[0]) {
			kmVec2Fill(&vTexCoord, mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			vertex.texCoord = vTexCoord;
		}
		else
			vertex.texCoord = *kmVec2Fill(&vTexCoord, 0.0f, 0.0f);

		push_back(&(obj->mesh.vertices), (void*)(&vertex));
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		struct aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			push_back(&(obj->mesh.indices), (void*)(&face.mIndices[j]));
		}
	}
}
