#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <vector>

#define DT GetFrameTime()
#define RAY_MAX 10
#define DIST_MAX 20

const int GRID_SIZE = 40;
#define MAP_WIDTH 20
#define MAP_HEIGHT 20
char map[MAP_WIDTH][MAP_WIDTH] = 
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

Vector2 operator+(Vector2 const& a, Vector2 const& b)
{
    return {a.x + b.x, a.y + b.y};
}

Vector2 operator+=(Vector2 const& a, Vector2 const& b)
{
    return {a.x + b.x, a.y + b.y};
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
    return {(float)cos(rotation), (float)sin(rotation)};
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

    void Draw()
    {
        DrawLineV(origin * GRID_SIZE, (origin + dir * length) * GRID_SIZE, YELLOW);
    }

    float shoot()
    {
        Vector2 xTarget;
        Vector2 yTarget;
        Vector2 xOffset;
        Vector2 yOffset;
        Vector2 xOffdir;
        Vector2 yOffdir;
    
        /*===========VERTICAL===========*/
        int xNeg = 0;
        if (dir.x > 0)
        {
            xOffset.x  = (float)((int)origin.x + 1) - origin.x;
            xOffdir.x = 1;
        }
        else
        {
            xOffset.x  = (float)((int)origin.x) - origin.x;
            xOffdir.x = -1;
            xNeg = -1;
        }

        /*==========HORIZONTAL==========*/
        int yNeg = 0;
        if (dir.y > 0)
        {
            yOffset.y  = (float)((int)origin.y + 1) - origin.y;
            yOffdir.y = 1;
        }
        else
        {
            yOffset.y  = (float)((int)origin.y) - origin.y;
            yOffdir.y = -1;
            yNeg = -1;
        }

        xTarget = xOffset;
        yTarget = yOffset;
        for (int i = 0; i < DIST_MAX; i++)
        {
            xTarget.y = xTarget.x / dir.x * dir.y;
            yTarget.x = yTarget.y / dir.y * dir.x;

            //printf("%d: %c\n", i + 1, map[(int)(xTarget + origin).y][(int)(xTarget + origin).x]);
            if (map[(int)(xTarget + origin).y][(int)(xTarget + origin).x + xNeg] == '#') break;
            if (map[(int)(yTarget + origin).y][(int)(yTarget + origin).x + yNeg] == '#') break;

            xTarget.x += xOffdir.x;
            yTarget.y += yOffdir.y;
            DrawCircleV((origin + xTarget) * GRID_SIZE, 5, YELLOW);
            DrawCircleV((origin + yTarget) * GRID_SIZE, 5, RED);
        }

        DrawLineV(origin * GRID_SIZE, (origin + xTarget) * GRID_SIZE, BLACK);
        DrawLineV(origin * GRID_SIZE, (origin + yTarget) * GRID_SIZE, BLACK);

        return Vector2Distance({0, 0}, xTarget);
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
    float radius;
    Vector2 d;
    float rotationSpeed;
    float movementSpeed;
    float FOV;
    int ray_count;
    float ray_gap;

    std::vector<Ray2> rays;

    public:
    Player(Vector2 _pos, float _radius) 
    : Object(_pos), radius(_radius) {
        rotationSpeed = 5;
        movementSpeed = 5;
        FOV = PI/2;
        ray_count = 1;
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

        float ray_angle = 0;//-(FOV / 2);
        ray_angle += rotation;
        for (int i = 0; i < ray_count; i++)
        {
            rays[i].origin = pos;
            rays[i].angle = ray_angle;
            rays[i].dir = rad_to_vec(ray_angle);     
            rays[i].length = 0;       
            ray_angle += ray_gap;
        }
    }

    void DrawMapSprite()
    {
        DrawCircleV(pos * GRID_SIZE, radius * GRID_SIZE, WHITE);

        for (Ray2 r : rays)
        {
            r.shoot();
            //r.Draw();
            //printf("%f %f\n", r.dir.x, r.dir.y);
            //printf("%.2f %.2f to %.2f %.2f\n", r.origin.x, r.origin.y, (r.origin + r.dir * r.length).x, (r.origin + r.dir * r.length).y);
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

void DrawMap()
{
    
    for (int i = 0; i < 20; i++)
    for (int j = 0; j < 20; j++)
    {
        DrawRectangle(j * GRID_SIZE + 1, i * GRID_SIZE + 1, GRID_SIZE - 2, GRID_SIZE - 2, map[i][j] == ' ' ? GRAY : GREEN);
    }
    p.DrawMapSprite();
}

void Draw()
{
    ClearBackground(BLACK);
    DrawMap();
}