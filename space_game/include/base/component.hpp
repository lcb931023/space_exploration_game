#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <vector>
#include <algorithm>

namespace space {
    class component {
    public:
        component (const int _type) : type (_type) {
            comps.push_back (this);
        }

        ~component () {
            std::remove (comps.begin (), comps.end (), this);
        }

        virtual void update (entity& _owner, game_context& _context) = 0;

        const int type;

    public:
        static std::vector<component*> comps;
    };
}

#endif
