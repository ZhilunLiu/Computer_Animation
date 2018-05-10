#include "Model.hpp"
#include "Paths.hpp"
#include "LayoutLocations.glsl"

#include <atlas/core/ImGUI.hpp>

namespace gl = atlas::gl;
namespace math = atlas::math;

Model::Model(std::string const& filename, std::string const& mtl, 
    bool triangulate) :
    mVertexBuffer(GL_ARRAY_BUFFER),
    mNormalBuffer(GL_ARRAY_BUFFER),
    mTextureBuffer(GL_ARRAY_BUFFER),
    mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER),
    mStride(0),
    mRenderMode(2),

    // spring
    sl0(0.5f),
    sks(1.0f),
    kd(0.4f),
    sip(math::Vector(0.0000001f, 0.0000001f, 0.0000001f)),

    // mass
    mpt(math::Vector(0.0000001f, -20.0f, 0.0000001f)), // initial position
    mvt(math::Vector(0.0f, 3.0f, 0.0f)), // initial velocity
    mw(1.0f),
    fg(math::Vector(0.0f, -0.6f, 0.0f))
{
    atlas::utils::Mesh::fromFile(filename, mMesh, mtl, triangulate);

    mVao.bindVertexArray();
    mVertexBuffer.bindBuffer();
    mVertexBuffer.bufferData(gl::size<math::Point>(mMesh.vertices().size()),
        mMesh.vertices().data(), GL_DYNAMIC_DRAW);
    mVertexBuffer.vertexAttribPointer(VERTICES_LAYOUT_LOCATION, 3, GL_FLOAT,
        GL_FALSE, 0, gl::bufferOffset<math::Point>(0));
    mVao.enableVertexAttribArray(VERTICES_LAYOUT_LOCATION);
    mVertexBuffer.unBindBuffer();

    if (!mMesh.normals().empty())
    {
        mNormalBuffer.bindBuffer();
        mNormalBuffer.bufferData(gl::size<math::Normal>(mMesh.normals().size()),
            mMesh.normals().data(), GL_DYNAMIC_DRAW);
        mNormalBuffer.vertexAttribPointer(NORMALS_LAYOUT_LOCATION, 3, GL_FLOAT,
            GL_FALSE, 0, gl::bufferOffset<math::Normal>(0));
        mVao.enableVertexAttribArray(NORMALS_LAYOUT_LOCATION);
        mNormalBuffer.unBindBuffer();
    }

    if (!mMesh.texCoords().empty())
    {
        mTextureBuffer.bindBuffer();
        mTextureBuffer.bufferData(gl::size<math::Point2>(mMesh.texCoords().size()),
            mMesh.texCoords().data(), GL_DYNAMIC_DRAW);
        mTextureBuffer.vertexAttribPointer(TEX_COORDS_LAYOUT_LOCATION, 2,
            GL_FLOAT, GL_FALSE, 0, gl::bufferOffset<math::Point2>(0));
        mVao.enableVertexAttribArray(TEX_COORDS_LAYOUT_LOCATION);
        mTextureBuffer.unBindBuffer();
    }

    mIndexBuffer.bindBuffer();
    mIndexBuffer.bufferData(gl::size<GLuint>(mMesh.indices().size()),
        mMesh.indices().data(), GL_DYNAMIC_DRAW);
    mVao.unBindVertexArray();

    // Load the shaders here.
    std::vector<gl::ShaderUnit> shaders 
    {
        {std::string(ShaderDirectory) + "Model.vs.glsl", GL_VERTEX_SHADER},
        {std::string(ShaderDirectory) + "Model.fs.glsl", GL_FRAGMENT_SHADER}
    };

    mShaders.push_back(gl::Shader(shaders));
    mShaders[0].setShaderIncludeDir(ShaderDirectory);
    mShaders[0].compileShaders();
    mShaders[0].linkShaders();

    auto var = mShaders[0].getUniformVariable("model");
    mUniforms.insert(UniformKey("model", var));
    var = mShaders[0].getUniformVariable("projection");
    mUniforms.insert(UniformKey("projection", var));
    var = mShaders[0].getUniformVariable("view");
    mUniforms.insert(UniformKey("view", var));
    var = mShaders[0].getUniformVariable("renderMode");
    mUniforms.insert(UniformKey("renderMode", var));

}

void Model::renderGeometry(atlas::math::Matrix4 const& projection, atlas::math::Matrix4 const& view)
{
    mShaders[0].hotReloadShaders();
    if (!mShaders[0].shaderProgramValid())
    {
        return;
    }
    mShaders[0].enableShaders();

    glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);
    glUniform1i(mUniforms["renderMode"], mRenderMode);

    mVao.bindVertexArray();

    mIndexBuffer.bindBuffer();

    if (mRenderMode == 0)
    {
        // Point cloud.
        auto verts = mMesh.vertices().size();
        glDrawArrays(GL_POINTS, 0, (GLsizei)verts);
    }
    else if (mRenderMode == 1)
    {
        // Wireframe.
        glUniform1i(mUniforms["renderMode"], 0);
        auto verts = mMesh.vertices().size();
        glDrawArrays(GL_POINTS, 0, (GLsizei)verts);

        glUniform1i(mUniforms["renderMode"], 1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        auto idx = mMesh.indices().size();
        glDrawElements(GL_TRIANGLES, (GLsizei)idx, 
            GL_UNSIGNED_INT, gl::bufferOffset<GLuint>(0));
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (mRenderMode == 2 || mRenderMode == 3)
    {
        auto idx = mMesh.indices().size();
        glDrawElements(GL_TRIANGLES, (GLsizei)idx, 
            GL_UNSIGNED_INT, gl::bufferOffset<GLuint>(0));
    }
    
    mIndexBuffer.unBindBuffer();
    mVao.unBindVertexArray();
    mShaders[0].disableShaders();
}

void Model::updateGeometry(atlas::core::Time<> const& t)
{
    // Compute the current spring length
    slt = std::sqrt(std::pow(mpt[0], 2) + std::pow(mpt[1], 2));

    // Compute the spring force fs
    fs[0] = sks * ((sip[0] - mpt[0]) / slt) * (slt - sl0);
    fs[1] = sks * ((sip[1] - mpt[1]) / slt) * (slt - sl0);
    fs[2] = sks * ((sip[2] - mpt[2]) / slt) * (slt - sl0);

    // Compute the damping force fd
    fd[0] = -kd * mvt[0];
    fd[1] = -kd * mvt[1];
    fd[2] = -kd * mvt[2];

    // Compute the total force
    f[0] = fs[0] + fd[0] + fg[0];
    f[1] = fs[1] + fd[1] + fg[1];
    f[2] = fs[2] + fd[2] + fg[2];

    // Update mass position
    mpt[0] = mpt[0] + (t.currentTime/100 * mvt[0]);
    mpt[1] = mpt[1] + (t.currentTime/100 * mvt[1]);
    mpt[2] = mpt[2] + (t.currentTime/100 * mvt[2]);

    // Update mass velocity
    mvt[0] = mvt[0] + ((t.currentTime/100 * f[0]) / mw);
    mvt[1] = mvt[1] + ((t.currentTime/100 * f[1]) / mw);
    mvt[2] = mvt[2] + ((t.currentTime/100 * f[2]) / mw);

    auto translate = glm::translate(math::Matrix4(1.0f), mpt);

    mModel = translate;
}

void Model::drawGui()
{
    ImGui::SetNextWindowSize(ImVec2(300, 100),
        ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Render Controls");

    std::vector<const char*> renderNames = { "Vertices", "Wireframe", "Shaded", 
        "Normals", "Lines" };
    ImGui::Combo("Render Mode", &mRenderMode, renderNames.data(),
        ((int)renderNames.size()));
    ImGui::End();
}
