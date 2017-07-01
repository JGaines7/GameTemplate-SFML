#ifndef RESOURCEHOLDER_H
#define RESOURCEHOLDER_H

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

//This class is mostly ripped from SFML Game Development book.
//It is a template that can be used on any set of resources (fonts, textures, sounds)
//Templates cannot be split up into a cpp and hpp file. Instead we make a .inl implementation file
//and include it at the bottom of this file (compiler will see it as one big file)

template<typename Resource, typename Identifier>
class ResourceHolder
{
    public:
        void load(Identifier id, const std::string& fileName);
        Resource& get(Identifier id);
        const Resource& get(Identifier id) const;

    protected:

    private:
        void insertResource(Identifier id, std::unique_ptr<Resource> resource);

        std::map<Identifier, std::unique_ptr<Resource>> m_resourceMap;
};

#include "ResourceHolder.inl"

#endif // RESOURCEHOLDER_H
