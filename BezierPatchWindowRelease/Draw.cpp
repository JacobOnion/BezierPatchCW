#include "string.h"
#include"iostream"
#include "Draw.hpp"
using namespace std;

void TestFunc() {
    cout << "\nhello world\n";

    printf("\nis this working\n");
}


Matrix4 MakePerspective(float camOffset)
{
    Matrix4 m;
    m[0][0] = 1.f;
    m[1][1] = 1.f;
    m[2][2] = 1.f;
    m[3][2] = 1.f;
    m[3][3] = camOffset;
    return m;
}

Matrix4 MakeOrtho(double left, double right, double bottom, double top, double near_val, double far_val)
{
    Matrix4 m;
    m[0][0] = 2 / (right - left);
    m[1][1] = 2 / (top - bottom);
    m[2][2] = -2 / (far_val - near_val);
    m[0][3] = (right + left) / (right - left);
    m[1][3] = (top + bottom) / (top - bottom);
    m[2][3] = (far_val + near_val) / (far_val - near_val);
    return m;
}

Matrix4 MakeFrustum(double left, double right, double bottom, double top, double near_val, double far_val)
{
    Matrix4 m;
    m[0][0] = (2 * near_val) / (right - left);
    m[1][1] = (2 * near_val) / (top - bottom);
    m[2][2] = - (far_val + near_val) / (far_val - near_val);
    m[3][3] = 0.f;
    m[0][2] = (right + left) / (right - left);
    m[1][2] = (top + bottom) / (top - bottom);
    m[2][3] = - (2 * far_val * near_val) / (far_val - near_val);
    m[3][2] = -1.f;
    return m;

}


void ProjectPoint(Point3& point, float camOffset)
{
    
    point.x /= (camOffset + point.z);
    point.y /= (camOffset + point.z);
    
}

Point3 GetProjectedPoint(Point3& point, float camOffset)
{
    Point3 returnPoint;
    returnPoint.x = point.x / (camOffset + point.z);
    returnPoint.y = point.y / (camOffset + point.z);
    return returnPoint;
}


void DrawLine(RGBAImage& frameBuffer, const Point3& p0, const Point3& p1, RGBAValue& colour)
{
    // Convert coords from NDC to pixel space
    float x0 = (p0.x + 1) / 2;
    float x1 = (p1.x + 1) / 2;

    float y0 = (p0.y + 1) / 2;
    float y1 = (p1.y + 1) / 2;

    x0 *= frameBuffer.width - 1;
    y0 *= frameBuffer.height - 1;
    x1 *= frameBuffer.width - 1;
    y1 *= frameBuffer.height - 1;


    int dx = x1 - x0;
    int dy = y1 - y0;

    if (dx == 0 && dy == 0)
    {
        int x = round(x0);
        int y = round(y0);
        frameBuffer[y][x] = colour;
        return;
    }

    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

    for (int i = 0; i <= steps; i++)
    {
        float t = (float)i / steps;
        int x = round(x0 + t * dx);
        int y = round(y0 + t * dy);
        if ( y >= frameBuffer.height || x >= frameBuffer.width)
            continue;
        if ( y < 0 || x < 0)
            continue;
        frameBuffer[y][x] = colour;
    }

}


// midpoint method for drawing filled circles
void DrawCircle(RGBAImage& frameBuffer, Point3& origin, float radius, RGBAValue& colour)
{
    // Convert coords from NDC to pixel space
    int x0 = round(((origin.x + 1) / 2) * frameBuffer.width - 1);
    int y0 = round(((origin.y + 1) / 2) * frameBuffer.height - 1);

    int x = 0;
    int y = radius;
    int d = 1 - radius;

    while(x <= y) {
        
        int maxY = y0 + y;
        int minY = y0 - y;
        int maxX = y0 + x;
        int minX = y0 - x;

        if (minY >= 0 && minY < frameBuffer.height && maxY >= 0 && maxY < frameBuffer.height)
        {
            int left = max(0, x0-x);
            int right = min((int)frameBuffer.width, x0+x);
            for (int i = left; i < right; i++)
            {
                frameBuffer[maxY][i] = colour;
                frameBuffer[minY][i] = colour;
            }
        }


        if (minX >= 0 && minX < frameBuffer.height && maxX >= 0 && maxX < frameBuffer.height)
        {
            int left = max(0, x0-y);
            int right = min((int)frameBuffer.width, x0+y);
            for (int i = left; i < right; i++)
            {
                frameBuffer[maxX][i] = colour;
                frameBuffer[minX][i] = colour;
            }
        }


        x++;

        if (d < 0)
            d = d + 2*x + 1;
        else {
            y--;
            d = d + 2*x + 1 - 2*y;
        }
    }
}


void DrawGrid(RGBAImage& frameBuffer, const Matrix4& rot,float size, int segments, RGBAValue& colour, Vector3 transVector, const Matrix4& proj)
{

    float segmentWidth = size / segments;
    float halfSize = size / 2.f;

    //int x = (frameBuffer.width - 1) / 2 - planeSize / 2;
    //int y = (frameBuffer.height - 1) / 2 - planeSize / 2;
    float x = -halfSize;
    float y = -halfSize;
    
    Point3 yStart = Point3(x, y, 0.f);
    Point3 yEnd = Point3(x, y + size, 0.f);
    
    Point3 xStart = Point3(x, y, 0.f);
    Point3 xEnd = Point3(x + size, y, 0.f);

    yStart = rot * yStart;
    yEnd = rot * yEnd;

    xStart = rot * xStart;
    xEnd = rot * xEnd;
    Vector3 xStep = rot * Vector3(segmentWidth, 0.f, 0.f);
    Vector3 yStep = rot * Vector3(0.f, segmentWidth, 0.f);


    yStart = yStart + transVector;
    yEnd = yEnd + transVector;

    xStart = xStart + transVector;
    xEnd = xEnd + transVector;

    for (int i = 0; i <= segments; i++)
        {
            Point3 projYStart = proj * yStart;
            Point3 projYEnd = proj * yEnd;
            DrawLine(frameBuffer, projYStart, projYEnd, colour);
            yStart = yStart + xStep;
            yEnd = yEnd + xStep;


            Point3 projXStart = proj * xStart;
            Point3 projXEnd = proj * xEnd;
            DrawLine(frameBuffer, projXStart, projXEnd, colour);
            xStart = xStart + yStep;
            xEnd = xEnd + yStep;
        }
}

void DrawBezier(RGBAImage& frameBuffer, const Matrix4& mvp, std::vector<Point3> points)
{
    RGBAValue red = RGBAValue(255.f, 0.f, 0.f);

    for (int i = 0; i < points.size(); i++)
    {
        std::cout << "projected point [" << i << "] " << points[i] << std::endl;
    }

    int N_PTS = 4;
    Point3 bezPoints[N_PTS][N_PTS];
    bezPoints[0][0] = points[0];
    bezPoints[0][1] = points[4];
    bezPoints[0][2] = points[8];
    bezPoints[0][3] = points[12];
        

    for (float t = 0.f; t <= 1.0f; t += 0.01f)
    { //Parametric loop
        for (int diag = N_PTS - 2; diag >= 0; diag--)
        { // diagonal loop
            for (int i = 0; i <= diag; i++)
            { // i loop
                int j = diag - i;
                bezPoints[i][j] = bezPoints[i][j+1] * (1.0f-t) + bezPoints[i+1][j] * t;
            }
        }
        //frameBuffer[bezPoints[0][0].y][bezPoints[0][0].x] = red;
    }
}