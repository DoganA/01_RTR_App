#pragma once

#include <QOpenGLShaderProgram>

#include <memory>

/*
 *   Interface for surface materials.
 *
 */
class Material
{
public:

    /*
     * constructor requires an existing shader program
     *
     */
    Material(std::shared_ptr<QOpenGLShaderProgram> prog)
        :prog_(prog)
    {}

    /*
     *  apply: bind underlying shader program and set required uniforms
     *  This method needs to be overwritten by the derived class.
     *
     *  light_pass is an optional argument so the shader may be called
     *  once for each light (or type of light).
     *
     */
    virtual void apply(unsigned int light_pass = 0) = 0;

    /*
     * returns the underlying OpenGL shader program object
     *
     */
    QOpenGLShaderProgram& program() const { return *prog_; }

protected:

    // reference to underlying shader program
    std::shared_ptr<QOpenGLShaderProgram> prog_;
};


