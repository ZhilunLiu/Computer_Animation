#ifndef OBJECT_LOADER_INCLUDE_MODEL_HPP
#define OBJECT_LOADER_INCLUDE_MODEL_HPP

#pragma once

#include <atlas/utils/Geometry.hpp>
#include <atlas/utils/Mesh.hpp>
#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>

class Model : public atlas::utils::Geometry
{
public:
    Model(std::string const& filename, std::string const& mtl = "", 
        bool triangulate = true);

    // Override this to render!
    void renderGeometry() override;
    
    // Override this to draw a GUI.
    void drawGui() override;

private:
    atlas::utils::Mesh mMesh;
    atlas::gl::Buffer mVertexBuffer;
    atlas::gl::Buffer mNormalBuffer;
    atlas::gl::Buffer mTextureBuffer;
    atlas::gl::Buffer mIndexBuffer;
    atlas::gl::VertexArrayObject mVao;
    std::size_t mStride;
    int mRenderMode;
};

#endif