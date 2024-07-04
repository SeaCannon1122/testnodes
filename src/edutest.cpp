#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include <mutex>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

class EduTest : public rclcpp::Node {
public:
  EduTest() : Node("edutest"), publishing_(true) {
    motor_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("vel/teleop", 10);
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(10), 
      std::bind(&EduTest::Publish, this)
    );
    get_msgs_thread_ = std::thread(&EduTest::get_msgs, this);
  }

  ~EduTest() {
    if (get_msgs_thread_.joinable()) {
      get_msgs_thread_.join();
    }
  }

private:
  void Publish() {
    std::lock_guard<std::mutex> lock(mutex_);
    auto msg = geometry_msgs::msg::Twist();
    msg.linear.x = linearx_;
    msg.linear.y = lineary_;
    msg.angular.z = angular_;
    motor_vel_pub_->publish(msg);
  }

  void get_msgs() {
    while (rclcpp::ok()) {
      std::cout << "Enter values to publish (linear x, linear y, angular z):" << std::endl;
      float lx;
      float ly;
      float a;
      std::cout << "linear x: ";
      std::cin >> lx;
      std::cout << "linear y: ";
      std::cin >> ly;
      std::cout << "angular z: ";
      std::cin >> a;

      linearx_ = lx;
      lineary_ = ly;
      angular_ = a;

      std::cout << "Publishing new values..." << std::endl;
    }
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr motor_vel_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::thread get_msgs_thread_;
  std::mutex mutex_;
  bool publishing_;
  float linearx_ = 0.0;
  float lineary_ = 0.0;
  float angular_ = 0.0;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<EduTest>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
