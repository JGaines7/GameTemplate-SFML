#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics.hpp>
//Base movable entity for our game.
//Position/size/rotation properties provided by sf::Sprite
class Entity : public sf::CircleShape
{
    public:
        Entity();
        virtual ~Entity();

        void setVelocity(sf::Vector2f vel) { m_velocity = vel;}
        const sf::Vector2f getVelocity() const {return m_velocity;}
        void updateVelocity(sf::Vector2f vel) { m_velocity += vel;}

    protected:
        sf::Vector2f m_velocity;

    private:
};

#endif // ENTITY_H
