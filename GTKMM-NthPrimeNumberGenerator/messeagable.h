#ifndef MESSEAGABLE_H
#define MESSEAGABLE_H

#include <memory>

namespace templatious {
    struct VirtualPack;
}

class Messageable {
public:
    // this is for sending message across threads
    virtual void message(std::shared_ptr< templatious::VirtualPack > msg) = 0;

    // this is for sending stack allocated (faster)
    // if we know we're on the same thread as GUI
    virtual void message(templatious::VirtualPack& msg) = 0;
};

#endif // MESSEAGABLE_H
