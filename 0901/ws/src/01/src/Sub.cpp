#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_msgs/msg/string.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <functional>
#include <memory>

using namespace std;
using namespace cv;  // ✅ 添加 OpenCV 命名空间

class MySubscriber : public rclcpp::Node
{
public:
    MySubscriber() : Node("Sub_node")
    {
        // 订阅文字话题
        text_sub_ = this->create_subscription<std_msgs::msg::String>(
            "topic", 10,
            std::bind(&MySubscriber::text_callback, this, std::placeholders::_1)
        );
        
        // 订阅图片话题
        image_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
            "topic_image", 10,  // ✅ 修正：添加 > 符号
            std::bind(&MySubscriber::image_callback, this, std::placeholders::_1)  // ✅ 使用不同的回调
        );
        
        // 创建显示窗口
        namedWindow("Received Image", WINDOW_NORMAL);
        
        RCLCPP_INFO(this->get_logger(), "Subscriber started!");
    }

private:
    // ===== 文字回调 =====
    void text_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Received text: '%s'", msg->data.c_str());
    }
    
    // ===== 图片回调 =====
    void image_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        try {
            // 转换为 OpenCV 格式
            cv_bridge::CvImagePtr cv_image = cv_bridge::toCvCopy(msg, "bgr8");  // ✅ 修正拼写
            Mat image = cv_image->image;  // ✅ 注意是 image 不是 data
            
            // 显示图片
            imshow("Received Image", image);
            waitKey(1);
            
            RCLCPP_INFO(this->get_logger(), "Received image: %dx%d", 
                       image.cols, image.rows);
            
        } catch (const cv_bridge::Exception& e) {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge error: %s", e.what());
        } catch (const cv::Exception& e) {
            RCLCPP_ERROR(this->get_logger(), "OpenCV error: %s", e.what());
        }
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr text_sub_;  // ✅ 重命名
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MySubscriber>());
    rclcpp::shutdown();
    return 0;
}