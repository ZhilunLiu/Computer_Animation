#include "ModelViewer.hpp"
#include "Paths.hpp"

#include <atlas/gl/GL.hpp>
#include <atlas/utils/GUI.hpp>

namespace gl = atlas::gl;
namespace math = atlas::math;
namespace utils = atlas::utils;
namespace tools = atlas::tools;

ModelViewer::ModelViewer() :
    mCurrentModel(0)
{
    mModels.emplace_back(std::string(DataDirectory) + "cube.obj");
    mModels.emplace_back(std::string(DataDirectory) + "dragon.obj");
    mModels.emplace_back(std::string(DataDirectory) + "suzanne.obj");
}

void ModelViewer::renderScene()
{
    utils::Gui::getInstance().newFrame();
    const float grey = 92.0f / 255.0f;
    glClearColor(grey, grey, grey, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mProjection = glm::perspective(
        glm::radians(mCamera.getCameraFOV()),
        (float)mWidth / mHeight, 1.0f, 1000000.0f);

    mView = mCamera.getCameraMatrix();

    mUniformMatrixBuffer.bindBuffer();
    mUniformMatrixBuffer.bufferSubData(0, sizeof(math::Matrix4),
        &mProjection[0][0]);
    mUniformMatrixBuffer.bufferSubData(sizeof(math::Matrix4),
        sizeof(math::Matrix4), &mView[0][0]);
    mUniformMatrixBuffer.unBindBuffer();

    if (mShowGrid)
    {
        mGrid.renderGeometry(mProjection, mView);
    }

    // Global HUD
    ImGui::SetNextWindowSize(ImVec2(350, 140), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Global HUD");
    ImGui::Checkbox("Show grid", &mShowGrid);
    if (ImGui::Button("Reset Camera"))
    {
        mCamera.resetCamera();
    }

    // Model select.
    std::vector<const char*> names = { "Cube", "Dragon", "Suzanne" };
    ImGui::Combo("Select model", &mCurrentModel, names.data(), 
        ((int)names.size()));

    ImGui::Text("Application average %.3f ms/frame (%1.f FPS)",
        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    // If you are using GUI's for your geometries, call it here.
    mModels[mCurrentModel].drawGui();

    // Make sure you call this so the GUI gets drawn!
    ImGui::Render();

    // Render your models.
    mModels[mCurrentModel].renderGeometry();
}