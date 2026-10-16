#pragma once

#include <QOpenGLWidget>
#include "math.h"

#include "RGBAImage.h"
#include "RGBAValue.h"
#include "Point3.h"
#include "Matrix4.h"
#include "Vector3.h"

void TestFunc();

Matrix4 MakePerspective(float camOffset);

Matrix4 MakeOrtho(double left, double right, double bottom, double top, double near_val, double far_val);

Matrix4 MakeFrustum(double left, double right, double bottom, double top, double near_val, double far_val);

bool ClipNear(Homogeneous4& a, Homogeneous4& b);

void ProjectPoint(Point3& point, float camOffset);

Point3 GetProjectedPoint(Point3& point, float camOffset);

void DrawLine(RGBAImage& frameBuffer, const Point3& p0, const Point3& p1, RGBAValue& colour);

void DrawCircle(RGBAImage& frameBuffer, Point3& origin, float radius, RGBAValue& colour);

void DrawGrid(RGBAImage& frameBuffer, const Matrix4& rot,float size, int segments, RGBAValue& colour, Vector3 transVector, const Matrix4& proj);

void DrawBezier(RGBAImage& frameBuffer, const Matrix4& mvp, std::vector<Point3> points);

static Homogeneous4 deCasteljau(Homogeneous4 p0, Homogeneous4 p1, Homogeneous4 p2, Homogeneous4 p3, float t);

void DrawBezierPatch(RGBAImage& frameBuffer, std::vector<Homogeneous4>& points, float radius, bool perspective);
