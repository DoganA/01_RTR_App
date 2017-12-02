#pragma once

#include "navigator/nodenavigator.h"

/*
 *  Simple navigator class to rotate a node around its Y axis using cursor keys
 */

class RotateY : public NodeNavigator {
public:

    // this navigator only needs the actual node, and is independent of world and camera
    RotateY(std::shared_ptr<Node> node,
            std::shared_ptr<Node> ,
            std::shared_ptr<Node> );

    // use cursor keys to rotate camera left/right around Y
    void keyPressEvent(QKeyEvent *event) override;

    // elevation angle above X-Z plane (rotation around X axis)
    RotateY& setElevationAngle(float degrees);

    // rotation speed in degrees
    RotateY& setRotationSpeed(float degrees);

    // zoom speed in scene coordinate units
    RotateY& setZoomSpeed(float length);

    // what shall be the distance of the camera to the rotation center?
    RotateY &setDistance(float d);

    // add/subtract to/form current rotation angle
    RotateY& rotate(float degrees);

protected:

    float distance_to_center_ = 1.0;
    float rotation_angle_ = 0;
    float elevation_angle_ = 15;
    float rotation_speed_ = 5.0;
    float zoom_speed_ = 0.25;

    void updateTransformation_();

};

