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

    struct PointLight
    {
        QVector4D position_WC = QVector4D(0,1,5,1);
        QVector3D color = QVector3D(0,0,0);
        float intensity = 0.5;
    };

    float time = 0.0;

    std::vector<PointLight> lights;
    /*
     *  apply: bind underlying shader program and set required uniforms
     *  This method needs to be overwritten by the derived class.
     *
     *  light_pass is an optional argument so the shader may be called
     *  once for each light (or type of light).
     *
     */
    virtual void apply(unsigned int light_pass = 0) = 0;
    virtual QString getAppliedShader() = 0;

    /*
     * returns the underlying OpenGL shader program object
     *
     */
    QOpenGLShaderProgram& program() const { return *prog_; }

protected:

    // reference to underlying shader program
    std::shared_ptr<QOpenGLShaderProgram> prog_;
};


