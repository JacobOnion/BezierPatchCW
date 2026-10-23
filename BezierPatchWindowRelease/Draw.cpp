#include "string.h"
#include"iostream"
#include "Draw.hpp"
using namespace std;


// Set up orthographic projection matrix
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

// Set up perspective projection matrix
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


// Returns false if the segment is entirely behind the near plane.
bool ClipNear(Homogeneous4& a, Homogeneous4& b)
{
    float da = a.z + a.w;
    float db = b.z + b.w;
    if (da < 0.f && db < 0.f) return false;
    if (da >= 0.f && db >= 0.f) return true;

    float t = da / (da - db);
    Homogeneous4 p(a.x + t * (b.x - a.x),
                   a.y + t * (b.y - a.y),
                   a.z + t * (b.z - a.z),
                   a.w + t * (b.w - a.w));
    if (da < 0.f) a = p; else b = p;
    return true;
}


void DrawLine(FragmentBuffer& fragBuffer, const Point3& p0, const Point3& p1, RGBAValue& colour)
{
    // Convert coords from NDC to pixel space
    float x0 = (p0.x + 1) / 2;
    float x1 = (p1.x + 1) / 2;

    float y0 = (p0.y + 1) / 2;
    float y1 = (p1.y + 1) / 2;

    x0 *= fragBuffer.width - 1;
    y0 *= fragBuffer.height - 1;
    x1 *= fragBuffer.width - 1;
    y1 *= fragBuffer.height - 1;


    int dx = x1 - x0;
    int dy = y1 - y0;

    // Line is only 1 pixel
    if (dx == 0 && dy == 0)
    {
        int x = round(x0);
        int y = round(y0);
        fragBuffer.Add(x, y, p0.z, colour);
        return;
    }

    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

    for (int i = 0; i <= steps; i++)
    {
        // interpolate line at t
        float t = (float)i / steps;
        int x = round(x0 + t * dx);
        int y = round(y0 + t * dy);
        // Check if fragment is on screen
        if ( y >= fragBuffer.height || x >= fragBuffer.width)
            continue;
        if ( y < 0 || x < 0)
            continue;
        fragBuffer.Add(x, y, p0.z + t * (p1.z - p0.z), colour);
    }

}


// midpoint method for drawing filled circles
void DrawCircle(FragmentBuffer& fragBuffer, Point3& origin, float radius, RGBAValue& colour)
{
    // Convert coords from NDC to pixel space
    int x0 = round(((origin.x + 1) / 2) * fragBuffer.width - 1);
    int y0 = round(((origin.y + 1) / 2) * fragBuffer.height - 1);

    int x = 0;
    int y = (int)round(radius);
    int d = 1 - radius;

    
    auto span = [&](int y, int left, int right) {
        // Only draw on screen parts of circle
        if (y < 0 || y >= fragBuffer.height) return;
        left = max(left, 0);
        right = min(right, (int)(fragBuffer.width - 1));

        for (int i = left; i <= right; i++)
            fragBuffer.Add(i, y, origin.z, colour);
    };

    while(x <= y) {
        
        int maxY = y0 + y;
        int minY = y0 - y;
        int maxX = y0 + x;
        int minX = y0 - x;

        // Draw each segment of the circle
        span(maxY, x0 - x, x0 + x);
        span(minY, x0 - x, x0 + x);
        span(maxX, x0 - y, x0 + y);
        span(minX, x0 - y, x0 + y);


        x++;

        if (d < 0)
            d = d + 2*x + 1;
        else {
            y--;
            d = d + 2*x + 1 - 2*y;
        }
    }
}


void DrawGrid(FragmentBuffer& fragBuffer, const Matrix4& rot,float size, int segments, RGBAValue& colour, Vector3 transVector, const Matrix4& proj)
{

    float segmentWidth = size / segments;
    float halfSize = size / 2.f;

    float x = -halfSize;
    float y = -halfSize;

    // Define starting points and transformations
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
            // Project and draw points
            Homogeneous4 projYStart = proj * Homogeneous4(yStart);
            Homogeneous4 projYEnd = proj * Homogeneous4(yEnd);
            if (ClipNear(projYStart, projYEnd))
                DrawLine(fragBuffer, projYStart.Point(), projYEnd.Point(), colour);
            yStart = yStart + xStep;
            yEnd = yEnd + xStep;


            Homogeneous4 projXStart = proj * Homogeneous4(xStart);
            Homogeneous4 projXEnd = proj * Homogeneous4(xEnd);
            if (ClipNear(projXStart, projXEnd))
                DrawLine(fragBuffer, projXStart.Point(), projXEnd.Point(), colour);
            xStart = xStart + yStep;
            xEnd = xEnd + yStep;
        }
}

void DrawBezier(FragmentBuffer& fragBuffer, const Matrix4& mvp, std::vector<Point3> points)
{
    RGBAValue red = RGBAValue(255.f, 0.f, 0.f);

    // NDC to pixel
    for (int i = 0; i < points.size(); i++)
    {
        std::cout << "projected point [" << i << "] " << points[i] << std::endl;
        points[i].x = round(((points[i].x + 1) / 2) * fragBuffer.width - 1);
        points[i].y = round(((points[i].y + 1) / 2) * fragBuffer.height - 1);
        std::cout << "pixel point [" << i << "] " << points[i] << std::endl;
    }

    // Assign starting data
    int N_PTS = 4;
    Point3 bezPoints[N_PTS][N_PTS];
    bezPoints[0][3] = points[0];
    bezPoints[1][2] = points[4];
    bezPoints[2][1] = points[8];
    bezPoints[3][0] = points[12];
        

    for (float t = 0.f; t <= 1.0f; t += 0.001f)
    { //Parametric loop
        for (int diag = N_PTS - 2; diag >= 0; diag--)
        { // diagonal loop
            for (int i = 0; i <= diag; i++)
            { // i loop
                int j = diag - i;
                bezPoints[i][j] = bezPoints[i][j+1] * (1.0f-t) + bezPoints[i+1][j] * t;
            }
        }
        if ( bezPoints[0][0].y >= fragBuffer.height || bezPoints[0][0].x >= fragBuffer.width)
            continue;
        if ( bezPoints[0][0].y < 0 || bezPoints[0][0].x < 0)
            continue;
        RGBAValue pointCol = RGBAValue(t*255.f, 127.5f, t*255.f);
        fragBuffer.Add((int)bezPoints[0][0].x, (int)bezPoints[0][0].y, (int)bezPoints[0][0].z, pointCol);
    }
}

// Blend pairs of points to ind the average of the 4 points at t
static Homogeneous4 deCasteljau(Homogeneous4 p0, Homogeneous4 p1, Homogeneous4 p2, Homogeneous4 p3, float t)
{
    Homogeneous4 a = (1.f - t) * p0 + t * p1;
    Homogeneous4 b = (1.f - t) * p1 + t * p2;
    Homogeneous4 c = (1.f - t) * p2 + t * p3;
    Homogeneous4 d = (1.f - t) * a + t * b;
    Homogeneous4 e = (1.f - t) * b + t * c;
    return (1.f - t) * d + t * e;
}

void DrawBezierPatch(FragmentBuffer& fragBuffer, std::vector<Homogeneous4>& points, float radius, bool perspective)
{
    #pragma omp parallel
    {
        std::vector<Fragment> tempFrags;
        
        #pragma omp for schedule(dynamic, 16) nowait
        for (int si = 0; si < 1000; si++)  // Consider dynamically setting sample count based on control net size
        { // Paramtric loop
            float s = si * 0.001f;
            Homogeneous4 q[4];
            
                for (int i = 0; i < 4; i++)
                { // Get one bezier point per row
                    q[i] = deCasteljau(points[4*i], points[4*i+1], points[4*i+2], points[4*i+3], s);
                }
            for (int ti = 0; ti < 2500; ti++)
            { // Parametric loop
                float t = ti * 0.0004f;
                // Blend the 4 rows to get the final point p
                Homogeneous4 h = deCasteljau(q[0], q[1], q[2], q[3], t);

                if (!perspective)
                    h.w = 1.f;

                if (h.z + h.w < 0.f || h.w <= 0.f)
                    continue;

                float ndc_x = h.x / h.w;
                float ndc_y = h.y / h.w;
                float ndc_z = h.z / h.w;
                
                // Convert to pixel coords
                int px = static_cast<int>((ndc_x + 1.0f) * 0.5f * fragBuffer.width);
                int py = static_cast<int>((ndc_y + 1.0f) * 0.5f * fragBuffer.height); 

                // Screen-space boundary check
                if (px < 0 || px >= fragBuffer.width || py < 0 || py >= fragBuffer.height)
                    continue;
                    
                RGBAValue pointCol = RGBAValue(t * 255.f, 127.5f, s * 255.f);
                tempFrags.push_back({px, py, ndc_z, pointCol});
            }
        }
        // insert the bezier fragments into the main fragment vector
        #pragma omp critical
            fragBuffer.frags.insert(fragBuffer.frags.end(), tempFrags.begin(), tempFrags.end());
    }
}