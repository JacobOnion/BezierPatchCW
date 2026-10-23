#include "../BezierPatchWindowRelease/Vector3.h"
#include "../BezierPatchWindowRelease/Point3.h"
#include "../BezierPatchWindowRelease/Matrix4.h"
#include "../BezierPatchWindowRelease/Homogeneous4.h"
#include <iostream>
#include <iomanip>
#include "limits"

int main() {

    // Assign data
    Point3 p = Point3(0.f, 1.0f, 3.0f);
    Point3 q = Point3(1.f, -1.f, 2.f);
    Point3 r = Point3(0.f, 0.f, 1.f);

    Vector3 u = Vector3(-1.f, 2.f, 3.f);
    Vector3 v = Vector3(1.f, 2.f, -1.f);
    Vector3 w = Vector3(-1.f, 2.f, 4.f);

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


    // Compute questions
    Point3 question1 = p + u;

    Homogeneous4 question2 = Homogeneous4(q) + m1 * Homogeneous4(v);

    Homogeneous4 question3 = Homogeneous4(r) + m3 * (m2 * Homogeneous4(w));

    Matrix4 question4 =  m2.invert();

    float question5 = u.dot(v);

    Vector3 question6 = v.cross(w);

    Matrix4 vectorMat = {u[0], v[0], w[0], 0,
                         u[1], v[1], w[1], 0,
                         u[2], v[2], w[2], 0,
                         0,    0,    0,    0};

    float question7 = vectorMat.determinant3();

    // Calculate orthonormal basis
    Vector3 e0 = u.unit();
    Vector3 u1 = v - (v.dot(u) / u.dot(u)) * u;
    Vector3 e1 = u1.unit();
    Vector3 u2 = w - (w.dot(u) / u.dot(u)) * u - (w.dot(u1) / u1.dot(u1)) * u1;
    Vector3 e2 = u2.unit();


    // Output results
    std::cout << "q1: " << question1 << std::endl;
    std::cout << "q2: " << question2 << std::endl;
    std::cout << "q3: " << question3 << std::endl;
    std::cout << "q4: " << question4 << std::endl;
    std::cout << "q5: " << question5 << std::endl;
    std::cout << "q6: " << question6 << std::endl;
    if (abs(question7) >  1e-6f)
    {
      std::cout << "q7: determinant = " << question7 << " != 0, hence the vectors are linearly independent." << std::endl;
      std::cout << "orthonormal basis: " << "(" << e0 << "), "<< "(" << e1 << "), "<< "(" << e2 << ")" << std::endl;
    }
      else
      std::cout << "q7: determinant = " << question7 << " , hence the vectors are linearly dependent." << std::endl;
}