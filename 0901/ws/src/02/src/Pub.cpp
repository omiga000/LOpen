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
    MyPublisher() : Node("Pub_node_image")
    {
        
        this->declare_parameter<string>("image_path", "/home/epsilon/LOpencv/0901/ws/image.png");
        
        
        this->get_parameter("image_path", image_path);

       
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

        
        this->get_parameter("image_path",image_path);

       


        try
        {
            Mat image=imread(image_path);
            
            if (image.empty()) 
            {
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

    
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_image_;
    rclcpp::TimerBase::SharedPtr timer_;
    
    string image_path;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MyPublisher>());
    rclcpp::shutdown();
    return 0;
}