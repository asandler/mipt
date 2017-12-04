#include <Manipulator.h>

#include <iostream>

#include <osgUtil/IntersectVisitor>
#include <osg/Notify>
#include <osg/Timer>
#include <osg/ShapeDrawable>
#include <osg/io_utils>

using namespace osg;
using namespace osgGA;

void Manipulator::home(const GUIEventAdapter& ea,GUIActionAdapter& us) {
    set(_homeEye, _homeCenter, _homeUp);
    us.requestRedraw();
    us.requestContinuousUpdate(false);
}

void Manipulator::init(const GUIEventAdapter& ea,GUIActionAdapter& us) {
    set(_homeEye, _homeCenter, _homeUp);
    us.requestContinuousUpdate(false);
}

bool Manipulator::handle(const GUIEventAdapter& ea,GUIActionAdapter& us) {
    if (!_camera.valid()) {
        _camera = us.asView()->getCamera();
    }

    switch(ea.getEventType()) {
        case(GUIEventAdapter::FRAME):
            {
#ifdef USE_VRPN
                if(_headTracker) {
                    _headTracker->update();
                }
#endif
                update();

                return false;
            }

        case(GUIEventAdapter::RESIZE):
            init(ea,us);
            us.requestRedraw();
            return true;
        default:
            break;
    }

    if (ea.getHandled())
        return false;

    switch(ea.getEventType()) {
        case(GUIEventAdapter::MOVE):
            {
                return true;
            }
        case(GUIEventAdapter::KEYDOWN):
            {
                if (ea.getKey()==GUIEventAdapter::KEY_Space) {
                    home(ea, us);
                    return true;
                } else if (ea.getKey()=='w') {
                    PitchingSpeed = 1.0f;
                    return true;
                } else if (ea.getKey()=='s') {
                    PitchingSpeed = -1.0f;
                    return true;
                } else if (ea.getKey()=='a') {
                    YawingSpeed = 1.0f;
                    return true;
                }
                else if (ea.getKey()=='d') {
                    YawingSpeed = -1.0f;
                    return true;
                }
                else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Up ||
                        ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Up)
                {
                    return true;
                }
                else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Down ||
                        ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Down)
                {
                    return true;
                }
                else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Left ||
                        ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Left)
                {
                    keyboardOffset = osg::Vec3(-0.008, 0, 0);
                    return true;
                }
                else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Right ||
                        ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Right)
                {
                    keyboardOffset = osg::Vec3(0.008, 0, 0);
                    return true;
                }
                return false;
            }

        case(GUIEventAdapter::KEYUP):
            {
                if (ea.getKey()=='w') {
                    PitchingSpeed = 0.0f;
                    return true;
                } else if (ea.getKey()=='s') {
                    PitchingSpeed = 0.0f;
                    return true;
                } else if (ea.getKey()=='a') {
                    YawingSpeed = 0.0f;
                    return true;
                } else if (ea.getKey()=='d') {
                    YawingSpeed = 0.0f;
                    return true;
                }
                else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Up ||
                        ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Up)
                {
                    return true;
                }
                else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Down ||
                        ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Down)
                {
                    return true;
                }
                else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Left ||
                        ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Left)
                {
                    keyboardOffset = osg::Vec3(0, 0, 0);
                    return true;
                }
                else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Right ||
                        ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Right)
                {
                    keyboardOffset = osg::Vec3(0, 0, 0);
                    return true;
                }
                return false;
            }

        default:
            return false;
    }
}

osg::Matrixd Manipulator::getMatrix() const {
#ifdef USE_VRPN
    return osg::Matrixd::rotate(rotation) * osg::Matrixd::translate(PosHead);
#else
    return osg::Matrixd(  sdir[0],      sdir[1],     sdir[2],      0.0,
            udir[0],      udir[1],     udir[2],      0.0,
            -ldir[0],     -ldir[1],    -ldir[2],      0.0,
            PosHead.x(),      PosHead.y(),     PosHead.x(),      1.0);
#endif
}

osg::Matrixd Manipulator::getInverseMatrix() const {
#ifdef USE_VRPN
    return osg::Matrixd::translate(-PosHead) * osg::Matrixd::rotate(rotation.inverse());
#else
    return osg::Matrixd(  sdir[0],      udir[0],     -ldir[0],     0.0,
            sdir[1],      udir[1],     -ldir[1],     0.0,
            sdir[2],      udir[2],     -ldir[2],     0.0,
            -PosHead*sdir,    -PosHead*udir,     PosHead*ldir,     1.0);
#endif
}

void Manipulator::yaw(float angle) {
    osg::Quat yaw_rotation;
    yaw_rotation.makeRotate(angle,udir);
    sdir = yaw_rotation*sdir;
    ldir = yaw_rotation*ldir;
}

void Manipulator::pitch(float angle) {
    osg::Quat pitch_rotation;
    pitch_rotation.makeRotate(angle,sdir);
    udir = pitch_rotation*udir;
    ldir = pitch_rotation*ldir;
}

void Manipulator::set(const osg::Vec3d& eye, const osg::Vec3d& look, const osg::Vec3d& up) {
    osg::Matrix m = osg::Matrix::inverse(osg::Matrix::lookAt(eye, look, up));

    rotation = m.getRotate();
    PosHead = eye;

    //    updir = osg::Vec3(0.0, 0.0, 1.0);
    //    updir.normalize();

    ldir = look - eye;
    ldir.normalize();

    sdir = (ldir^up);
    sdir.normalize();

    udir = (sdir^ldir);
    udir.normalize();

    xdir = ldir;
    ydir = sdir;
    zdir = udir;
    updir = up;
    updir.normalize();
}

void Manipulator::rotate_SKO(const Vec3& from, const Vec3& to) {
    osg::Quat rot;
    rot.makeRotate(from, to);

    xdir = rot*xdir;
    ydir = rot*ydir;
    zdir = rot*zdir;
}

void Manipulator::rotate_SKC(const Vec3& from, const Vec3& to) {
    osg::Quat rot;
    rot.makeRotate(from, to);

    ldir = rot*ldir;
    sdir = rot*sdir;
    udir = rot*udir;
}

bool Manipulator::update() {
    static Timer timer;

    double dT = timer.time_s();

    if (dT < 0.001)
        return true;

    double step = 0.02;
    while (dT > 0.0) {
        double DT = osg::minimum(dT, step);
        dT -= step;

#ifdef USE_VRPN
        if (_headTracker) {
            PosHead = _headTracker->getMatrix().getTrans();
        }
#else
        pitch(DT*PitchingSpeed);
        yaw(DT*YawingSpeed);
        PosHead += keyboardOffset;
#endif
        std::cout << PosHead.x() << " " << PosHead.y() << " " << PosHead.z() << std::endl;

    }

    timer.setStartTick();
    return true;
}
