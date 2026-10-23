//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include "Matrix4.h"
#include "Quaternion.h"
#include <limits>
#include <math.h>
#include <algorithm>

// constructor - default to the zero matrix
Matrix4::Matrix4()
    { // default constructor
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            coordinates[row][col] = 0.0;
    } // default constructor

Matrix4::Matrix4(const Matrix4 &other)
    { // copy constructor
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            coordinates[row][col] = other.coordinates[row][col];
    } // copy constructor

// Adding a new constructor for an initialiser list
Matrix4::Matrix4(std::initializer_list<float> elements) : coordinates {}
{
    auto i = elements.begin();
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            coordinates[row][col] = *i++;
}

// equality operator
bool Matrix4::operator ==(const Matrix4 &other) const
    { // operator ==()
    // loop through, testing for mismatches
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            if (abs(coordinates[row][col] - other.coordinates[row][col]) > std::numeric_limits<float>::epsilon())
                return false;
    // if no mismatches, matrices are the same
    return true;
    } // operator ==()

// scalar operations
// multiplication operator (no division operator)
Matrix4 Matrix4::operator *(float factor) const
    { // operator *()
    // start with a zero matrix
    Matrix4 returnMatrix;

    // TODO: multiply by the factor
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            returnMatrix.coordinates[row][col] = coordinates[row][col] * factor;

    // and return it
    return returnMatrix;
    } // operator *()
    

// vector operations on homogeneous coordinates
// multiplication is the only operator we use
Homogeneous4 Matrix4::operator *(const Homogeneous4 &vector) const
    { // operator *()
    // get a zero-initialised vector
    Homogeneous4 productVector;

        for (int row = 0; row < 4; row++)
            for (int col = 0; col < 4; col++)
                productVector[row] += coordinates[row][col] * vector[col];

    // return the result
    return productVector;
    } // operator *()

// and on Cartesian coordinates
Point3 Matrix4::operator *(const Vector3 &vector) const
    { // cartesian multiplication
    // convert to Homogeneous coords and multiply
    Homogeneous4 productVector = (*this) * Homogeneous4(vector);

    // then divide back through
    return productVector.Point();
    } // cartesian multiplication

Point3 Matrix4::operator *(const Point3 &vector) const
    { // cartesian multiplication
    // convert to Homogeneous coords and multiply
    Homogeneous4 productVector = (*this) * Homogeneous4(vector);

    // then divide back through
    return productVector.Point();
    } // cartesian multiplication


// matrix operations
// addition operator
Matrix4 Matrix4::operator +(const Matrix4 &other) const
    { // operator +()
    // start with a zero matrix
    Matrix4 sumMatrix;

    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            sumMatrix[row][col] += coordinates[row][col] + other.coordinates[row][col];

    // return the result
    return sumMatrix;
    } // operator +()

// subtraction operator
Matrix4 Matrix4::operator -(const Matrix4 &other) const
    { // operator -()
    // start with a zero matrix
    Matrix4 differenceMatrix;

    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            differenceMatrix[row][col] += coordinates[row][col] - other.coordinates[row][col];

    // return the result
    return differenceMatrix;
    } // operator -()

// multiplication operator
Matrix4 Matrix4::operator *(const Matrix4 &other) const
    { // operator *()
    // start with a zero matrix
    Matrix4 productMatrix;
    // This function is provided to give the correct result ...
    // ... on the OpenGL render widget in the app UI
    // loop, adding products
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            for (int entry = 0; entry < 4; entry++)
                productMatrix.coordinates[row][col] += coordinates[row][entry] * other.coordinates[entry][col];

    // return the result
    return productMatrix;
    } // operator *()

// matrix transpose
Matrix4 Matrix4::transpose() const
    { // transpose()
    // start with a zero matrix
    Matrix4 transposeMatrix;
    
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            transposeMatrix[row][col] = coordinates[col][row];

    // return the result
    return transposeMatrix;
    } // transpose()

Matrix4 Matrix4::invert() const
{ //invert()
    // Gauss-Jordan elimination
    float a[4][8];
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 8; col++)
            a[row][col] = (col < 4) ? coordinates[row][col] : (float)(col - 4 == row);
 
    for (int col = 0; col < 4; col++)
        {
        // partial pivoting: find the row with the largest entry in this column
        int pivot = col;
        for (int row = col + 1; row < 4; row++)
            if (fabs(a[row][col]) > fabs(a[pivot][col]))
                pivot = row;
 
        // singular matrix: return the zero matrix
        if (fabs(a[pivot][col]) < 1e-12)
            return Matrix4();
 
        // swap the pivot row into place and scale it so the pivot is 1
        for (int k = 0; k < 8; k++)
            std::swap(a[col][k], a[pivot][k]);
        float scale = 1.0f / a[col][col];
        for (int k = 0; k < 8; k++)
            a[col][k] *= scale;
 
        // eliminate this column from every other row
        for (int row = 0; row < 4; row++)
            if (row != col)
                {
                float factor = a[row][col];
                for (int k = 0; k < 8; k++)
                    a[row][k] -= factor * a[col][k];
                }
        }
 
    // the right half is the inverse
    Matrix4 invertMatrix;
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            invertMatrix[row][col] = a[row][col + 4];
 
    // return the result
    return invertMatrix;
}

float Matrix4::determinant3() const
{
    float det;
    det = coordinates[0][0] * (coordinates[1][1] * coordinates[2][2] - coordinates[1][2] * coordinates[2][1])
         - coordinates[0][1] * (coordinates[1][0] * coordinates[2][2] - coordinates[1][2] * coordinates[2][0])
         + coordinates[0][2] * (coordinates[1][0] * coordinates[2][1] - coordinates[1][1] * coordinates[2][0]);

    return det;
}



void Matrix4::SetTranslation(const Vector3 &vector)
    { // SetTranslation()
    // start with an identity matrix
    SetIdentity();

    // TODO: put the translation in the w column
    for (int row = 0; row < 3; row++)
        coordinates[row][3] = vector[row];

    } // SetTranslation()

void Matrix4::SetScale(float xScale, float yScale, float zScale)
    { // SetScale()
        // start off with a zero matrix
        SetZero();

        coordinates[0][0] = xScale;
        coordinates[1][1] = yScale;
        coordinates[2][2] = zScale;
        coordinates[3][3] = 1.0f;

    } // SetScale()

void Matrix4::SetRotation(const Vector3 &axis, float theta)
    { // SetRotation()
    // This is derived from quaternions, so we invoke them
    Quaternion rotationQuaternion(axis.unit(), theta * 0.5);
    (*this) = rotationQuaternion.GetMatrix();
    } // SetRotation()


// scalar operations
// additional scalar multiplication operator
Matrix4 operator *(float factor, const Matrix4 &matrix)
    { // operator *()
    // since this is commutative, call the other version
    return matrix * factor;
    } // operator *()

// factory methods that create specific matrices
// the zero matrix
void Matrix4::SetZero()
    { // SetZero()
        for (int row = 0; row < 4; row++)
            for (int col = 0; col < 4; col++)
                coordinates[row][col] = 0.0;
    } // SetZero()

// the identity matrix
void Matrix4::SetIdentity()
    { // SetIdentity()
        // start with a zero matrix
        SetZero();
        // fill in the diagonal with 1's
        for (int row = 0; row < 4; row++)
            coordinates[row][row] = 1.0;
    } // SetIdentity()

// returns a column-major array of 16 values
// for use with OpenGL
columnMajorMatrix Matrix4::columnMajor() const
    { // columnMajor()
        // start off with an unitialised array
        columnMajorMatrix returnArray;
        // loop to fill in
        for (int row = 0; row < 4; row++)
            for (int col = 0; col < 4; col++)
                returnArray.coordinates[4 * col + row] = coordinates[row][col];
        // now return the array
        return returnArray;
    } // columnMajor()


// indexing - retrieves the beginning of a line
// array indexing will then retrieve an element
float * Matrix4::operator [](const int rowIndex)
    { // operator *()
        // return the corresponding row
        return coordinates[rowIndex];
    } // operator *()

// similar routine for const pointers
const float * Matrix4::operator [](const int rowIndex) const
    { // operator *()
        // return the corresponding row
        return coordinates[rowIndex];
    } // operator *()


// stream input
std::istream & operator >> (std::istream &inStream, Matrix4 &matrix)
    { // operator >>()
    // just loop, reading them in
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            inStream >> matrix.coordinates[row][col];   
    // and return the stream
    return inStream;
    } // operator >>()

// stream output
std::ostream & operator << (std::ostream &outStream, const Matrix4 &matrix)
    { // operator <<()
    // just loop, reading them in
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            outStream << std::setprecision(4) << std::setw(8) << matrix.coordinates[row][col] << ((col == 3) ? "\n" : " "); 
    // and return the stream
    return outStream;
    } // operator <<()
