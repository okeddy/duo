#ifndef DUO_NET_CALLBACKS_H
#define DUO_NET_CALLBACKS_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "../base/Timestamp.h"

namespace duo {
    // All client visible callbacks go here.

    typedef boost::function<void()> TimerCallback;

}

#endif  // DUO_NET_CALLBACKS_H
