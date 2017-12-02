#pragma once

#include "navigator/nodenavigator.h"

/*
 *  PositionNavigator: use cursor keys to translate a node
 *  along the X, Y, Z axes, from the camera's perspective.
 *
 */
class PositionNavigator : public NodeNavigator {
public:

    /* constructor requires all three:
     *  - node to be manipulated,
     *  - root node of the scene graph
     *  - node that the camera is attached to
     */
    PositionNavigator(std::shared_ptr<Node> node,
                      std::shared_ptr<Node> world,
                      std::shared_ptr<Node> camera);

    // use (shift) cursor keys to move things around
    void keyPressEvent(QKeyEvent *event) override;

    // how much translation for one key press?
    float speed = 0.05f;

};

