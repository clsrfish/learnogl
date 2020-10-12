//
//  model.cpp
//
//  Created by Clsrfish on 08/10/2020
//

#include "./model.h"
#include "../gl/gl_utils.h"

model_loading::Model::Model(const std::string &modelPath)
{
    loadModel(modelPath);
}
void model_loading::Model::Draw(const Shader &shader)
{
    for (unsigned int i = 0; i < Meshes.size(); i++)
    {
        Meshes[i].Draw(shader);
    }
}

void model_loading::Model::loadModel(const std::string &modelPath)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(modelPath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
    {
        LOG_E("ASSMIP::%s", importer.GetErrorString());
        return;
    }
    directory = modelPath.substr(0, modelPath.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void model_loading::Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        Meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

model_loading::Mesh model_loading::Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 pos;
        pos.x = mesh->mVertices[i].x;
        pos.y = mesh->mVertices[i].y;
        pos.z = mesh->mVertices[i].z;
        vertex.Position = pos;

        glm::vec3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        vertex.Normal = normal;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 texCoord;
            texCoord.x = mesh->mTextureCoords[0][i].x;
            texCoord.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = texCoord;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        // process all materials
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "u_TextureDiffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "u_TextureSpecular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "u_TextureNormal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "u_TextureHeight");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    return model_loading::Mesh(vertices, indices, textures);
}

std::vector<model_loading::Texture> model_loading::Model::loadMaterialTextures(aiMaterial *material, aiTextureType texType, const std::string &typeName)
{
    std::vector<model_loading::Texture> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(texType); ++i)
    {

        aiString str;
        material->GetTexture(texType, i, &str);
        auto file = std::string(str.C_Str());
        bool useCache = false;
        for (unsigned int j = 0; j < loadedTextures.size(); j++)
        {
            if (file.compare(loadedTextures[j].path) == 0)
            {
                LOG_I("Texture loaded preciously: %s", file.c_str());
                textures.push_back(loadedTextures[j]);
                useCache = true;
                break;
            }
        }
        if (!useCache)
        {
            model_loading::Texture texture;
            texture.id = TextureFromFile(file, directory);
            texture.type = typeName;
            texture.path = file;

            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }
    return textures;
}
