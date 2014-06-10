#ifndef WORLD_HPP
#define WORLD_HPP

namespace space {
    class game_context;

    class world {
    public:
        void update (game_context& _context);
        void render (game_context& _context);
    };
}

#endif
