#include <iostream>
#include <opencv2/opencv.hpp>
#include<cmath>
using namespace std;
using namespace cv;

typedef struct  {
    int id;
    int color;
    int width;
    int height;
    Point position;
}ArmorRect;
class Armor 
{

 private:
  ArmorRect rect;   
 public:


    Armor():rect(ArmorRect{0,0,0,0,Point(0,0)}){};

     Armor(ArmorRect rect)
    : rect(rect){
        cout<<"Armor created with id: " << rect.id << endl;
    };

  Point Central_Point()
  {
        return Point(rect.position.x + rect.width / 2, rect.position.y + rect.height / 2);
  } 

  double  Diagonal()
  {
    //保留两位小数   
    double result = sqrt(rect.width * rect.width + rect.height * rect.height);
    return round(result * 100) / 100;
  }
  void Armor_Point()
  {
    //左上
    cout<<rect.position.x<< ' '<<rect.position.y<<endl;
    //右上
    cout<<rect.position.x + rect.width<< ' '<<rect.position.y<<endl;
    //左下
    cout<<rect.position.x<< ' '<<rect.position.y + rect.height<<endl;
    //右下
    cout<<rect.position.x + rect.width<< ' '<<rect.position.y + rect.height<<endl;
  }
  int Armor_Color()
  {
    return rect.color;
  }
  int Armor_Id()
  {
    return rect.id;
  }

};

int main()
{
    ArmorRect rect;
    int id, color, width, height, x, y;
    cin>> id >> color >> width >> height >> x >> y;
    rect.id = id;
    rect.color = color;
    rect.width = width;
    rect.height = height;
    rect.position = Point(x, y);


    Armor armor(rect);
    
    Point p=armor.Central_Point();
    double len=armor.Diagonal();
    cout<<"Central Point: ("<<p.x<<", "<<p.y<<")"<<endl;
    cout<<"Diagonal Length: "<<fixed<<setprecision(2)<< len<<endl;
    cout<<"Armor Points: "<<endl;
    armor.Armor_Point();
    cout<<"Armor Color: "<<armor.Armor_Color()<<endl;
    
}
