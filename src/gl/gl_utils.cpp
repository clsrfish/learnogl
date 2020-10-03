#include "./gl_utils.h"
#include "../utils/log.h"
#include <glad/glad.h>

void printGLInfo()
{
    LOG_I("GLVendor:   %s", glGetString(GL_VENDOR));
    LOG_I("GLVersion:  %s", glGetString(GL_VERSION));
    LOG_I("GLRenderer: %s", glGetString(GL_RENDERER));
}