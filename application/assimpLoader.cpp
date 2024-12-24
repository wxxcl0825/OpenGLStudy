#include <iostream>

#include "assimpLoader.h"
#include "../glframework/tools/tools.h"

#include "../glframework/material/phongMaterial.h"


Object* AssimpLoader::load(const std::string& path) {
    std::size_t lastIndex = path.find_last_of("/");
    auto rootPath = path.substr(0, lastIndex + 1);


    Object* rootNode = new Object();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error: Model Read Failed!" << std::endl;
        return nullptr;
    }

    processNode(scene->mRootNode, rootNode, scene, rootPath);

    return rootNode;
}

void AssimpLoader::processNode(aiNode* ainode, Object* parent, const aiScene* scene, const std::string& rootPath) {
    Object* node = new Object();
    parent->addChild(node);
    
    glm::mat4 localMatrix = getMat4f(ainode->mTransformation);

    glm::vec3 position, enlerAngle, scale;
    Tools::decompose(localMatrix, position, enlerAngle, scale);
    node->setPosition(position);
    node->setAngleX(enlerAngle.x);
    node->setAngleY(enlerAngle.y);
    node->setAngleZ(enlerAngle.y);
    node->setScale(scale);

    // 解析mesh
    for (int i = 0; i < ainode->mNumMeshes; i++) {
        int meshID = ainode->mMeshes[i];
        aiMesh* aimesh = scene->mMeshes[meshID];
        auto mesh = processMesh(aimesh, scene, rootPath);
        node->addChild(mesh);
    }

    for (int i = 0; i < ainode->mNumChildren; i++) {
        processNode(ainode->mChildren[i], node, scene, rootPath);
    }
}

Mesh* AssimpLoader::processMesh(aiMesh* aimesh, const aiScene* scene, const std::string& rootPath) {
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<unsigned int> indices;

    for (int i = 0; i < aimesh->mNumVertices; i++) {
        positions.push_back(aimesh->mVertices[i].x);
        positions.push_back(aimesh->mVertices[i].y);
        positions.push_back(aimesh->mVertices[i].z);

        normals.push_back(aimesh->mNormals[i].x);
        normals.push_back(aimesh->mNormals[i].y);
        normals.push_back(aimesh->mNormals[i].z);

        // 关注第0套uv(贴图数据)
        if (aimesh->mTextureCoords[0]) {
            uvs.push_back(aimesh->mTextureCoords[0][i].x);
            uvs.push_back(aimesh->mTextureCoords[0][i].y);
        } else {
            uvs.push_back(0.0f);
            uvs.push_back(0.0f);
        }
    }

    // 索引值解析
    for (int i = 0; i < aimesh->mNumFaces; i++) {
        aiFace face = aimesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    auto geometry = new Geometry(positions, normals, uvs, indices);
    auto material = new PhongMaterial();
    
    if (aimesh->mMaterialIndex >= 0) {
        aiMaterial* aiMat = scene->mMaterials[aimesh->mMaterialIndex];
        material->mDiffuse = processTexture(aiMat, aiTextureType_DIFFUSE, scene, rootPath);
        // Texture* SpecularMask = processTexture(aiMat, aiTextureType_SPECULAR, scene, rootPath);
        // SpecularMask->setUnit(1); // 修改纹理单元
        // material->mSpecularMask = SpecularMask;
    } else {
        // 原模型无贴图, 使用默认贴图
        material->mDiffuse = Texture::createTexture("assets/textures/defaultTexture.jpg", 0);
    }

    return new Mesh(geometry, material);
}

Texture* AssimpLoader::processTexture(const aiMaterial* aimat, const aiTextureType& type, const aiScene* scene, const std::string& rootPath) {
    Texture* texture;
    // 获取图片读取路径
    aiString aipath;
    aimat->Get(AI_MATKEY_TEXTURE(type, 0), aipath);
    if (!aipath.length) // 找不到该类型的贴图, 返回默认贴图
        return Texture::createTexture("assets/textures/defaultTexture.jpg", 0);;
    const aiTexture* aitexture = scene->GetEmbeddedTexture(aipath.C_Str());
    if (aitexture) {
        // 内嵌
        unsigned char* dataIn = reinterpret_cast<unsigned char*>(aitexture->pcData);
        uint32_t widthIn = aitexture->mWidth;
        uint32_t heightIn = aitexture->mHeight;
        texture = Texture::createTexture(aipath.C_Str(), 0, dataIn, widthIn, heightIn);
    } else {
        std::string fullPath = rootPath + aipath.C_Str();
        texture = Texture::createTexture(fullPath, 0);
    }

    return texture;
}

glm::mat4 AssimpLoader::getMat4f(aiMatrix4x4 value) {
    glm::mat4 to(
        value.a1, value.a2, value.a3, value.a4,
        value.b1, value.b2, value.b3, value.b4,
        value.c1, value.c2, value.c3, value.c4,
        value.d1, value.d2, value.d3, value.d4
    );
    return to;
}