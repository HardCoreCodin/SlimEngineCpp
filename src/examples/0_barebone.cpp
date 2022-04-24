#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

struct MySlimEngine : SlimEngine {
    // You code goes here:
};
SlimEngine* createEngine() {
    return (SlimEngine*)new MySlimEngine();
}