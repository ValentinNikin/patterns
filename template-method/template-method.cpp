// Паттерн Шаблонный метод (template method)

#include <iostream>

class AbstractClass {
public:
    virtual ~AbstractClass() = default;

    virtual bool step1() {
        std::cout << "AbstractClass::step1" << std::endl;
        return true;
    }

    virtual void step2() = 0;
    virtual void step3() = 0;

    void templateMethod() {
        if (step1()) {
            step2();
        }
        step3();
    }
};

class ConcreteClass : public AbstractClass {
public:
    bool step1() override {
        std::cout << "ConcreteClass::step1" << std::endl;
        return true;
    }

    void step2() override {
        std::cout << "ConcreteClass::step2" << std::endl;
    }

    void step3() override {
        std::cout << "ConcreteClass::step3" << std::endl;
    }
};

int main(int, char *[]) {
    auto c = new ConcreteClass();
    c->templateMethod();

    delete c;

    return 0;
}