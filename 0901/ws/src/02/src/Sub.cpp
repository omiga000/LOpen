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
    MySubscriber() : Node("Sub_node_image")
    {
        
        
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
    
    
    // ===== 图片回调 =====
    void image_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        try {
            // 转换为 OpenCV 格式
            cv_bridge::CvImagePtr cv_image = cv_bridge::toCvCopy(msg, "bgr8");  // ✅ 修正拼写
            Mat image = cv_image->image;  // ✅ 注意是 image 不是 data
            
        
            

            Mat hsv,mask1;

            //转换成hsv
            cvtColor(image,hsv,COLOR_BGR2HSV); 


            Scalar lower_red(170, 150, 150);    // 下限：H-9, S-55, V-55
            Scalar upper_red(180, 255, 255); 
           
            
            inRange(hsv,lower_red,upper_red,mask1);
           
            Mat kernel=getStructuringElement(MORPH_RECT,Size(15,15));
            morphologyEx(mask1,mask1,MORPH_CLOSE,kernel);

            
           // 高斯模糊
            Mat blurred;
            GaussianBlur(mask1, blurred, Size(5, 5), 0);

            // 二值化（阈值可以调高一点，比如 200）
            threshold(blurred, mask1, 200, 255, THRESH_BINARY);



            Mat edges;
            Canny(mask1, edges, 50, 150, 3);


            Mat result = Mat::zeros(image.size(), CV_8UC3);

            
            
            result.setTo(Scalar(255,255,200),edges);

            
            namedWindow("Original Image", WINDOW_NORMAL);
            resizeWindow("Original Image", 1280, 720);

            namedWindow("Binary Mask", WINDOW_NORMAL);
            resizeWindow("Binary Mask", 1280, 720);

            namedWindow("Edge Detection", WINDOW_NORMAL);
            resizeWindow("Edge Detection", 1280, 720);


            imshow("Original Image",image);
            imshow("Binary Mask",mask1);
            imshow("Edge Detection",result);


            string filename = "/home/epsilon/LOpencv/0901/ws/image/result.jpg";
            imwrite(filename,result);
            waitKey(1);
            
            RCLCPP_INFO(this->get_logger(), "Received image: %dx%d", 
                       image.cols, image.rows);
            
        } catch (const cv_bridge::Exception& e) {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge error: %s", e.what());
        } catch (const cv::Exception& e) {
            RCLCPP_ERROR(this->get_logger(), "OpenCV error: %s", e.what());
        }
    }

    
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MySubscriber>());
    rclcpp::shutdown();
    return 0;
}