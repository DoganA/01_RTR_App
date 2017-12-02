#include "navigator/rotate_y.h"

#include <QtMath>
#include <iostream>

using namespace std;

RotateY::RotateY(std::shared_ptr<Node> node,
                 std::shared_ptr<Node>,
                 std::shared_ptr<Node>)
    : NodeNavigator(node,nullptr,nullptr)
{
    updateTransformation_();
}

void RotateY::keyPressEvent(QKeyEvent *event)
{
    // depending on key press, change position
    // bool isShiftPressed = event->modifiers() & Qt::ShiftModifier;
    switch(event->key()) {
        case Qt::Key_Left:
            rotate(-rotation_speed_);
            break;
        case Qt::Key_Right:
            rotate(+rotation_speed_);
            break;
        case Qt::Key_Up:
            setDistance(distance_to_center_-zoom_speed_);
            break;
        case Qt::Key_Down:
            setDistance(distance_to_center_+zoom_speed_);
            break;
        default:
            return;
    }

}

RotateY &RotateY::setElevationAngle(float degrees)
{
    elevation_angle_ = degrees;
    updateTransformation_();
    return *this;
}

RotateY &RotateY::setRotationSpeed(float degrees)
{
    rotation_speed_ = degrees;
    return *this;
}

RotateY &RotateY::setZoomSpeed(float length)
{
    zoom_speed_ = length;
    return *this;

}

RotateY& RotateY::setDistance(float d)
{
    distance_to_center_ = std::max(d, zoom_speed_);
    updateTransformation_();
    return *this;
}

RotateY& RotateY::rotate(float degrees)
{
    rotation_angle_ = rotation_angle_ + degrees;
    updateTransformation_();
    return *this;
}

// calculate camera's tranformation matrix from distance and rotation angle
void RotateY::updateTransformation_()
{
    QMatrix4x4 mat;

    // third, rotate around Y axis
    mat.rotate(rotation_angle_, QVector3D(0,1,0));

    // second, elevate node above X-Z axis by rotating around -X
    mat.rotate(elevation_angle_, QVector3D(-1,0,0));

    // first, translate along camera axis
    mat.translate(0, 0, distance_to_center_);

    // update matrix in camera node
    node_->transformation = mat;
}

