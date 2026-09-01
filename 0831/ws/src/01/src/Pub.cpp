#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include <functional>
#include <cstdio>      // ✅ 添加
#include <iostream>    // ✅ 添加（可选）

using namespace std;
using namespace cv;

class ImageNode : public rclcpp::Node
{
public:
    ImageNode() : Node("ImageNode")
    {
        // 创建发布器
        publisher_ = this->create_publisher<sensor_msgs::msg::Image>("/ImageTopic", 10);

        // ✅ 1. 声明参数
        this->declare_parameter<std::string>("image_path", 
            "/home/epsilon/LOpencv/0831/ws/Draw.jpg");
        this->declare_parameter<double>("frequency", 2.0);

        // ✅ 2. 获取参数
        this->get_parameter("image_path", image_path_);   // ← 用 image_path_
        this->get_parameter("frequency", frequency_);      // ← 用 frequency_

        // ✅ 3. 加载图片
        image_ = imread(image_path_);  // ← 用 image_path_
        if (image_.empty()) {
            // ✅ 使用 image_path_（一致）
            RCLCPP_ERROR(this->get_logger(), "Failed to load image from: %s", 
                        image_path_.c_str());
            image_loaded_ = false;
        } else {
            image_loaded_ = true;
            // ✅ 使用 image_path_（一致）
            RCLCPP_INFO(this->get_logger(), "Image loaded: %dx%d from %s", 
                        image_.cols, image_.rows, image_path_.c_str());
        }

        // ✅ 4. 创建定时器
        create_timer();

        RCLCPP_INFO(this->get_logger(), "Image publisher started at %.1f Hz", frequency_);
        RCLCPP_INFO(this->get_logger(), "Use 'ros2 param set /ImageNode <param> <value>' to modify");

        // ✅ 5. 添加参数回调
        param_callback_handle_ = this->add_on_set_parameters_callback(
            std::bind(&ImageNode::on_parameter_change, this, std::placeholders::_1)
        );
    }

private:
    // ✅ 成员变量（统一使用下划线命名）
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    cv::Mat image_;
    bool image_loaded_ = false;

    std::string image_path_;    // ← 统一用 image_path_
    double frequency_;          // ← 统一用 frequency_

    rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr param_callback_handle_;

    void create_timer()
    {
        auto period_ms = std::chrono::milliseconds(
            static_cast<int>(1000.0 / frequency_)  // ← 用 frequency_
        );

        timer_ = this->create_wall_timer(
            period_ms,
            std::bind(&ImageNode::publish_image, this)
        );
        RCLCPP_INFO(this->get_logger(), "Timer created at %.1f Hz", frequency_);
    }

    void publish_image()
    {
        if (!image_loaded_ || image_.empty()) {
            return;
        }

        cv_bridge::CvImage cv_image;
        cv_image.header.stamp = this->now();
        cv_image.header.frame_id = "camera";
        cv_image.encoding = "bgr8";
        cv_image.image = image_;

        sensor_msgs::msg::Image ros_image;
        cv_image.toImageMsg(ros_image);
        publisher_->publish(ros_image);
    }

    // ✅ 参数回调
    rcl_interfaces::msg::SetParametersResult on_parameter_change(
        const std::vector<rclcpp::Parameter>& params)
    {
        rcl_interfaces::msg::SetParametersResult result;
        result.successful = true;

        for (const auto& param : params) {
            if (param.get_name() == "image_path") {
                if (param.get_type() == rclcpp::ParameterType::PARAMETER_STRING) {
                    std::string new_path = param.as_string();
                    RCLCPP_INFO(this->get_logger(), "🔄 Changing image path to: %s", 
                                new_path.c_str());

                    cv::Mat new_image = cv::imread(new_path);
                    if (new_image.empty()) {
                        RCLCPP_ERROR(this->get_logger(), "❌ Failed to load image from: %s", 
                                    new_path.c_str());
                        result.successful = false;
                        result.reason = "Failed to load image from new path";
                    } else {
                        image_ = new_image;
                        image_loaded_ = true;
                        image_path_ = new_path;
                        RCLCPP_INFO(this->get_logger(), "✅ Image reloaded: %dx%d", 
                                    image_.cols, image_.rows);
                    }
                }
            }
            else if (param.get_name() == "frequency") {
                if (param.get_type() == rclcpp::ParameterType::PARAMETER_DOUBLE) {
                    double new_freq = param.as_double();
                    if (new_freq <= 0) {
                        RCLCPP_ERROR(this->get_logger(), "❌ Frequency must be positive");
                        result.successful = false;
                        result.reason = "Frequency must be greater than 0";
                    } else {
                        frequency_ = new_freq;
                        RCLCPP_INFO(this->get_logger(), "🔄 Changing frequency to: %.1f Hz", 
                                    frequency_);
                        timer_.reset();
                        create_timer();
                        RCLCPP_INFO(this->get_logger(), "✅ Timer updated to %.1f Hz", 
                                    frequency_);
                    }
                }
            }
        }

        return result;
    }
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ImageNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}