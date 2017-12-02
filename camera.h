#pragma once

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

#include "material/material.h"

#include <string> // std::string

/*
 * Camera class that stores a view and projection matrix and
 * calculates and sets all the desired transformation
 * matrices in the shader material (M/V/P matrices).
 *
 */
class Camera
{
public:

    // constructor with optional view and projection matrices
    Camera(QMatrix4x4 view = QMatrix4x4(), QMatrix4x4 projection = QMatrix4x4());

    virtual QMatrix4x4 viewMatrix() const { return viewMatrix_; }
    virtual QMatrix4x4 projectionMatrix() const { return projectionMatrix_; }

    virtual void setViewMatrix(QMatrix4x4 mat) { viewMatrix_ = mat; }
    virtual void setProjectionMatrix(QMatrix4x4 mat) { projectionMatrix_ = mat; }

    /*
     *  Set OpenGL shader uniform variables for M / V / P matrices etc.
     *  Always apply before using the actual material for rendering!
     *
     *  Uniforms to be set: see setMatrixUniformNames()
     *
     */
    void setShaderTransformationMatrices(Material& material,
                                         QMatrix4x4 modelMatrix) const;

    /*
     *  Set the uniform names to which the camera matrices will be bound
     *  in future calls of setMatrices() for this camera object.
     */
    void setMatrixUniformNames(const std::string m    = "modelMatrix",
                               const std::string v    = "viewMatrix",
                               const std::string iv   = "inverseViewMatrix",
                               const std::string p    = "projectionMatrix",
                               const std::string mv   = "modelViewMatrix",
                               const std::string n    = "normalMatrix",
                               const std::string mvp  = "modelViewProjectionMatrix");

protected:

    QMatrix4x4 viewMatrix_, projectionMatrix_;

    // uniform names for the calculated matrices
    std::string name_m_, name_v_, name_iv_, name_p_, name_mv_, name_n_, name_mvp_;

};

