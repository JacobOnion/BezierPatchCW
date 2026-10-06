#include "../BezierPatchWindowRelease/Vector3.h"
#include "../BezierPatchWindowRelease/Point3.h"
#include "../BezierPatchWindowRelease/Matrix4.h"
#include "../BezierPatchWindowRelease/Homogeneous4.h"


int main() {
    Point3 p = Point3(0.f, 1.0f, 3.0f);
    Point3 q = Point3(1.f, -1.f, 2.f);
    Point3 r = Point3(0.f, 0.f, 1.f);

    Vector3 u = Vector3(-1.f, 2.f, 3.f);
    Vector3 v = Vector3(1.f, 2.f, -1.f);
    Vector3 w = Vector3(-1.f, 2.f, 3.f);

    Matrix4 m1 = {1.f, 0.f, 3.f, 0.f,
                  1.f, 9.f, 2.f, 0.f,
                  4.f, 3.f, 6.f, 0.f,
                  0.f, 0.f, 0.f, 1.f
                };

    Matrix4 m2 = {0.707f, 0.707f, 0.f, 0.f,
                  -0.707f, 0.707f, 0, 0.f,
                  0.f, 0.f, 1.f, 0.f,
                  0.f, 0.f, 0.f, 1.f
                };

    Matrix4 m3 = {6.f, 0.f, 0.f, 0.f,
                  -1.f, 4.f, 0.f, 0.f,
                  4.f, 2.f, -3.f, 0.f,
                  0.f, 0.f, 0.f, 1.f
                };

    Point3 question1 = p + u;

    Homogeneous4 question2 = Homogeneous4(u) + m1 * Homogeneous4(v);

    Homogeneous4 question3 = Homogeneous4(r) + m3 * (m2 * Homogeneous4(w));

    //Homogeneous4 question4 =  implement invert matrix function i guess

    float question5 = u.dot(v);

    Vector3 question6 = v.cross(w);

    //Vector3 question7 = independance?
}