#pragma once

#include <QOpenGLWidget>
#include "math.h"

#include "RGBAImage.h"
#include "RGBAValue.h"
#include "Point3.h"

void TestFunc();

void DrawAxes();

void paintGL();


void DrawLine(RGBAImage& frameBuffer, Point3& p0, Point3& p1, RGBAValue& colour);

void DrawCircle(RGBAImage& frameBuffer, Point3& origin, float radius, RGBAValue& colour);