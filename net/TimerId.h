#ifndef DUO_NET_TIMERID_H
#define DUO_NET_TIMERID_H

#include "../base/copyable.h"

namespace duo {

    class Timer;

    // An opaque identifier, for canceling Timer.
    class TimerId : public duo::copyable {
    public:
        explicit TimerId(Timer* timer)
            : value_(timer) {
        }

        // default copy-ctor, dtor and assignment are okay
    private:
        Timer* value_;
    };

}

#endif  // DUO_NET_TIMERID_H
