// Цепочка обязанностей (chain of responsibility)

#include <iostream>
#include <array>
#include <algorithm>

class Handler {
public:
    virtual ~Handler() = default;

    void setSuccessor(Handler* successor) {
        _successor = successor;
    }

    virtual void handleRequest(int value) {
        if (_successor != nullptr) {
            _successor->handleRequest(value);
        }
    }

private:
    Handler* _successor;
};

class EvenHandler : public Handler {
public:
    void handleRequest(int value) override {
        if (value % 2 == 0) {
            std::cout << "Even number: " << value << std::endl;
        }
        else {
            Handler::handleRequest(value);
        }
    }
};

class OddHandler : public Handler {
public:
    void handleRequest(int value) override {
        if (value % 2 != 0) {
            std::cout << "Odd number: " << value << std::endl;
        }
        else {
            Handler::handleRequest(value);
        }
    }
};

int main(int, char *[]) {
    auto evenHandler = new EvenHandler();
    auto oddHandler = new OddHandler();
    evenHandler->setSuccessor(oddHandler);

    Handler* handler = evenHandler;

    std::array<int, 10> arr { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    std::for_each(arr.cbegin(), arr.cend(), [&handler](const auto& i) {
        handler->handleRequest(i);
    });

    handler = nullptr;
    delete evenHandler;
    delete oddHandler;

    return 0;
}