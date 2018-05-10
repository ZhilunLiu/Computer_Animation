#include "ModelViewer.hpp"
#include "Paths.hpp"

#include <atlas/gl/GL.hpp>
#include <atlas/utils/GUI.hpp>

namespace gl = atlas::gl;
namespace math = atlas::math;
namespace utils = atlas::utils;
namespace tools = atlas::tools;

ModelViewer::ModelViewer() :
    mCurrentModel(3),
    mCounter(60.0f),
    mShowGrid(false)
{
    mModels.emplace_back(std::string(DataDirectory) + "dragon.obj");
    mModels.emplace_back(std::string(DataDirectory) + "suzanne.obj");
    mModels.emplace_back(std::string(DataDirectory) + "cube.obj");
    mModels.emplace_back(std::string(DataDirectory) + "ball.obj");
}

void ModelViewer::renderScene()
{
    utils::Gui::getInstance().newFrame();
    // setting the background color for the scene
    const float grey = 92.0f / 255.0f;
    glClearColor(grey, grey, grey, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setting the camera position for the scene
    mProjection = glm::perspective(
        glm::radians(mCamera.getCameraFOV()),
        (float)mWidth / mHeight, 10.0f, 1000000.0f);

    mView = mCamera.getCameraMatrix();

    mUniformMatrixBuffer.bindBuffer();
    mUniformMatrixBuffer.bufferSubData(0, sizeof(math::Matrix4),
        &mProjection[0][0]);
    mUniformMatrixBuffer.bufferSubData(sizeof(math::Matrix4),
        sizeof(math::Matrix4), &mView[0][0]);
    mUniformMatrixBuffer.unBindBuffer();

    // whether to show the grid or not
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
    std::vector<const char*> names = { "Dragon", "Suzanne", "Cube", "Ball" };
    ImGui::Combo("Select model", &mCurrentModel, names.data(), 
        ((int)names.size()));

    ImGui::Text("Application average %.3f ms/frame (%1.f FPS)",
        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    mModels[mCurrentModel].drawGui();

    ImGui::Render();

    mModels[mCurrentModel].renderGeometry(mProjection, mView);
}

void ModelViewer::updateScene(double time)
{
    // Call this funtction first. It will handle all of the necessary updates
    // to the time variable and GUI options.
    ModellingScene::updateScene(time);

    // We use a counter to ensure that things only get updated at specific
    // times. So all that we have to do is query our FPSCounter to see if 
    // we need to update our geometry.
    if (mCounter.isFPS(mTime))
    {
        mModels[mCurrentModel].updateGeometry(mTime);
    }
}
