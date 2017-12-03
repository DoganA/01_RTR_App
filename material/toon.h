#pragma once

#include "material/material.h"
#include "material/phong.h"

#include <QOpenGLTexture>


class ToonMaterial : public PhongMaterial {
public:

    // constructor requires existing shader program
    ToonMaterial(std::shared_ptr<QOpenGLShaderProgram> prog) : PhongMaterial(prog)
    {

    }

    // bind underlying shader program and set required uniforms
    void apply(unsigned int light_pass = 0) override;
    QString getAppliedShader();// override;


    struct ToonShader {
       bool toon = false;
       bool silhoutte = false;
       float threshold = 0.3f;
       int discretize = 0;
    } toonShader;

    struct Texture {
       int density = 5;
       float radius = 0.3f;
       QVector3D circleColor = QVector3D(0.6f, 0.2f, 0.8f);
       QVector3D backgroundColor = QVector3D(0.3f, 0.4f, 0.6f);
    } texture;

};

