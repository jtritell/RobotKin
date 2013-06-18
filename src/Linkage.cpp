/*
 -------------------------------------------------------------------------------
 Linkage.cpp
 Robot Library Project
 
 Initially created by Rowland O'Flaherty ( rowlandoflaherty.com ) on 5/11/13.
 
 Version 1.0
 -------------------------------------------------------------------------------
 */



//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "Linkage.h"
#include "Robot.h"


//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------
using namespace RobotKin;


//------------------------------------------------------------------------------
// Linkage Nested Classes
//------------------------------------------------------------------------------
// Joint Class Constructors

//Linkage::Joint::Joint(Joint &joint)
//    : Frame::Frame(joint.respectToFixed_, joint.name(), joint.id(), JOINT),
//      min_(joint.min_),
//      max_(joint.max_),
//      value_(joint.value_),
//      linkage_(0),
//      robot_(0),
//      hasRobot(false),
//      hasLinkage(false)
//{
    
//}

Linkage::Joint::Joint(Isometry3d respectToFixed,
                      string name,
                      size_t id,
                      JointType jointType,
                      double minValue,
                      double maxValue)
            : Frame::Frame(respectToFixed, name, id, JOINT),
              respectToFixedTransformed_(respectToFixed),
              respectToLinkage_(respectToFixed),
              jointType_(jointType),
              min_(minValue),
              max_(maxValue),
              value_(0),
              linkage_(0),
              robot_(0),
              hasRobot(false),
              hasLinkage(false)
{
    value(value_);
}


// Joint Destructor
Linkage::Joint::~Joint()
{

}

// Joint Overloaded Operators
const Linkage::Joint& Linkage::Joint::operator=(double aValue)
{
    value(aValue);
    return *this;
}

// Joint Methods
double Linkage::Joint::value() const { return value_; }
void Linkage::Joint::value(double value)
{
    value_ = value;
    if (jointType_ == REVOLUTE) {
        respectToFixedTransformed_ = respectToFixed_ * Eigen::AngleAxisd(value_, Eigen::Vector3d::UnitZ());
    } else {
        respectToFixedTransformed_ = respectToFixed_ * Eigen::Translation3d(Eigen::Vector3d(0., 0., value_));
    }
    if ( hasLinkage )
        linkage_->updateFrames();
}


const Isometry3d& Linkage::Joint::respectToFixed() const { return respectToFixed_; }
void Linkage::Joint::respectToFixed(Isometry3d aCoordinate)
{
    respectToFixed_ = aCoordinate;
    value(value_);
}

const Isometry3d& Linkage::Joint::respectToFixedTransformed() const
{
    return respectToFixedTransformed_;
}

const Isometry3d& Linkage::Joint::respectToLinkage() const
{
    return respectToLinkage_;
}

Isometry3d Linkage::Joint::respectToRobot() const
{
    if(hasLinkage)
        return linkage_->respectToRobot_ * respectToLinkage_;
    else
        return Isometry3d::Identity();
}

Isometry3d Linkage::Joint::respectToWorld() const
{
    if(hasLinkage)
        return linkage_->respectToWorld() * respectToLinkage_;
    else
        return Isometry3d::Identity();
}

const Linkage* Linkage::Joint::linkage() const
{
    return linkage_;
}

void Linkage::Joint::printInfo() const
{
    cout << frameTypeString() << " Info: " << name() << " (ID: " << id()  << ")" << endl;
    cout << "Joint value: " << value() << endl;
    cout << "Respect to fixed frame:" << endl;
    cout << respectToFixed().matrix() << endl << endl;
    cout << "Respect to fixed after transformation: " << endl;
    cout << respectToFixedTransformed().matrix() << endl << endl;
    cout << "Respect to linkage frame:" << endl;
    cout << respectToLinkage().matrix() << endl << endl;
    cout << "Respect to robot frame:" << endl;
    cout << respectToRobot().matrix() << endl << endl;
    cout << "Respect to world frame:" << endl;
    cout << respectToWorld().matrix() << endl << endl;
}


// Tool Class
Linkage::Tool::Tool(const Tool &tool)
    : Frame::Frame(tool.respectToFixed_, tool.name_, tool.id_),
      respectToLinkage_(tool.respectToLinkage_),
      linkage_(0),
      robot_(0),
      hasRobot(false),
      hasLinkage(false)
{

}

Linkage::Tool::Tool(Isometry3d respectToFixed, string name, size_t id)
    : Frame::Frame(respectToFixed, name, id, TOOL),
      respectToLinkage_(respectToFixed),
      linkage_(0),
      robot_(0),
      hasRobot(false),
      hasLinkage(false)
{

}

// Tool Destructor
Linkage::Tool::~Tool()
{
    
}

// Tool Methods
const Isometry3d& Linkage::Tool::respectToFixed() const { return respectToFixed_; }
void Linkage::Tool::respectToFixed(Isometry3d aCoordinate)
{
    respectToFixed_ = aCoordinate;
    if(hasLinkage)
        linkage_->updateFrames();
}

const Isometry3d& Linkage::Tool::respectToLinkage() const
{
    return respectToLinkage_;
}

Isometry3d Linkage::Tool::respectToRobot() const
{
    if(hasLinkage)
        return linkage_->respectToRobot_ * respectToLinkage_;
    else
        return respectToLinkage_;
}

Isometry3d Linkage::Tool::respectToWorld() const
{
    if(hasLinkage)
        return linkage_->respectToWorld() * respectToLinkage_;
    else
        return respectToLinkage_;
}

const Linkage* Linkage::Tool::parentLinkage() const
{
    if(hasLinkage)
        return linkage_;
    else
        return NULL;
}

const Robot* Linkage::Tool::parentRobot() const
{
    if(hasRobot)
        return robot_;
    else
        return NULL;
}

size_t Linkage::Tool::getLinkageID()
{
    if(hasLinkage)
        return linkage_->id();
    else
        return 0;
}

string Linkage::Tool::getLinkageName()
{
    if(hasLinkage)
        return linkage_->name();
    else
        return "";
}

size_t Linkage::Tool::getRobotID()
{
    if(hasRobot)
        return robot_->id();
    else
        return 0;
}

string Linkage::Tool::getRobotName()
{
    if(hasRobot)
        return robot_->name();
    else
        return "";
}

size_t Linkage::Tool::getParentJointID()
{
    if(hasLinkage)
        if(linkage_->joints_.size() > 0)
            return linkage_->joints_.size()-1;

    return 0;
}

string Linkage::Tool::getParentJointName()
{
    if(hasLinkage)
        if(linkage_->joints_.size() > 0)
            return linkage_->joints().back()->name();

    return "";
}

size_t Linkage::Joint::getLinkageID()
{
    if(hasLinkage)
        return linkage_->id();
    else
        return 0;
}

string Linkage::Joint::getLinkageName()
{
    if(hasLinkage)
        return linkage_->name();
    else
        return "";
}

size_t Linkage::Joint::getRobotID()
{
    if(hasRobot)
        return robot_->id();
    else
        return 0;
}

string Linkage::Joint::getRobotName()
{
    if(hasRobot)
        return robot_->name();
    else
        return "";
}


void Linkage::Tool::printInfo() const
{
    cout << frameTypeString() << " Info: " << name() << " (ID: " << id() << ")" << endl;
    cout << "Respect to fixed frame:" << endl;
    cout << respectToFixed().matrix() << endl << endl;
    cout << "Respect to linkage frame:" << endl;
    cout << respectToLinkage().matrix() << endl << endl;
    cout << "Respect to robot frame:" << endl;
    cout << respectToRobot().matrix() << endl << endl;
    cout << "Respect to world frame:" << endl;
    cout << respectToWorld().matrix() << endl << endl;
}

Linkage::Tool Linkage::Tool::Identity()
{
    Linkage::Tool tool;
    return tool;
}


//------------------------------------------------------------------------------
// Linkage Lifecycle
//------------------------------------------------------------------------------
// Constructors
Linkage::Linkage(const Linkage &linkage)
    : Frame::Frame(linkage.respectToFixed_, linkage.name_,
                   linkage.id_, linkage.frameType_),
      respectToRobot_(linkage.respectToRobot_),
      initializing_(false),
      hasRobot(false),
      hasChildren(false)
{
    vector<Linkage::Joint> joints; joints.resize(0);
//    joints.resize(linkage.joints_.size());
//    for(size_t i=0; i<linkage.joints_.size(); i++)
//    {
//        joints[i] = *(linkage.joints_[i]);
//    }

    //initialize(joints, linkage.tool_);

    Linkage::Tool newTool;
    initialize(joints, newTool);
}

Linkage::Linkage()
    : Frame::Frame(Isometry3d::Identity(), "", 0, LINKAGE),
      respectToRobot_(Isometry3d::Identity()),
      initializing_(false),
      hasRobot(false),
      hasChildren(false)
{
    analyticalIK = Linkage::defaultAnalyticalIK;
}

Linkage::Linkage(Isometry3d respectToFixed, string name, size_t id, Linkage::Joint joint, Linkage::Tool tool)
    : Frame::Frame(respectToFixed, name, id, LINKAGE),
      respectToRobot_(respectToFixed),
      initializing_(false),
      hasRobot(false),
      hasChildren(false)
{
    analyticalIK = Linkage::defaultAnalyticalIK;
    vector<Linkage::Joint> joints(1);
    joints[0] = joint;
    initialize(joints, tool);
}

Linkage::Linkage(Isometry3d respectToFixed, string name, size_t id, vector<Linkage::Joint> joints, Linkage::Tool tool)
    : Frame::Frame(respectToFixed, name, id, LINKAGE),
      respectToRobot_(respectToFixed),
      initializing_(false),
      hasRobot(false),
      hasChildren(false)
{
    analyticalIK = Linkage::defaultAnalyticalIK;
    initialize(joints, tool);
}

// Destructor
Linkage::~Linkage()
{
    
}

//------------------------------------------------------------------------------
// Linkage Overloaded Operators
//------------------------------------------------------------------------------
// Assignment operator
const Linkage& Linkage::operator=(const VectorXd& someValues)
{
    values(someValues);
    return *this;
}

//--------------------------------------------------------------------------
// Linkage Public Member Functions
//--------------------------------------------------------------------------

Linkage* Linkage::parentLinkage() { return parentLinkage_; }

size_t Linkage::nChildren() const { return childLinkages_.size(); }

size_t Linkage::nJoints() const { return joints_.size(); }

const Linkage::Joint& Linkage::const_joint(size_t jointIndex) const
{
    assert(jointIndex < nJoints());
    return *joints_[jointIndex];
}
const Linkage::Joint& Linkage::const_joint(string jointName) const { return *joints_[jointNameToIndex_.at(jointName)]; }

Linkage::Joint& Linkage::joint(size_t jointIndex)
{
    assert(jointIndex < nJoints());
    return *joints_[jointIndex];
}
Linkage::Joint& Linkage::joint(string jointName) { return *joints_[jointNameToIndex_.at(jointName)]; }

const vector<Linkage::Joint*>& Linkage::const_joints() const { return joints_; }
vector<Linkage::Joint*>& Linkage::joints() { return joints_; }


const Linkage::Tool& Linkage::const_tool() const { return tool_; }
Linkage::Tool& Linkage::tool() { return tool_; }


VectorXd Linkage::values() const
{
    VectorXd theValues(nJoints(),1);
    for (size_t i = 0; i < nJoints(); ++i) {
        theValues[i] = joints_[i]->value();
    }
    return theValues;
}

bool Linkage::values(const VectorXd& someValues)
{    
    if(someValues.size() == nJoints())
    {
        for (size_t i = 0; i < nJoints(); ++i) {
            joints_[i]->value(someValues(i));
        }
        updateFrames();
        return true;
    }
    
    std::cerr << "ERROR! Number of values (" << someValues.size() << ") does not match "
              << "the number of joints (" << nJoints() << ")!" << std::endl;
    return false;
}

const Isometry3d& Linkage::respectToFixed() const { return respectToFixed_; }
void Linkage::respectToFixed(Isometry3d aCoordinate)
{
    respectToFixed_ = aCoordinate;
    updateFrames();
}


const Isometry3d& Linkage::respectToRobot() const
{
    return respectToRobot_;
}


Isometry3d Linkage::respectToWorld() const
{
    if(hasRobot)
        return robot_->respectToWorld_ * respectToRobot_;
    else
        return Isometry3d::Identity();
}

void Linkage::jacobian(MatrixXd& J, const vector<Linkage::Joint*>& jointFrames, Vector3d location, const Frame* refFrame) const
{ // location should be specified respect to linkage coordinate frame
    
    size_t nCols = jointFrames.size();
    J.resize(6, nCols);
    
    Vector3d o_i, d_i, z_i; // Joint i location, offset, axis
    
    for (size_t i = 0; i < nCols; ++i) {
        
        o_i = jointFrames[i]->respectToLinkage_.translation(); // Joint i location
        d_i = o_i - location; // Vector from location to joint i
        z_i = jointFrames[i]->respectToLinkage_.rotation().col(2); // Joint i joint axis
        
        // Set column i of Jocabian
        if (jointFrames[i]->jointType_ == REVOLUTE) {
            J.block(0, i, 3, 1) = d_i.cross(z_i);
            J.block(3, i, 3, 1) = z_i;
        } else {
            J.block(0, i, 3, 1) = z_i;
            J.block(3, i, 3, 1) = Vector3d::Zero();
        }
        
    }
    
    // Jacobian transformation
    Matrix3d r(refFrame->respectToWorld().rotation().inverse() * respectToWorld().rotation());
    MatrixXd R(6,6);
    R << r, Matrix3d::Zero(), Matrix3d::Zero(), r;
    J = R * J;
}


void Linkage::printInfo() const
{
    cout << frameTypeString() << " Info: " << name() << " (ID: " << id() << ")" << endl;
    
    cout << "Respect to fixed frame:" << endl;
    cout << respectToFixed().matrix() << endl << endl;
    cout << "Respect to robot frame:" << endl;
    cout << respectToRobot().matrix() << endl << endl;
    cout << "Respect to world frame:" << endl;
    cout << respectToWorld().matrix() << endl << endl;
    
    cout << "Joints (ID, Name, Value): " << endl;
    for (vector<Linkage::Joint*>::const_iterator jointIt = const_joints().begin();
         jointIt != const_joints().end(); ++jointIt) {
        cout << (*jointIt)->id() << ", " << (*jointIt)->name() << ", " << (*jointIt)->value() << endl;
    }
    
    for (vector<Linkage::Joint*>::const_iterator jointIt = const_joints().begin();
         jointIt != const_joints().end(); ++jointIt) {
        (*jointIt)->printInfo();
    }
    const_tool().printInfo();
    
    MatrixXd J;
    jacobian(J, const_joints(), const_tool().respectToLinkage().translation(), this);
    
    cout << "Jacobian for " << name() << ":" << endl;
    cout << J.matrix() << endl;
}

//------------------------------------------------------------------------------
// Linkage Private Member Functions
//------------------------------------------------------------------------------
void Linkage::initialize(vector<Linkage::Joint> joints, Linkage::Tool tool)
{
    initializing_ = true;
    joints_.resize(joints.size());
    for(size_t i = 0; i != joints_.size(); ++i) {
        joints_[i] = new Joint(joints[i]);
        joints_[i]->id_ = i;
        joints_[i]->linkage_ = this;
        joints_[i]->hasLinkage = true;
        jointNameToIndex_[joints_[i]->name()] = i;
    }
    tool_ = tool;
    tool_.id_ = joints_.size();
    tool_.linkage_ = this;

    if(hasRobot)
        tool_.Tool::robot_ = robot_;
    tool_.Tool::hasRobot = hasRobot;
    
    initializing_ = false;

    updateFrames();
}

//void Linkage::addJoint(Linkage::Joint newJoint)
//{
//    Joint* tempJoint = new Joint(newJoint);
//}


void Linkage::updateFrames()
{
    if (~initializing_) {
        for (size_t i = 0; i < joints_.size(); ++i) {
            if (i == 0) {
                joints_[i]->respectToLinkage_ = joints_[i]->respectToFixedTransformed_;
                
            } else {
                joints_[i]->respectToLinkage_ = joints_[i-1]->respectToLinkage_ * joints_[i]->respectToFixedTransformed_;
            }
        }
        tool_.respectToLinkage_ = joints_[joints_.size()-1]->respectToLinkage_ * tool_.respectToFixed_;
        
        if(hasChildren)
            updateChildLinkage();
    }
}


void Linkage::updateChildLinkage()
{
    for (size_t i = 0; i < nChildren(); ++i) {
        childLinkages_[i]->respectToRobot_ = tool_.respectToRobot() * childLinkages_[i]->respectToFixed_;
        childLinkages_[i]->updateChildLinkage();
    }
}

bool Linkage::defaultAnalyticalIK(VectorXd& q, const Isometry3d& B, const VectorXd& qPrev) {
    // This function is just a place holder and should not be used. The analyticalIK function pointer should be set to the real analytical IK function.
    q = NAN * qPrev;
    return false;
}






