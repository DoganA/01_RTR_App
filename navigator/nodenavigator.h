#pragma once

#include "node.h"
#include "camera.h"
#include <QMatrix4x4>
#include <assert.h>

#include <QKeyEvent>
#include <QMouseEvent>

/*
 * NodeNavigator: interface class to process key/mouse events and
 * manipulate the transformation of a node.
 *
 * In addition to the node being manipulated, the root node of the
 * scene graph (world) and the current camera node can be provided,
 * in order to be able to map transformations from camera coordinates
 * to the node's model coordinates.
 *
 */

class NodeNavigator
{
public:

    /* constructor must be called by derived classes to store the relevant node references */
    NodeNavigator(std::shared_ptr<Node> node,
                  std::shared_ptr<Node> world,
                  std::shared_ptr<Node> camera);

    /* methods to react on mouse/keyboard events */
    virtual void keyPressEvent(QKeyEvent *) {}
    virtual void mousePressEvent(QMouseEvent *) {}
    virtual void mouseMoveEvent(QMouseEvent *) {}
    virtual void mouseReleaseEvent(QMouseEvent *) {}
    virtual void wheelEvent(QWheelEvent *) {}

protected:

    std::shared_ptr<Node> node_;
    std::shared_ptr<Node> world_;
    std::shared_ptr<Node> camera_;

};
