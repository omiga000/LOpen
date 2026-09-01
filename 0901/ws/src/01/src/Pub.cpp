#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>   // ROS图片消息类型
#include <cv_bridge/cv_bridge.h>       // ROS ↔ OpenCV 转换桥
#include <opencv2/opencv.hpp>          // OpenCV图像处理
#include <opencv2/imgcodecs.hpp>        
#include <std_msgs/msg/string.hpp>
#include <string>
#include <memory>
#include <functional>

using namespace std;
using namespace cv;
class MyPublisher : public rclcpp::Node
{
public:
    MyPublisher() : Node("Pub_node")
    {
        // 声明参数
        this->declare_parameter<string>("message", "hello");
        this->declare_parameter<int>("count", 0);
        this->declare_parameter<string>("image_path", "/home/epsilon/LOpencv/0901/ws/Draw.jpg");
        
        // 读取参数到成员变量
        this->get_parameter("message", message_);
        this->get_parameter("count", count_);
        this->get_parameter("image_path", image_path);

        // 创建发布者（需要队列长度参数）
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
        publisher_image_=this->create_publisher<sensor_msgs::msg::Image>("topic_image",10);
        
        // 创建定时器
        timer_ = this->create_wall_timer(
            500ms, 
            std::bind(&MyPublisher::timer_callback, this)
        );
    }

private:
    void timer_callback()
    {

        this->get_parameter("message", message_);
        this->get_parameter("count", count_);
        this->get_parameter("image_path",image_path);

        auto Message = std_msgs::msg::String();
        Message.data = message_ + std::to_string(count_);
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", Message.data.c_str());
        publisher_->publish(Message);


        try
        {
            Mat image=imread(image_path);
            

            if (image.empty()) {
    RCLCPP_ERROR(this->get_logger(), "Failed to load image");
    return;
}
        cv_bridge::CvImage cv_image;
        cv_image.header.stamp = this->now();
        cv_image.header.frame_id = "camera";
        cv_image.encoding = "bgr8";
        cv_image.image = image;

        sensor_msgs::msg::Image ros_image;
        cv_image.toImageMsg(ros_image);
        publisher_image_->publish(ros_image);
        }
        catch (const cv::Exception& e) {
            RCLCPP_ERROR(this->get_logger(), "OpenCV error: %s", e.what());
        } catch (const std::exception& e) {
            RCLCPP_ERROR(this->get_logger(), "Error: %s", e.what());
        }
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_image_;
    rclcpp::TimerBase::SharedPtr timer_;
    string message_;
    int count_;
    string image_path;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MyPublisher>());
    rclcpp::shutdown();
    return 0;
}