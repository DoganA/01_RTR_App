
#include "mesh.h"
#include "objloader.h"

#include <iostream>
#include <assert.h>

using namespace std;


// just a convenience constructor
Mesh::Mesh(const string& filename,
           shared_ptr<Material> material)
    : Mesh(make_shared<GeometryOBJ>(filename), material)
{
}

// construct from existing GeometryBuffers
Mesh::Mesh(shared_ptr<GeometryBuffers> geometry,
           shared_ptr<Material> material)

    : geometry_(geometry), material_(material)

{
    if(!geometry_)
        qFatal("Cannot construct Mesh without geometry");

    if(!material)
        qFatal("Cannot construct Mesh with material");

    if (!vao_.create())
        qFatal("Mesh: unable to create VAO");

    geometry_->bind(vao_, material_->program());

}

void Mesh::draw(unsigned int light_pass)
{

    // qDebug() << "drawing mesh, bbox max extent = " << geometry_->bbox().maxExtent();

    // set the right shader, set all uniforms to their correct values
    material_->apply(light_pass);

    // bind VAO with all required buffer states, then draw
    vao_.bind();
    glDrawElements(GL_TRIANGLES, GLsizei(geometry_->numIndices()), GL_UNSIGNED_INT, Q_NULLPTR);
    vao_.release();
}

void Mesh::replaceMaterial(std::shared_ptr<Material> material)
{
    if(!material)
        qFatal("Mesh: cannot replace material with no material");

    // discard old VAO, create new one
    vao_.destroy();
    if (!vao_.create())
        qFatal("Mesh: unable to create VAO");

    // set new material and bind it in the new VAO
    material_ = material;
    geometry_->bind(vao_, material_->program());

}

