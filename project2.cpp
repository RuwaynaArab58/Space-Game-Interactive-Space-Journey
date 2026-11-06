#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
// Global Variables 
GLuint texture;
float shipX = 0.0f;
float shipY = 0.0f;
float astroX = -0.5f;
float astroY = -0.2f;
bool showMessage = false;
float rocketX = 0.0f;
float rocketY = -0.65f;
float rocketAngle = 0.0f;
int scene = 1;
bool gameOver = false;
bool win = false;
void drawRocket();
void drawText(float x, float y, const char* text);
float rocketScale = 1.0f;
float asteroidX[3] = { -0.7f, 0.0f, 0.6f };
float asteroidY[3] = { 0.2f, 0.55f, -0.1f };
float asteroidSpeed[3] = { 0.015f, 0.018f, 0.016f };
int level = 1;


//BMP imege loading function

unsigned char* loadBMP(const char* filename, int* width, int* height) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Cannot open file\n");
        return NULL;
    }

    unsigned char header[54];
    fread(header, 1, 54, file);

    *width = *(int*)&header[18];
    *height = *(int*)&header[22];

    int size = 3 * (*width) * (*height);
    unsigned char* data = (unsigned char*)malloc(size);

    fread(data, 1, size, file);
    fclose(file);

    // convert BGR TO RGB
    for (int i = 0; i < size; i += 3) {
        unsigned char temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }

    return data;
}


void loadTexture() {
    int width, height;
    unsigned char* data = loadBMP("earth4.bmp", &width, &height);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//Circle drawing function


void drawCircle(float cx, float cy, float r)

{

    glBegin(GL_POLYGON);

    for (int i = 0; i < 60; i++)

    {

        float angle = 2 * 3.14159f * i / 60;
        glVertex2f(

            cx + r * cos(angle),
            cy + r * sin(angle)

        );
    }

    glEnd();
}

//Asteroid drawing function


void drawAsteroid(float x, float y)
{
    // glow effect
    glColor3f(0.85f, 0.25f, 0.05f);
    drawCircle(x + 0.01f, y - 0.01f, 0.095f);

    // main body
    glColor3f(1.0f, 0.45f, 0.05f);
    drawCircle(x, y, 0.08f);

    // yellow highlight
    glColor3f(1.0f, 0.85f, 0.15f);
    drawCircle(x - 0.02f, y + 0.02f, 0.045f);

    // details
    glColor3f(0.65f, 0.05f, 0.02f);
    drawCircle(x - 0.03f, y + 0.02f, 0.014f);
    drawCircle(x + 0.03f, y - 0.02f, 0.012f);
    drawCircle(x + 0.015f, y + 0.035f, 0.010f);
}

// Space Background drawing


void drawSpaceBackground()
{

    //color
    glColor3f(0.01f, 0.01f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();

    //large stars
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(-0.90f, 0.85f);
    glVertex2f(-0.75f, 0.50f);
    glVertex2f(-0.55f, 0.75f);
    glVertex2f(-0.30f, 0.25f);
    glVertex2f(-0.10f, 0.60f);
    glVertex2f(0.20f, 0.85f);
    glVertex2f(0.45f, 0.35f);
    glVertex2f(0.70f, 0.70f);
    glVertex2f(0.90f, 0.25f);
    glVertex2f(0.50f, -0.55f);
    glVertex2f(-0.60f, -0.45f);
    glVertex2f(-0.20f, -0.70f);
    glEnd();

    // small stars
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex2f(-0.85f, -0.10f);
    glVertex2f(-0.40f, -0.55f);
    glVertex2f(0.10f, -0.25f);
    glVertex2f(0.35f, 0.10f);
    glVertex2f(0.75f, -0.35f);
    glVertex2f(0.95f, -0.05f);
    glEnd();

    // Moon body
    glColor3f(0.75f, 0.75f, 0.70f);
    drawCircle(0.0f, 0.82f, 0.18f);

    //moon craters
    glColor3f(0.50f, 0.50f, 0.48f);
    drawCircle(-0.05f, 0.88f, 0.03f);
    drawCircle(0.06f, 0.80f, 0.025f);
    drawCircle(0.01f, 0.72f, 0.02f);
}


// moon drawing function

void drawMoon()
{
    glColor3f(0.75f, 0.75f, 0.70f);
    drawCircle(0.0f, 0.85f, 0.18f);

    glColor3f(0.50f, 0.50f, 0.48f);
    drawCircle(-0.05f, 0.90f, 0.03f);
    drawCircle(0.06f, 0.82f, 0.025f);
}




void drawScene2()
{
    drawSpaceBackground();

    for (int i = 0; i < 3; i++)
    {
        drawAsteroid(asteroidX[i], asteroidY[i]);
    }

    glPushMatrix();
    glTranslatef(rocketX, rocketY, 0.0f);
    glRotatef(rocketAngle, 0.0f, 0.0f, 1.0f);
    glScalef(rocketScale, rocketScale, 1.0f);


    drawRocket();
    glPopMatrix();

    
    char levelText[20];
    sprintf(levelText, "Level: %d", level);
    drawText(-0.95f, 0.90f, levelText);

    if (gameOver)
        drawText(-0.18f, 0.0f, "GAME OVER");

    if (win)
        drawText(-0.12f, 0.0f, "YOU WIN!");
}




void drawRocket() {
    // Rocket body
    glColor3f(0.45f, 0.48f, 0.52f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.10f, -0.20f);
    glVertex2f(0.10f, -0.20f);
    glVertex2f(0.10f, 0.20f);
    glVertex2f(-0.10f, 0.20f);
    glEnd();

    // Rocket head
    glColor3f(0.55f, 0.05f, 0.05f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.10f, 0.20f);
    glVertex2f(0.10f, 0.20f);
    glVertex2f(0.0f, 0.35f);
    glEnd();

    // Left wing
    glColor3f(0.45f, 0.02f, 0.02f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.10f, -0.03f);
    glVertex2f(-0.22f, -0.30f);
    glVertex2f(-0.10f, -0.20f);
    glEnd();

    // Right wing
    glBegin(GL_TRIANGLES);
    glVertex2f(0.10f, -0.03f);
    glVertex2f(0.22f, -0.30f);
    glVertex2f(0.10f, -0.20f);
    glEnd();

    // Window 1
    glColor3f(0.0f, 0.35f, 0.75f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 50; i++) {
        float angle = 2 * 3.14159 * i / 50;
        glVertex2f(0.0f + 0.035f * cos(angle),
            0.09f + 0.035f * sin(angle));
    }
    glEnd();

    // Window 2
    glBegin(GL_POLYGON);
    for (int i = 0; i < 50; i++) {
        float angle = 2 * 3.14159 * i / 50;
        glVertex2f(0.0f + 0.035f * cos(angle),
            0.0f + 0.035f * sin(angle));
    }
    glEnd();

    // Engine
    glColor3f(0.15f, 0.15f, 0.15f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.05f, -0.20f);
    glVertex2f(0.05f, -0.20f);
    glVertex2f(0.04f, -0.25f);
    glVertex2f(-0.04f, -0.25f);
    glEnd();

    // Outer flame
    glColor3f(0.85f, 0.25f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.06f, -0.25f);
    glVertex2f(0.06f, -0.25f);
    glVertex2f(0.0f, -0.40f);
    glEnd();

    // Inner flame
    glColor3f(1.0f, 0.75f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.03f, -0.25f);
    glVertex2f(0.03f, -0.25f);
    glVertex2f(0.0f, -0.34f);
    glEnd();
}


void drawAstronaut() {
    // transformation

    glPushMatrix();
    glTranslatef(astroX, astroY, 0);

    // scale size

    glScalef(0.45f, 0.45f, 1.0f);

    // Backpack

    glColor3f(0.0f, 0.75f, 0.95f);

    drawCircle(-0.13f, 0.02f, 0.11f);

    // Helmet outer

    glColor3f(0.95f, 0.95f, 0.98f);

    drawCircle(0.0f, 0.23f, 0.17f);

    // Face glass

    glColor3f(0.08f, 0.10f, 0.45f);

    drawCircle(0.02f, 0.22f, 0.12f);

    // Shine

    glColor3f(1.0f, 1.0f, 1.0f);

    drawCircle(0.08f, 0.29f, 0.025f);

    drawCircle(0.12f, 0.23f, 0.012f);

    // Side headset

    glColor3f(0.0f, 0.85f, 0.95f);

    glBegin(GL_POLYGON);

    glVertex2f(-0.22f, 0.18f);

    glVertex2f(-0.14f, 0.18f);

    glVertex2f(-0.14f, 0.28f);

    glVertex2f(-0.22f, 0.28f);

    glEnd();

    // Body

    glColor3f(0.96f, 0.96f, 0.99f);

    glBegin(GL_POLYGON);

    glVertex2f(-0.12f, 0.05f);

    glVertex2f(0.12f, 0.05f);

    glVertex2f(0.14f, -0.18f);

    glVertex2f(-0.14f, -0.18f);

    glEnd();

    // Chest panel

    glColor3f(0.82f, 0.82f, 0.90f);

    glBegin(GL_POLYGON);

    glVertex2f(0.02f, -0.02f);

    glVertex2f(0.10f, -0.02f);

    glVertex2f(0.10f, -0.10f);

    glVertex2f(0.02f, -0.10f);

    glEnd();

    // Left arm

    glColor3f(0.96f, 0.96f, 0.99f);

    glBegin(GL_POLYGON);

    glVertex2f(-0.12f, 0.02f);

    glVertex2f(-0.20f, -0.02f);

    glVertex2f(-0.18f, -0.12f);

    glVertex2f(-0.10f, -0.08f);

    glEnd();

    // Right arm

    glBegin(GL_POLYGON);

    glVertex2f(0.12f, 0.02f);

    glVertex2f(0.24f, 0.02f);

    glVertex2f(0.24f, -0.08f);

    glVertex2f(0.12f, -0.08f);

    glEnd();

    // Left glove

    drawCircle(-0.18f, -0.12f, 0.04f);

    // Legs

    glBegin(GL_POLYGON);

    glVertex2f(-0.10f, -0.18f);

    glVertex2f(-0.01f, -0.18f);

    glVertex2f(-0.02f, -0.36f);

    glVertex2f(-0.11f, -0.36f);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(0.01f, -0.18f);

    glVertex2f(0.10f, -0.18f);

    glVertex2f(0.11f, -0.36f);

    glVertex2f(0.02f, -0.36f);

    glEnd();

    // Boots

    glColor3f(0.90f, 0.90f, 0.95f);

    glBegin(GL_POLYGON);

    glVertex2f(-0.12f, -0.36f);

    glVertex2f(-0.01f, -0.36f);

    glVertex2f(-0.01f, -0.42f);

    glVertex2f(-0.13f, -0.42f);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(0.01f, -0.36f);

    glVertex2f(0.12f, -0.36f);

    glVertex2f(0.13f, -0.42f);

    glVertex2f(0.01f, -0.42f);

    glEnd();

    glPopMatrix();
}


//keyboard function

void specialKeys(int key, int x, int y)
{
    //speed
    float speed = 0.04f;

    //controls 

    if (scene == 1)
    {
        if (key == GLUT_KEY_RIGHT) astroX += speed;
        else if (key == GLUT_KEY_LEFT) astroX -= speed;
        else if (key == GLUT_KEY_UP) astroY += speed;
        else if (key == GLUT_KEY_DOWN) astroY -= speed;

        if (astroX > -0.45f && astroX < 0.25f &&
            astroY > -0.45f && astroY < 0.20f)
        {
            showMessage = true;
        }
    }

    else if (scene == 2 && !gameOver && !win)
    {
        if (key == GLUT_KEY_RIGHT)
        {
            rocketX += speed;
            rocketAngle = -12;
        }
        else if (key == GLUT_KEY_LEFT)
        {
            rocketX -= speed;
            rocketAngle = 12;
        }
        else if (key == GLUT_KEY_UP)
        {
            rocketY += speed;
            rocketAngle = 0;
        }
        else if (key == GLUT_KEY_DOWN)
        {
            rocketY -= speed;
            rocketAngle = 0;
        }
    }

    glutPostRedisplay();
}

//keyboard function

void keyboard(unsigned char key, int x, int y)
{
    if ((key == 'g' || key == 'G') && showMessage)
    {
        scene = 2;
        showMessage = false;

        rocketX = 0.0f;
        rocketY = -0.65f;
        rocketAngle = 0.0f;

        glutPostRedisplay();
    }
    if (key == 'l' || key == 'L')
    {
        rocketScale += 0.1f;
    }


    if (key == 's' || key == 'S')
    {
        rocketScale -= 0.1f;

        if (rocketScale < 0.3f)
            rocketScale = 0.3f;
    }
    

}


//text function

void drawText(float x, float y, const char* text)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);

    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

//timer function


void Timer(int value)
{
    if (scene == 2 && !gameOver && !win)
    {

        for (int i = 0; i < 3; i++)
        {
            asteroidX[i] += asteroidSpeed[i];

            if (asteroidX[i] > 1.15f)
                asteroidX[i] = -1.15f;

            float dx = rocketX - asteroidX[i];
            float dy = rocketY - asteroidY[i];

            if (sqrt(dx * dx + dy * dy) < 0.16f)
            {
                gameOver = true;

                level = 1;

                asteroidSpeed[0] = 0.015f;
                asteroidSpeed[1] = 0.018f;
                asteroidSpeed[2] = 0.016f;

            }
        }
        // increase the  asteroid Speed for each level
        if (rocketY > 0.70f && rocketX > -0.25f && rocketX < 0.25f)
        {
            if (level == 1)
            {
                level = 2;
               
                asteroidSpeed[0] = 0.030f;
                asteroidSpeed[1] = 0.032f;
                asteroidSpeed[2] = 0.031f;

                rocketX = 0.0f;
                rocketY = -0.65f;
            }

            else if (level == 2)
            {
                level = 3;

                asteroidSpeed[0] = 0.035f;
                asteroidSpeed[1] = 0.040f;
                asteroidSpeed[2] = 0.037f;

                rocketX = 0.0f;
                rocketY = -0.65f;
            }

            else if (level == 3)
            {
                win = true;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, Timer, 0);
}


//display

void display()
{

    glClear(GL_COLOR_BUFFER_BIT);

    if (scene == 1)
    {

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glColor3f(1, 1, 1);

        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-1, -1);
        glTexCoord2f(1, 0); glVertex2f(1, -1);
        glTexCoord2f(1, 1); glVertex2f(1, 1);
        glTexCoord2f(0, 1); glVertex2f(-1, 1);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        drawRocket();
        drawAstronaut();

        if (showMessage)
            drawText(-0.35f, 0.85f, "I will go to the moon!");
    }

    else if (scene == 2)
    {
        drawScene2();
    }

    glutSwapBuffers();
}




void init() {
    glClearColor(0, 0, 0, 1);
    loadTexture();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("space game");

    init();
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(30, Timer, 0);

    glutMainLoop();
    return 0;
}