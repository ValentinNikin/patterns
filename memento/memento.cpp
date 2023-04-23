// Паттерн Хранитель (Memento)

#include <iostream>
#include <memory>
#include <string>

class Originator;

class Memento {
    friend class Originator;
private:
    Memento(const std::string& state)
        : _state(state) {}

    std::string getState() const {
        return _state;
    }

private:
    std::string _state;
};

class Originator {
public:
    Originator(const std::string& state)
        : _state(state) {}

    void changeState(const std::string& state) {
        _state = state;
    }

    std::string getState() const {
        return _state;
    }

    std::unique_ptr<Memento> createMemento() {
        return std::unique_ptr<Memento>(new Memento(_state));
    }

    void resetFromMemento(std::unique_ptr<Memento> memento) {
        _state = memento->getState();
    }

private:
    std::string _state;
};

int main(int, char *[]) {
    auto originator = std::make_unique<Originator>("1");
    std::cout << "Original state: " << originator->getState() << std::endl;
    auto memento = originator->createMemento();

    originator->changeState("2");
    std::cout << "Changed state: " << originator->getState() << std::endl;

    originator->resetFromMemento(std::move(memento));
    std::cout << "Reset state: " << originator->getState() << std::endl;

    return 0;
}