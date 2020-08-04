#define _USE_MATH_DEFINES
#include "Vector2D.h"
#include <cmath>

Vector2D &Vector2D::right() {
  static Vector2D right(1, 0);
  return right;
};

Vector2D &Vector2D::left() {
  static Vector2D left(-1, 0);
  return left;
};

Vector2D &Vector2D::up() {
  static Vector2D up(0, -1);
  return up;
};

Vector2D &Vector2D::down() {
  static Vector2D down(0, 1);
  return down;
};

Vector2D::Vector2D()
{
    x = 0.0f;
    y = 0.0f;
}

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2D &Vector2D::Add(const Vector2D &vec)
{
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}
Vector2D &Vector2D::Subtract(const Vector2D &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}
Vector2D &Vector2D::Multiply(const Vector2D &vec)
{
    this->x *= vec.x;
    this->y *= vec.y;
    return *this;
}
Vector2D &Vector2D::Divide(const Vector2D &vec)
{
    this->x /= vec.x;
    this->y /= vec.y;
    return *this;
}

Vector2D &operator+(Vector2D &v1, const Vector2D &v2)
{
    return v1.Add(v2);
}
Vector2D &operator-(Vector2D &v1, const Vector2D &v2)
{
    return v1.Subtract(v2);
}
Vector2D &operator*(Vector2D &v1, const Vector2D &v2)
{
    return v1.Multiply(v2);
}
Vector2D &operator/(Vector2D &v1, const Vector2D &v2)
{
    return v1.Divide(v2);
}
bool Vector2D::operator==(Vector2D &v1)
{
    return v1.x == this->x && v1.y == this->y;
}
bool Vector2D::operator==(int number)
{
    return number == this->x && number == this->y;
}

bool Vector2D::operator!=(Vector2D &v1)
{
    return !(*this == v1);
}
Vector2D &Vector2D::operator+=(const Vector2D &vec)
{
    return this->Add(vec);
}
Vector2D &Vector2D::operator-=(const Vector2D &vec)
{
    return this->Subtract(vec);
}
Vector2D &Vector2D::operator*=(const Vector2D &vec)
{
    return this->Multiply(vec);
}
Vector2D &Vector2D::operator/=(const Vector2D &vec)
{
    return this->Divide(vec);
}

Vector2D &Vector2D::operator*(const int &i)
{
    this->x *= i;
    this->y *= i;
    return *this;
}

Vector2D &Vector2D::operator/(const int &i)
{
    this->x /= i;
    this->y /= i;
    return *this;
}
float Vector2D::Length()
{
    return sqrt(pow(this->x, 2) + pow(this->y, 2));
}

int Vector2D::getAngle()
{
    return (std::atan2(y, x) * 180 / M_PI);
}

Vector2D Vector2D::getDirection()
{
    auto sign = [](float x) {
        return ((0 < x) - (0 > x));
    };
    return Vector2D(sign(this->x), sign(this->y));
}

void Vector2D::Zero()
{
    this->x = 0;
    this->y = 0;
}

std::ostream &operator<<(std::ostream &stream, const Vector2D &vec)
{
    stream << "(" << vec.x << "," << vec.y << ")";
    return stream;
}
