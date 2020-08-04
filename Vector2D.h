#pragma once
#include <iostream>

class Vector2D
{
public:
    float x;
    float y;
    Vector2D();
    Vector2D(float x, float y);

    Vector2D &Add(const Vector2D &vec);
    Vector2D &Subtract(const Vector2D &vec);
    Vector2D &Multiply(const Vector2D &vec);
    Vector2D &Divide(const Vector2D &vec);
    friend Vector2D &operator+(Vector2D &v1, const Vector2D &v2);
    friend Vector2D &operator-(Vector2D &v1, const Vector2D &v2);
    friend Vector2D &operator*(Vector2D &v1, const Vector2D &v2);
    friend Vector2D &operator/(Vector2D &v1, const Vector2D &v2);

    Vector2D &operator+=(const Vector2D &vec);
    Vector2D &operator-=(const Vector2D &vec);
    Vector2D &operator*=(const Vector2D &vec);
    Vector2D &operator/=(const Vector2D &vec);

    bool operator==(Vector2D &v1);
    bool operator==(int number);
    bool operator!=(Vector2D &v1);
    void Zero();
    static Vector2D &right();
    static Vector2D &left();
    static Vector2D &up();
    static Vector2D &down();
    float Length();
    int getAngle();
    Vector2D getDirection();
    Vector2D &operator*(const int &i);
    Vector2D &operator/(const int &i);

    friend std::ostream &operator<<(std::ostream &stream, const Vector2D &vec);
};
