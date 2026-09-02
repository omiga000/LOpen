#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

Mat image, img_display;
bool drawing = false;
bool selected = false;
Point start_point, end_point;

void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        drawing = true;
        selected = false;
        start_point = Point(x, y);
        end_point = Point(x, y);
        img_display = image.clone();
        cout << "📍 开始框选: (" << x << ", " << y << ")" << endl;
    }
    else if (event == EVENT_MOUSEMOVE && drawing) {
        end_point = Point(x, y);
        img_display = image.clone();

        int x1 = min(start_point.x, end_point.x);
        int y1 = min(start_point.y, end_point.y);
        int x2 = max(start_point.x, end_point.x);
        int y2 = max(start_point.y, end_point.y);

        rectangle(img_display, Point(x1, y1), Point(x2, y2), Scalar(255, 255, 200), 2);
        Vec3b pixel = image.at<Vec3b>(y, x);
        int b = pixel[0];
        int g = pixel[1];
        int r = pixel[2];

        string info = "(" + to_string(x) + ", " + to_string(y) + ")";
        string rgb_info = "RGB: (" + to_string(r) + ", " + to_string(g) + ", " + to_string(b) + ")";
        putText(img_display, info, Point(x + 10, y - 10),
                FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 2);

        putText(img_display, rgb_info, Point(x + 50, y + 50),
                FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 2);

        imshow("Image", img_display);
    }
    else if (event == EVENT_LBUTTONUP) {
        drawing = false;
        selected = true;
        end_point = Point(x, y);

        img_display = image.clone();

        int x1 = min(start_point.x, end_point.x);
        int y1 = min(start_point.y, end_point.y);
        int x2 = max(start_point.x, end_point.x);
        int y2 = max(start_point.y, end_point.y);

        rectangle(img_display, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 2);

        Rect select_rect(x1, y1, x2 - x1, y2 - y1);

        int center_x = (x1 + x2) / 2;
        int center_y = (y1 + y2) / 2;

        cout << "🎯 框中心点坐标: (" << center_x << ", " << center_y << ")" << endl;

        if (select_rect.width > 0 && select_rect.height > 0) {
            Mat cropped = image(select_rect);

            if (!cropped.empty()) {
                namedWindow("Selected Region", WINDOW_NORMAL);
                resizeWindow("Selected Region", 400, 400);
                imshow("Selected Region", cropped);
                cout << "📐 选中区域大小: " << cropped.cols << " x " << cropped.rows << endl;

                string save_path = "selected_region.jpg";
                imwrite(save_path, cropped);
                cout << "💾 已保存: " << save_path << endl;
            }
        } else {
            cout << "⚠️ 选区无效，请重新框选" << endl;
        }

        string center_info = "Center: (" + to_string(center_x) + ", " + to_string(center_y) + ")";
        putText(img_display, center_info, Point(center_x + 15, center_y - 10),
                FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 2);

        imshow("Image", img_display);
    }
}

int main() {
    string image_path = "/home/epsilon/LOpencv/Opencv-4/image/Cat.png";

    image = imread(image_path);

    if (image.empty()) {
        cout << "❌ 无法读取图片！请检查路径：" << image_path << endl;
        return -1;
    }
    cout << "✅ 图片加载成功！尺寸：" << image.cols << " x " << image.rows << endl;

    img_display = image.clone();

    namedWindow("Image", WINDOW_NORMAL);
    resizeWindow("Image", 800, 600);
    setMouseCallback("Image", mouseCallback);

    imshow("Image", img_display);

    cout << "========================================" << endl;
    cout << "🐭 操作说明:" << endl;
    cout << "  1. 按下鼠标左键开始框选" << endl;
    cout << "  2. 拖动鼠标调整选区" << endl;
    cout << "  3. 松开鼠标完成框选" << endl;
    cout << "  4. 按 'r' 键重置" << endl;
    cout << "  5. 按 'q' 键退出" << endl;
    cout << "========================================" << endl;

    while (true) {
        char key = (char)waitKey(1);

        if (key == 'r' || key == 'R') {
            drawing = false;
            selected = false;
            img_display = image.clone();
            imshow("Image", img_display);
            destroyWindow("Selected Region");
            cout << "🔄 已重置" << endl;
        }

        if (key == 'q' || key == 'Q') {
            break;
        }
    }

    destroyAllWindows();
    return 0;
}