#include <osgGA/CameraManipulator>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>

#ifdef USE_VRPN
#include <osgVRPN/TrackerBase.h>
#endif

namespace osgGA {

class Manipulator: public CameraManipulator {
public:
    Manipulator():_headTracker(0) {}

    virtual const char* className() const { return "SlideManipulator"; }

    /** set the position of the matrix manipulator using a 4x4 Matrix.*/
    virtual void setByMatrix(const osg::Matrixd& matrix) {}

    /** set the position of the matrix manipulator using a 4x4 Matrix.*/
    virtual void setByInverseMatrix(const osg::Matrixd& matrix) { setByMatrix(osg::Matrixd::inverse(matrix)); }

    /** get the position of the manipulator as 4x4 Matrix.*/
    virtual osg::Matrixd getMatrix() const;

    /** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
    virtual osg::Matrixd getInverseMatrix() const;

    virtual void setNode(osg::Node* node) { Node = node; }

    virtual const osg::Node* getNode() const { return Node.get(); }

    virtual osg::Node* getNode() { return Node.get(); }

    virtual void computeHomePosition() {}

    virtual void home(const GUIEventAdapter& ea,GUIActionAdapter& us);

    virtual void init(const GUIEventAdapter& ea,GUIActionAdapter& us);

    virtual bool handle(const GUIEventAdapter& ea,GUIActionAdapter& us);

    virtual void getUsage(osg::ApplicationUsage& usage) const {}

#ifdef USE_VRPN
    void setHeadTracker(osgVRPN::TrackerBase* tracker) { _headTracker = tracker; }
    osgVRPN::TrackerBase* getHeadTracker() const { return _headTracker; }
#endif

    osg::Vec3 getPosHead() { return PosHead; }

protected:

    void yaw(float angle);
    void pitch(float angle);

    virtual ~Manipulator() {}

    void set(const osg::Vec3d& eye,const osg::Vec3d& look,const osg::Vec3d& up);

    bool update();

    void rotate_SKO(const osg::Vec3& from, const osg::Vec3& to);
    void rotate_SKC(const osg::Vec3& from, const osg::Vec3& to);

    osg::Vec3   PosHead;

    osg::Quat   rotation;

    osg::Vec3   keyboardOffset;

    osg::Vec3   xdir;
    osg::Vec3   ydir;
    osg::Vec3   zdir;

    osg::Vec3   updir;

    osg::Vec3   ldir;
    osg::Vec3   sdir;
    osg::Vec3   udir;

    osg::ref_ptr<osg::Node>        Node;

    float   PitchingSpeed;
    float   YawingSpeed;

#ifdef USE_VRPN
    osg::ref_ptr<osgVRPN::TrackerBase> _headTracker;
#endif

    osg::ref_ptr<osg::Camera> _camera;
};

}
