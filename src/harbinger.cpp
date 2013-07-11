
#include <FreeImage.h>
#include "harbinger.h"

bool hge::init() {
    FreeImage_Initialise(true);
    return window::init();
}

void hge::terminate() {
    FreeImage_DeInitialise();
    window::terminate();
}
