#include<math.h>
#include"Vec2.h"

Vec2::Vec2() = default;

Vec2::Vec2(float xin, float yin)
    :   x{xin}, y{yin} 
{}

Vec2 Vec2::operator + (const Vec2 & rhs) const
{
    return Vec2(x+rhs.x,y+rhs.y);
}

Vec2 Vec2::operator - (const Vec2 & rhs) const
{
    return Vec2(x-rhs.x,y-rhs.y);
}

Vec2 Vec2::operator * (const float val) const
{
    return Vec2(x*val,y*val);
}

Vec2 Vec2::operator / (const float val) const
{
    return Vec2(x/val,y/val);
}

bool Vec2::operator == (const Vec2 & rhs) const
{
    return rhs.x == x && rhs.y == y;
}

bool Vec2::operator != (const Vec2 & rhs) const
{
    return rhs.x != x || rhs.y != y;
}

void Vec2::operator += (const Vec2 & rhs)
{
    x += rhs.x;
    y += rhs.y;
}

void Vec2::operator -= (const Vec2 & rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator *= (const float val)
{
    x *= val;
    y *= val;
}

void Vec2::operator /= (const float val)
{
    x /= val;
    y /= val;
}

float Vec2::length(){
    return sqrtf((x*x)+(y*y));
}

float Vec2::dist(const Vec2 & rhs) const
{
    return (rhs - * this).length();
}
