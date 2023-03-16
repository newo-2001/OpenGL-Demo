#pragma once

#include "ShadowMap.h"

class OmniDirectionalShadowMap : public ShadowMap
{
public:
    OmniDirectionalShadowMap(glm::ivec2 shadowDimensions);
    ~OmniDirectionalShadowMap();

    void Write();
    void Read(GLenum textureUnit);
};

