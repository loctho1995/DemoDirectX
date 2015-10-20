#include <gdiplus.h>
#include <objidl.h>
#include <math.h>
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

//tinh n giao thua
 unsigned int factorial( int n)
 {
 	unsigned int retval = 1;

 	for (int i = n; i > 1; --i)
 		retval *= i;

 	return retval;
 }

 //tinh phuong trinh duong cong bezier
 //dung cong thuc tinh he so cua tam giac pascal
 unsigned int calculateCoefficient( int n,  int i)
 {
	 return factorial(n) / (factorial(i) * factorial( n - i )  );
 }

//lay diem tuong ung de ve len
 PointF calculateSinglePoint(float t, std :: vector< PointF > pointVector)
 {
	 int n = pointVector.size() - 1;
	 PointF result(0, 0);
	 for (int i = 0; i <= n; i++)
	 {
		PointF temp = pointVector[i];
		float coefficient = calculateCoefficient(n , i ) * pow(1 - t , n - i ) * pow ( t , i );
		float x =  coefficient * temp.X;
		float y =  coefficient * temp.Y;
		result = result + PointF(x, y);
	 }
	 return result;
 }


void drawPoint(Graphics* graphics, PointF point, Color c = Color :: White )
{
	SolidBrush* b = new SolidBrush(c);
	graphics -> FillRectangle(b, point.X , point.Y , 1.0f , 1.0f );
}

void drawLines(Graphics* graphics,  PointF* pPoint ,int n, Color c = Color :: White)
{
	graphics ->DrawLines(new Pen(Color::White), pPoint, n);
}

void drawBenzier(Graphics* graphics, std :: vector< PointF > pointVector, Color c = Color::Yellow)
{
	if(pointVector.size() <= 0 )
		return;
	PointF pPoint[101];

	// increase t from 0 to 100
	float t = 0;
	for (int i = 0; i <= 100; i++)
	{
		t = 0.01f * i;
		pPoint[i] = calculateSinglePoint(t, pointVector );
	}
	graphics -> DrawLines(new Pen(c), pPoint, 100 );

}