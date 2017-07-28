#include "sphere_game.hpp"

void SphereGame::dig(int depth)
{
    for (size_t d = 0; d < depth; d++)
    {
        auto length = indices.size() / 3;
        while (length != 0)
        {
            auto iA = indices[0];
            auto iB = indices[1];
            auto iC = indices[2];
            indices.erase(indices.begin(), indices.begin() + 3);

            auto vA = vertices[iA];
            auto vB = vertices[iB];
            auto vC = vertices[iC];

            auto vA_B = (glm::vec3(0.5)*vB) + (glm::vec3(0.5)*vA);
            auto vB_C = (glm::vec3(0.5)*vC) + (glm::vec3(0.5)*vB);
            auto vC_A = (glm::vec3(0.5)*vA) + (glm::vec3(0.5)*vC);

            auto iA_B = vertices.size();
            vertices.push_back(vA_B);
            auto iB_C = vertices.size();
            vertices.push_back(vB_C);
            auto iC_A = vertices.size();
            vertices.push_back(vC_A);

            indices.push_back(iA);
            indices.push_back(iA_B); 
            indices.push_back(iC_A);

            indices.push_back(iB);
            indices.push_back(iB_C);
            indices.push_back(iA_B);

            indices.push_back(iC);
            indices.push_back(iC_A);
            indices.push_back(iB_C);

            indices.push_back(iA_B);
            indices.push_back(iB_C);
            indices.push_back(iC_A);

            length--;
        }
    }
}