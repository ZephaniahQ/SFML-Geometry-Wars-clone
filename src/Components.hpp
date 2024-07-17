#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>
#include <iostream>

struct BorderColision{bool left{false}, right{false}, up{false}, down{false};};

class CTransform{
public:
    Vec2 pos        =   { 0.0, 0.0 };
    Vec2 velocity   =   { 0.0, 0.0 };
    float angle     =   0;

    CTransform(const Vec2 & p, const Vec2 & v, float a )
        : pos(p), velocity(v), angle(a)
    {

    }
    
};

class CShape{
public:
    sf::CircleShape circle;
    float m_FR{}, m_FG{}, m_FB{};
    float m_alphaChannel = 0;
    int   m_points{64};
    float m_radius{};
    sf::Color outlineColor;
    float m_OT;

    CShape(float radius, int points,const float FR, const float FG, const float FB, float alphaChannel, const sf::Color & outline, float thickness)
        : circle(radius, points), m_FR(FR), m_FG(FG), m_FB(FB), m_alphaChannel(alphaChannel), m_points(points), m_radius(radius), outlineColor(outline), m_OT(thickness)
    {
        circle.setFillColor(sf::Color(m_FR,m_FG,m_FB,m_alphaChannel));
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);
        circle.setOrigin(radius, radius);
    }

    CShape(float radius, int points,const sf::Color & fill, const sf::Color & outline, float thickness)
        : circle(radius, points), m_points(points), m_radius(radius), outlineColor(outline), m_OT(thickness)
    {
        circle.setFillColor(fill);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);
        circle.setOrigin(radius, radius);
    }

    void changeAlpha(float newalphaChannel)
    {
        m_alphaChannel = newalphaChannel;
        circle.setFillColor(sf::Color(m_FR,m_FG,m_FB,static_cast<sf::Uint8>(m_alphaChannel)));
        //std::cout << "changed alpha new circle: "<< m_FR << "," << m_FG << "," << m_FB << "," << m_alphaChannel << "\n";
    }
};

class CCollision
{
public:
    float radius = 0;
    CCollision(float r)
        :   radius(r) {}

    BorderColision BColision;

    void resetBcol()
    {
        BColision.up = false;
        BColision.down = false;
        BColision.left = false;
        BColision.right = false;
    }

};

class CScore
{
public:
    int score = 0;
    CScore(int s)
        : score(s) {}
};

class CLifespan
{
public:
    int remaining   = 0;
    int total       = 0;

    CLifespan(int total)
        : remaining(total), total(total) {}
};

class CInput
{
public:
    bool up     = false;
    bool left   = false;
    bool right  = false;
    bool down   = false;
    bool shoot  = false;

    CInput(){}

    void clear()
    {
        up     = false;
        left   = false;
        right  = false;
        down   = false;
        shoot  = false;
    }
};