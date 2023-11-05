#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <vector>

#define DT GetFrameTime()

const int GRID_SIZE = 20;
char map[20][20] = 
{
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',}
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
    return {(float)cos(rotation), (float)-sin(rotation)};
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

    void Draw()
    {
        DrawLineV(origin * GRID_SIZE, (origin + dir * length) * GRID_SIZE, YELLOW);
    }
};


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
        ray_count = 80;
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
            rotation -= rotationSpeed * DT;
        }

        if (IsKeyDown(KEY_LEFT))
        {
            rotation += rotationSpeed * DT;
        }
        pos = pos + vel;
    }
    void Update()
    {
        Movement();

        float ray_angle = -(FOV / 2);
        for (int i = 0; i < ray_count; i++)
        {
            rays[i].origin = pos;
            rays[i].dir = rad_to_vec(rotation + ray_angle);
            
            rays[i].length = 10;

            ray_angle += ray_gap;
        }
    }

    void DrawMapSprite()
    {
        DrawCircleV(pos * GRID_SIZE, radius * GRID_SIZE, WHITE);

        for (Ray2 r : rays)
        {
            r.Draw();
            printf("%f %f\n", r.dir.x, r.dir.y);
            //printf("%.2f %.2f to %.2f %.2f\n", r.origin.x, r.origin.y, (r.origin + r.dir * r.length).x, (r.origin + r.dir * r.length).y);
        }
    }
    
};



Player p({1, 1}, 0.5f);

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