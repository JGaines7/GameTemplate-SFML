#ifndef QUADTREE_H
#define QUADTREE_H

#include <SFML/Graphics.hpp>
#include "Entity.h"


class QuadTree : public sf::Drawable
{
    public:
        QuadTree(int level, sf::Rect<int> bounds);
        virtual ~QuadTree();

        void clear();
        void insert(Entity* entityPtr); //assuming entities will all be rectangular sprites. soft references
        std::vector<Entity*> retrieve(sf::Rect<float> inputRect) const; //return list of objects under key that possibly collide with input rect


        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


    protected:

    private:

        static const int MAX_OBJECTS = 5;
        static const int MAX_LEVELS = 6;

        void split();
        std::vector<int> findEncompassingNode(sf::Rect<float>) const;

        std::vector<Entity*> m_entityList;
        sf::Rect<int> m_boundingRect;
        std::array<std::unique_ptr<QuadTree>, 4> m_childNodes; // Start NW, Clockwise
        int m_level;





};

#endif // QUADTREE_H
