/*
 -------------------------------------------------------------------------------
 Robot.h
 robotTest Project
 
 CLASS NAME:
 Robot
 
 DESCRIPTION:
 description...
 
 FILES:
 Robot.h
 Robot.cpp
 
 DEPENDENCIES:
 
 
 CONSTRUCTORS:
 Robot();
 
 PROPERTIES:
 prop1 - description... .
 
 prop2 - description... .
 
 METHODS:
 type method1(type arg1);
 Description... .
 arg1 - description... .
 
 NOTES:
 
 
 EXAMPLES:
 Example 1: description
 ----------------------------------------------------------------------------
 code...
 ----------------------------------------------------------------------------
 
 
 VERSIONS:
 1.0 - 5/15/13 - Rowland O'Flaherty ( rowlandoflaherty.com )
 
 -------------------------------------------------------------------------------
 */



#ifndef _Robot_h_
#define _Robot_h_



//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "Frame.h"
#include "Linkage.h"
#include <vector>
#include <map>
#include <string>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>



// TODO: Add a user-specified level of assertiveness

namespace RobotKin {

    typedef enum {
        RK_SOLVED = 0,
        RK_CONVERGED,
        RK_DIVERGED,
        RK_NO_SOLUTION,
        RK_INVALID_JOINT,
        RK_INVALID_LINKAGE,

        RK_SOLVER_NOT_READY,

        RK_TYPE_SIZE
    } rk_result_t;

    static const char *rk_result_string[RK_TYPE_SIZE] =
    {
        "RK_SOLVED",
        "RK_CONVERGED", // TODO: Is this really any different than RK_SOLVED?
        "RK_DIVERGED",
        "RK_NO_SOLUTION",
        "RK_INVALID_JOINT",
        "RK_INVALID_LINKAGE",

        "RK_SOLVER_NOT_READY"
    };

    std::string rk_result_to_string(rk_result_t result);
    
    //------------------------------------------------------------------------------
    // Typedefs
    //------------------------------------------------------------------------------
    typedef Eigen::Matrix<double, 6, Eigen::Dynamic> Matrix6Xd;

	// Sort parentIndices and linkages
    struct indexParentIndexPair {
        size_t I;
        int pI;
    
        bool operator<( const indexParentIndexPair& rhs ) const {
            return pI < rhs.pI;
        }
    };

    
    
    class Robot : public Frame
    {
        //--------------------------------------------------------------------------
        // Robot Friends
        //--------------------------------------------------------------------------
        friend class Linkage;
        friend class Frame;
        
    public:
        //--------------------------------------------------------------------------
        // Robot Lifecycle
        //--------------------------------------------------------------------------
        // Constructors
        Robot();
        Robot(std::vector<Linkage> linkageObjs, std::vector<int> parentIndices);

        Robot(std::string filename, std::string name="", size_t id=0);
        bool loadURDF(std::string filename);
        
        // Destructor
        virtual ~Robot();


        double errorClamp;
        double deltaClamp;
        double gammaMax;
        double tolerance;
        double transTolerance;
        double damp;
        size_t maxIterations;
        
        //--------------------------------------------------------------------------
        // Robot Public Member Functions
        //--------------------------------------------------------------------------
        size_t nLinkages() const;
        
        size_t linkageIndex(std::string linkageName) const;
        
        // Getting individual linkages
        const Linkage& const_linkage(size_t linkageIndex) const;
        const Linkage& const_linkage(std::string linkageName) const;
        
        Linkage& linkage(size_t linkageIndex);
        Linkage& linkage(std::string linkageName);
        
        // Getting all the linkages
        const std::vector<Linkage*>& const_linkages() const;
        std::vector<Linkage*>& linkages();
        
        // Adding new linkages
        void addLinkage(int parentIndex, std::string name);
        void addLinkage(std::string parentName, std::string name);
        void addLinkage(Linkage linkage, std::string parentName, std::string name);
        void addLinkage(Linkage linkage, int parentIndex, std::string name);
        
        // Getting joint information
        size_t nJoints() const;
        size_t jointIndex(std::string jointName) const;

        const Joint& const_joint(size_t jointIndex) const;
        const Joint& const_joint(std::string jointName) const;

        Joint& joint(size_t jointIndex);
        Joint& joint(std::string jointName);

        // Convenience function
        void setJointValue(size_t jointIndex, double val);
        void setJointValue(std::string jointName, double val);
        
        const std::vector<Joint*>& const_joints() const;
        std::vector<Joint*>& joints();
        
        Eigen::VectorXd values() const;
        void values(const Eigen::VectorXd& someValues);
        void values(const std::vector<size_t> &jointIndices, const Eigen::VectorXd& jointValues);
        
        const TRANSFORM& respectToFixed() const;
        void respectToFixed(TRANSFORM aCoordinate);
        
        TRANSFORM respectToWorld() const;
        
        void jacobian(Eigen::MatrixXd& J, const std::vector<Joint*>& jointFrames, TRANSLATION location, const Frame* refFrame) const;
        
        void updateFrames();
        void printInfo() const;
        
        //--------------------------------------------------------------------------
        // Kinematics Solvers
        //--------------------------------------------------------------------------

        rk_result_t selectivelyDampedLeastSquaresIK_chain(const std::vector<size_t> &jointIndices, Eigen::VectorXd &jointValues,
                                         const TRANSFORM &target, const TRANSFORM &finalTF = TRANSFORM::Identity());

        rk_result_t selectivelyDampedLeastSquaresIK_chain(const std::vector<std::string>& jointNames, Eigen::VectorXd& jointValues,
                                         const TRANSFORM& target, const TRANSFORM &finalTF = TRANSFORM::Identity());

        rk_result_t selectivelyDampedLeastSquaresIK_linkage(const std::string linkageName, Eigen::VectorXd &jointValues,
                                         const TRANSFORM& target, const TRANSFORM &finalTF = TRANSFORM::Identity());


        //////////////////

        rk_result_t pseudoinverseIK_chain(const std::vector<size_t> &jointIndices, Eigen::VectorXd &jointValues,
                                          const TRANSFORM &target, const TRANSFORM &finalTF = TRANSFORM::Identity());

        rk_result_t pseudoinverseIK_chain(const std::vector<std::string>& jointNames, Eigen::VectorXd& jointValues,
                                          const TRANSFORM& target, const TRANSFORM &finalTF = TRANSFORM::Identity());

        rk_result_t pseudoinverseIK_linkage(const std::string linkageName, Eigen::VectorXd &jointValues,
                                            const TRANSFORM& target, const TRANSFORM &finalTF = TRANSFORM::Identity());

        //////////////////

        rk_result_t jacobianTransposeIK_chain(const std::vector<size_t> &jointIndices, Eigen::VectorXd &jointValues,
                                          const TRANSFORM &target, const TRANSFORM &finalTF = TRANSFORM::Identity());

        rk_result_t jacobianTransposeIK_chain(const std::vector<std::string>& jointNames, Eigen::VectorXd& jointValues,
                                          const TRANSFORM& target, const TRANSFORM &finalTF = TRANSFORM::Identity());

        rk_result_t jacobianTransposeIK_linkage(const std::string linkageName, Eigen::VectorXd &jointValues,
                                            const TRANSFORM& target, const TRANSFORM &finalTF = TRANSFORM::Identity());

        /////////////////

        rk_result_t dampedLeastSquaresIK_chain(const std::vector<size_t> &jointIndices, Eigen::VectorXd &jointValues,
                                          const TRANSFORM &target, const TRANSFORM &finalTF = TRANSFORM::Identity());

        rk_result_t dampedLeastSquaresIK_chain(const std::vector<std::string>& jointNames, Eigen::VectorXd& jointValues,
                                          const TRANSFORM& target, const TRANSFORM &finalTF = TRANSFORM::Identity());

        rk_result_t dampedLeastSquaresIK_linkage(const std::string linkageName, Eigen::VectorXd &jointValues,
                                            const TRANSFORM& target, const TRANSFORM &finalTF = TRANSFORM::Identity());


    protected:
        //--------------------------------------------------------------------------
        // Robot Protected Member Functions
        //--------------------------------------------------------------------------
        TRANSFORM respectToWorld_; // Coordinates with respect to robot base frame
        std::vector<Linkage*> linkages_;
        std::map<std::string, size_t> linkageNameToIndex_;
        std::vector<Joint*> joints_;
        std::map<std::string, size_t> jointNameToIndex_;
        
        
        //--------------------------------------------------------------------------
        // Robot Protected Member Variables
        //--------------------------------------------------------------------------
        virtual void initialize(std::vector<Linkage> linkageObjs, std::vector<int> parentIndices);
        
        
    private:
        //--------------------------------------------------------------------------
        // Robot Constants, Enums, and Types
        //--------------------------------------------------------------------------

        
        
        
        //--------------------------------------------------------------------------
        // Robot Private Member Variables
        //--------------------------------------------------------------------------
        bool initializing_;
        
        
        
    }; // class Robot
    
    //------------------------------------------------------------------------------
    // Postfix Increment Operators
    //------------------------------------------------------------------------------
    
} // namespace RobotKin

#endif


