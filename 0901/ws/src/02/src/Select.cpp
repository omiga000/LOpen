#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace cv;
using namespace std;

Mat img, hsv_img;
ofstream color_file;

// 鼠标回调函数：点击打印并保存颜色值
void onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        // 获取 BGR 值
        Vec3b bgr = img.at<Vec3b>(y, x);
        // 获取 HSV 值
        Vec3b hsv = hsv_img.at<Vec3b>(y, x);
        
        // 1. 在控制台打印
        cout << "========================================" << endl;
        cout << "位置: (" << x << ", " << y << ")" << endl;
        cout << "BGR: B=" << (int)bgr[0] << ", G=" << (int)bgr[1] << ", R=" << (int)bgr[2] << endl;
        cout << "HSV: H=" << (int)hsv[0] << ", S=" << (int)hsv[1] << ", V=" << (int)hsv[2] << endl;
        cout << "已保存到 color_values.txt" << endl;
        cout << "========================================" << endl;
        
        // 2. 保存到文本文件（追加模式）
        if (color_file.is_open()) {
            // 获取当前时间
            time_t now = time(0);
            char* dt = ctime(&now);
            dt[strlen(dt) - 1] = '\0';  // 去掉换行符
            
            color_file << "========================================" << endl;
            color_file << "时间: " << dt << endl;
            color_file << "位置: (" << x << ", " << y << ")" << endl;
            color_file << "BGR: B=" << (int)bgr[0] << ", G=" << (int)bgr[1] << ", R=" << (int)bgr[2] << endl;
            color_file << "HSV: H=" << (int)hsv[0] << ", S=" << (int)hsv[1] << ", V=" << (int)hsv[2] << endl;
            color_file << "----------------------------------------" << endl;
            color_file.flush();  // 立即写入文件
        }
        
        // 3. 在图片上标记点击位置
        Mat display = img.clone();
        circle(display, Point(x, y), 5, Scalar(0, 255, 0), -1);
        circle(display, Point(x, y), 10, Scalar(0, 255, 0), 2);
        string text = "(" + to_string(x) + ", " + to_string(y) + ")";
        putText(display, text, Point(x + 15, y - 10), 
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
        imshow("Image", display);
    }
}

int main() {
    // 1. 打开文件（追加模式）
    color_file.open("color_values.txt", ios::app);
    if (!color_file.is_open()) {
        cout << "无法创建文件！" << endl;
        return -1;
    }
    
    // 2. 读取图片
    cout << "请输入图片路径: ";
    string path;
    cin >> path;
    
    img = imread(path);
    if (img.empty()) {
        cout << "无法加载图片！" << endl;
        color_file.close();
        return -1;
    }
    
    // 3. 转换为 HSV
    cvtColor(img, hsv_img, COLOR_BGR2HSV);
    
    // 4. 显示图片并设置鼠标回调
    namedWindow("Image", WINDOW_NORMAL);
    setMouseCallback("Image", onMouse);
    imshow("Image", img);
    
    cout << "========================================" << endl;
    cout << "点击图片上的任意位置取色" << endl;
    cout << "颜色值将保存到 color_values.txt" << endl;
    cout << "按 'q' 或 'Q' 退出" << endl;
    cout << "========================================" << endl;
    
    // 5. 等待退出
    while (true) {
        char key = (char)waitKey(0);
        if (key == 'q' || key == 'Q') break;
    }
    
    // 6. 关闭文件
    color_file.close();
    destroyAllWindows();
    cout << "程序退出，颜色数据已保存到 color_values.txt" << endl;
    
    return 0;
}