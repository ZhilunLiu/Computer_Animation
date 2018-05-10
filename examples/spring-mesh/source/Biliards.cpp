#include "Biliards.hpp"
#include "Paths.hpp"
#include "atlas/gl/Shader.hpp"

namespace math = atlas::math;
namespace utils = atlas::utils;
namespace gl = atlas::gl;


Biliards::Biliards() :
    mVertexBuffer(GL_ARRAY_BUFFER),
//    mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER),
//    mRenderMode(1),
    mNumIndices(7),
    g(1.0),

    mMeshes(mNumIndices)
{
    mModel = math::Matrix4(1.0f);

    mMeshes[0].pos = { 0.0 , 0.0, 0.0}; // initial position
    mMeshes[0].vt = { 0.0f, 5.0f, 0.0f }; // initial velocity

    mMeshes[1].pos = {0.0,5.0, 0.0};
    mMeshes[2].pos = {-0.3,6.0,0.0};
    mMeshes[3].pos = {0.3,6.0,0.0};
    mMeshes[4].pos = {-0.75,7.0,0.0};
    mMeshes[5].pos = {0.0,7.0,0.0};
    mMeshes[6].pos = {0.75,7.0,0.0};
    //mMeshes[1].vt = {0.0f,0.0f,0.0f};



    mVao.bindVertexArray();
    mVertexBuffer.bindBuffer();
    mVertexBuffer.bufferData(sizeof(struct mass) * mMeshes.size(),
        mMeshes.data(), GL_DYNAMIC_DRAW);
    // try using CLOTHS_LAYOUT_LOCATION
    mVertexBuffer.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct mass),
        gl::bufferOffset<float>(0));
    mVao.enableVertexAttribArray(0);
    mVertexBuffer.unBindBuffer();

    mVao.unBindVertexArray();

    // Load the shaders here.
    std::vector<gl::ShaderUnit> shaders
    {
        {std::string(ShaderDirectory) + "Model.vs.glsl", GL_VERTEX_SHADER},
        {std::string(ShaderDirectory) + "Model.fs.glsl", GL_FRAGMENT_SHADER}
    };

    mShaders.push_back(gl::Shader(shaders));
    mShaders[0].setShaderIncludeDir(ShaderDirectory);
    mShaders[0].compileShaders();
    mShaders[0].linkShaders();
    mShaders[0].disableShaders();

    auto var = mShaders[0].getUniformVariable("projection");
    mUniforms.insert(UniformKey("projection", var));
    var = mShaders[0].getUniformVariable("view");
    mUniforms.insert(UniformKey("view", var));
    var = mShaders[0].getUniformVariable("model");
    mUniforms.insert(UniformKey("model", var));
//    var = mShaders[0].getUniformVariable("colour");
//    mUniforms.insert(UniformKey("colour", var));
    var = mShaders[0].getUniformVariable("renderMode");
    mUniforms.insert(UniformKey("renderMode", var));
}

Biliards::~Biliards()
{
    // do destruction here
}

void Biliards::renderGeometry(math::Matrix4 const& projection,
    math::Matrix4 const& view)
{
    if (!mShaders[0].shaderProgramValid())
    {
        return;
    }
    mShaders[0].enableShaders();

    glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE,
        &projection[0][0]);
    glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);
//    glUniform1i(mUniforms["renderMode"], mRenderMode);

    glUniform4f(mUniforms["colour"], 0, 0, 0, 1);
    int n = (int)(mNumIndices / 2) - 2;
    glDrawArrays(GL_LINES, n, 4);

    mVao.bindVertexArray();



    glPointSize(10.0f);
//    float rgb = (64.0f / 255.0f);
//    glUniform4f(mUniforms["colour"], rgb, rgb, rgb, 1);
    glDrawArrays(GL_POINTS, 0, (int)mNumIndices);

//    glUniform1i(mUniforms["renderMode"], 1);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    // avoid hard-code indices
//    glDrawElements(GL_TRIANGLES, 15,
//        GL_UNSIGNED_INT, gl::bufferOffset<float>(0));
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//    mIndexBuffer.unBindBuffer();
    mVao.unBindVertexArray();
    mShaders[0].disableShaders();
}

void Biliards::updateGeometry(atlas::core::Time<> const& t)
{
    std::vector<mass> meshes = mMeshes;
     for(int i =0; i<7; i++){
        // Compute the damping force fd

        fd = math::Vector(-0.0001*mMeshes[i].vt[0],-0.0001*mMeshes[i].vt[1],-0.0001*mMeshes[i].vt[2]);

        //detection hitting wall
        if(abs(meshes[i].pos[0]-10)<=0.05){
            std::printf("--------hitting right wall--------\n");
            meshes[i].vt = math::Vector(-0.8*meshes[i].vt[0],-0.8*meshes[i].vt[1],-0.8*meshes[i].vt[2]);
        }
        if(abs(meshes[i].pos[0]+10)<=0.05){
            std::printf("--------hitting left wall--------\n");
            meshes[i].vt = math::Vector(-0.8*meshes[i].vt[0],-0.8*meshes[i].vt[1],-0.8*meshes[i].vt[2]);
        }
        if(abs(meshes[i].pos[1]-10)<=0.05){
            std::printf("--------hitting top wall--------\n");
            meshes[i].vt = math::Vector(-0.8*meshes[i].vt[0],-0.8*meshes[i].vt[1],-0.8*meshes[i].vt[2]);
        }
        if(abs(meshes[i].pos[1]+10)<=0.05){
            std::printf("--------hitting down wall--------\n");
            meshes[i].vt = math::Vector(-0.8*meshes[i].vt[0],-0.8*meshes[i].vt[1],-0.8*meshes[i].vt[2]);
        }


        //collision detection

            for(int j=0; j<7;j++){
                if(abs(meshes[i].pos[0]-meshes[j].pos[0])<=0.5 && abs(meshes[i].pos[1]-meshes[j].pos[1])<=0.5){

                    if(i==j){
                       // std::printf("--------iiiiii=====jjjjjj--------\n");
                        continue;

                    }
                    std::printf("-----------------collide---------------------\n%f,%f",meshes[i].vt[1],meshes[j].vt[1],"\n");
                //collide,then compute the Force
                //1.Find Normal Vector:
                    math::Vector n = math::Vector(meshes[j].pos[0]-meshes[i].pos[0],meshes[j].pos[1]-meshes[i].pos[1],0.0);
                //2.Find unit Normal vector
                    math::Vector un = n/sqrt((n[0]*n[0])+(n[1]*n[1]));
                //3.Find the unit Tangent vector
                    math::Vector ut = math::Vector(-un[1],un[0],0.0);
               //4.Calculate the old scalar velocity of i,j in normal and tangent direction by taking dot product
                    v1n = un[0]*meshes[i].vt[0]+un[1]*meshes[i].vt[1];
                    v1t = ut[0]*meshes[i].vt[0]+ut[1]*meshes[i].vt[1];
                    v2n = un[0]*meshes[j].vt[0]+un[1]*meshes[j].vt[1];
                    v2t = ut[0]*meshes[j].vt[0]+ut[1]*meshes[j].vt[1];
               //5.Find the new normal velocity
                    nv1n = v2n;
                    nv2n = v1n;
              //6.convert the new velocity to vector form
                    math::Vector newv1n = nv1n*un;
                    math::Vector newv2n = nv2n*un;
                    math::Vector newv1t = v1t*ut;
                    math::Vector newv2t = v2t*ut;
              //Combine 2 velcocity
                    meshes[i].vt = newv1n+newv1t;
                    meshes[j].vt = newv2n+newv2t;

                    //compute the collision force
                    //meshes[i].fc = meshes[j].vt+math::Vector(-1.5*meshes[i].vt[0],-1.5*meshes[i].vt[1],-1.5*meshes[i].vt[2]);
                    //meshes[i].vt = meshes[j].vt+math::Vector(-1.5*meshes[i].vt[0],-1.5*meshes[i].vt[1],-1.5*meshes[i].vt[2]);
                    //meshes[i].vt = math::Vector(0.0,0.0,0.0);

                    meshes[i].f = meshes[i].f+meshes[i].fc;

                }
            }
        meshes[i].f = meshes[i].f+fd;
        math::Vector a = meshes[i].f;
        math::Vector v = meshes[i].vt * t.deltaTime + math::Vector(0.5*meshes[i].f[0],0.5*meshes[i].f[1],0.5*meshes[i].f[2]) * a * t.deltaTime * t.deltaTime;

        // Update each mass velocity
        meshes[i].vt = meshes[i].vt + a * t.deltaTime;

        // Update each mass position
        meshes[i].pos = meshes[i].pos + v;


        std::printf("a: [%f,%f,%f], ", a[0],a[1],a[2]);
        std::printf("\n");
        std::printf("fd: [%f, %f, %f], \n", fd[0], fd[1], fd[2]);
        std::printf("meshes[%d].fc: [%f, %f, %f], \n",i, meshes[i].fc[0], meshes[i].fc[1], meshes[i].fc[2]);
        std::printf("meshes[%d].f: [%f, %f, %f], \n", i,meshes[i].f[0], meshes[i].f[1], meshes[i].f[2]);
        std::printf("meshes[%d].pos: [%f, %f, %f], ", i, meshes[i].pos[0], meshes[i].pos[1], meshes[i].pos[2]);
        std::printf("\n");
        std::printf("meshes[%d].vt: [%f, %f, %f]", i, meshes[i].vt[0], meshes[i].vt[1], meshes[i].vt[2]);
        std::printf("\n");
//        std::printf("\n");
    }

    mMeshes = meshes;

    // update position
    mVao.bindVertexArray();
    mVertexBuffer.bindBuffer();
    mVertexBuffer.bufferData(sizeof(struct mass) * mMeshes.size(),
        mMeshes.data(), GL_DYNAMIC_DRAW);
    mVertexBuffer.unBindBuffer();
    mVao.unBindVertexArray();
}
