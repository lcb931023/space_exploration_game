#ifndef WORLD_HPP
#define WORLD_HPP

#include <forward_list>
#include <base/entity.hpp>

namespace space {
    class game_context;

    class world {
    public:
        void add_entity (entity* _e);
        void del_entity (entity* _e);
        void del_entity (const int _id);

        entity* get_entity (const int _id);

        std::forward_list<entity*> ents;
    };
}
#endif
