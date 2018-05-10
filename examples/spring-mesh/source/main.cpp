#include "ModelViewer.hpp"

#include <atlas/utils/Application.hpp>
#include <atlas/utils/WindowSettings.hpp>
#include <atlas/gl/ErrorCheck.hpp>

int main()
{
    using atlas::utils::WindowSettings;
    using atlas::utils::ContextVersion;
    using atlas::utils::Application;
    using atlas::utils::ScenePointer;
    using atlas::tools::ModellingScene;

    atlas::gl::setGLErrorSeverity(ATLAS_GL_ERROR_SEVERITY_HIGH | 
        ATLAS_GL_ERROR_SEVERITY_MEDIUM);

    WindowSettings settings;
    settings.contextVersion = ContextVersion(4, 1);
    settings.isForwardCompat = true;
    settings.isMaximized = true;

    Application::getInstance().createWindow(settings);

    // The tools submodule provides some pre-built classes that you can use
    // in your application to save time. An example of this is the Modelling 
    // Scene, which provides you with a 3D camera, a reference grid, and a simple
    // GUI.
    Application::getInstance().addScene(ScenePointer(new ModelViewer));

    Application::getInstance().runApplication();

    return 0;
}
