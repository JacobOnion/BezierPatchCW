//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
////////////////////////////////////////////////////////////////////////


#include <math.h>
#include <thread>
#include <random>
#include <QTimer>

// include open mp for the advanced tasks
#include <omp.h>
#include <algorithm>

// include the header file
#include "BezierPatchRenderWidget.h"

#include <QElapsedTimer>
#include <GL/glut.h>
#include <GL/gl.h>

#include <chrono>
#include <thread>

//	Ken Shoemake's ArcBall
#include "ArcBall.h"

#define N_THREADS 16

// constructor
BezierPatchRenderWidget::BezierPatchRenderWidget
        (   
        // the Bezier patch control points to show
        ControlPoints       *newPatchControlPoints,
        // the render parameters to use
        RenderParameters    *newRenderParameters,
        // parent widget in visual hierarchy
        QWidget             *parent
        )
    // the : indicates a member initialiser list ...
    // ... it is good practice to use it where possible
    : 
    // start by calling inherited constructor with parent widget's pointer
    QOpenGLWidget(parent),
    // then store the pointers that were passed in
    patchControlPoints(newPatchControlPoints),
    renderParameters(newRenderParameters)
    { // constructor
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &BezierPatchRenderWidget::forceRepaint);
        timer->start(30);
    // leaves nothing to put into the constructor body
    } // constructor

void BezierPatchRenderWidget::forceRepaint(){
    update();
}
// destructor
BezierPatchRenderWidget::~BezierPatchRenderWidget()
    { // destructor
    // empty (for now)
    // all of our pointers are to data owned by another class
    // so we have no responsibility for destruction
    // and OpenGL cleanup is taken care of by Qt
    } // destructor                                                                 

// called when OpenGL context is set up
void BezierPatchRenderWidget::initializeGL()
    { // BezierPatchRenderWidget::initializeGL()
    } // BezierPatchRenderWidget::initializeGL()

// called every time the widget is resized
void BezierPatchRenderWidget::resizeGL(int w, int h)
    { // BezierPatchRenderWidget::resizeGL()
    // resize the render image
    renderParameters->windowWidth = w;
    renderParameters->windowHeight = h;
    //std::cout << "my window resize\n" << std::endl;
    frameBuffer.Resize(w, h);
    } // BezierPatchRenderWidget::resizeGL()


// called every time the widget needs painting
void BezierPatchRenderWidget::paintGL()
{ // BezierPatchRenderWidget::paintGL()

    //glClearColor(1.f, 0.f, 1.f, 1.f);   // magenta
    //glClear(GL_COLOR_BUFFER_BIT);
    //return;

    // TODO:
    // To match the OpenGL widget, refer to "RenderWidget.{h/cpp}.
    // There you shall find the precise colours, positions, etc.
    // No additional OpenGL calls should be added here for drawing.
    // Make use of the renderParameters matrices for converting
    //  points from local coordinates to screen space.
    // The final screen space coordinates (row/column) should then be used
    //  to set the 2D frameBuffer array with the final colour.
    // (glDrawPixels then puts the frameBuffer on the screen
    //   to display the final image at the end of paintGL)

    QElapsedTimer frametimer;
    frametimer.start();
    //glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    //glLoadIdentity();
    //#pragma omp parallel
    //{

    // clear the (non-OpenGL) buffer where we will set pixels to:
    frameBuffer.clear(renderParameters->theClearColor);
    //}
    /*for (int r = 50; r < 100; r++)
    for (int c = 50; c < 100; c++)
        frameBuffer[r][c] = RGBAValue(255, 0, 0, 255);*/

    // now clear the OpenGL buffer:
    glClearColor(0.8, 0.8, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    Matrix4 identity_matrix;
    identity_matrix.SetIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    //std::cout << "ready to begin new frame: " << frametimer.nsecsElapsed() / 1e6 << std::endl;

    //if(renderParameters->triggerResize)
    //{
    //    this->resizeGL(renderParameters->windowWidth, renderParameters->windowHeight);
    //}

    //std::cout<< "triggerResize: " << renderParameters->triggerResize << std::endl;


    // setup mvp
    Matrix4 mvp;

    Matrix4 trans;

    Matrix4 currentRot = renderParameters->rotationMatrix;

    Vector3 translationVector = Vector3(renderParameters->xTranslate,
         renderParameters->yTranslate, renderParameters->zTranslate-9.f);
    //translationVector = translationVector * 0.1f;

    //std::cout << renderParameters->zTranslate << "\n" << std::endl;
    trans.SetTranslation(translationVector);
    
    // calculate aspect ratio
    double aspectRatio = double(renderParameters->windowWidth) / double(renderParameters->windowHeight);

    Matrix4 proj;
    if (aspectRatio > 1.0)
    {
        if (renderParameters->orthoProjection)
            proj = MakeOrtho(-aspectRatio * 10/renderParameters->zTranslate, aspectRatio * 10/renderParameters->zTranslate,
                    -10/renderParameters->zTranslate, 10/renderParameters->zTranslate,
                    0.01, 200.0);
        else
            proj = MakeFrustum(-aspectRatio * 0.01, aspectRatio * 0.01,
                      -0.01, 0.01,
                      0.01, 200.0);
    }
    else
    {
        aspectRatio = 1.0 / aspectRatio;
        if (renderParameters->orthoProjection)
            proj = MakeOrtho(-10/renderParameters->zTranslate, 10/renderParameters->zTranslate,
                    -aspectRatio * 10/renderParameters->zTranslate, aspectRatio * 10/renderParameters->zTranslate,
                    0.01, 200.0);
        else
            proj = MakeFrustum(-0.01, 0.01,
                      -aspectRatio * 0.01, aspectRatio * 0.01,
                      0.01, 200.0);
    }
    
    mvp = proj * trans * currentRot;

    //std::cout << "model view matrix calculated: " << frametimer.nsecsElapsed()/1e6 << std::endl;
    

    Point3 origin = Point3(0.f, 0.f, 0.f);
    Point3 x = Point3(5.f, 0.f, 0.f);
    Point3 y = Point3(0.f, 5.f, 0.f);
    Point3 z = Point3(0.f, 0.f, 5.f);

    Point3 circle1 = Point3(5.f, 5.f, 5.f);

    RGBAValue red = RGBAValue(255.f, 0.0f, 0.0f, 255.f);
    RGBAValue green = RGBAValue(0.f, 255.f, 0.f, 255.f);
    RGBAValue blue = RGBAValue(0.f, 0.f, 255.f, 255.f);

    RGBAValue purple = RGBAValue(64.f, 0.0f, 64.f);
    RGBAValue cyan = RGBAValue(0.f, 64.f, 64.f);
    RGBAValue yellow = RGBAValue(64.f, 64.f, 0.f);

    RGBAValue grey = RGBAValue(191.25f, 191.25f, 191.25f);


    //Matrix4 currentRot = renderParameters->rotationMatrix;
    
    origin = mvp * origin;
    x = mvp * x;
    y = mvp * y;
    z = mvp * z;
    circle1 = mvp * circle1;
    

    // Project vertices into clip space
    std::vector<Point3> projectedPoints;
    std::vector<Homogeneous4> clipPoints;
    std::vector<int> pointRadii;
    
    float worldRadius = 1.25f;
    if (!renderParameters->orthoProjection)
        worldRadius /= 10.f;
    const float halfWidth = frameBuffer.width / 2.f;

    for(int i = 0 ; i < (*patchControlPoints).vertices.size(); i++)
    {
        Homogeneous4 vertexCoords = mvp * Homogeneous4(renderParameters->patchControlPoints->vertices[i]);
        float radius = 0;
        // bit hacky, use different methods of calculating radii depending on which projection is being used
        if (renderParameters->orthoProjection)
            radius = (proj * Vector3(40.f, 0.f, 0.f)).x;
        else
            radius = worldRadius * proj[0][0] / vertexCoords.w * halfWidth;
        pointRadii.push_back(radius);


        //if (vertexCoords[i].z + vertexCoords[i].w < 0.f)


        /*if (vertexCoords.w < 0)
        {
            std::cout << vertexCoords.w << std::endl;
            vertexCoords = vertexCoords * -1.f;
        }*/
        
        Point3 vertexPos = vertexCoords.Point();
        projectedPoints.push_back(vertexPos);
        clipPoints.push_back(vertexCoords);
    }

    if(renderParameters->verticesEnabled)
    {// UI control for showing vertices

        for(int i = 0 ; i < (*patchControlPoints).vertices.size(); i++)
        {
            // draw each vertex as a point
            // (paint the active vertex in red, ...
            //  ... keep the others in white)

            // consider ways to make the rendered points bigger than just 1x1 pixel on the screen
            
            // Skip vertices behind near plane
            if (clipPoints[i].z + clipPoints[i].w < 0.f)
                continue;

            RGBAValue currentColour = grey;
            if (i == renderParameters->activeVertex)
                currentColour = red;
            
            //Point3 vertexPos = mvp * renderParameters->patchControlPoints->vertices[i];
            
            //float radius = (proj * Vector3(40.f, 0.f, 0.f)).x;
            //float radius = 40.f * 
            //radius = 5.f;
            //if (!renderParameters->orthoProjection)
            //    radius /= 5.f;

            DrawCircle(frameBuffer, projectedPoints[i], pointRadii[i], currentColour); // Draw red circle
        }
    }// UI control for showing vertices

    if(renderParameters->netEnabled)
    {// UI control for showing the Bezier control net
     // (control points connected with lines)
        for(int i = 0 ; i < (*patchControlPoints).vertices.size(); i++)
        {
            if ((i + 1) % 4 != 0)
            {
                // Clip lines to visible segments
                Homogeneous4 a = clipPoints[i], b = clipPoints[i+1];
                if (ClipNear(a, b))
                    DrawLine(frameBuffer, a.Point(), b.Point(), green);                
            }
            if (i < 12)
            {
                Homogeneous4 a = clipPoints[i], b = clipPoints[i+4];
                if (ClipNear(a, b))
                    DrawLine(frameBuffer, a.Point(), b.Point(), green);
            }
        }
    }// UI control for showing the Bezier control net


    if(renderParameters->planesEnabled)
    {// UI control for showing axis-aligned planes

        // Planes are axis aligned grids made up of lines
        // Draw the vertical x axis plane (in purple)
        // Draw the vertical y axis plane (in blue)
        // Draw the flat plane (in brown)
        // Refer to RenderWidget.cpp for the precise colours.

        Matrix4 XtoY = {1.f, 0.f, 0.f, 0.f,
                        0.f, 0.f, -1.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        0.f, 0.f, 0.f, 1.f};
        Matrix4 mvpY = proj * trans * currentRot * XtoY;

        Matrix4 XtoZ = {0.f, 0.f, 1.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        -1.f, 0.f, 0.f, 0.f,
                        0.f, 0.f, 0.f, 1.f};

        Matrix4 mvpZ = proj * trans * currentRot * XtoZ;


        DrawGrid(frameBuffer, currentRot, 10.f, 10, yellow, translationVector, proj);
        DrawGrid(frameBuffer, currentRot * XtoY, 10.f, 5, purple, translationVector, proj);
        DrawGrid(frameBuffer, currentRot * XtoZ, 10.f, 5, cyan, translationVector, proj);

    }// UI control for showing axis-aligned planes

    if(renderParameters->bezierEnabled)
    {// UI control for showing the Bezier curve
        //DrawBezier(frameBuffer, mvp, projectedPoints);
        DrawBezierPatch(frameBuffer, clipPoints, 10.f, !renderParameters->orthoProjection);
    }

    // Put the custom framebufer on the screen to display the image
    glDrawPixels(frameBuffer.width, frameBuffer.height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer.block);
        
    std::cout << "total frame time: " << frametimer.nsecsElapsed()/1e9 << "s" << std::endl;
    frametimer.restart();
} // BezierPatchRenderWidget::paintGL()

// mouse-handling
void BezierPatchRenderWidget::mousePressEvent(QMouseEvent *event)
    { // BezierPatchRenderWidget::mousePressEvent()
    // store the button for future reference
    int whichButton = int(event->button());
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width   
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y() ) / size;

    
    // and we want to force mouse buttons to allow shift-click to be the same as right-click
    unsigned int modifiers = event->modifiers();
    
    // shift-click (any) counts as right click
    if (modifiers & Qt::ShiftModifier)
        whichButton = Qt::RightButton;
    
    // send signal to the controller for detailed processing
    emit BeginScaledDrag(whichButton, x,y);
    } // BezierPatchRenderWidget::mousePressEvent()
    
void BezierPatchRenderWidget::mouseMoveEvent(QMouseEvent *event)
    { // BezierPatchRenderWidget::mouseMoveEvent()
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width   
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y() ) / size;
    
    // send signal to the controller for detailed processing
    emit ContinueScaledDrag(x,y);
    } // BezierPatchRenderWidget::mouseMoveEvent()
    
void BezierPatchRenderWidget::mouseReleaseEvent(QMouseEvent *event)
    { // BezierPatchRenderWidget::mouseReleaseEvent()
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width   
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y() ) / size;
    
    // send signal to the controller for detailed processing
    emit EndScaledDrag(x,y);
    } // BezierPatchRenderWidget::mouseReleaseEvent()
