#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define DT GetFrameTime()

char map[20][20] = 
{
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',},
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

class Object {
    protected:
    Vector2 pos;
    Vector2 vel;
    float rotation;

    public:
    Object(Vector2 _pos)
    : pos(_pos) {}   
};


class Player : public Object {
    private:
    float radius;
    Vector2 d;
    float rotationSpeed;
    float movementSpeed;

    public:
    Player(Vector2 _pos, float _radius) 
    : Object(_pos), radius(_radius) {
        rotationSpeed = 5;
        movementSpeed = 200;
    }

    void Update()
    {
        vel = {0, 0};
        d = {(float)cos(rotation), (float)-sin(rotation)};

        if (IsKeyDown(KEY_UP))
        {
            vel = d * movementSpeed * DT;
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

    void Draw()
    {
        DrawCircleV(pos * 20, radius, WHITE);
        DrawLineV(pos * 20, pos * 20 + d * radius, RED);
    }
    
};



Player p({1, 1}, 9);

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
        DrawRectangle(j * 20, i * 20, 18, 18, map[i][j] == ' ' ? GRAY : GREEN);
    }
}

void Draw()
{
    ClearBackground(BLACK);
    DrawMap();
    p.Draw();
}