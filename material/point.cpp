#include "point.h"

void PointMaterial::apply(unsigned int light_pass)
{
   ToonMaterial::apply(light_pass);

   prog_->setUniformValue("texture.density", texture.density);
   prog_->setUniformValue("texture.radius", texture.radius);
   prog_->setUniformValue("texture.circleColor", texture.circleColor);
   prog_->setUniformValue("texture.backgroundColor", texture.backgroundColor);
   prog_->setUniformValue("texture.shouldDiscard", texture.shouldDiscard);

}

QString
PointMaterial::getAppliedShader(){
    return "point";
}

