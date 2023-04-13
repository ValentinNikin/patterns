// Прототип (Prototype)

#include <unordered_map>
#include <string>
#include <algorithm>

class Elevator {
public:
    Elevator* clone() {
        return new Elevator(*this);
    }
};

// Базовый класс прототипа постройки. Определяет виртуальный метод clone()
class Building {
public:

    Building() = default;

    Building(const std::size_t width, const std::size_t height)
            : _width(width), _height(height) {}

    Building(const Building& other) {
        _floorsCount = other._floorsCount;
        _width = other._width;
        _height = other._height;
        _elevator = other._elevator->clone();
    }

    virtual ~Building() {
        delete _elevator;
    }

    virtual Building* clone() {
        return new Building(*this);
    }

    void setElevator(Elevator* elevator) {
        _elevator = elevator;
    }

    void setWidth(const std::size_t width) {
        _width = width;
    }

    void setHeight(const std::size_t height) {
        _height = height;
    }

    void setFloorsCount(const std::size_t floorsCount) {
        _floorsCount = floorsCount;
    }

private:
    Elevator* _elevator;
    std::size_t _width;
    std::size_t _height;
    std::size_t _floorsCount;

    // other properties
};

class Parking {
public:
    Parking* clone() {
        return new Parking(*this);
    }
};

class Cinema {
public:
    Cinema* clone() {
        return new Cinema(*this);
    }
};

// Расширенная реализация прототипа - торговый центр
class ShoppingMall : public Building {
public:

    ShoppingMall() = default;

    ShoppingMall(const ShoppingMall& other) : Building(other) {
        _undergroundParking = other._undergroundParking->clone();
        _cinema = other._cinema->clone();
    }

    ~ShoppingMall() override {
        delete _undergroundParking;
        delete _cinema;
    }

    Building* clone() override {
        return new ShoppingMall(*this);
    }

    void setUndergroundParking(Parking* parking) {
        _undergroundParking = parking;
    }

    void setCinema(Cinema* cinema) {
        _cinema = cinema;
    }

private:
    Parking* _undergroundParking;
    Cinema* _cinema;
};

class BoilerRoom {
public:
    BoilerRoom* clone() {
        return new BoilerRoom(*this);
    }
};

// Расширенная реализация прототипа - жилой дом
class House : public Building {
public:

    House() = default;

    House(const House& other) : Building(*this) {
        _boilerRoom = other._boilerRoom->clone();
    }

    ~House() override {
        delete _boilerRoom;
    }

    Building* clone() override {
        return new House(*this);
    }

    void setBoilerRoom(BoilerRoom* boilerRoom) {
        _boilerRoom = boilerRoom;
    }

private:
    BoilerRoom* _boilerRoom;
};

int main(int, char *[]) {
    // Создаем различного рода типовые проекты построек и и добавляем их в коллекцию
    std::unordered_map<std::string, Building*> _typicalBuildingProjects;

    {
        auto stock = new Building(100, 30);
        stock->setFloorsCount(1);
        stock->setElevator(new Elevator());
        _typicalBuildingProjects.emplace("Stock", stock);
    }

    {
        auto shoppingMall = new ShoppingMall();
        shoppingMall->setWidth(300);
        shoppingMall->setHeight(30);
        shoppingMall->setUndergroundParking(new Parking());
        shoppingMall->setCinema(new Cinema());
        _typicalBuildingProjects.emplace("ShoppingMallBig", shoppingMall);
    }

    {
        auto shoppingMall = new ShoppingMall();
        shoppingMall->setWidth(30);
        shoppingMall->setHeight(10);
        _typicalBuildingProjects.emplace("ShoppingMallSmall", shoppingMall);
    }

    {
        auto house = new House();
        house->setWidth(10);
        house->setHeight(5);
        house->setBoilerRoom(new BoilerRoom());
        _typicalBuildingProjects.emplace("House", house);
    }

    // Для дальнейшей работы можем использовать типовые постройки из коллекции.
    auto house = _typicalBuildingProjects.at("House")->clone();

    // TODO: какая-то работа с созданным экземпляром дома

    delete house;
    std::for_each(_typicalBuildingProjects.begin(), _typicalBuildingProjects.end(), [](auto& i) {
        delete i.second;
    });

    return 0;
}