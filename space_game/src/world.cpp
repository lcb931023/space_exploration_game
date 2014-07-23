#include <algorithm>
#include <world.hpp>

namespace space {
    void world::add_entity (entity* _e) {
        ents.push_front (_e);
    }

    void world::del_entity (entity* _e) {
        ents.remove (_e);
    }

    void world::del_entity (const int _id) {
        std::remove_if (ents.begin (), ents.end (), [_id](entity* _e)->bool { return (_e->id == _id); });
    }

    entity* world::get_entity (const int _id) {
        auto ent = std::find_if (ents.begin (), ents.end (), [_id](entity* _e)->bool { return (_e->id == _id); });

        return (ent != ents.end () ) ? *ent : nullptr;
    }
}
