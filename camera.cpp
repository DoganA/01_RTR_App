#include "camera.h"
#include <assert.h>

using namespace std;

Camera::Camera(QMatrix4x4 view,
               QMatrix4x4 projection)
    : viewMatrix_(view), projectionMatrix_(projection)
{
    // set uniform names to their default values (IMPORTANT!)
    setMatrixUniformNames();
}

void Camera::setShaderTransformationMatrices(Material &material,
                                             QMatrix4x4 modelMatrix) const
{

    auto& prog = material.program();

    QMatrix4x4 mv  = viewMatrix_ * modelMatrix;
    QMatrix4x4 mvp = projectionMatrix_ * mv;

    prog.bind();
    prog.setUniformValue(name_m_.c_str(),   modelMatrix);
    prog.setUniformValue(name_v_.c_str(),   viewMatrix_);
    prog.setUniformValue(name_iv_.c_str(),  viewMatrix_.inverted());
    prog.setUniformValue(name_p_.c_str(),   projectionMatrix_);
    prog.setUniformValue(name_mv_.c_str(),  mv);
    prog.setUniformValue(name_n_.c_str(),   mv.normalMatrix());
    prog.setUniformValue(name_mvp_.c_str(), mvp);

#if 0
    qDebug() << "modelview: ";
    qDebug() << mv;
    qDebug() << "proj: ";
    qDebug() << projectionMatrix_;
    qDebug() << "mvp: ";
    qDebug() << mvp;
#endif

}

void Camera::setMatrixUniformNames(const string m,
                                   const string v,
                                   const string iv,
                                   const string p,
                                   const string mv,
                                   const string n,
                                   const string mvp)
{
    name_m_   = m;
    name_v_   = v;
    name_iv_  = iv;
    name_p_   = p;
    name_mv_  = mv;
    name_n_   = n;
    name_mvp_ = mvp;
}
