//
//  mesh.h
//
//  Created by Clsrfish on 08/10/2020
//

#if !defined(MESH_H)
#define MESH_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "../gl/shader.h"
#include "../utils/log.h"

namespace model_loading
{

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture
    {
        unsigned int id;
        std::string type;
        std::string path;
    };
    class Mesh
    {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        void Draw(const Shader &shader);

    private:
        unsigned int VAO, VBO, EBO;
        void setupMesh();
    };
} // namespace model_loading

#endif