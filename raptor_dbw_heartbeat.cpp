#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "raptor_dbw_msgs/msg/global_enable_cmd.hpp"
#include "raptor_dbw_msgs/msg/steering_cmd.hpp"
#include "raptor_dbw_msgs/msg/brake_cmd.hpp"
#include "raptor_dbw_msgs/msg/accelerator_pedal_cmd.hpp"
#include "raptor_dbw_msgs/msg/gear_cmd.hpp"
#include "raptor_dbw_msgs/msg/misc_cmd.hpp"
#include "raptor_dbw_msgs/msg/turn_signal.hpp"

using namespace std::chrono_literals;

class DbwHeartbeatNode : public rclcpp::Node
{
public:
  DbwHeartbeatNode()
  : Node("dbw_heartbeat_node"), counter_(0)
  {
    pub_accelerator_pedal_ = this->create_publisher<raptor_dbw_msgs::msg::AcceleratorPedalCmd>("/raptor_dbw_interface/accelerator_pedal_cmd", 1);
    pub_brake_ = this->create_publisher<raptor_dbw_msgs::msg::BrakeCmd>("/raptor_dbw_interface/brake_cmd", 1);
    pub_misc_ = this->create_publisher<raptor_dbw_msgs::msg::MiscCmd>("/raptor_dbw_interface/misc_cmd", 1);
    pub_steering_ = this->create_publisher<raptor_dbw_msgs::msg::SteeringCmd>("/raptor_dbw_interface/steering_cmd", 1);
    pub_global_enable_ = this->create_publisher<raptor_dbw_msgs::msg::GlobalEnableCmd>("/raptor_dbw_interface/global_enable_cmd", 1);
    pub_gear_ = this->create_publisher<raptor_dbw_msgs::msg::GearCmd>("/raptor_dbw_interface/gear_cmd", 1);

    timer_ = this->create_wall_timer(10ms, std::bind(&DbwHeartbeatNode::cmdCallback, this));
  }

private:
  void cmdCallback()
  {
    counter_ = (counter_ + 1) % 16;

    // Accelerator Pedal
    raptor_dbw_msgs::msg::AcceleratorPedalCmd accelerator_pedal_msg;
    accelerator_pedal_msg.enable = false;
    accelerator_pedal_msg.ignore = false;
    accelerator_pedal_msg.rolling_counter = counter_;
    accelerator_pedal_msg.pedal_cmd = 0.0;
    accelerator_pedal_msg.control_type.value = raptor_dbw_msgs::msg::ActuatorControlMode::OPEN_LOOP;
    pub_accelerator_pedal_->publish(accelerator_pedal_msg);

    // Brake
    raptor_dbw_msgs::msg::BrakeCmd brake_msg;
    brake_msg.enable = false;
    brake_msg.rolling_counter = counter_;
    brake_msg.pedal_cmd = 0.0;
    brake_msg.control_type.value = raptor_dbw_msgs::msg::ActuatorControlMode::OPEN_LOOP;
    pub_brake_->publish(brake_msg);

    // Steering
    raptor_dbw_msgs::msg::SteeringCmd steering_msg;
    steering_msg.enable = false;
    steering_msg.ignore = false;
    steering_msg.rolling_counter = counter_;
    steering_msg.angle_cmd = 0.0;
    steering_msg.angle_velocity = 0.0;
    steering_msg.control_type.value = raptor_dbw_msgs::msg::ActuatorControlMode::CLOSED_LOOP_ACTUATOR;
    pub_steering_->publish(steering_msg);

    // Gear
    raptor_dbw_msgs::msg::GearCmd gear_msg;
    gear_msg.cmd.gear = raptor_dbw_msgs::msg::Gear::NEUTRAL;
    gear_msg.enable = false;
    gear_msg.rolling_counter = counter_;
    pub_gear_->publish(gear_msg);

    // Turn signal (Misc)
    raptor_dbw_msgs::msg::MiscCmd misc_msg;
    misc_msg.cmd.value = raptor_dbw_msgs::msg::TurnSignal::NONE;
    misc_msg.rolling_counter = counter_;
    pub_misc_->publish(misc_msg);

    // Global Enable
    raptor_dbw_msgs::msg::GlobalEnableCmd globalEnable_msg;
    globalEnable_msg.global_enable = false;
    globalEnable_msg.enable_joystick_limits = false;
    globalEnable_msg.rolling_counter = counter_;
    pub_global_enable_->publish(globalEnable_msg);
  }

  // Publishers
  rclcpp::Publisher<raptor_dbw_msgs::msg::AcceleratorPedalCmd>::SharedPtr pub_accelerator_pedal_;
  rclcpp::Publisher<raptor_dbw_msgs::msg::BrakeCmd>::SharedPtr pub_brake_;
  rclcpp::Publisher<raptor_dbw_msgs::msg::MiscCmd>::SharedPtr pub_misc_;
  rclcpp::Publisher<raptor_dbw_msgs::msg::SteeringCmd>::SharedPtr pub_steering_;
  rclcpp::Publisher<raptor_dbw_msgs::msg::GlobalEnableCmd>::SharedPtr pub_global_enable_;
  rclcpp::Publisher<raptor_dbw_msgs::msg::GearCmd>::SharedPtr pub_gear_;
  rclcpp::TimerBase::SharedPtr timer_;
  int counter_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<DbwHeartbeatNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}