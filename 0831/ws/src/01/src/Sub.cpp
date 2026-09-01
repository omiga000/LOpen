#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include <functional>

using namespace std;
using namespace cv;

class ImageSubNode : public rclcpp::Node
{
public:
    ImageSubNode() : Node("ImageSubNode")
    {
        // ✅ 1. 声明参数
        this->declare_parameter<std::string>("window_name", "Received Image");
        this->declare_parameter<bool>("show_image", true);
        this->declare_parameter<int>("wait_key_delay", 1);

        // ✅ 2. 获取参数
        this->get_parameter("window_name", window_name_);
        this->get_parameter("show_image", show_image_);
        this->get_parameter("wait_key_delay", wait_key_delay_);

        // ✅ 3. 创建订阅
        sub_ = this->create_subscription<sensor_msgs::msg::Image>(
            "/ImageTopic",
            10,
            std::bind(&ImageSubNode::call_back, this, std::placeholders::_1)
        );

        RCLCPP_INFO(this->get_logger(), "Image subscriber started");
        RCLCPP_INFO(this->get_logger(), "Window name: %s, Show image: %s", 
                    window_name_.c_str(), show_image_ ? "true" : "false");
        RCLCPP_INFO(this->get_logger(), "Use 'ros2 param set /ImageSubNode <param> <value>' to modify");

        // ✅ 4. 添加参数回调（支持动态修改）
        param_callback_handle_ = this->add_on_set_parameters_callback(
            std::bind(&ImageSubNode::on_parameter_change, this, std::placeholders::_1)
        );
    }

    ~ImageSubNode()
    {
        cv::destroyAllWindows();
    }

private:
    // 成员变量
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
    std::string window_name_;
    bool show_image_;
    int wait_key_delay_;

    rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr param_callback_handle_;

    // ✅ 5. 回调函数
    void call_back(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        try {
            cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
            Mat image = cv_ptr->image;

            if (show_image_) {
                cv::imshow(window_name_, image);
                cv::waitKey(wait_key_delay_);
                RCLCPP_DEBUG(this->get_logger(), "Displayed image: %dx%d", 
                            image.cols, image.rows);
            }

            RCLCPP_INFO(this->get_logger(), "Received image: %dx%d", 
                        image.cols, image.rows);
        }
        catch (const cv_bridge::Exception& e) {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge error: %s", e.what());
        }
    }

    // ✅ 6. 参数变化回调
    rcl_interfaces::msg::SetParametersResult on_parameter_change(
        const std::vector<rclcpp::Parameter>& params)
    {
        rcl_interfaces::msg::SetParametersResult result;
        result.successful = true;

        for (const auto& param : params) {
            // 处理窗口名称参数
            if (param.get_name() == "window_name") {
                if (param.get_type() == rclcpp::ParameterType::PARAMETER_STRING) {
                    std::string new_name = param.as_string();
                    RCLCPP_INFO(this->get_logger(), "🔄 Changing window name to: %s", 
                                new_name.c_str());
                    
                    // 关闭旧窗口，创建新窗口
                    if (show_image_) {
                        cv::destroyWindow(window_name_);
                        window_name_ = new_name;
                        cv::namedWindow(window_name_);
                        RCLCPP_INFO(this->get_logger(), "✅ Window name updated");
                    } else {
                        window_name_ = new_name;
                        RCLCPP_INFO(this->get_logger(), "✅ Window name stored");
                    }
                }
            }
            // 处理显示开关参数
            else if (param.get_name() == "show_image") {
                if (param.get_type() == rclcpp::ParameterType::PARAMETER_BOOL) {
                    bool new_show = param.as_bool();
                    RCLCPP_INFO(this->get_logger(), "🔄 Show image changing to: %s", 
                                new_show ? "true" : "false");
                    
                    show_image_ = new_show;
                    if (!show_image_) {
                        cv::destroyWindow(window_name_);
                        RCLCPP_INFO(this->get_logger(), "✅ Window closed");
                    } else {
                        cv::namedWindow(window_name_);
                        RCLCPP_INFO(this->get_logger(), "✅ Window opened");
                    }
                }
            }
            // 处理按键延迟参数
            else if (param.get_name() == "wait_key_delay") {
                if (param.get_type() == rclcpp::ParameterType::PARAMETER_INTEGER) {
                    int new_delay = param.as_int();
                    if (new_delay < 0) {
                        RCLCPP_ERROR(this->get_logger(), "❌ Delay must be >= 0");
                        result.successful = false;
                        result.reason = "Delay must be >= 0";
                    } else {
                        wait_key_delay_ = new_delay;
                        RCLCPP_INFO(this->get_logger(), "🔄 Wait key delay changed to: %d ms", 
                                    wait_key_delay_);
                        RCLCPP_INFO(this->get_logger(), "✅ Delay updated");
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
    auto node = std::make_shared<ImageSubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}