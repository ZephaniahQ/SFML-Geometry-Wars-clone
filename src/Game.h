#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

struct WindowConfig { int sW, sH, FL; bool FS;};
struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S;};
struct EnemyConfig  { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI, EMAX; float SMIN, SMAX;};
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S;};

class Game
{
    sf::RenderWindow    m_window;               // the window we will draw to
    EntityManager       m_entities;             // vector of entities to maintain
    sf::Font            m_font;                 // the font we wil use to draw
    sf::Text            m_text;                 // the score text to be darwn to the screen
    WindowConfig        m_windowConfig;
    PlayerConfig        m_playerConfig;
    EnemyConfig         m_enemyConfig;
    BulletConfig        m_bulletConfig;
    sf::Clock           m_deltaClock;
    float               m_deltaTime;
    int                 m_score         = 0;
    int                 m_currentFrame  = 0;
    float               m_rotationAngle = 5;

    bool                m_running       = false;
    bool                m_paused        = false;

    bool                m_runcollisions = true;
    bool                m_runMovement   = true;
    bool                m_runLifespan   = true;
    bool                m_runEnemySpawn = true;
    bool                m_runRender     = true;
    bool                m_runRotate     = true;

    std::vector<sf::RectangleShape> m_background;


    std::shared_ptr<Entity>     m_player;
    std::shared_ptr<Entity>     m_powerCirc;
    size_t                      m_enemyCount;
    size_t                      m_lastEnemySpawnTime{};     
    size_t                      m_lastSpecialWeaponTime{};

public:

    Game(const std::string & config);
    void init(const std::string & path);
    void run();

    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemeis(std::shared_ptr<Entity> e);
    void spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & target);
    void spawnBullet(std::shared_ptr<Entity> entity, const float theta);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity, const Vec2& target);
    void createCheckerBackground();

    void sMovement();
    void sLifespan();
    void sCollision();
    void sEnemySpawner();
    void sGUI();
    void sRender();
    void sUserInput();
    void sScore();
    
    //helper functions:
    float genRandRGB();
    int genRandDir();
    float deg2rad(float deg);
    void rotate(std::shared_ptr<Entity> e);

    PlayerConfig & getPlayerconfig()
    {
        return m_playerConfig;
    }

    EnemyConfig & getEnemyconfig()
    {
        return m_enemyConfig;
    }

    float & getRotationAngle()
    {
        return m_rotationAngle;
    }

};
