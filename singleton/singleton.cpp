// Одиночка (Singleton)

#include <memory>
#include <map>
#include <string>

namespace simple_implementation {

// Сторонняя реализация DI контейнера
class DIContainer {
public:
    template<typename T>
    std::shared_ptr<T> resolve() {
        return nullptr;
    }
};

// Обертка над DI контейнером
class ServiceLocator {
private:
    ServiceLocator() = default;
    static std::unique_ptr<ServiceLocator> _instance;
public:
    static ServiceLocator* getInstance();

    template<typename T>
    std::shared_ptr<T> resolve() {
        return _container->resolve<T>();
    }

    template<typename T>
    std::shared_ptr<T> resolve(const T&) {
        return _container->resolve<T>();
    }

private:
    void init();
    std::shared_ptr<DIContainer> _container;
};

std::unique_ptr<ServiceLocator> ServiceLocator::_instance = nullptr;

ServiceLocator* ServiceLocator::getInstance() {
    if (_instance == nullptr) {
        _instance = std::unique_ptr<ServiceLocator>(new ServiceLocator());
        _instance->init();
    }

    return _instance.get();
}

void ServiceLocator::init() {
    // TODO: initialize DI container
}

class TempClass {};

void example() {
    auto serviceLocator = ServiceLocator::getInstance();
    auto instance = serviceLocator->resolve<TempClass>();
}

}

namespace register_implementation {
class Singleton {
public:
    static void add(const std::string& name, Singleton* singleton) {
        _registry.insert(std::make_pair(name, singleton));
    }

    static Singleton* getInstance() {
        if (_instance == 0) {
            _instance = Lookup("someName");
        }

        return _instance;
    }
protected:
    static Singleton* Lookup(const std::string& name) {
        if (_registry.find(name) == _registry.end()) {
            return nullptr;
        }

        return _registry.at(name);
    }
private:
    static std::map<std::string, Singleton*> _registry;
    static Singleton* _instance;
};

std::map<std::string, Singleton*> Singleton::_registry = {};
Singleton* Singleton::_instance = nullptr;

class TempSingleton : public Singleton {
public:
    TempSingleton() {
        Singleton::add("TempSingleton", this);
    }

private:
    static TempSingleton tempSingleton;
};

void example() {
    auto instance = Singleton::getInstance();
}
}


int main(int, char *[]) {
    simple_implementation::example();
    register_implementation::example();

    return 0;
}