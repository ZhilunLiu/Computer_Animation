#ifndef ATLAS_INCLUDE_ATLAS_TOOLS_BILIARDS_HPP
#define ATLAS_INCLUDE_ATLAS_TOOLS_BILIARDS_HPP

#pragma once


#define BILIARDS_DEFAULT_KIND 0

#include "atlas/tools/Tools.hpp"
#include "atlas/utils/Geometry.hpp"
#include "atlas/gl/VertexArrayObject.hpp"
#include "atlas/gl/Buffer.hpp"

namespace math = atlas::math;
namespace utils = atlas::utils;

class Biliards final : public utils::Geometry
{
public:
    Biliards();
    ~Biliards();

    void renderGeometry(atlas::math::Matrix4 const& projection,
        atlas::math::Matrix4 const& view) override;

    // Override this to animate things.
    void updateGeometry(atlas::core::Time<> const& t) override;

private:

    struct mass {
        math::Vector pos;
        math::Vector vt;
        math::Vector f;
        math::Vector fc;
        float weight;
    };

private:
    atlas::gl::VertexArrayObject mVao;
    atlas::gl::Buffer mVertexBuffer;
//    atlas::gl::Buffer mIndexBuffer;
    std::size_t mNumIndices;
//    int mRenderMode;
    std::vector<mass> mMeshes;

    float g;//weight
    // Define the damping force
    math::Vector fd;


    float v1n,v1t,v2n,v2t,nv1n,nv2n;

};

#endif
