#include "Game.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game(const std::string & config)
{
    init(config);
}

void Game::init(const std::string & path)
{
    // TODO read in config file here
    //      use the premade PlayerConfig, EnemyConfig, BulletConfig variables here

    std::ifstream fin(path);

    if(!fin){
        std::cerr << "Error: could not open config " << path << std::endl;
    }
    
    std::string lnstart;

    while(fin >> lnstart){
        if(lnstart == "Window")
        {
            fin >> m_windowConfig.sW >> m_windowConfig.sH >> m_windowConfig.FL >> m_windowConfig.FS;
            std::cout << "Loading Window config:\n"
                << "sW: " << m_windowConfig.sW << "\n"
                << "sH: " << m_windowConfig.sH << "\n"
                << "FL: " << m_windowConfig.FL << "\n"
                << "FS: " << m_windowConfig.FS << "\n";
        }
        else if(lnstart == "Font")
        {
            std::string name;
            int size, R,G,B;
            fin >> name >> size >> R >> G >> B;

            std::cout << "Loading Font config:\n"
                << "Name: " << name << "\n"
                << "Size: " << size << "\n"
                << "R: " << R << "\n"
                << "G: " << G << "\n"
                << "B: " << B << "\n";

            m_font.loadFromFile(name);
            m_text.setFont(m_font);
            m_text.setCharacterSize(size);
            m_text.setFillColor(sf::Color(R,G,B));
        }
        else if(lnstart == "Player")
        {
            fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR 
                >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG
                >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;

            std::cout << "Loading Player config:\n"
                << "SR: " << m_playerConfig.SR << "\n"
                << "CR: " << m_playerConfig.CR << "\n"
                << "S: " << m_playerConfig.S << "\n"
                << "FR: " << m_playerConfig.FR << "\n"
                << "FG: " << m_playerConfig.FG << "\n"
                << "FB: " << m_playerConfig.FB << "\n"
                << "OR: " << m_playerConfig.OR << "\n"
                << "OG: " << m_playerConfig.OG << "\n"
                << "OB: " << m_playerConfig.OB << "\n"
                << "OT: " << m_playerConfig.OT << "\n"
                << "V: " << m_playerConfig.V << "\n";
        }
        else if(lnstart == "Enemy")
        {
            fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX 
                >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB   >> m_enemyConfig.OT 
                >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;

            std::cout << "Loading Enemy config:\n"
                << "SR: " << m_enemyConfig.SR << "\n"
                << "CR: " << m_enemyConfig.CR << "\n"
                << "SMIN: " << m_enemyConfig.SMIN << "\n"
                << "SMAX: " << m_enemyConfig.SMAX << "\n"
                << "OR: " << m_enemyConfig.OR << "\n"
                << "OG: " << m_enemyConfig.OG << "\n"
                << "OB: " << m_enemyConfig.OB << "\n"
                << "OT: " << m_enemyConfig.OT << "\n"
                << "VMIN: " << m_enemyConfig.VMIN << "\n"
                << "VMAX: " << m_enemyConfig.VMAX << "\n"
                << "L: " << m_enemyConfig.L << "\n"
                << "SI: " << m_enemyConfig.SI << "\n";
        }
        else if(lnstart == "Bullet")
        {
            fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR 
                >> m_bulletConfig.FB >> m_bulletConfig.FG >> m_bulletConfig.OR >> m_bulletConfig.OG 
                >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
            std::cout << "Loading Bullet config:\n"
                << "SR: " << m_bulletConfig.SR << "\n"
                << "CR: " << m_bulletConfig.CR << "\n"
                << "S: " << m_bulletConfig.S << "\n"
                << "FR: " << m_bulletConfig.FR << "\n"
                << "FG: " << m_bulletConfig.FG << "\n"
                << "FB: " << m_bulletConfig.FB << "\n"
                << "OR: " << m_bulletConfig.OR << "\n"
                << "OG: " << m_bulletConfig.OG << "\n"
                << "OB: " << m_bulletConfig.OB << "\n"
                << "OT: " << m_bulletConfig.OT << "\n"
                << "V: " << m_bulletConfig.V << "\n"
                << "L: " << m_bulletConfig.L << "\n";
        }
    }
    
    // set up window 
    m_window.create(sf::VideoMode(m_windowConfig.sW, m_windowConfig.sH), "GeometryWars Clone", m_windowConfig.FS);
    m_window.setFramerateLimit(m_windowConfig.FL);

    ImGui::SFML::Init(m_window);

    spawnPlayer();
}

void Game::run()
{
    // TODO: add pause functionality in here
    // some systems should function while pasued (rendering)
    // some systems shoudln't (movement / input)

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

        //sKeyboardInput();
        sUserInput();
        // sEnemySpawner();
        sMovement();
        sCollision();
        sGUI();
        sRender();

        // incrememnt the current frame
        // may need to be moved when pause implemented
        m_currentFrame++;
    }
}

// void Game::setPaused(bool paused)
// {
//     // TODO
// }

void Game::spawnPlayer()
{
    // TODO: Finish adding all properties of the player wih the correct values form the config

    auto entity = m_entities.addEntity("player");

    entity->cTransform = std::make_shared<CTransform>(Vec2 (m_windowConfig.sW/2 - m_playerConfig.SR, m_windowConfig.sH/2 - m_playerConfig.SR), Vec2 (0.0f, 0.0f), 0.0f);
    
    // The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
    entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);

    entity->cCollision = std::make_shared<CCollision>(m_playerConfig.SR);
    // Add an input componenet to the player so that we can use inputs
    entity->cInput = std::make_shared<CInput>();

    // Since we want this Entity to be our player, set our Game's player variable to be this Entity
    // This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it

    m_player = entity;
}

// spawn an enemy at a random position

void Game::spawnEnemy()
{
    // TODO: make sure the enemy is spawned properly wiht the m_enemyConfig variables
    //       the enemy must be spawned completely wuithin the bounds of the window
    //
    //  record when the most recent enemy was spawned

    srand(time(0));
    
    int points = (rand() % (m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1)) + m_enemyConfig.VMIN;

    auto enemy = m_entities.addEntity("enemy");

    enemy->cShape = std::make_shared<CShape>(m_enemyConfig.SR, points,sf::Color(rand() % 255,rand() % 255,rand() % 255),sf::Color(m_enemyConfig.OR,m_enemyConfig.OG,m_enemyConfig.OB),m_enemyConfig.OT);

    enemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);

    int posX = (rand() % m_windowConfig.sW-m_enemyConfig.SR - m_enemyConfig.SR + 1) + m_enemyConfig.SR;
    int posY = (rand() % m_windowConfig.sH-m_enemyConfig.SR - m_enemyConfig.SR + 1) + m_enemyConfig.SR;

    float speed = ((rand() % (int)m_enemyConfig.SMAX - (int)m_enemyConfig.SMIN + 1)) + (int)m_enemyConfig.SMIN;

    enemy->cTransform = std::make_shared<CTransform>(Vec2(posX,posY),Vec2(speed,speed),1);

    m_lastEnemySpawnTime = m_currentFrame;
}

//spawns the small enemies when a big one (input entity e) explodes

void Game::spawnSmallEnemeis(std::shared_ptr<Entity> e)
{
    // TODO: spawn small enemies at the location of the input enemy e

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
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
        // TODO: implement your own special weapon
}

void Game::sMovement()
{

    //reset player velocity
    m_player->cTransform->velocity.x = 0;
    m_player->cTransform->velocity.y = 0;

    //player input-based movement:
    if(m_player->cInput->up && !m_player->cCollision->BColision.up)
    {
        m_player->cTransform->velocity.y -= m_playerConfig.S;

    }
    if(m_player->cInput->down && !m_player->cCollision->BColision.down)
    {
        m_player->cTransform->velocity.y += m_playerConfig.S;
    }
    if(m_player->cInput->right && !m_player->cCollision->BColision.right)
    {
        m_player->cTransform->velocity.x += m_playerConfig.S;
    }
    if(m_player->cInput->left && !m_player->cCollision->BColision.left)
    {
        m_player->cTransform->velocity.x -= m_playerConfig.S;
    }

    // movement for all entities:
    for (auto e : m_entities.getEntities()){

        if(e->cCollision && !(e->tag() == "player"))
        {
            if(e->cCollision->BColision.up || e->cCollision->BColision.down)
            {
                e->cTransform->velocity.y *= -1;
            }
            if(e->cCollision->BColision.left || e->cCollision->BColision.right)
            {
                e->cTransform->velocity.x *= -1;
            }
        }

        e->cTransform->pos.x += e->cTransform->velocity.x * m_deltaTime;
        e->cTransform->pos.y += e->cTransform->velocity.y * m_deltaTime;
    }
}

void Game::sLifespan()
{
    // TODO implement all lifespan functionality
    //
    // for all entities
    //      if entity has no lifespan component, skip it
    //      if entity has > 0 remaining lifespan, subtract 1
    //      if it has lifespan and is alive
    //              scale its alpha channel properly
    //      if it has lifspan and its time is up
    //              destroy the entity
}

void Game::sCollision()
{
    // TODO: implement all proper collisions between entities
    //          be sure ti use the collision radius, NOT the shape radius

    //screen border collission
    for( auto e : m_entities.getEntities())
    {
        if(e->cCollision)
        {
            e->cCollision->ResetBcol();

            if (e->cTransform->pos.x - e->cCollision->radius <= 0)
            {
                e->cTransform->pos.x = e->cCollision->radius;
                e->cCollision->BColision.left = true;
            }
            if (e->cTransform->pos.x + e->cCollision->radius >= m_windowConfig.sW)
            {
                e->cTransform->pos.x = m_windowConfig.sW - e->cCollision->radius;
                e->cCollision->BColision.right = true;
            }
            if (e->cTransform->pos.y - e->cCollision->radius <= 0)
            {
                e->cTransform->pos.y = e->cCollision->radius;
                e->cCollision->BColision.up = true;
            }
            if (e->cTransform->pos.y + e->cCollision->radius >= m_windowConfig.sH)
            {
                e->cTransform->pos.y = m_windowConfig.sH - e->cCollision->radius;
                e->cCollision->BColision.down = true;
            }
        }
    }


    // {
    //     m_player->cCollision->borderColision = true;
    //     std::cout<< "player border collision detected!\n";
    // }

    // for ( auto bullet: m_entities.getEntities("bullet"))
    // {
    //     for ( auto enemy : m_entities.getEntities("enemy")){
    //         // if collision(bullet, enemy)
    //         //{
    //         //  enemy->destroy();
    //         //  bullet->destroy();
    //         //}
    //     }
    // }
}

void Game::sEnemySpawner()
{
    // TODO: code which implemens enemy spawning should go here

    // check how many frames since last enemy spawned
    // if number of frames is greater the spawn interval spawn an enemy
}

void Game::sGUI()
{
    ImGui::Begin("Geometry Wars");

    ImGui::Text("Frametime: %.5f", m_deltaTime);

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
        // set the position of the shape based on the entity's transform->pos
        e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

        // set the rotation of the shape based on the entity's transform-angle
        e->cTransform->angle += 1.0f;
        e->cShape->circle.rotate(e->cTransform ->angle * m_deltaTime);

        // draw the entitys sf::CircleShape
        m_window.draw(e->cShape->circle);
    }
  
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
                std::cout << "Left mouse Button clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")\n";
                // call spawnBullet here
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                std::cout << "Right Mouse Button clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")\n";
                // call spawnSpecialWeapon here
            }
            
        }
    }
}