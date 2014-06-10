#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>

namespace space {
    class component;

    class entity {
    public:
        int id;
        std::vector<component*> comps;
    };
}

#endif
