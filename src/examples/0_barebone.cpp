#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngine.h"

struct MyEngine : SlimEngine {
    // You code goes here:
};
SlimEngine* createEngine() {
    return (SlimEngine*)new MyEngine();
}

