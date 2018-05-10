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

    void renderGeometry(atlas::math::Matrix4 const& projection,
                        atlas::math::Matrix4 const& view) override;

    // Override this to animate things.
    void updateGeometry(atlas::core::Time<> const& t) override;

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

    // Define a spring with:
    double slt; // spring length
    double sks; // stiffness
    double kd; // damping coefficient
    double fs; // spring force
    atlas::math::Vector so; // spring origin
    atlas::math::Vector sru; // spring rest unit vector
    atlas::math::Vector sou; // spring origin unit vector
    double torque;
    double anglet; // current angle
    double anglevt; // angular velocity

    // Define a mass with:
    atlas::math::Vector mpt; // mass position
    atlas::math::Vector mvt; // mass velocity

    // Define the damping force
    double fd;
};

#endif
