#include <clocale>
#include <assert.h>
#include <string.h>
#include <iostream>

#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/LightModel>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <Manipulator.h>

#ifdef USE_VRPN
#include <osgVRPN/Tracker.h>
#endif

class MyViewer: public osgViewer::Viewer
{
    public:
        MyViewer::MyViewer():
            osgViewer::Viewer()
    {}

        MyViewer::MyViewer(osg::ArgumentParser& arguments):
            osgViewer::Viewer(arguments)
    {}

        void updateTraversal() override;
};

void MyViewer::updateTraversal()
{
    osgViewer::Viewer::updateTraversal();

    osg::DisplaySettings* ds = osg::DisplaySettings::instance();

    double fovy, aspect, znear, zfar;
    _camera->getProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);

    osgGA::Manipulator* manipulator = dynamic_cast<osgGA::Manipulator*>(getCameraManipulator());

    if (manipulator && manipulator->getHeadTracker())
    {
        osg::Vec3 posHead = manipulator->getPosHead();

        double sw = ds->getScreenWidth();
        double sh = ds->getScreenHeight();

        double screenPos = -4.0;

        double distanceToScreen = fabs(screenPos - posHead.z());
        double dx = posHead.x();

        double xright = (sw * 0.5 - dx) * znear / distanceToScreen;
        double xleft = -(sw * 0.5 + dx) * znear / distanceToScreen;

        double y1 = (sh * 0.5) * znear / distanceToScreen;

        _camera->setProjectionMatrix(osg::Matrix::frustum(xleft, xright, -y1, y1, znear, zfar));

        std::cout << "Set projection. Distance to screen " << distanceToScreen << std::endl;
    } else {
        //default behavior of base class

        _camera->setViewMatrix(_cameraManipulator->getInverseMatrix());

        double sw = osg::DisplaySettings::instance()->getScreenWidth();
        double sh = osg::DisplaySettings::instance()->getScreenHeight();			
        double sd = osg::DisplaySettings::instance()->getScreenDistance();

        double vfov = osg::RadiansToDegrees(atan2(sh / 2.0f, sd) * 2.0);
        _camera->setProjectionMatrixAsPerspective(vfov, sw / sh, znear, zfar);
    }
}

struct AppSettings {
    osg::Vec4 clearColor;
    bool keyboardControl;
    std::string headTrackerName;
};

osg::Camera* makeHUD() {
    osg::Camera* hud = new osg::Camera;

    hud->setProjectionMatrix(osg::Matrix::ortho2D(0,1024,0,768));
    hud->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    hud->setViewMatrix(osg::Matrix::identity());
    hud->setClearMask(GL_DEPTH_BUFFER_BIT);
    hud->setRenderOrder(osg::Camera::POST_RENDER);

    return hud;
}

AppSettings setAppSettings() {
    AppSettings settings;
    settings.clearColor = osg::Vec4(0.5f,0.5f,0.5f,1.0f);
    settings.keyboardControl = false;
    settings.headTrackerName = "Caret@localhost";
    return settings;
}

void initStateSet(osg::Group* root, osg::Camera* camera, AppSettings settings) {
    assert(root);
    assert(camera);

    osg::StateSet* ss = root->getOrCreateStateSet();
    osg::Group* lightGroup = new osg::Group;

    float diffIntense = 1.0f;

    osg::Light* sunLight = new osg::Light;
    sunLight->setLightNum(0);
    sunLight->setPosition(osg::Vec4(1.0f, 0.0f, 1.0f, 0.0f));
    sunLight->setAmbient(osg::Vec4(0.0f, 0.0f, 0.0f,1.0f));
    sunLight->setDiffuse(osg::Vec4(diffIntense, diffIntense, diffIntense, 1.0f));

    float intens = 0.3f;

    osg::LightModel* lm = new osg::LightModel;
    lm->setAmbientIntensity(osg::Vec4(intens, intens, intens,1.0f));
    ss->setAttributeAndModes(lm, osg::StateAttribute::ON);

    osg::LightSource* lightSource1 = new osg::LightSource;
    lightSource1->setLight(sunLight);
    lightSource1->setLocalStateSetModes(osg::StateAttribute::ON);
    lightSource1->setStateSetModes(*ss, osg::StateAttribute::ON);

    lightGroup->addChild(lightSource1);

    root->addChild(lightGroup);

    camera->setClearColor(settings.clearColor);
    camera->setNearFarRatio(0.001);
}

int main(int argc, char **argv) {
    std::setlocale(LC_CTYPE, "");

    osg::DisplaySettings::instance()->setNumMultiSamples(4);

    osg::ArgumentParser arguments(&argc, argv);
    AppSettings settings = setAppSettings();

    MyViewer viewer(arguments);
    osg::Group* root = new osg::Group;
    initStateSet(root, viewer.getCamera(), settings);
    osg::Camera* hud = makeHUD();
    root->addChild(hud);

    osg::Geode* geode = new osg::Geode;

    osg::ShapeDrawable* on = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0, 1.2121, -3), 0.02));
    osg::ShapeDrawable* sdz = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0, 0.75, -3), 0.02));
    osg::ShapeDrawable* sdy = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0, 0, -5), 0.02));
    on->setColor(osg::Vec4(1.0, 1.0, 0.0, 1.0));
    sdy->setColor(osg::Vec4(1.0, 0.0, 1.0, 1.0));
    sdz->setColor(osg::Vec4(0.0, 1.0, 1.0, 1.0));

    geode->addDrawable(on);
    geode->addDrawable(sdy);
    geode->addDrawable(sdz);

    osg::ref_ptr<osg::PositionAttitudeTransform> marker = new osg::PositionAttitudeTransform;
    marker->addChild(geode);

    root->addChild(marker);

    osg::Cylinder* cylinder = new osg::Cylinder(osg::Vec3(0.0, 1.2121 / 2, -3), 0.01, 1.2121);
    cylinder->setRotation(osg::Quat(3.14159 / 2, osg::Vec3(1, 0, 0)));
    osg::ShapeDrawable* sd = new osg::ShapeDrawable(cylinder);
    sd->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));
    geode->addDrawable(sd);

    osg::ref_ptr<osg::PositionAttitudeTransform> palka = new osg::PositionAttitudeTransform;
    palka->addChild(geode);

    root->addChild(palka);

    osgGA::Manipulator* manipulator = new osgGA::Manipulator;
    viewer.setCameraManipulator(manipulator);

    manipulator->setHomePosition(
            osg::Vec3(0, 0, 0),//start position
            osg::Vec3(0, 0, -1),//gaze direction
            osg::Vec3(0.0f, 1.0f, 0.0f));//up direction
#ifdef USE_VRPN
    osg::ref_ptr<osgVRPN::TrackerBase> headTracker = new osgVRPN::Tracker(settings.headTrackerName.c_str());
    manipulator->setHeadTracker(headTracker);
#endif

    viewer.addEventHandler(new osgViewer::ThreadingHandler);
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);
    viewer.setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
    viewer.setSceneData(root);

    while (!viewer.done()) {
        viewer.frame();
    }
}
