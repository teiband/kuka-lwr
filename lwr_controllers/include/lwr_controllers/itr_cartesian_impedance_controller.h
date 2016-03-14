#ifndef LWR_CONTROLLERS__CARTESIAN_IMPEDANCE_CONTROLLER_H
#define LWR_CONTROLLERS__CARTESIAN_IMPEDANCE_CONTROLLER_H

// ROS added
#include <geometry_msgs/WrenchStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <lwr_controllers/Stiffness.h>
#include <std_msgs/Float64MultiArray.h>

// KDL added
#include <kdl/stiffness.hpp>
#include <kdl/trajectory.hpp>
#include <kdl/frames.hpp>

// BOOST added
#include <boost/scoped_ptr.hpp>

// Base class with useful URDF parsing and kdl chain generator
#include "lwr_controllers/KinematicChainControllerBase.h"

// The format of the command specification
#include "lwr_controllers/SetCartesianImpedanceCommand.h"

// to read KDL chain for the world robot transform
#include "KinematicChainControllerBase.h"

namespace lwr_controllers
{
    //class ITRCartesianImpedanceController: public controller_interface::Controller<hardware_interface::PositionCartesianInterface>
    class ITRCartesianImpedanceController: public controller_interface::KinematicChainControllerBase<hardware_interface::PositionCartesianInterface>
	{
	public:
        ITRCartesianImpedanceController();
        ~ITRCartesianImpedanceController();

        bool init(hardware_interface::PositionCartesianInterface *robot, ros::NodeHandle &n);
		void starting(const ros::Time& time);
		void stopping(const ros::Time& time);
		void update(const ros::Time& time, const ros::Duration& period);
        //void command(const geometry_msgs::PoseConstPtr &msg);
        void pose(const geometry_msgs::PoseConstPtr &msg);
        //void command(const lwr_controllers::CartesianImpedancePoint::ConstPtr &msg);
        bool command_cb(lwr_controllers::SetCartesianImpedanceCommand::Request &req, lwr_controllers::SetCartesianImpedanceCommand::Response &res);
        //void updateFT(const geometry_msgs::WrenchStamped::ConstPtr &msg);
        void additionalFT(const geometry_msgs::WrenchStamped::ConstPtr &msg);
        void gains(const std_msgs::Float64MultiArrayConstPtr &msg);

        // experimental set a world pose, which is then transformed into the robot KS
        void pose_world(const geometry_msgs::PoseConstPtr &msg);

    protected:

        std::string robot_namespace_;
        std::vector<std::string> joint_names_, cart_12_names_, cart_6_names_;
        std::vector<hardware_interface::JointHandle> joint_handles_, cart_handles_;
		
        // ROS API (topic, service and dynamic reconfigure)
        ros::Subscriber sub_pose_;
        ros::Subscriber sub_pose_world_;
        ros::Subscriber sub_gains_;
        ros::Subscriber sub_addFT_;

        ros::ServiceServer srv_command_;
        ros::Subscriber sub_ft_measures_;
        ros::Publisher pub_goal_;
        ros::Publisher pub_msr_pos_;

		// Cartesian vars
        KDL::Frame x_ref_;
        KDL::Frame x_des_;
        KDL::Frame x_cur_;
        KDL::Frame x_FRI_;
		KDL::Twist x_err_;

        // transform of the robot mounting position
        KDL::Frame world2robot_;
        // world coordinates of received command
        KDL::Frame x_world_;

		// The jacobian at q_msg
        // KDL::Jacobian J_;

        // Measured external force
        KDL::Wrench f_cur_;
        KDL::Wrench f_des_;
        // KDL::Wrench f_error_;

		// That is, desired kx, ky, kz, krx, kry, krz, they need to be expressed in the same ref as x, and J
		KDL::Stiffness k_des_;
        KDL::Stiffness d_des_;

		// This class does not exist, should we ask for it? 0, for now.
		// KDL::Damping d_des_; 

		// Solver to compute x_cur and x_des
        // boost::scoped_ptr<KDL::ChainFkSolverPos_recursive> fk_pos_solver_;

		// Solver to compute the Jacobian at q_msr
        // boost::scoped_ptr<KDL::ChainJntToJacSolver> jnt_to_jac_solver_;

		// Computed torque
        // KDL::JntArray tau_cmd_;

		// Because of the lack of the Jacobian transpose in KDL
		void multiplyJacobian(const KDL::Jacobian& jac, const KDL::Wrench& src, KDL::JntArray& dest);

        // FRI<->KDL conversion
        void fromKDLtoFRI(const KDL::Frame& in, std::vector<double>& out);
        void fromKDLtoFRI(const KDL::Stiffness& in, std::vector<double>& out);
        void fromKDLtoFRI(const KDL::Wrench& in, std::vector<double>& out);
        void fromFRItoKDL(const std::vector<double>& in, KDL::Frame& out);
        void fromFRItoKDL(const std::vector<double>& in, KDL::Stiffness& out);
        void fromFRItoKDL(const std::vector<double>& in, KDL::Wrench& out);
	};

}

#endif