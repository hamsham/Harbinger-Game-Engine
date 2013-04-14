
#include <FreeImage.h>
#include "harbinger.h"

bool hge::init() {
    FreeImage_Initialise(true);
    return display::init();
}

void hge::terminate() {
    FreeImage_DeInitialise();
    display::terminate();
}
