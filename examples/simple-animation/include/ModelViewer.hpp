#ifndef OBJECT_LOADER_INCLUDE_MODEL_VIEWER_HPP
#define OBJECT_LOADER_INCLUDE_MODEL_VIEWER_HPP

#pragma once

#include "Model.hpp"

#include <atlas/tools/ModellingScene.hpp>
#include <atlas/utils/FPSCounter.hpp>

class ModelViewer : public atlas::tools::ModellingScene
{
public:
    ModelViewer();

    void renderScene() override;

    // If you want to do animations you need to override this function in both
    // your scene class and your geometry.
    void updateScene(double time) override;

private:
    int mCurrentModel;
    std::vector<Model> mModels;
    atlas::utils::FPSCounter mCounter;
};


#endif