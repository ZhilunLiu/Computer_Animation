#include <atlas/utils/Application.hpp>
#include <atlas/utils/WindowSettings.hpp>
#include <atlas/gl/ErrorCheck.hpp>
#include <atlas/utils/Scene.hpp>

int main()
{
    using atlas::utils::WindowSettings;
    using atlas::utils::ContextVersion;
    using atlas::utils::Application;
    using atlas::utils::ScenePointer;
    using atlas::utils::Scene;

    // Atlas provides an error callback function that captures OpenGL errors and
    // formats them in a more human-readable fashion. This callback is enabled
    // if the system supports it. 
    // We can fine-tune which notifications we recieve from the callback (by
    // default it will send everything) by doing the following:
    atlas::gl::setGLErrorSeverity(ATLAS_GL_ERROR_SEVERITY_HIGH | 
        ATLAS_GL_ERROR_SEVERITY_MEDIUM);

    // In order to create a window, we need to set its parameters. These are
    // contained in the WindowSettings struct. so let's create one.
    WindowSettings settings;
    
    // Now we must choose the OpenGL context version. By default it is set to
    // 3.3 but you can change it for higher versions if your system supports it.
    settings.contextVersion = ContextVersion(4, 5);

    // The next two attributes are completely optional. The first one disables
    // the fixed-function pipeline (anything prior to OpenGL 3.3) while the 
    // second one tells the window to be maximized upon creation.
    settings.isForwardCompat = true;
    settings.isMaximized = true;

    // Now we tell the Application class to create a new window with the 
    // provided settings.
    Application::getInstance().createWindow(settings);

    // Every applicaton needs at least one scene. For the time being we will
    // just add an empty (default) scene.
    Application::getInstance().addScene(ScenePointer(new Scene));

    // Finally, we run our application.
    Application::getInstance().runApplication();

    return 0;
}