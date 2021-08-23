//
//  main.cpp
//  Brick_Break
//
//  Created by Ohub on 7/29/21.
//  Copyright © 2021 Revilo. All rights reserved.
//
//
// COMPILE: g++ -Wall -Wno-deprecated-declarations -O3 -o brick main.cpp -DMAC -framework OPENGL -framework GLUT
// EXECUTE: ./brick

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif



//TASK TASK TASK TASK TASK TASK TASK TASK TASK TASK TASK TASK

//CORE
//#1    CLEAN UP UN-NEEDED CODE         [DONE]
//#2    RE-SIZE BOARD           [SCRAPPED]
//#3    SET PADDLE TO RENDER ON BOTTOM OF THE SCREEN        [DONE]
//#3A   PADDLE MOVES LEFT AND RIGHT                                 [DONE]
//#4    SET BALL TO RENDER ON PADDLE                                            [DONE]
//#4A   SET BALL TO RENDER ON PADDLE DYNAMICALLY            [DONE]

//#5    SET BALL TO LAUNCH FROM PADDLE BASED ON WHERE MOUSE WAS CLICKED
//#5A   SET BALL TO BOUNCE OFF EDGES OF PLAYING ZONE    [DONE]
//#5B   SET BALL TO RESET WHEN GOING BELOW PADDLE       [DONE]
//#5C   SET BALL TO HIT PADDLE AND FREEZE UNTIL SHOT AGAIN  [DONE]

//#6    ADD BRICK RENDERING         [DONE]
//#6A   TEST BRICK COLLISION                    [DONE]
//#6B   ADD BRICK REMOVAL AFTER HIT     [DONE]

//#7    CREATE BRICK PATTERN RENDERING (CREATE MULTIPLE PATTERNS FOR ROWS THAT ALLOWS A RANDOM GENERATION TO SELECT PATTERN)    [DONE]
//#7A   IF A BRICK IS HIT ANOTHER BRICK CANNOT BE HIT IN THAT SAME COLLISION    [DONE]



//#8    TEST GAME FOR CORE MECHANICS FUNCTIONALITY

//ADDITIONAL NON-CORE TASK

// CLEAN UP PRODUCTION COMMENTS
//#9   ADD LEVELS (WHEN FINAL BRICK DESTROYED CREATE NEW PATTERN AND RESET BALL)
//#10  ADD SCORE
//#11  ADD VARIOUS BRICK TYPES
//#12  ADD VARIOUS POWER UPS
//#13  ADD END OF LEVEL ANIMATION
//#14  ADD TEXT TO SCREEN
//#15  ADD MENU OR KEY SELECTION TO CHANGE BACKGROUNDS, BALL COLOR, ETC.
//#16  NORMALIZE LAUNCH VALUES SO A CONSISTENT SPEED IS ACHIEVED WITH EACH LAUNCH ONLY DIRECTION IS RECORDED
//#17 ADD BALL COUNT









// Constants
const int SIZE = 500;

const int HALF = SIZE/2;
const int TENTH = SIZE/10;

const int RADIUS = 10;

const int BRICK = 40;
const int LENGTH = 15;

// Booleans
bool Launched = false;
bool collide = false;



// Variables
float Speed = 5;    //Ball speeds
float Stop = 0;

float Px, Py;   //Ball coordinates
float Vx, Vy;   //Ball velocities

float LeftPos, RightPos;    //Paddle position

float BallPosX;      //Position of setting ball on paddle
float BallPosY;

int x_cord;     //Position of ball
int y_cord;

int endX;   //Used for launching ball calculations
int endY;
int initialX;
int initialY;
int finalX;
int finalY;

int bricks[30][4];       //Brick arrays
int brick_allow[30];    //Allows brick to render
int brickLoc[30][2];    //Contains location info for brick

int collideAllow;   //Used to remove multi brick collision

int xBrickLoc;  //Used to set brick locations
int yBrickLoc;



//---------------------------------------
// Reset ball at center
//---------------------------------------
void reset_ball()
{
    
    
    BallPosX = LeftPos - RADIUS*3.5;
    BallPosY = -HALF + 3*RADIUS;
    
    x_cord = BallPosX;
    y_cord = BallPosY;
    
    Speed = 0;
    Vx = 0;
    Vy = 0;
}

void collide_ball_left()     //Ball was launched and hit paddle, move paddle left
{
    BallPosX -= RADIUS*2;
    
    x_cord = BallPosX;
    y_cord = BallPosY;
    
    Speed = 0;
    Vx = 0;
    Vy = 0;
}

void collide_ball_right()     //Ball was launched and hit paddle, move paddle right
{
    BallPosX += RADIUS*2;
    
    x_cord = BallPosX;
    y_cord = BallPosY;
    
    Speed = 0;
    Vx = 0;
    Vy = 0;
}

//-------------------------
// Brick Generation
//-------------------------
void generation(int brick, int position, int value)
{
    brickLoc[brick][position] = value;
    
}

//-------------------------
// Collision check for side of bricks
//-------------------------
bool Collision(float BallPositionZ, int leftVertex, int rightVertex)
{
    if (((leftVertex <= (BallPositionZ + RADIUS)) && (rightVertex >= (BallPositionZ + RADIUS)))
        ||
        ((leftVertex <= (BallPositionZ - RADIUS)) && (rightVertex >= (BallPositionZ - RADIUS)))
        ||
        ((leftVertex <= (BallPositionZ)) && (rightVertex >= (BallPositionZ))))
    {
        return true;
        
    }
    return false;
    
    
}


//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
    // Initialize OpenGL
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-HALF, HALF, -HALF, HALF, -HALF, HALF);
    
    // Initialize pong board
    srand(clock());
    
    LeftPos = TENTH;
    RightPos = -TENTH;
    BallPosX = LeftPos - (LeftPos/2) - 2*RADIUS + (RADIUS/2);
    
    collideAllow = 0;
    xBrickLoc = -200;
    yBrickLoc = 15;
    
    
    for (int i = 0; i < 30; i++)              //BRICK ALLOW CHANGE i BASED ON BRICK COUNT
    {
        xBrickLoc += 40;
        if (i == 10 || i == 20)
        {
            xBrickLoc = -160;
            yBrickLoc += 60;
        }
        
        brick_allow[i] = 1;
        
        for (int j = 0; j < 2; j++)
        {
            generation(i, 0, xBrickLoc);
            generation(i, 1, yBrickLoc);
        }
        
        
    }
    
    
    
    
    reset_ball();
    
}



//-------------------------
// Brick Render
//-------------------------
void brick_render(int brick_select, int x_cordinates, int y_cordinates, float color)
{
    
    int x1 = x_cordinates;
    int x2 = x_cordinates - BRICK;
    int y1 = y_cordinates;
    int y2 = y_cordinates + LENGTH;
    
    for (int i =0; i < 4; i++)  //Record brick coordinates for collision
    {
        if (i == 0)
        {
            bricks[brick_select][i] = x1;
        }
        else if (i== 1)
        {
            bricks[brick_select][i] = x2;
        }
        else if (i== 2)
        {
            bricks[brick_select][i] = y1;
        }
        else if (i== 3)
        {
            bricks[brick_select][i] = y2;
        }
        
    }
    
    
    glBegin(GL_POLYGON);
    glColor3f(0.3, color, 0.1);
    glVertex2f(bricks[brick_select][0], bricks[brick_select][2]);   //bottom right
    glVertex2f(bricks[brick_select][1], bricks[brick_select][2]);   //bottom left
    glVertex2f(bricks[brick_select][1], bricks[brick_select][3]);   //top left
    glVertex2f(bricks[brick_select][0], bricks[brick_select][3]);   //top right
    glEnd();
    
    
    
    
    
}


//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
    Launched = false;
    collide = false;
    
    // Handle mouse down
    if (state == GLUT_DOWN)
    {
        initialX = x;
        initialY = y;
        
    }
    
    
    
    if (state == GLUT_UP)
    {
        finalX = x;
        finalY = y;
        
        if (finalX < initialX)
        {
            endX = -(initialX - finalX);
            endY = initialY - finalY;
        }
        
        if (finalX > initialX)
        {
            endX = (finalX - initialX);
            endY = initialY - finalY;
        }
        
        endX = endX/15;
        endY = endY/15;
        
        Vx = endX;
        Vy = endY;
        
        Launched = true;
        Speed = 1;
    }
    
    
    
    glutPostRedisplay();
    
    
}


//---------------------------------------
//Display callback for OpenGL
//---------------------------------------
void display()
{
    
    
    //Clear screen
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //Draw pong board
    glBegin(GL_POLYGON);        //CHANGE COLOR MAYBE ADD TEXTURE
    glColor3f(0.1, 0.1, 0.8);
    glVertex2f(-HALF + RADIUS, -HALF + RADIUS);
    glVertex2f(-HALF + RADIUS,  HALF - RADIUS);
    glVertex2f( HALF - RADIUS,  HALF - RADIUS);
    glVertex2f( HALF - RADIUS, -HALF + RADIUS);
    glEnd();
    
    //Draw paddle
    glBegin(GL_POLYGON);            //CHANGE PADDLE TO RENDER ON BOTTOM OF FRAME, REMOVE OG PADDLES [DONE]
    glColor3f(0.8, 0.8, 0.8);
    glVertex2f(LeftPos + 1*RADIUS, -HALF + 2*RADIUS);
    glVertex2f(RightPos + 1*RADIUS,  -HALF + 2*RADIUS);
    glVertex2f(RightPos + 1*RADIUS, -HALF + 1*RADIUS);
    glVertex2f(LeftPos + 1*RADIUS, -HALF + 1*RADIUS);
    glEnd();
    
    
    //Draw ball
    glBegin(GL_POLYGON);    //SET BALL TO START FROM CENTER OF PADDLE [DONE]
    glColor3f(0.8, 0.1, 0.1);
    glVertex2f(BallPosX - RADIUS, BallPosY - RADIUS);       //TOP LEFT
    glVertex2f(BallPosX - RADIUS, BallPosY + RADIUS);       //TOP RIGHT
    glVertex2f(BallPosX + RADIUS, BallPosY + RADIUS);       //BOTTOM RIGHT
    glVertex2f(BallPosX + RADIUS, BallPosY - RADIUS);       //BOTTOM LEFT
    glEnd();
    
    
    //Brick Render
    for (int i = 0; i < 30; i++)
    {
        if (i % 2 == 0)
        {
            if ( brick_allow[i] ==  1)
            {
                brick_render(i, brickLoc[i][0], brickLoc[i][1], 0.65);
            }
        }
        else
            if ( brick_allow[i] ==  1)
            {
                brick_render(i, brickLoc[i][0], brickLoc[i][1], 0.8);
            }
        
    }
    
    glFlush();
}

//---------------------------------------
// Idle callback for OpenGL
//---------------------------------------
void idle()
{
    collideAllow = 0;
    
    // Move bouncing ball
    BallPosX += Vx * Speed;
    BallPosY += Vy * Speed;
    
    
    // Bounce off walls
    if (BallPosY > HALF - 2*RADIUS)     //Ball hits top wall
    {
        BallPosY = HALF - 2*RADIUS; Vy *= -1;
    }
    
    if (BallPosX > HALF - 2*RADIUS)     //Ball hits right wall
    {
        BallPosX = HALF - 2*RADIUS; Vx*= -1;
    }
    
    if (BallPosX < -HALF + 2*RADIUS)    //Ball hits left wall
    {
        BallPosX = -HALF + 2*RADIUS; Vx*= -1;
        
    }
    
    if (BallPosY < -HALF - 2*RADIUS)     //Ball hits top wall
    {
        Launched = false;
        reset_ball();
        
    }
    
    
    if ((RightPos <= BallPosX) && (LeftPos >= BallPosX) && (BallPosY <= -220))      //Ball collides with paddle
    {
        BallPosY = -220;
        Vx = 0;
        Vy = 0;
        collide = true;
        
    }
    
    
    for (int i = 0; i < 30; i++ )        //Collision for bricks check
    {
        if(collideAllow != 0)
        {
            break;
        }
        
        if (brick_allow[i] == 1)
        {
            if ((Collision(BallPosX, bricks[i][1], bricks[i][0]) == true) && (Collision(BallPosY, bricks[i][2], bricks[i][3]))) //Collision on sides of brick
            {
                collideAllow++;
                Vx*= -1;
                brick_allow[i] = 0;
            }
            
            if ((Collision(BallPosX, bricks[i][1], bricks[i][0]) == true) && ((Collision(BallPosY, bricks[i][2], bricks[i][3]) == true)))//Collision on top or bottom
            {
                collideAllow++;
                Vy *= -1;
                Vx *= -1;
                brick_allow[i] = 0;
            }
            
        }
    }
    
    
    
    
    
    
    
    
    glutPostRedisplay();
}

//---------------------------------------
// Special callback for OpenGL
//---------------------------------------
void special(int key, int x, int y)
{
    // printf("%d\n", key);
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    
    if ((key == 'a') && (RightPos > -HALF))     //Paddle moves left
    {
        LeftPos -= RADIUS*2;
        RightPos -= RADIUS*2;
        
        if (Launched == false)              //Ball still on paddle
        {
            reset_ball();
        }
        else if (collide == true)
        {
            collide_ball_left();
        }
    }
    else if ((key == 'd') && (LeftPos < HALF - 2*RADIUS))       //Paddle moves right
    {
        LeftPos += RADIUS*2;
        RightPos += RADIUS*2;
        
        if (Launched == false)              //Ball still on paddle
        {
            reset_ball();
            
        }
        else if (collide == true)
        {
            collide_ball_right();
        }
    }
    
    
    
    // Redraw objects
    glutPostRedisplay();
}

//+++++++
// Print command menu
//+++++++
void print_menu()
{
    printf("\nGame controls:\n");
    printf("  'a' = Paddle Left\n");
    printf("  'd' = Paddle Right\n");
    printf("  To launch the ball click the ball and drag\n");
    printf("  in the direction you want it to launch then release\n");
    
    
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
    // Create OpenGL window
    glutInit(&argc, argv);
    glutInitWindowSize(SIZE, SIZE);
    glutInitWindowPosition(HALF, HALF);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("Brick Breaker");
    glutMouseFunc(mouse);
    init();
    print_menu();
    
    // Specify callback function
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMainLoop();
    return 0;
}

