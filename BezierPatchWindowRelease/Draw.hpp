#pragma once

#include <QOpenGLWidget>
#include "omp.h"
#include "math.h"

#include "RGBAImage.h"
#include "RGBAValue.h"
#include "Point3.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "FragmentManager.hpp"

// Sets up orthographic projection matrix based on provided parameters
Matrix4 MakeOrtho(double left, double right, double bottom, double top, double near_val, double far_val);

// Sets up perspective projection matrix based on provided parameters
Matrix4 MakeFrustum(double left, double right, double bottom, double top, double near_val, double far_val);

// Returns false if the two points are behind the camera, returns true otherwise and clips points if necessary
bool ClipNear(Homogeneous4& a, Homogeneous4& b);

// Draws a line between the provided points, clipped to the viewbox
void DrawLine(FragmentBuffer& fragBuffer, const Point3& p0, const Point3& p1, RGBAValue& colour);

// Draws a circle of specified radius at the provided point
void DrawCircle(FragmentBuffer& fragBuffer, Point3& origin, float radius, RGBAValue& colour);

// Draws a grid of specified size with a provided number of segments
void DrawGrid(FragmentBuffer& fragBuffer, const Matrix4& rot,float size, int segments, RGBAValue& colour, Vector3 transVector, const Matrix4& proj);

// Draws a cubic bezier curve using the De Casteljau algorithm
void DrawBezier(FragmentBuffer& fragBuffer, const Matrix4& mvp, std::vector<Point3> points);

// Draws a bicubic bezier tensor -product patch using the De Casteljau algorithm
void DrawBezierPatch(FragmentBuffer& fragBuffer, std::vector<Homogeneous4>& points, float radius, bool perspective);
