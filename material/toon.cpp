#include "material/toon.h"
#include <assert.h>

void ToonMaterial::apply(unsigned int light_pass)
{
    // qDebug() << "applying TexturedPhongMaterial";

   PhongMaterial::apply(light_pass);


   prog_->setUniformValue("toonShader.threshold", toonShader.threshold);
   prog_->setUniformValue("toonShader.toon", toonShader.toon);
   prog_->setUniformValue("toonShader.silhoutte", toonShader.silhoutte);
   prog_->setUniformValue("toonShader.discretize", toonShader.discretize);
   //prog_->setAttributeValue("texCoord",QVector2D(0.5,0.5) );
//   prog_->setUniformValue("texture.density", texture.density);
//   prog_->setUniformValue("texture.radius", texture.radius);
//   prog_->setUniformValue("texture.backgroundColor", texture.backgroundColor);
//   prog_->setUniformValue("texture.circleColor", texture.circleColor);

}



QString
ToonMaterial::getAppliedShader(){
    return "toon";
}
