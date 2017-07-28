#include "assimp_game.hpp"

void AssimpGame::initialize(int argc, char *argv[])
{
    Game::initialize(argc, argv);

    GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

    {
        std::ifstream vsif("resources/assimp_game/general.vs.glsl");
        auto vs_src = std::string
        (
            std::istreambuf_iterator<char>(vsif),
            std::istreambuf_iterator<char>()
        ).c_str();
        glShaderSource(vsID, 1, &vs_src, NULL);
        glCompileShader(vsID);

        std::ifstream fsif("resources/assimp_game/general.fs.glsl");
        auto fs_src = std::string
        (
            std::istreambuf_iterator<char>(fsif),
            std::istreambuf_iterator<char>()
        ).c_str();
        glShaderSource(fsID, 1, &fs_src, NULL);
        glCompileShader(fsID);
    }

    pID = glCreateProgram();
    glAttachShader(pID, vsID);
    glAttachShader(pID, fsID);
    glLinkProgram(pID);

    mvpID = glGetUniformLocation(pID, "MVP");

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile
    (
        "resources/assimp_game/block.obj",
        aiProcessPreset_TargetRealtime_Quality
    );

    if (!scene)
    {
        std::cerr << "error while loading obj" << '\n';
        exit(0);
    }

    {
        auto mesh = scene->mMeshes[0];
        
        for (int i = 0; i < mesh->mNumVertices; i++)
        {
            vertices.push_back(glm::vec3
            (
                mesh->mVertices[i].x, 
                mesh->mVertices[i].y, 
                mesh->mVertices[i].z
            ));
        }

        for (int i = 0; i < mesh->mNumFaces; i++)
        {
            indices.push_back(mesh->mFaces[i].mIndices[0]);
            indices.push_back(mesh->mFaces[i].mIndices[1]);
            indices.push_back(mesh->mFaces[i].mIndices[2]);
        }
    }

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData
    (
        GL_ARRAY_BUFFER, 
        vertices.size() * sizeof(glm::vec3), 
        vertices.data(), 
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData
    (
        GL_ELEMENT_ARRAY_BUFFER, 
        indices.size() * sizeof(uint32_t), 
        indices.data(), 
        GL_STATIC_DRAW
    );
}

void AssimpGame::update()
{
    Game::update();

    //camera.add_phi(t.delta);
    //camera.theta = std::sin(t.full) / 2 + TAU/4;
}

void AssimpGame::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    auto mvp = projection() * view() * model();

    glUseProgram(pID);

    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
    glUniform1f(tID, t.full);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glDrawElements
    (
        GL_TRIANGLES,
        indices.size(),
        GL_UNSIGNED_INT,
        (void*)0
    );
    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void AssimpGame::deinitialize()
{
    Game::deinitialize();
}
