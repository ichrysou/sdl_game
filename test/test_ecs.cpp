#include "../ECS.h"

class comp1 {
private:
    int a;
    float b;
public:
    comp1() {
        a = 0;
        b = 0.0f;
    }
};
class comp2 {
private:
    int a;
public:
    comp2() {
        a = 0;
    }
};

comp1 c11, c12, c13;
comp2 c21, c22, c23;

int main() {
    std::cout << getComponentTypeID<comp2>() << std::endl;
    std::cout << getComponentTypeID<comp1>() << std::endl;
    std::cout << getComponentTypeID<comp1>() << std::endl;
    std::cout << getComponentTypeID<comp1>() << std::endl;

    std::cout << getComponentTypeID<comp2>() << std::endl;
    std::cout << getComponentTypeID<comp2>() << std::endl;
    std::cout << getComponentTypeID<comp1>() << std::endl;
}
