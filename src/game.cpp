#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <vector>

#define DT GetFrameTime()
#define RAY_MAX 10
#define DIST_MAX 10

const int GRID_SIZE = 10;
#define MAP_WIDTH 20
#define MAP_HEIGHT 20
char map[MAP_HEIGHT][MAP_WIDTH] = 
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',},
};

float capFloat(float a, float low, float high)
{
    if (a < low) return low;
    if (a > high) return high;
    return a;
}

Vector2 operator+(Vector2 const& a, Vector2 const& b)
{
    return {a.x + b.x, a.y + b.y};
}

Vector2 operator+=(Vector2 & a, Vector2 const& b)
{
    {a.x += b.x; a.y += b.y;};
    return a;
}

Vector2 operator-(Vector2 const& a, Vector2 const& b)
{
    return {a.x - b.x, a.y - b.y};
}

Vector2 operator*(Vector2 const& a, float s)
{
    Vector2 ret;
    ret.x = a.x * s;
    ret.y = a.y * s;
    return ret;
}

Vector2 rad_to_vec(float rotation)
{
    return {(float)(cos(rotation) + 0.00000001), (float)(sin(rotation) + 0.00000001)};
}

class Object {
    protected:
    Vector2 pos;
    Vector2 vel;
    float rotation;

    public:
    Object(Vector2 _pos)
    : pos(_pos) {}   
};


class Ray2 {
    public:
    Vector2 origin;
    Vector2 dir;
    float length;
    float angle;
    char hitType;

    void Draw()
    {
        DrawLineV(origin * GRID_SIZE, (origin + dir * length) * GRID_SIZE, YELLOW);
    }

    void shoot()
    {
        //Vector2 target;
        Vector2 offset;
        Vector2 xStepSize;
        Vector2 yStepSize;
        Vector2 xStep;
        Vector2 yStep;
        Vector2 len;
        Vector2 lenStep;

        float nTan = dir.y / dir.x;
        float aTan = dir.x / dir.y;
    
        /*===========VERTICAL===========*/
        bool xNeg = 0;
        if (dir.x > 0)
        {
            offset.x  = ceil(origin.x) - origin.x;
            xStepSize.x = 1;
        }
        else
        {
            offset.x  = floor(origin.x) - origin.x;
            xStepSize.x = -1;
            xNeg = 1;
        }
        offset.y = offset.x * nTan;

        xStepSize.y = nTan * xStepSize.x;
        
        xStep = origin + offset;
        len.x = Vector2Length(offset);
        lenStep.x = Vector2Length(xStepSize);
        /*=================================*/


        /*===========HORIZONTAL===========*/
        bool yNeg = 0;
        if (dir.y > 0)
        {
            offset.y  = ceil(origin.y) - origin.y;
            yStepSize.y = 1;
        }
        else
        {
            offset.y  = floor(origin.y) - origin.y;
            yStepSize.y = -1;
            yNeg = 1;
        }
        offset.x = offset.y * aTan;

        yStepSize.x = aTan * yStepSize.y;
        
        yStep = origin + offset;
        len.y = Vector2Length(offset);
        lenStep.y = Vector2Length(yStepSize);
        /*=================================*/

        for (int i = 0; i < 50; i++)
        {
            if (len.x < len.y)
            {   
                hitType = map[(int)capFloat(xStep.y, 0, MAP_HEIGHT - 1)][(int)capFloat(xStep.x, 0, MAP_WIDTH - 1) - xNeg];
                if (hitType != ' ') 
                {
                    length = len.x;
                    break;
                } 

                xStep += xStepSize;
                len.x += lenStep.x;
            }
            else
            {   
                hitType = map[(int)capFloat(yStep.y, 0, MAP_HEIGHT - 1) - yNeg][(int)capFloat(yStep.x, 0, MAP_WIDTH - 1)];
                if (hitType != ' ') 
                {
                    length = len.y;
                    break;
                }
                
                yStep += yStepSize;
                len.y += lenStep.y;
            }

            if (len.x >= DIST_MAX && len.y >= DIST_MAX) 
            {
                length = DIST_MAX;
                break;
            }
        }
    }
};


float fixedRotation(float rotation)
{
    if (rotation < 0)     rotation += 2*PI;
    if (rotation >= 2*PI) rotation -= 2*PI;
    return rotation;
}

class Player : public Object {
    private:
    Vector2 d;
    float radius;
    float rotationSpeed;
    float movementSpeed;
    float FOV;
    int ray_count;
    float ray_gap;

    std::vector<Ray2> rays;

    public:
    Player(Vector2 _pos, float _radius) 
    : Object(_pos), radius(_radius) {
        rotationSpeed = 2;
        movementSpeed = 5;
        FOV = 10 * PI / 18;
        ray_count = 50;
        ray_gap = FOV / ray_count;

        for (int i = 0; i < ray_count; i++)
        {
            rays.push_back(Ray2());
        }
    }

    void Movement()
    {
        vel = {0, 0};
        d = rad_to_vec(rotation);

        if (IsKeyDown(KEY_UP))
        {
            vel = d * movementSpeed * DT;
            if (map[(int)(pos.y)][(int)(pos.x + vel.x)] == '#') vel.x = 0;
            if (map[(int)(pos.y + vel.y)][(int)(pos.x)] == '#') vel.y = 0;
        }

        if (IsKeyDown(KEY_DOWN))
        {
            vel = d * movementSpeed * -DT;
            if (map[(int)(pos.y)][(int)(pos.x + vel.x)] == '#') vel.x = 0;
            if (map[(int)(pos.y + vel.y)][(int)(pos.x)] == '#') vel.y = 0;
        }

        if (IsKeyDown(KEY_RIGHT))
        {
            rotation = fixedRotation(rotation + rotationSpeed * DT);
        }

        if (IsKeyDown(KEY_LEFT))
        {
            rotation = fixedRotation(rotation - rotationSpeed * DT);
        }

        if (IsKeyDown(KEY_SPACE)) system("pause");
        pos = pos + vel;
    }
    void Update()
    {
        Movement();

        float ray_angle = -(FOV / 2);
        ray_angle += rotation;
        for (int i = 0; i < ray_count; i++)
        {
            rays[i].origin = pos;
            rays[i].angle = ray_angle;
            rays[i].dir = rad_to_vec(ray_angle);     
            rays[i].shoot();      
            ray_angle += ray_gap;
        }
    }

    void DrawMapSprite()
    {
        DrawCircleV(pos * GRID_SIZE, radius * GRID_SIZE, WHITE);

        for (Ray2 r : rays)
        {
            r.Draw();
        }
    }

    void Draw()
    {
        float delta = GetScreenWidth() / ray_count;
        float x = 0;
        float height = 0;
        for (Ray2 r : rays)
        {
            height = GetScreenHeight()/2 * (DIST_MAX / r.length);
            DrawRectangle(x, (GetScreenHeight()/2 - height/2), delta, height, GREEN);
            x += delta;
        }
    }
    
};



Player p({10, 10}, 0.5f);

void Setup()
{

}

void Update()
{
    p.Update();
}

void DrawMap(int x, int y)
{
    DrawRectangle(x, y, MAP_WIDTH * GRID_SIZE, MAP_HEIGHT * GRID_SIZE, BLACK);
    for (int i = 0; i < 20; i++)
    for (int j = 0; j < 20; j++)
    {
        DrawRectangle(x + j * GRID_SIZE + 1, y + i * GRID_SIZE + 1, GRID_SIZE - 2, GRID_SIZE - 2, map[i][j] == ' ' ? GRAY : GREEN);
    }
    p.DrawMapSprite();
}

void Draw()
{
    ClearBackground(BLACK);
    p.Draw();

    DrawMap(0, 0);
}