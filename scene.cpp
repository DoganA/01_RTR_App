#include "scene.h"

#include <iostream> // std::cout etc.
#include <assert.h> // assert()
#include <random>   // random number generation

#include "geometry/cube.h" // geom::Cube

#include <QtMath>
#include <QMessageBox>

using namespace std;

Scene::Scene(QWidget* parent, QOpenGLContext *context) :
    QOpenGLFunctions(context),
    parent_(parent),
    timer_(),
    firstDrawTime_(clock_.now()),
    lastDrawTime_(clock_.now())
{

    // check some OpenGL parameters
    {
        int minor, major;
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        cout << "OpenGL context version " << major << "." << minor << endl;

        int texunits_frag, texunits_vert;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texunits_frag);
        glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &texunits_vert);
        cout << "texture units: " << texunits_frag << " (frag), "
             << texunits_vert << " (vert)" << endl;

        int texsize;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texsize);
        cout << "max texture size: " << texsize << "x" << texsize << endl;
    }

    // construct map of nodes
    makeNodes();

    // from the nodes, construct a hierarchical scene (adding more nodes)
    makeScene();

    // initialize navigation controllers
    cameraNavigator_ = std::make_unique<RotateY>(nodes_["Camera"], nullptr, nullptr);
    cameraNavigator_->setDistance(3.0);

    // make sure we redraw when the timer hits
    connect(&timer_, SIGNAL(timeout()), this, SLOT(update()) );

}

void Scene::makeNodes()
{
    // load shader source files and compile them into OpenGL program objects
    auto phong_prog = createProgram(":/shaders/phong.vert", ":/shaders/phong.frag");

    // Phong materials
    auto red = std::make_shared<PhongMaterial>(phong_prog);
    phongMaterials_["red"] = red;
    red->phong.k_diffuse = QVector3D(0.8f,0.1f,0.1f);
    red->phong.k_ambient = red->phong.k_diffuse * 0.3f;
    red->phong.shininess = 80;

    auto goblin_Material = std::make_shared<PhongMaterial>(phong_prog);
    phongMaterials_["goblin_Material"] = goblin_Material;
    goblin_Material->phong.k_diffuse = QVector3D(0.8f,0.6f,0.1f);
    goblin_Material->phong.k_ambient = red->phong.k_diffuse * 0.4f;
    goblin_Material->phong.shininess = 90;

    // which material to use as default for all objects?
    auto std = red;

    // load meshes from .obj files and assign shader programs to them
    meshes_["Duck"]    = std::make_shared<Mesh>(":/models/duck/duck.obj", std);
    meshes_["Teapot"]  = std::make_shared<Mesh>(":/models/teapot/teapot.obj", std);
    meshes_["Goblin"]  = std::make_shared<Mesh>(":/models/goblin.obj", goblin_Material);
    meshes_["Yoda"]    = std::make_shared<Mesh>(":/models/yoda/yoda.obj", std);
    meshes_["Torus"]    = std::make_shared<Mesh>(":/models/torus.obj", std);
    meshes_["Cessna"]    = std::make_shared<Mesh>(":/models/cessna.obj", std);

    // add meshes of some procedural geometry objects (not loaded from OBJ files)
    meshes_["Cube"]   = std::make_shared<Mesh>(make_shared<geom::Cube>(), std);

    // pack each mesh into a scene node, along with a transform that scales
    // it to standard size [1,1,1]
    nodes_["Cube"]    = createNode(meshes_["Cube"], true);
    nodes_["Duck"]    = createNode(meshes_["Duck"], true);
    nodes_["Teapot"]  = createNode(meshes_["Teapot"], true);
    nodes_["Goblin"]  = createNode(meshes_["Goblin"], true);
    nodes_["Yoda"]    = createNode(meshes_["Yoda"], true);
    nodes_["Torus"]    = createNode(meshes_["Torus"], true);
    nodes_["Cessna"]    = createNode(meshes_["Cessna"], true);


}

// once the nodes_ map is filled, construct a hierarchical scene from it
void Scene::makeScene()
{
    // world contains the scene plus the camera
    nodes_["World"] = createNode(nullptr, false);

    // scene means everything but the camera
    nodes_["Scene"] = createNode(nullptr, false);
    nodes_["World"]->children.push_back(nodes_["Scene"]);

    // add camera node
    nodes_["Camera"] = createNode(nullptr, false);
    nodes_["World"]->children.push_back(nodes_["Camera"]);

    // add a light relative to the scene or world or camera
    nodes_["Light0"] = createNode(nullptr, false);
    lightNodes_.push_back(nodes_["Light0"]);

    // light attached to camera, placed right above camera
    nodes_["Camera"]->children.push_back(nodes_["Light0"]);
    nodes_["Light0"]->transformation.translate(QVector3D(0, 1, 0));

}

// this method is called implicitly by the Qt framework when a redraw is required.
void Scene::draw()
{
    // calculate animation time
    chrono::milliseconds millisec_since_first_draw;
    chrono::milliseconds millisec_since_last_draw;

    // calculate total elapsed time and time since last draw call
    auto current = clock_.now();
    millisec_since_first_draw = chrono::duration_cast<chrono::milliseconds>(current - firstDrawTime_);
    millisec_since_last_draw = chrono::duration_cast<chrono::milliseconds>(current - lastDrawTime_);
    lastDrawTime_ = current;

    // set time uniform in animated shader(s)
    float t = millisec_since_first_draw.count() / 1000.0f;

    for(auto mat : phongMaterials_)
        mat.second->time = t;

    draw_scene_();
}

void Scene::draw_scene_()
{

    // set camera based on node in scene graph
    float aspect = float(parent_->width())/float(parent_->height());
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(30.0f,aspect,0.01f,1000.0f);

    auto camToWorld = nodes_["World"]->toParentTransform(nodes_["Camera"]);
    auto viewMatrix = camToWorld.inverted();
    Camera camera(viewMatrix, projectionMatrix);

    // clear buffer
    glClearColor(bgcolor_[0], bgcolor_[1], bgcolor_[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // first light pass: standard depth test, no blending
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);

    // draw one pass for each light
    for(unsigned int i=0; i<lightNodes_.size(); i++) {

        // determine current light position and set it in all materials
        QMatrix4x4 lightToWorld = nodes_["World"]->toParentTransform(lightNodes_[i]);
        for(auto mat : phongMaterials_) {
            auto phong = mat.second; // mat is of type (key, value)
            phong->lights[i].position_WC = lightToWorld * QVector3D(0,0,0);
        }

        // draw light pass i
        nodes_["World"]->draw(camera, i);

        // settings for i>0 (add light contributions using alpha blending)
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE,GL_ONE);
        glDepthFunc(GL_EQUAL);
    }
}

// helper to load shaders and create programs
shared_ptr<QOpenGLShaderProgram> Scene::createProgram(const string& vertex,
                                                      const string& fragment,
                                                      const string& geom)
{
    auto p = make_shared<QOpenGLShaderProgram>();
    if(!p->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex.c_str()))
        qFatal("could not add vertex shader");
    if(!p->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment.c_str()))
        qFatal("could not add fragment shader");
    if(!geom.empty()) {
        if(!p->addShaderFromSourceFile(QOpenGLShader::Geometry, geom.c_str()))
            qFatal("could not add geometry shader");
    }
    if(!p->link())
        qFatal("could not link shader program");

    return p;
}

// helper to make a node from a mesh, and
// scale the mesh to standard size 1 of desired
shared_ptr<Node> Scene::createNode(shared_ptr<Mesh> mesh,
                                   bool scale_to_1)
{
    QMatrix4x4 transform;
    if(scale_to_1) {
        float r = mesh->geometry()->bbox().maxExtent();
        transform.scale(QVector3D(1.0/r,1.0/r,1.0/r));
    }

    return make_shared<Node>(mesh,transform);
}

void Scene::toggleAnimation(bool flag)
{
    if(flag) {
        timer_.start(1000.0 / 60.0); // update *roughly* every 60 ms
    } else {
        timer_.stop();
    }
}

void Scene::setSceneNode(QString node)
{
    auto n = nodes_[node];
    assert(n);

    nodes_["Scene"]->children.clear();
    nodes_["Scene"]->children.push_back(n);

    update();
}

// change background color
void Scene::setBackgroundColor(QVector3D rgb) {
    bgcolor_ = rgb; update();
}

// methods to change common material parameters
void Scene::setLightIntensity(size_t i, float v)
{
    if(i>=lightNodes_.size())
        return;

    for(auto mat : phongMaterials_)
        mat.second->lights[i].intensity = v; update();
}

// pass key/mouse events on to navigator objects
void Scene::keyPressEvent(QKeyEvent *event) {

    cameraNavigator_->keyPressEvent(event);
    update();

}
// mouse press events all processed by trackball navigator
void Scene::mousePressEvent(QMouseEvent *)
{
}
void Scene::mouseMoveEvent(QMouseEvent *)
{
}
void Scene::mouseReleaseEvent(QMouseEvent *)
{
}
void Scene::wheelEvent(QWheelEvent *)
{
}

// trigger a redraw of the widget through this method
void Scene::update()
{
    parent_->update();
}

void Scene::updateViewport(size_t width, size_t height)
{
    glViewport(0,0,GLint(width),GLint(height));
}


