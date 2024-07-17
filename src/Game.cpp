#include "Game.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

Game::Game(const std::string & config)
{
    init(config);
}

void Game::init(const std::string & path)
{
    std::ifstream fin(path);
    srand(time(0));

    if(!fin){
        std::cerr << "Error: could not open config " << path << std::endl;
    }
    
    std::string lnstart;

    while(fin >> lnstart){
        if(lnstart == "Window")
        {
            fin >> m_windowConfig.sW >> m_windowConfig.sH >> m_windowConfig.FL >> m_windowConfig.FS;
            // std::cout << "Loading Window config:\n"
            //     << "sW: " << m_windowConfig.sW << "\n"
            //     << "sH: " << m_windowConfig.sH << "\n"
            //     << "FL: " << m_windowConfig.FL << "\n"
            //     << "FS: " << m_windowConfig.FS << "\n";
        }
        else if(lnstart == "Font")
        {
            std::string name;
            int size, R,G,B;
            fin >> name >> size >> R >> G >> B;

            // std::cout << "Loading Font config:\n"
            //     << "Name: " << name << "\n"
            //     << "Size: " << size << "\n"
            //     << "R: " << R << "\n"
            //     << "G: " << G << "\n"
            //     << "B: " << B << "\n";

            m_font.loadFromFile(name);
            m_text.setFont(m_font);
            m_text.setString(std::to_string(m_score));
            m_text.setCharacterSize(size);
            m_text.setFillColor(sf::Color(R,G,B));
            m_text.setPosition(0,0);
        }
        else if(lnstart == "Player")
        {
            fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR 
                >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG
                >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;

            // std::cout << "Loading Player config:\n"
            //     << "SR: " << m_playerConfig.SR << "\n"
            //     << "CR: " << m_playerConfig.CR << "\n"
            //     << "S: " << m_playerConfig.S << "\n"
            //     << "FR: " << m_playerConfig.FR << "\n"
            //     << "FG: " << m_playerConfig.FG << "\n"
            //     << "FB: " << m_playerConfig.FB << "\n"
            //     << "OR: " << m_playerConfig.OR << "\n"
            //     << "OG: " << m_playerConfig.OG << "\n"
            //     << "OB: " << m_playerConfig.OB << "\n"
            //     << "OT: " << m_playerConfig.OT << "\n"
            //     << "V: " << m_playerConfig.V << "\n";
        }
        else if(lnstart == "Enemy")
        {
            fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX 
                >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB   >> m_enemyConfig.OT 
                >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;

            // std::cout << "Loading Enemy config:\n"
            //     << "SR: " << m_enemyConfig.SR << "\n"
            //     << "CR: " << m_enemyConfig.CR << "\n"
            //     << "SMIN: " << m_enemyConfig.SMIN << "\n"
            //     << "SMAX: " << m_enemyConfig.SMAX << "\n"
            //     << "OR: " << m_enemyConfig.OR << "\n"
            //     << "OG: " << m_enemyConfig.OG << "\n"
            //     << "OB: " << m_enemyConfig.OB << "\n"
            //     << "OT: " << m_enemyConfig.OT << "\n"
            //     << "VMIN: " << m_enemyConfig.VMIN << "\n"
            //     << "VMAX: " << m_enemyConfig.VMAX << "\n"
            //     << "L: " << m_enemyConfig.L << "\n"
            //     << "SI: " << m_enemyConfig.SI << "\n"
            //     << "EMAX: " << m_enemyConfig.EMAX << "\n";
        }
        else if(lnstart == "Bullet")
        {
            fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR 
                >> m_bulletConfig.FB >> m_bulletConfig.FG >> m_bulletConfig.OR >> m_bulletConfig.OG 
                >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
            // std::cout << "Loading Bullet config:\n"
            //     << "SR: " << m_bulletConfig.SR << "\n"
            //     << "CR: " << m_bulletConfig.CR << "\n"
            //     << "S: " << m_bulletConfig.S << "\n"
            //     << "FR: " << m_bulletConfig.FR << "\n"
            //     << "FG: " << m_bulletConfig.FG << "\n"
            //     << "FB: " << m_bulletConfig.FB << "\n"
            //     << "OR: " << m_bulletConfig.OR << "\n"
            //     << "OG: " << m_bulletConfig.OG << "\n"
            //     << "OB: " << m_bulletConfig.OB << "\n"
            //     << "OT: " << m_bulletConfig.OT << "\n"
            //     << "V: " << m_bulletConfig.V << "\n"
            //     << "L: " << m_bulletConfig.L << "\n";
        }
    }
    
    // set up window 
    m_window.create(sf::VideoMode(m_windowConfig.sW, m_windowConfig.sH), "GeometryWars Clone", m_windowConfig.FS);
    m_window.setFramerateLimit(m_windowConfig.FL);
    //m_text.setPosition(10,10);
    ImGui::SFML::Init(m_window);

    spawnPlayer();
}

void Game::run()
{

    m_deltaClock = sf::Clock();
    m_running = true;

    while(m_running)
    {

        m_deltaTime = m_deltaClock.restart().asSeconds();

        //update the entity manager
        m_entities.update();

        if(!m_player->isActive())
        {
            spawnPlayer();
        }

        //require update call to imgui
        ImGui::SFML::Update(m_window, m_deltaClock.restart());

        sUserInput();
        if(!m_paused){       
            if(m_runEnemySpawn)
            {
                sEnemySpawner();
            }
            if(m_runLifespan)
            {
               sLifespan();
            }
            if(m_runcollisions)
            {
                sCollision();
            }
            if(m_runMovement)
            {
                sMovement();
            }
        }
        sScore();
        sGUI();
        if(m_runRender)
        {
            sRender();
        }

        ++m_currentFrame;
    }
}

void Game::spawnPlayer()
{

    auto entity = m_entities.addEntity("player");
    entity->cTransform = std::make_shared<CTransform>(Vec2 (m_windowConfig.sW/2 - m_playerConfig.SR, m_windowConfig.sH/2 - m_playerConfig.SR), Vec2 (0.0f, 0.0f), 0.0f);
    entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB,128, sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
    entity->cCollision = std::make_shared<CCollision>(m_playerConfig.SR);
    entity->cInput = std::make_shared<CInput>();

    m_player = entity;
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{

    int points = (rand() % (m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1)) + m_enemyConfig.VMIN;
    auto enemy = m_entities.addEntity("enemy");

    enemy->cScore = std::make_shared<CScore>(points * 100);

    enemy->cShape = std::make_shared<CShape>(m_enemyConfig.SR, points,genRandRGB(),genRandRGB(),genRandRGB(),255,sf::Color(m_enemyConfig.OR,m_enemyConfig.OG,m_enemyConfig.OB),m_enemyConfig.OT);
    enemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);

    int posX = (rand() % m_windowConfig.sW-m_enemyConfig.SR - m_enemyConfig.SR + 1) + m_enemyConfig.SR;
    int posY = (rand() % m_windowConfig.sH-m_enemyConfig.SR - m_enemyConfig.SR + 1) + m_enemyConfig.SR;

    float speed = ((rand() % (int)m_enemyConfig.SMAX - (int)m_enemyConfig.SMIN + 1)) + (int)m_enemyConfig.SMIN;

    Vec2 velocity = Vec2(genRandDir(),genRandDir());

    velocity *= speed;

    enemy->cTransform = std::make_shared<CTransform>(Vec2(posX,posY),velocity,5);

    enemy->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L * m_windowConfig.FL);

    enemy->cLifespan->remaining = enemy->cLifespan->total;

    m_lastEnemySpawnTime = m_currentFrame;
}

//spawns the small enemies when a big one (input entity e) explodes

void Game::spawnSmallEnemeis(std::shared_ptr<Entity> e)
{
    // TODO: spawn small enemies at the location of the input enemy e

    // auto smallEnemy = m_entities.addEntity("smallenemy");
    
    // smallEnemy->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);

    // when we create the smaller enemy, we have to read the values of the original enemy
    // - spawn a number of small enemies equal to the vertices of the original enemy
    // - set each small enemy to the same color as the original, half the size
    // - small enemeis are worth double points of the original enemy
}

// spawns a bullet from a given entity to a target location

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & target)
{
    // TODO: implement the spawning of a bullet which travels toward target
    //       - bullet speed is given as a scalar speed
    //       - you must set the velocity by using formula in notes

    auto bullet = m_entities.addEntity("bullet");
    
    bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
    
    bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR,16,m_bulletConfig.FR,m_bulletConfig.FG,m_bulletConfig.FB, 255,sf::Color(m_bulletConfig.OR,m_bulletConfig.OG,m_bulletConfig.OB),m_bulletConfig.OT);
    
    Vec2 velocity = target - m_player->cTransform->pos;
    velocity.normalize();
    velocity *= m_bulletConfig.S;

    bullet->cTransform = std::make_shared<CTransform>(m_player->cTransform->pos,velocity,1);

    bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L * m_windowConfig.FL);
    
    bullet->cLifespan->remaining = bullet->cLifespan->total;
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const float theta)
{

    auto bullet = m_entities.addEntity("bullet");
    
    bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
    
    bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR,16,m_bulletConfig.FR,m_bulletConfig.FG,m_bulletConfig.FB, 255,sf::Color(m_bulletConfig.OR,m_bulletConfig.OG,m_bulletConfig.OB),m_bulletConfig.OT);
    
    Vec2 direction = Vec2(cos(theta), sin(theta));
    direction *= m_bulletConfig.S;

    bullet->cTransform = std::make_shared<CTransform>(m_player->cTransform->pos,direction,1);

    bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L * m_windowConfig.FL);
    
    bullet->cLifespan->remaining = bullet->cLifespan->total;
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity,const Vec2 & target)
{

    if((m_currentFrame - m_lastSpecialWeaponTime) > 120)
    {
        float dy = target.y - entity->cTransform->pos.y;
        float dx = target.x - entity->cTransform->pos.x;

        float theta = atan2(dy,dx);

        spawnBullet(entity, theta);
        spawnBullet(entity, theta - deg2rad(10));
        spawnBullet(entity, theta + deg2rad(10));

        m_lastSpecialWeaponTime = m_currentFrame;

        if(m_powerCirc) {
            m_powerCirc->destroy();
            m_powerCirc = nullptr;
        }
    }
    else
    {
        if(!m_powerCirc){
            auto powerCirc = m_entities.addEntity("powercircle");
            float chargeProgress = (m_currentFrame - m_lastSpecialWeaponTime) / 120.0f;
            float radius = chargeProgress * m_playerConfig.SR * 0.9f; // 90% of player radius at max
            powerCirc->cShape = std::make_shared<CShape>(radius, 64, 0, 255, 255, 128, sf::Color::Yellow, 2);
            powerCirc->cTransform = std::make_shared<CTransform>(m_player->cTransform->pos, Vec2(0,0), 0);
            powerCirc->cLifespan = std::make_shared<CLifespan>(121); // Slightly more than charge time
            m_powerCirc = powerCirc;
        }
        else
        {
            float chargeProgress = (m_currentFrame - m_lastSpecialWeaponTime) / 120.0f;
            float radius = chargeProgress * m_playerConfig.SR * 0.9f;
            m_powerCirc->cShape->circle.setRadius(radius);
            m_powerCirc->cTransform->pos = m_player->cTransform->pos;
            m_powerCirc->cShape->circle.setFillColor(sf::Color(0, 255, 255, 128 + (int)(chargeProgress * 127)));
        }

    }


}

void Game::sMovement()
{

    //reset player velocity

    //player input-based movement:
    if(m_player->cInput->up && !m_player->cCollision->BColision.up)
    {
        m_player->cTransform->pos.y -= m_playerConfig.S * m_deltaTime;
        m_powerCirc->cTransform->pos.y = m_player->cTransform->pos.y;
    }
    if(m_player->cInput->down && !m_player->cCollision->BColision.down)
    {
        m_player->cTransform->pos.y += m_playerConfig.S * m_deltaTime;
        m_powerCirc->cTransform->pos.y = m_player->cTransform->pos.y;
    }
    if(m_player->cInput->right && !m_player->cCollision->BColision.right)
    {
        m_player->cTransform->pos.x += m_playerConfig.S * m_deltaTime;
        m_powerCirc->cTransform->pos.x = m_player->cTransform->pos.x;
        
    }
    if(m_player->cInput->left && !m_player->cCollision->BColision.left)
    {
        m_player->cTransform->pos.x -= m_playerConfig.S * m_deltaTime;
        m_powerCirc->cTransform->pos.x = m_player->cTransform->pos.x;
    }

    // movement for all entities:
    for (auto e : m_entities.getEntities()){

        if(e->cCollision && e->tag() != "player")
        {
            if(e->cCollision->BColision.up || e->cCollision->BColision.down)
            {
                e->cTransform->velocity.y *= -1;
            }
            if(e->cCollision->BColision.left || e->cCollision->BColision.right)
            {
                e->cTransform->velocity.x *= -1;
            }

            e->cTransform->pos.x += e->cTransform->velocity.x * m_deltaTime;
            e->cTransform->pos.y += e->cTransform->velocity.y * m_deltaTime;

        }


    }
}

void Game::sLifespan()
{

    for(auto e : m_entities.getEntities())
    {
        if(e->cLifespan)
        {
            if(e->cLifespan->remaining>0)
            {
                e->cLifespan->remaining--;
            }
            else
            {
                e->destroy();
            }
        }
    }
}

void Game::sCollision()
{
    //screen border collission
    for( auto e : m_entities.getEntities())
    {
        if(e->cCollision)
        {
            //screen border colision
            e->cCollision->resetBcol();

            if (e->cTransform->pos.x - e->cCollision->radius <= 0)
            {
                e->cTransform->pos.x = e->cCollision->radius;
                e->cCollision->BColision.left = true;
                if(e->tag() == "bullet")
                {
                    e->destroy();
                }
            }
            if (e->cTransform->pos.x + e->cCollision->radius >= m_windowConfig.sW)
            {
                e->cTransform->pos.x = m_windowConfig.sW - e->cCollision->radius;
                e->cCollision->BColision.right = true;
                if(e->tag() == "bullet")
                {
                    e->destroy();
                }
            }
            if (e->cTransform->pos.y - e->cCollision->radius <= 0)
            {
                e->cTransform->pos.y = e->cCollision->radius;
                e->cCollision->BColision.up = true;
                if(e->tag() == "bullet")
                {
                    e->destroy();
                }
            }
            if (e->cTransform->pos.y + e->cCollision->radius >= m_windowConfig.sH)
            {
                e->cTransform->pos.y = m_windowConfig.sH - e->cCollision->radius;
                e->cCollision->BColision.down = true;
                if(e->tag() == "bullet")
                {
                    e->destroy();
                }
            }

            //enemy-player colision
            if(e->tag() == "enemy")
            {
                float dx = (e->cTransform->pos.x - m_player->cTransform->pos.x);
                float dy = (e->cTransform->pos.y - m_player->cTransform->pos.y);
                float r1 = e->cCollision->radius;
                float r2 = m_player->cCollision->radius;
                if((dx*dx + dy*dy) < (r1+r1)*(r1+r2))
                {
                    m_player->destroy();
                    m_score = 0;
                }
            }
        }
    }

    for(auto bullet : m_entities.getEntities("bullet"))
    {
        for(auto enemy : m_entities.getEntities("enemy"))
        {
            float dx = (enemy->cTransform->pos.x - bullet->cTransform->pos.x);
            float dy = (enemy->cTransform->pos.y - bullet->cTransform->pos.y);
            float r1 = enemy->cCollision->radius;
            float r2 = bullet->cCollision->radius;
            if((dx*dx + dy*dy) < (r1+r1)*(r1+r2))
            {
                if(enemy->isActive()){
                    m_score += enemy->cScore->score;
                }
                enemy->destroy();
                bullet->destroy();
            }
        }
    }
}

void Game::sEnemySpawner()
{
    if((m_currentFrame - m_lastEnemySpawnTime) > m_enemyConfig.SI)
    {
        spawnEnemy();
    }
}

void Game::sGUI()
{
    ImGui::Begin("Geometry Wars");

    ImGui::Text("Frametime: %.5f", m_deltaTime);

    if(ImGui::CollapsingHeader("Systems"))
    {
        if(ImGui::Button("Movement"))
        {
            m_runMovement = !m_runMovement;
        }
        ImGui::SameLine();
        ImGui::Text((m_runMovement)? "on" : "off");

        if(ImGui::Button("Colision"))
        {
            m_runcollisions = !m_runcollisions;
        }
        ImGui::SameLine();
        ImGui::Text((m_runcollisions)? "on" : "off");

        if(ImGui::Button("EnemySpawn"))
        {
            m_runEnemySpawn = !m_runEnemySpawn;
        }
        ImGui::SameLine();
        ImGui::Text((m_runEnemySpawn)? "on" : "off");

        if(ImGui::Button("Lifespan"))
        {
            m_runLifespan = !m_runLifespan;
        }
        ImGui::SameLine();
        ImGui::Text((m_runLifespan)? "on" : "off");
    }

    if(ImGui::CollapsingHeader("Player menu")){
        ImGui::Text("Info:");
        ImGui::Text("Position:\n X: %.2f Y: %.2f", m_player->cTransform->pos.x,m_player->cTransform->pos.y);
        ImGui::Text("Velocity:\n X: %.2f Y: %.2f", m_player->cTransform->velocity.x,m_player->cTransform->velocity.y);
        ImGui::Spacing();
        if(ImGui::Button("Kill Player"))
        {
            m_player->destroy();
        }
    }

    if(ImGui::CollapsingHeader("Enemy Menu")){
        if(ImGui::Button("Add enemy"))
        {
            spawnEnemy();
        }
    }
    ImGui::End();
}

void Game::sRender()
{
    m_window.clear();

    for(auto e : m_entities.getEntities())
    {
        if (e->cTransform && e->cShape) {
            e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
            e->cShape->circle.setRotation(e->cTransform->angle += 5 * m_deltaTime);
        }
        if(e->cLifespan)
        {
            float newAlphaVal = (static_cast<float>(e->cLifespan->remaining) / e->cLifespan->total) * 255.0f;
            e->cShape->changeAlpha(newAlphaVal);
        }
        m_window.draw(e->cShape->circle);
    }

    if(m_paused)
    {
        sf::RectangleShape pauseBox(sf::Vector2f(m_windowConfig.sW/3, m_windowConfig.sH/8));
        pauseBox.setPosition(m_windowConfig.sW/2 - (m_windowConfig.sW/3)/2, m_windowConfig.sH/2 - (m_windowConfig.sH/8)/2);
        pauseBox.setOutlineThickness(5);
        pauseBox.setFillColor(sf::Color(128,128,128));

        sf::Text pauseText;
        pauseText.setString("Game Paused");
        pauseText.setCharacterSize(24);
        pauseText.setFont(m_font);

        sf::FloatRect textRect = pauseText.getGlobalBounds();
        float textX = pauseBox.getPosition().x + (pauseBox.getSize().x - textRect.width) / 2.0f;
        float textY = pauseBox.getPosition().y + (pauseBox.getSize().y - textRect.height) / 2.0f;
        pauseText.setPosition(textX, textY);
        pauseText.Bold;

        pauseText.setFillColor(sf::Color::Black);

        m_window.draw(pauseBox);
        m_window.draw(pauseText);
    }
    m_window.draw(m_text);
  
    // draw the ui last
    ImGui::SFML::Render(m_window);

    m_window.display();
}

void Game::sUserInput()
{
    sf::Event event;
    while ( m_window.pollEvent(event))
    {
        // pass the event to imgui to be parsed
        ImGui::SFML::ProcessEvent(event);

        // this event triggers when the window is closed
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }
        
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                m_player->cInput->up = true;
                break;
            case sf::Keyboard::S:
                m_player->cInput->down = true;
                break;
            case sf::Keyboard::A:
                m_player->cInput->left = true;
                break;
            case sf::Keyboard::D:
                m_player->cInput->right = true;
                break;
            case sf::Keyboard::Escape:
                m_running = false;
            case sf::Keyboard::P:
                m_paused = !m_paused;
            default: break;
            }
        }

        if ( event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                m_player->cInput->up = false;
                break;
            case sf::Keyboard::S:
                m_player->cInput->down = false;
                break;
            case sf::Keyboard::A:
                m_player->cInput->left = false;
                break;
            case sf::Keyboard::D:
                m_player->cInput->right = false;
                break;
                default: break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            // this line ignores mouse event is ImGui is the thing being clicked on
            if ( ImGui::GetIO().WantCaptureMouse) { continue; }

            if ( event.mouseButton.button == sf::Mouse::Left)
            {
                m_player->cInput->shoot = true;
                spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                spawnSpecialWeapon(m_player,Vec2(event.mouseButton.x, event.mouseButton.y));
            }
            
        }
    }
}

void Game::sScore()
{
    m_text.setString("Score: " + std::to_string(m_score));
    m_window.draw(m_text);
}

float Game::genRandRGB()
{
    float colorVal = rand() % 255;
    return colorVal;
}

int Game::genRandDir()
{
    int randval = rand() % 2 + 1; 
    int dir = (randval == 1)? 1 : -1;
    return dir;
}

float Game::deg2rad(float deg)
{
    return deg * M_PI / 180.0;
}