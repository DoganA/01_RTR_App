#pragma once

#include "material/material.h"
#include "material/toon.h"

#include <QOpenGLTexture>

class PointMaterial :public ToonMaterial {
public:


    // constructor requires existing shader program
    PointMaterial(std::shared_ptr<QOpenGLShaderProgram> prog) : ToonMaterial(prog)
    {

    }

    // bind underlying shader program and set required uniforms
    void apply(unsigned int light_pass = 0) override;
    QString getAppliedShader() override;


    struct Texture {
       float density= 0.1;
       float radius=0.5;
       QVector3D backgroundColor=QVector3D(0.40f,0.50f,0.21f);
       QVector3D circleColor=QVector3D(0.10f,0.80f,0.80f);
       bool shouldDiscard = false;
    }texture;

};
