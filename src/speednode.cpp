#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include <mutex>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

#define wheelradius 0.056f

class SpeedNode : public rclcpp::Node {
public:
  SpeedNode() : Node("speednode"), publishing_(true) {
    motor_vel_pub_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/n10/motor_vel", 10);
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(9), 
      std::bind(&SpeedNode::Publish, this)
    );
    get_msgs_thread_ = std::thread(&SpeedNode::get_msgs, this);
  }

  ~SpeedNode() {
    if (get_msgs_thread_.joinable()) {
      get_msgs_thread_.join();
    }
  }

private:
  void Publish() {
    std::lock_guard<std::mutex> lock(mutex_);
    auto msg = std_msgs::msg::Float32MultiArray();
    msg.data.resize(6);
    msg.data[wheel_] = 60 * speed_ / (2 * wheelradius * M_PI);
    motor_vel_pub_->publish(msg);
  }

  void get_msgs() {
    while (rclcpp::ok()) {
      int wheel;
      float speed;
      std::cout << "wheel: ";
      std::cin >> wheel;
      std::cout << "speed: ";
      std::cin >> speed;
      wheel_ = wheel;
      speed_ = speed;

      std::cout << "Publishing new values..." << std::endl;
    }
  }

  rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr motor_vel_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::thread get_msgs_thread_;
  std::mutex mutex_;
  bool publishing_;
  int wheel_ = 0;
  float speed_ = 0;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SpeedNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}

