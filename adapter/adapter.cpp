// Паттерн Адаптер (adapter)

#include <memory>

using Point = std::pair<int, int>;

class Manipulator {};
class TextManipulator : public Manipulator {};

// Целевой интерфейс
class Shape {
public:
    virtual void boundingBox(Point& bottomLeft, Point& topRight) = 0;
    virtual std::unique_ptr<Manipulator> createManipulator() = 0;
};

// Класс, который необходимо адаптировать
class TextView {
public:
    void getOrigin(Point& p) const {
        // TODO: some implementation
    }

    void getExtent(int& width, int& height) const {
        // TODO: some implementation
    }

    bool isEmpty() const {
        // TODO: some implementation
        return false;
    }
};

// Адаптация интерфейса - наследуемся и от класса интерфейса, и от класса реализации
namespace interface_adaptation {

// Применяем множественное наследование, интерфейс наследуется открыто,
// реализация - закрыто
class TextShape : public Shape, private TextView {
public:
    void boundingBox(Point& bottomLeft, Point& topRight) override {
        Point p;
        int width, height;

        getOrigin(p);
        getExtent(width, height);

        bottomLeft = p;
        topRight = std::make_pair(p.first + width, p.second + height);
    }

    std::unique_ptr<Manipulator> createManipulator() override {
        return std::make_unique<TextManipulator>();
    }
};

void example() {
    std::unique_ptr<Shape> shape = std::make_unique<TextShape>();

    Point buttomLeft, topRight;
    shape->boundingBox(buttomLeft, topRight);

    auto manipulator = shape->createManipulator();
}

}

// Адаптация объекта - наследуемся от класса интерфейса, связь с реализацией осуществляется через
// отношение - композиция
namespace instance_adaption {

class TextShape : public Shape {
public:
    // Ссылку на экземпляр класса реализации передаем через конструктор.
    TextShape(TextView* tv) : _tv(tv) {};

    ~TextShape() {
        delete _tv;
    }

    void boundingBox(Point& bottomLeft, Point& topRight) override {
        Point p;
        int width, height;

        _tv->getOrigin(p);
        _tv->getExtent(width, height);

        bottomLeft = p;
        topRight = std::make_pair(p.first + width, p.second + height);
    }

    std::unique_ptr<Manipulator> createManipulator() override {
        return std::make_unique<TextManipulator>();
    }
private:
    TextView* _tv;
};

void example() {
    std::unique_ptr<Shape> shape = std::make_unique<TextShape>(new TextView());

    Point buttomLeft, topRight;
    shape->boundingBox(buttomLeft, topRight);

    auto manipulator = shape->createManipulator();
}

}

int main(int, char *[]) {
    interface_adaptation::example();
    instance_adaption::example();
}