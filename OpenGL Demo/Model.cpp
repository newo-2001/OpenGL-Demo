#include "Model.h"

#include <assimp\Importer.hpp>
#include <format>
#include <iostream>

Model::Model(const std::string& filePath)
{
    Assimp::Importer importer;
    
    unsigned int flags =  aiProcess_Triangulate
                        | aiProcess_FlipUVs
                        | aiProcess_GenSmoothNormals
                        | aiProcess_JoinIdenticalVertices;

    const aiScene* scene = importer.ReadFile(filePath, flags);
    
    if (!scene)
    {
        throw std::runtime_error(std::format("Failed to load model '{}':,\n\t{}", filePath, importer.GetErrorString()));
    }

    LoadNode(scene->mRootNode, scene);
    LoadMaterials(scene);
}

void Model::Render() const
{
    for (size_t i = 0; i < m_meshes.size(); i++)
    {
        unsigned int materialIndex = m_meshTextureIndices[i];
        
        if (materialIndex < m_textures.size() && m_textures[materialIndex])
        {
            m_textures[materialIndex]->Use();
        }

        m_meshes[i]->Render();
    }
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        LoadNode(node->mChildren[i], scene);
    }
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<GLfloat> vertices;
    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        const aiVector3D& vertex = mesh->mVertices[i];
        vertices.insert(vertices.end(), { vertex.x, vertex.y, vertex.z });

        if (mesh->mTextureCoords[0])
        {
            const aiVector3D& uvs = mesh->mTextureCoords[0][i];
            vertices.insert(vertices.end(), { uvs.x, uvs.y });
        }
        else
        {
            vertices.insert(vertices.end(), { 0.0f, 0.0f });
        }
        
        const aiVector3D normal = mesh->mNormals[i];
        vertices.insert(vertices.end(), { -normal.x, -normal.y, -normal.z });
    }

    std::vector<unsigned int> indices;
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];
        
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    m_meshes.push_back(std::make_unique<Mesh>(vertices.data(), vertices.size(), indices.data(), indices.size()));
    m_meshTextureIndices.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
    std::shared_ptr<Texture> missingTexture = std::make_shared<Texture>("Resources/images/plain.png", true);

    m_textures.resize(scene->mNumMaterials);

    for (size_t i = 0; i < scene->mNumMaterials; i++)
    {
        const aiMaterial& material = *scene->mMaterials[i];

        m_textures[i] = nullptr;

        if (material.GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString path;
            if (material.GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS)
            {
                int index = std::string(path.data).rfind("\\");
                std::string filename = std::string(path.data).substr(index + 1);
                std::string texturePath = "Resources/images/" + filename;

                try
                {
                    m_textures[i] = std::make_unique<Texture>(texturePath);
                }
                catch (...)
                {
                    std::cerr << "Failed to load texture: " << texturePath << std::endl;
                    m_textures[i] = nullptr;
                }
            }
        }

        if (!m_textures[i])
        {
            m_textures[i] = missingTexture;
        }
    }
}

