#include "string.h"
#include"iostream"
#include "Draw.hpp"
using namespace std;

void TestFunc() {
    cout << "\nhello world\n";

    printf("\nis this working\n");
}


void DrawLine(RGBAImage& frameBuffer, Point3& p0, Point3& p1, RGBAValue& colour)
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
        frameBuffer[y][x] = colour;
    }

}


void DrawCircle(RGBAImage& frameBuffer, Point3& origin, float radius, RGBAValue& colour)
{
    // Convert coords from NDC to pixel space
    int x0 = round(((origin.x + 1) / 2) * frameBuffer.width - 1);
    int y0 = round(((origin.y + 1) / 2) * frameBuffer.height - 1);

    int x = radius;
    //frameBuffer[y0][x0 + radius] = colour;

}




void DrawAxes() {
    /*glClearColor(0.f, 0.5f, 0.5f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.f, 0.f, 0.f);
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();

        printf("\n AXISSSSING \n");*/
}

void paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    glColor3f(1.f, 0.f, 0.f);
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();

        printf("\nPAINTINGGGGG\n");

}