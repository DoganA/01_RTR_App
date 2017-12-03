#pragma once
//-----------------------------------------------------------------------------
// Basic Librarys
#include <memory> // std::unique_ptr
#include <map>    // std::map
#include <chrono> // clock, time calculations
//-----------------------------------------------------------------------------
// QT Library
#include <QWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#include <QOpenGLFramebufferObject>
//-----------------------------------------------------------------------------
// QT Library
#include "node.h"
#include "camera.h"
#include "material/phong.h"
#include "material/toon.h"
#include "material/point.h"
#include "navigator/rotate_y.h"



/*
 * OpenGL-based scene. Required objects are created in the constructor,
 * and the scene is rendered using render().
 *
 * Also, the scene derives from QObject so it can use Qt's
 * signal and slot mechanism to connect to the app's UI.
 *
 * Do not call render() directly, use update() instead.
 *
 */

class Scene : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Scene(QWidget* parent, QOpenGLContext *context);

    QMatrix4x4& worldTransform() { return nodes_["World"]->transformation; }

signals:

    void displayBufferContents(unsigned int id, QString label, const QImage& img);

public slots:

    // change the node to be rendered in the scene
    void setSceneNode(QString node);

// own created functions <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    QString getCurrentSceneNode();

    void setShader(QString shader);
    void enableSilhoutte(bool enable);
    void setThreshold(float threshold);
    void setAmountOfDiscretiz(int amount);

    void setRedIntensity(float redIntensitiy);
    void setGreenIntensity(float greenIntensitiy);
    void setBlueIntensity(float blueIntensitiy);
    void setRadius(float radius);
    void setDensity(float density);
    void revertPoint(bool revert);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // change background color
    void setBackgroundColor(QVector3D rgb);

    // methods to change common material parameters
    void toggleAnimation(bool flag);
    void setLightIntensity(size_t i, float v);

    // key/mouse events from UI system, pass on to navigators or such
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    /*
     *  perform OpenGL rendering of the entire scene.
     *  Don't call this yourself (!) - use update() instead.
     *
     */
    void draw();

    // anyone can trigger a redraw of the widget through this method
    void update();

    // adjust camera / viewport / ... if drawing surface changes
    void updateViewport(size_t width, size_t height);
    void setRotateAxis(RotateY::Axis axis);


protected:

    // draw the actual scene
    void draw_scene_();

    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    void replaceMaterialAndDrawScene(const Camera &camera, std::shared_ptr<Material> mat);
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // parent widget
    QWidget* parent_;

    // periodically update the scene for animations
    QTimer timer_;

    // track time for animations
    std::chrono::high_resolution_clock clock_;
    std::chrono::time_point<std::chrono::high_resolution_clock> firstDrawTime_;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastDrawTime_;

    // rotation
    double angle = 0.0;
    bool rotationOn = true;

    std::shared_ptr<Material> material_;
    // bg color
    QVector3D bgcolor_ = QVector3D(0.4f,0.4f,0.4f);

    // different materials to be demonstrated
    std::map<QString, std::shared_ptr<PhongMaterial>> phongMaterials_;
    std::map<QString, std::shared_ptr<PhongMaterial>> mapOfPhongMaterials_;
    std::map<QString, std::shared_ptr<ToonMaterial>> mapOfToonMaterials_;
    std::map<QString, std::shared_ptr<PointMaterial>> mapOfPointMaterials_;

    std::vector<std::shared_ptr<Material>> allMaterials_;

    // mesh(es) to be used / shared
    std::map<QString, std::shared_ptr<Mesh>> meshes_;

    // nodes to be used
    std::map<QString, std::shared_ptr<Node>> nodes_;

    // light nodes for any number of lights
    std::vector<std::shared_ptr<Node>> lightNodes_;

    // navigation
    std::unique_ptr<RotateY>     cameraNavigator_;

    // helper for creating programs from shader files
    std::shared_ptr<QOpenGLShaderProgram> createProgram(const std::string& vertex,
                                                        const std::string& fragment,
                                                        const std::string& geom = "");

    // helper for creating a node scaled to size 1
    std::shared_ptr<Node> createNode(std::shared_ptr<Mesh> mesh, bool scale_to_1 = true);

    // helpers to construct the objects and to build the hierarchical scene
    void makeNodes();
    void makeScene();

    QString currentSceneNode;//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
};

