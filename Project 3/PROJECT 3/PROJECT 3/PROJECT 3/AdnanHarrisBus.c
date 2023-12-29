#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define KEY_ESC 27
#define TRUE 1
#define FALSE 0
#define MOVE_EYE 0
#define TWIST_EYE 1
#define ZOOM 2
#define MOVE_NONE 3

int width, height;
int eye = TRUE;
int flat = FALSE;
int action;
double xStart = 0.0, yStart = 0.0;
float nearClip, farClip, distance, twistAngle, incAngle, azimAngle;
GLfloat spin = 0.0;
GLfloat x, y;
int singleb, doubleb;
GLfloat cameraX = 2.0f;
GLfloat cameraY = 1.0f;
GLfloat cameraZ = 2.0f;

GLfloat targetX = 0.0f;
GLfloat targetY = 0.0f;
GLfloat targetZ = 0.0f;

GLfloat upX = 0.0f;
GLfloat upY = 1.0f;
GLfloat upZ = 0.0f;

GLfloat light_position[] = { 1.0f, 2.0f, 1.0f, 1.0f }; // Initial light position
GLfloat light_direction[] = { 0.0f, -1.0f, 0.0f };      // Initial light direction (pointing down)
GLfloat cutoff_angle = 30.0f;                            // Initial cutoff angle
//GLfloat spotlight_position[] = { 1.0f, 0.5f, 0.0f, 1.0f }; // Position of the spotlight
//GLfloat spotlight_direction[] = { -1.0f, 0.0f, 0.0f }; // Direction of the spotlight (pointing towards the center of the bus)

int enableTexture = 1; // Initially, texture is enabled


void drawCylinder(GLfloat radius, GLfloat height, GLint slices) {
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, slices, 1);
    gluDeleteQuadric(quad);
}

void drawDisk(GLfloat inner, GLfloat outer, GLfloat slices, GLfloat rings) {
    GLUquadric* quad = gluNewQuadric();
    gluDisk(quad, inner, outer, slices, rings);
    gluDeleteQuadric(quad);
}

void drawRect(double x1, double y1, double length_x, double length_y)
{
    glBegin(GL_POLYGON);

    glVertex2d(x1 + length_x / 2, y1 + length_y / 2);
    glVertex2d(x1 + length_x / 2, y1 - length_y / 2);
    glVertex2d(x1 - length_x / 2, y1 - length_y / 2);
    glVertex2d(x1 - length_x / 2, y1 + length_y / 2);

    glEnd();
}
// Function to draw the bus
void drawBus() {
    // Draw the body of the bus (cube-like shape)
    glColor3f(1.0, 1.0, 0.0); // Yellow color

    glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS); // Start drawing the first cube

    // Draw the body of the bus (cube-like shape)
    glColor3f(1.0, 1.0, 0.0); // Yellow color

    glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS); // Start drawing the first cube

    // Front face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, -0.3, 0.3);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, -0.3, 0.3);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 0.3, 0.3);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0, 0.3, 0.3);

    // Back face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0, -0.3, -0.3);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0, -0.3, -0.3);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0, 0.3, -0.3);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0, 0.3, -0.3);

    // Left face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0, -0.3, 0.3);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0, -0.3, -0.3);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0, 0.3, -0.3);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, 0.3, 0.3);


    // Right face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0, -0.3, 0.3);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, -0.3, -0.3);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 0.3, -0.3);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0, 0.3, 0.3);

    // Top face
    glVertex3f(-1.0, 0.3, 0.3);
    glVertex3f(1.0, 0.3, 0.3);
    glVertex3f(1.0, 0.3, -0.3);
    glVertex3f(-1.0, 0.3, -0.3);

    // Bottom face
    glVertex3f(-1.0, -0.3, -0.3);
    glVertex3f(1.0, -0.3, -0.3);
    glVertex3f(1.0, -0.3, 0.3);
    glVertex3f(-1.0, -0.3, 0.3);

    glEnd(); // End the first cube



    // Disable depth testing for the windows (temporarily)
    //glDisable(GL_DEPTH_TEST);

    // Attach the second cube to the front of the bus
    glColor3f(1.0, 1.0, 0.0); // Yellow color
    glPushMatrix();
    glTranslatef(1.0, -0.12, 0); // Adjust the position relative to the bus
    glScalef(0.5, 0.35, 0.58); // Adjust the size of the second cube
    glutSolidCube(1.0);
    glPopMatrix();


    // Draw the rectangular windscreen in blue color
    glColor3f(0.0, 1.0, 1.0); // Blue color for the windscreen
    glPushMatrix();
    glTranslatef(1.001, 0.15, 0); // Adjust the position to be just above the second cube
    glRotatef(90.0, 0.0, 1.0, 0.0); // Rotate the polygon horizontally (around the Y-axis)
    glScalef(0.6, 0.2, 0.1); // Scale down the polygon to fit above the second cube and shape it like a rectangle

    // Define the vertices for the rectangular windscreen
    glBegin(GL_POLYGON);
    glVertex3f(-0.5, 0.5, 0);   // Top-left vertex
    glVertex3f(0.5, 0.5, 0);    // Top-right vertex
    glVertex3f(0.5, -0.5, 0);   // Bottom-right vertex
    glVertex3f(-0.5, -0.5, 0);  // Bottom-left vertex
    glEnd();

    glPopMatrix();

    // Enable depth testing again
    glEnable(GL_DEPTH_TEST);

    // Draw the rectangular rear window in blue color
    glColor3f(0.0, 1.0, 1.0); // Blue color for the windscreen
    glPushMatrix();
    glTranslatef(-1.001, 0.15, 0); // Adjust the position to be just above the second cube
    glRotatef(90.0, 0.0, 1.0, 0.0); // Rotate the polygon horizontally (around the Y-axis)
    glScalef(0.6, 0.2, 0.1); // Scale down the polygon to fit above the second cube and shape it like a rectangle

    // Define the vertices for the rectangular windscreen
    glBegin(GL_POLYGON);
    glVertex3f(-0.5, 0.5, 0);   // Top-left vertex
    glVertex3f(0.5, 0.5, 0);    // Top-right vertex
    glVertex3f(0.5, -1, 0);   // Bottom-right vertex
    glVertex3f(-0.5, -1, 0);  // Bottom-left vertex
    glEnd();

    glPopMatrix();

    // Disable depth testing for the windows (temporarily)
    //glDisable(GL_DEPTH_TEST);

    // Draw the windows (rectangles) in blue color
    glColor3f(0.0, 1.0, 1.0); // Blue color for windows

    // Left window
    glPushMatrix(); // Save the current modelview matrix
    glTranslatef(1.1, -0.13, 0.0499); // Translate the left window to the left
    glBegin(GL_QUADS);
    glVertex3f(-2.1, 0.38, -0.35); // Top-left corner (reduced height and increased length)
    glVertex3f(-0.1, 0.38, -0.35); // Top-right corner (reduced height and increased length)
    glVertex3f(-0.1, 0.1, -0.35); // Bottom-right corner (no change in height)
    glVertex3f(-2.1, 0.1, -0.35); // Bottom-left corner (no change in height)
    glEnd();
    glPopMatrix();

    // Right window (translated to the other side)
    glPushMatrix(); // Save the current modelview matrix
    glTranslatef(-1.1, -0.13, 0.6519); // Translate the right window to the left
    glBegin(GL_QUADS);

    glVertex3f(2.1, 0.38, -0.35); // Top-left corner (reduced height and increased length)
    glVertex3f(0.1, 0.38, -0.35); // Top-right corner (reduced height and increased length)
    glVertex3f(0.1, 0.1, -0.35); // Bottom-right corner (no change in height)
    glVertex3f(2.1, 0.1, -0.35); // Bottom-left corner (no change in height)
    glEnd();
    glPopMatrix(); // Restore the previous modelview matrix

      // Enable depth testing again
    //glEnable(GL_DEPTH_TEST);

    // Draw the left headlight as a small sphere
    glColor3f(1.0, 1.0, 1.0); // White color for headlights
    glPushMatrix();
    glTranslatef(1.2, -0.15, 0.21); // Adjust the position of the left headlight
    glutSolidSphere(0.08, 20, 20); // Draw a small sphere for the headlight
    glPopMatrix();


    // Draw the right headlight as a small sphere
    glPushMatrix();
    glTranslatef(1.2, -0.15, -0.21); // Adjust the position of the right headlight
    glutSolidSphere(0.08, 20, 20); // Draw a small sphere for the headlight
    glPopMatrix();

    // Enable depth testing again
    //glEnable(GL_DEPTH_TEST);

    // Draw the left rear light as a small red sphere
    glColor3f(1.0, 0.0, 0.0); // Red color for rear lights
    glPushMatrix();
    glTranslatef(-0.95, -0.15, 0.23); // Adjust the position of the left rear light
    glutSolidSphere(0.07, 20, 20); // Draw a small sphere for the rear light
    glPopMatrix();

    // Draw the right rear light as a small red sphere
    glPushMatrix();
    glTranslatef(-0.95, -0.15, -0.23); // Adjust the position of the right rear light
    glutSolidSphere(0.07, 20, 20); // Draw a small sphere for the rear light
    glPopMatrix();


    // Draw the wheels (cylinders) with larger radius
    glColor3f(0.0, 0.0, 0.0); // Black color
    const float wheelRadius = 0.15; // Increase the wheel radius
    const float wheelOffsetX = 0.7;
    const float wheelOffsetY = -0.35;
    const float wheelOffsetZ = 0.30; // Adjust the Z-coordinate to attach the wheels
    const float wheelHeight = 0.1;

    // right rear wheel
    glColor3f(0, 0, 0);  // Black
    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, wheelOffsetZ - wheelHeight - 0.001);
    drawCylinder(wheelRadius, wheelHeight, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, wheelOffsetZ - wheelHeight - 0.001);
    drawCylinder(0.1, wheelHeight, 20);
    glPopMatrix();

    // right rear wheel disk
    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, wheelOffsetZ - 0.001);
    drawDisk(0.1, wheelRadius, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, wheelOffsetZ - wheelHeight - 0.001);
    drawDisk(0.1, wheelRadius, 20, 1);
    glPopMatrix();

    // right rear hub cap
    glColor3f(0.45, 0.45, 0.45);
    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, wheelOffsetZ - 0.001);
    drawDisk(0, 0.1, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, wheelOffsetZ - wheelHeight - 0.001);
    drawDisk(0, 0.1, 20, 1);
    glPopMatrix();

    // right front wheel
    glColor3f(0, 0, 0);  // Black
    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, wheelOffsetZ - wheelHeight - 0.001);
    drawCylinder(wheelRadius, wheelHeight, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, wheelOffsetZ - wheelHeight - 0.001);
    drawCylinder(0.1, wheelHeight, 20);
    glPopMatrix();

    // right front wheel disk.
    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, wheelOffsetZ - wheelHeight - 0.001);
    drawDisk(0.1, wheelRadius, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, wheelOffsetZ - 0.001);
    drawDisk(0.1, wheelRadius, 20, 1);
    glPopMatrix();

    // right front hub cap
    glColor3f(0.45, 0.45, 0.45);
    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, wheelOffsetZ - wheelHeight - 0.001);
    drawDisk(0, 0.1, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, wheelOffsetZ - 0.001);
    drawDisk(0, 0.1, 20, 1);
    glPopMatrix();

    // left rear wheel
    glColor3f(0, 0, 0);  // Black
    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, -wheelOffsetZ + 0.001); // Opposite side
    drawCylinder(wheelRadius, wheelHeight, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, -wheelOffsetZ + 0.001); // Opposite side
    drawCylinder(0.1, wheelHeight, 20);
    glPopMatrix();

    // left rear disks
    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, -wheelOffsetZ + 0.001);
    drawDisk(0.1, wheelRadius, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, -wheelOffsetZ + wheelHeight + 0.001);
    drawDisk(0.1, wheelRadius, 20, 1);
    glPopMatrix();

    // left rear hub caps
    glColor3f(0.45, 0.45, 0.45);
    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, -wheelOffsetZ + 0.001);
    drawDisk(0, 0.1, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, -wheelOffsetZ + wheelHeight + 0.001);
    drawDisk(0, 0.1, 20, 1);
    glPopMatrix();

    // left front wheel
    glColor3f(0, 0, 0);  // Black
    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, -wheelOffsetZ + 0.001); // Opposite side
    drawCylinder(wheelRadius, wheelHeight, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, -wheelOffsetZ + 0.001); // Opposite side
    drawCylinder(0.1, wheelHeight, 20);
    glPopMatrix();

    // left front disks
    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, -wheelOffsetZ + 0.001);
    drawDisk(0.1, wheelRadius, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, -wheelOffsetZ + wheelHeight + 0.001);
    drawDisk(0.1, wheelRadius, 20, 1);
    glPopMatrix();

    // left front hub caps
    glColor3f(0.45, 0.45, 0.45);
    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, -wheelOffsetZ + 0.001);
    drawDisk(0, 0.1, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, -wheelOffsetZ + wheelHeight + 0.001);
    drawDisk(0, 0.1, 20, 1);
    glPopMatrix();


    // Draw the air curtain in front of the bus
    glColor3f(0.5, 0.5, 0.5); // Blue color for the air curtain
    glPushMatrix();
    glTranslatef(1.27, -0.16, 0.0); // Position the air curtain in front of the bus

    // Define the vertices for the front air curtain as a smaller polygon
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.06, 0.11); // Top-right
    glVertex3f(0.0, -0.06, 0.11); // Bottom-right
    glVertex3f(0.0, -0.06, -0.11); // Bottom-left
    glVertex3f(0.0, 0.06, -0.11); // Top-left
    glEnd();

    glPopMatrix();

    // Draw the air curtain at the back of the bus
    glColor3f(0.5, 0.5, 0.5); // Blue color for the air curtain
    glPushMatrix();
    glTranslatef(-1.01, -0.16, 0.0); // Position the air curtain at the back of the bus

    // Define the vertices for the back air curtain as a small rectangle
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.06, 0.15); // Top-right
    glVertex3f(0.0, -0.06, 0.15); // Bottom-right
    glVertex3f(0.0, -0.06, -0.15); // Bottom-left
    glVertex3f(0.0, 0.06, -0.15); // Top-left
    glEnd();

    glPopMatrix();


    /*glPushMatrix();  // Save the current matrix
    glTranslatef(0.5f, -0.5f, -0.3f);  // Translate the quad to the center of rotation
    glRotatef(90, 100.0f, -0.5f, 1.0f);  // Rotate the quad 90 degrees around the Z-axis
    glTranslatef(0.0f, 0.4f, 0.0f);  // Translate the quad back to its original position

    glBegin(GL_QUADS);
    glVertex2f(-2.0f, -0.5f);  // Bottom-left
    glVertex2f(-2.0f, 0.5f);   // Top-left
    glVertex2f(2.0f, 0.5f);    // Top-right
    glVertex2f(2.0f, -0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();  // Save the current matrix
    glTranslatef(0.5f, -0.48f, -0.3f);  // Translate the quad to the center of rotation
    glRotatef(90, 200.0f, -0.5f, 1.0f);  // Rotate the quad 90 degrees around the Z-axis
    glTranslatef(0.0f, 0.4f, 0.0f);  // Translate the quad back to its original position

    // Set color to white for the lines
    glColor3f(1.0f, 1.0f, 1.0f);

    // Set the line width to make it thicker
    glLineWidth(50.0f);  // Adjust the line width as needed

    // Draw white lines on the road using GL_LINES
    glBegin(GL_LINES);
    // First white line
    glVertex2f(-2.0f, 0.0f);  // Left point of the line
    glVertex2f(2.0f, 0.0f);   // Right point of the line
    */


    // You can add more lines as needed
    glEnd();

    glPopMatrix();  // Restore the previous matrix 


}

void drawTextOnBusSide() {
    // Set the position and size of the text
    GLfloat xPos = -0.5f;  // X-coordinate of the text
    GLfloat yPos = -0.2f;  // Y-coordinate of the text
    GLfloat textScale = 0.0015f;  // Text size

    // Set the color for the text (you can change this)
    glColor3f(0.0, 0.0, 0.0);  // White color

    glPushMatrix();
    glTranslatef(xPos, yPos, 0.31);  // Translate to the text position

    glScalef(textScale, textScale, textScale);  // Scale the text size

    char text[] = "Tesla S'Bus";  // Text to be displayed

    // Use a thicker font
    glLineWidth(3.0);  // Adjust line width to make it thicker

    // Render each character of the text using GLUT stroke fonts
    for (int i = 0; i < strlen(text); i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    }

    glPopMatrix();
}

void drawTextOnRightBusSide() {
    // Set the position and size of the text
    GLfloat xPos = 0.5f;  // X-coordinate of the text
    GLfloat yPos = -0.2f;  // Y-coordinate of the text
    GLfloat textScale = 0.0015f;  // Text size

    // Set the color for the text (you can change this)
    glColor3f(0.0, 0.0, 0.0);  // White color

    glPushMatrix();
    glTranslatef(xPos, yPos, -0.31);  // Translate to the text position

    glScalef(textScale, textScale, textScale);  // Scale the text size

    char text[] = "Tesla S'Bus";  // Text to be displayed

    // Use a thicker font
    glLineWidth(3.0);  // Adjust line width to make it thicker
    // Rotate the text 180 degrees to flip it
    glRotatef(180.0, 0.0, 1.0, 0.0);  // Rotate around the Y-axis to flip horizontally

    // Render each character of the text using GLUT stroke fonts
    for (int i = 0; i < strlen(text); i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    }

    glPopMatrix();
}

static GLfloat theta[] = { 0.0,0.0,0.0 };
static GLint axis = 2;
static GLdouble viewer[] = { 0.0, 0.0, 3.0 }; /* initial viewer location */


void resetView()
{
    distance = (float)nearClip + (float)((farClip - nearClip) / 2.0);
    twistAngle = 0.0f;	/* rotation of viewing volume (camera) */
    incAngle = 0.0f;
    azimAngle = 0.0f;
}

void flythru1(float distance, float azimuth, float incidence,
    float twist)
{
    glTranslatef(-distance / 5.0, 0.0f, 0.0f);
    //printf("DISTANCE: %d\n", distance);
    glRotatef(-twist, 0.0f, 0.0f, 1.0f);
    glRotatef(-incidence, 1.0f, 0.0f, 0.0f);
    glRotatef(-azimuth, 0.0f, 1.0f, 0.0f);
}

void display() {
    glClearColor(0.0, 0.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set light position
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Set light direction
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);

    // Set cutoff angle
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff_angle);

    // Draw the bus
    // non orthogonal object. comment out for orthogonal view
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.33, 0.1, 5);

    gluLookAt(cameraX, cameraY, cameraZ, targetX, targetY, targetZ, upX, upY, upZ);

    // Rotate the bus
    glRotatef(spin, 0.0, 1.0, 0.0);

    flythru1(distance, azimAngle, incAngle, twistAngle);

    // Enable or disable texture based on the enableTexture variable
    if (enableTexture) {
        glEnable(GL_TEXTURE_2D);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }

    drawBus();

    // Call the function to draw text on the side of the bus
    drawTextOnBusSide();
    drawTextOnRightBusSide();

    // Disable lighting after drawing objects
    //glDisable(GL_LIGHTING);

    glFlush();
    glutSwapBuffers();
}
void
output(GLfloat x, GLfloat y, char* text)
{
    char* p;

    glPushMatrix();
    glTranslatef(x, y, 0);
    for (p = text; *p; p++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
}

void idle() {
    spin += 2;
    if (spin > 360.0) {
        spin -= 360.0;
    }
    glutPostRedisplay();
}

void motion(int x, int y)
{

    switch (action)
    {
    case MOVE_EYE:
        /* Adjust the eye position based on the mouse position */
        azimAngle += (float)(x - xStart);
        incAngle -= (float)(y - yStart);
        break;
    case TWIST_EYE:
        //    /* Adjust the eye twist based on the mouse position */
        //    //twistAngle = mod(twistAngle+(x - xStart), 360.0);
        twistAngle = (float)(twistAngle + (int)(x - xStart) % 360);
        break;
    case ZOOM:
        /* Adjust the eye distance based on the mouse position */
        distance -= (float)(10.0 * (y - yStart) / width);
        break;
    default:
        break;
    }

    /* Update the stored mouse position for later use */
    xStart = x;
    yStart = y;


    glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{
    printf("BTN: %d, STATE: %d, EYE: %d\n", btn, state, eye);
    if (state == GLUT_DOWN)
    {
        switch (btn)
        {
        case GLUT_LEFT_BUTTON:
            if (eye == 0) action = TWIST_EYE;
            if (eye == 1) action = MOVE_EYE;
            if (eye == 2) action = ZOOM;
            break;
        case GLUT_RIGHT_BUTTON:
            action = ZOOM;
            break;
        default:
            break;
        }
        xStart = x;
        yStart = y;
    }
    else
    {
        action = MOVE_NONE;
    }
}

GLvoid keyboard(GLubyte key, GLint x, GLint y)
{
    static GLboolean flat = GL_FALSE;

    switch (key) {
    case 'e':  // MOVE_EYE to TWIST_EYE
        if (eye == 0)
            eye = 1;
        else
            eye = 0;
        break;

    case 'f':  // front view
        cameraX = 2.0f;
        cameraY = -0.1f;
        cameraZ = 0.0f;

        spin = 0;

        upX = 0.0f;
        upY = 1.0f;
        upZ = 0.0f;

        glutPostRedisplay();
        break;

    case 'b':  // back view
        cameraX = -2.0f;
        cameraY = 0.0f;
        cameraZ = 0.0f;

        spin = 0;

        upX = 0.0f;
        upY = 1.0f;
        upZ = 0.0f;
        glutPostRedisplay();
        break;

    case 'r':  // right view (BUS'S RIGHT)
        cameraX = 0.0f;
        cameraY = 0.0f;
        cameraZ = 4.0f;

        spin = 0;

        upX = 0.0f;
        upY = 1.0f;
        upZ = 0.0f;
        glutPostRedisplay();
        break;

    case 'l':  // left view (BUS'S LEFT)
        cameraX = 0.0f;
        cameraY = 0.0f;
        cameraZ = -4.0f;

        spin = 0;

        upX = 0.0f;
        upY = 1.0f;
        upZ = 0.0f;
        glutPostRedisplay();
        break;

    case 't':  //top view
        cameraX = 0.0f;
        cameraY = 3.0f;
        cameraZ = 0.0f;

        spin = 0;

        upX = -1.0f;
        upY = 0.0f;
        upZ = 0.0f;
        glutPostRedisplay();
        break;
    case 'u':  // under view
        cameraX = 0.0f;
        cameraY = -2.0f;
        cameraZ = 0.0f;

        spin = 0;

        upX = 1.0f;
        upY = 0.0f;
        upZ = 0.0f;
        glutPostRedisplay();
        break;

    case 'i':  // Isometric view
        cameraX = 2.0f;
        cameraY = 2.0f;
        cameraZ = 2.0f;

        spin = 0;

        upX = 0.0f;
        upY = 1.0f;
        upZ = 0.0f;
        glutPostRedisplay();
        break;
    case ' ':
        resetView();
        glutPostRedisplay();
        break;

    case 'q':
    case KEY_ESC:	/* Exit when the Escape key or q is pressed */
        exit(0);

    case '1': // Move light to the front left
        light_position[0] = 1.0f;
        light_position[2] = 1.0f;
        glutPostRedisplay();
        break;

    case '2': // Move light to the front middle
        light_position[0] = 0.0f;
        light_position[2] = 1.0f;
        glutPostRedisplay();
        break;

    case '3': // Move light to the front right
        light_position[0] = 0.5f;
        light_position[2] = 1.0f;
        glutPostRedisplay();
        break;

    case '4': // Move light to the middle side left
        light_position[0] = -1.0f;
        light_position[2] = 0.0f;
        glutPostRedisplay();
        break;

    case '5': // Move light to the middle
        light_position[0] = 0.5f;
        light_position[2] = 1.0f;
        glutPostRedisplay();
        break;

    case '6': // Move light to the middle side right
        light_position[0] = 1.5f;
        light_position[2] = 0.0f;
        glutPostRedisplay();
        break;

    case '7': // Set light angle to top
        cutoff_angle = 30.0f; // Adjust the angle as needed
        glutPostRedisplay();
        break;

    case '8': // Set light angle to middle
        cutoff_angle = 15.0f; // Adjust the angle as needed
        glutPostRedisplay();
        break;

    case '9': // Set light angle to bottom
        cutoff_angle = 5.0f; // Adjust the angle as needed
        glutPostRedisplay();
        break;

    case 'p': // Move light to the middle side bottom
        light_position[0] = -1.1f;
        light_position[2] = 1.0f;
        glutPostRedisplay();
        break;

    case 'x': // Toggle texture on and off
        enableTexture = !enableTexture; // Toggle the texture state
        glutPostRedisplay();
        break;

    }
}

GLvoid processSpecialKeys(int key, int x, int y) {

    switch (key) {
    case GLUT_KEY_DOWN:

        glutIdleFunc(NULL);
        glutPostRedisplay();
        break;
    case GLUT_KEY_UP:

        glutIdleFunc(idle);
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:
        cameraX += 1.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT:
        cameraX -= 1.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_F1:
        cameraX = -2.0f;
        cameraY = 0.0f;
        cameraZ = 0.0f;

        spin = 0;

        upX = 0.0f;
        upY = 1.0f;
        upZ = 0.0f;
        glutPostRedisplay();
        break;
    }
}


void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glFrustum(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
            2.0 * (GLfloat)h / (GLfloat)w, -10, 10);
    else
        glFrustum(-1.0 * (GLfloat)w / (GLfloat)h,
            1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0, -5.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    width = glutGet(GLUT_SCREEN_WIDTH);
    height = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(width / 4, height / 4);
    glutInitWindowSize(width / 2, height / 2);
    glutCreateWindow("Adnan Harris BUS Version 2");
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // Load the texture image

    int textureWidth, textureHeight, channels;
    unsigned char* textureData = stbi_load("../../../../source/repos/PROJECT 3/PROJECT 3/buss.png", &textureWidth, &textureHeight, &channels, 0);
    if (textureData) {
        // Enable texture mapping and generate a texture ID
        glEnable(GL_TEXTURE_2D);
        GLuint textureID;
        glGenTextures(1, &textureID);

        // Bind the texture ID
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set the texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Specify the texture image
        if (channels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        }
        else if (channels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        }



        // Free the texture data
        stbi_image_free(textureData);

        // Set the texture environment and bind the texture
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    else {
        printf("Failed to load texture image.\n");
    }

    // Set the lighting properties
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff_angle);


    // Specify the material properties
    GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.33, 0.1, 5);

    // Initialize the viewing parameters
    resetView();


    // Register callback functions
    glutDisplayFunc(display);

    //glutIdleFunc(idle);
    glutReshapeFunc(myReshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(processSpecialKeys);

    // Enter the main loop
    glutMainLoop();

    return 0;
}
