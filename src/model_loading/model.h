//
//  model.h
//
//  Created by Clsrfish on 08/10/2020
//

#if !defined(MODEL_H)
#define MODEL_H

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "./mesh.h"
#include "../gl/shader.h"
#include "../utils/log.h"

namespace model_loading
{
    class Model
    {
    public:
        Model(const std::string &modelPath);

        void Draw(const Shader &shader);

    private:
        std::vector<Texture> loadedTextures;
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(const std::string &modelPath);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType texType, const std::string &typeName);
    };
} // namespace model_loading

#endif