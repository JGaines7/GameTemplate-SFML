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
        void setVelocity(float x, float y) {m_velocity.x = x ; m_velocity.y = y;}
        const sf::Vector2f getVelocity() const {return m_velocity;}
        void updateVelocity(sf::Vector2f vel) { m_velocity += vel;}

        void setUpdateGroup(unsigned int group) { m_updateGroup = group;}
        const unsigned int getUpdateGroup() const { return m_updateGroup;}

        const unsigned int getHealth() const { return m_health;}
        void updatePosition() { setPosition(getPosition() + getVelocity());}

    protected:
        sf::Vector2f m_velocity;
        unsigned int m_health;
        unsigned int m_maxSpeed;
        unsigned int m_updateGroup; //only some entities will be updated per tick, based on group.


};

#endif // ENTITY_H
