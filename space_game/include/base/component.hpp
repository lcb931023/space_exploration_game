#ifndef COMPONENT_HPP
#define COMPONENT_HPP

namespace space {
    class component {
    public:
        component (const int _type) : type (_type) {}

        const int type;
    };
}

#endif
