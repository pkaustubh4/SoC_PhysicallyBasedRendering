#pragma once

#include <GL/glew.h>
#include "va.h"
#include "shader.h"
#include "vb.h"
#include "ib.h"

class Renderer
{
public:

    void EnableBlending() const;
    void DisableBlending() const;
    void EnableDepthTesting() const;
    void DisableDepthTesting() const;
    void EnableFaceCulling() const;
    void DisableFaceCulling() const;
    void SetPolygonMode(bool fill) const;
    void SetLineWidth(float width) const;
    void SetPointSize(float size) const;

    // New function to load textures
    unsigned int LoadTexture(const std::string& filepath) const;
};
