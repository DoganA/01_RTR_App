#include "navigator/position_navigator.h"

#include <QtMath>
#include <iostream>

using namespace std;

PositionNavigator::PositionNavigator(std::shared_ptr<Node> node,
                                     std::shared_ptr<Node> world,
                                     std::shared_ptr<Node> camera)
    : NodeNavigator(node,world,camera)
{}

void PositionNavigator::keyPressEvent(QKeyEvent *event)
{
    // translation vector in eye coords
    QVector4D translation_ec(0,0,0,0);

    // depending on key press, change position
    bool isShiftPressed = event->modifiers() & Qt::ShiftModifier;
    switch(event->key()) {
        case Qt::Key_Up:
            if(isShiftPressed)
                translation_ec[2] = -speed;  // shift: Z
            else
                translation_ec[1] = speed; // Y
            break;
        case Qt::Key_Down:
            if(isShiftPressed)
                translation_ec[2] = speed; // shift: Z
            else
                translation_ec[1] = -speed; // Y
            break;
        case Qt::Key_Left:
            translation_ec[0] = -speed; // X
            break;
        case Qt::Key_Right:
            translation_ec[0] = speed; // X
            break;
        default:
            return;
    }

    // translate from camera coords into model coords
    QMatrix4x4 camToWorld = world_->toParentTransform(camera_);
    QMatrix4x4 worldToModel = world_->toParentTransform(node_).inverted();
    QVector4D translation_mc = worldToModel * camToWorld * translation_ec;
    node_->transformation.translate(translation_mc.toVector3D());

    // debugging for positioning
    QMatrix4x4 nodeToWorld = world_->toParentTransform(node_);
    QVector4D pos = nodeToWorld * QVector4D(0,0,0,1);
    qDebug() << pos;

}

