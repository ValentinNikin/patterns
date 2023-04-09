// Паттерн Посетитель (Visitor)

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

class Circle;
class Rectangle;
class Square;

class ShapeVisitor {
public:
    virtual void visitCircle(Circle* circle) = 0;
    virtual void visitRectangle(Rectangle* rectangle) = 0;
    virtual void visitSquare(Square* square) = 0;
};

class Shape {
public:
    virtual ~Shape() = default;
    virtual void accept(ShapeVisitor *visitor) = 0;
};

class Circle : public Shape {
public:
    Circle(int radius) : _radius(radius) {}

    int getRadius() const {
        return _radius;
    }

    // ... other fields

    void accept(ShapeVisitor *visitor) override {
        visitor->visitCircle(this);
    }

private:
    int _radius;
};

class Rectangle : public Shape {
public:
    Rectangle(int width, int height) : _width(width), _height(height) {}

    int getWidth() const {
        return _width;
    }

    int getHeight() const {
        return _height;
    }

    // ... other fields

    void accept(ShapeVisitor *visitor) override {
        visitor->visitRectangle(this);
    }

private:
    int _width;
    int _height;
};

class Square : public Shape {
public:
    Square(int sideWidth) : _sideWidth(sideWidth) {}

    int getSideWidth() const {
        return _sideWidth;
    }

    // ... other fields

    void accept(ShapeVisitor *visitor) override {
        visitor->visitSquare(this);
    }

private:
    int _sideWidth;
};

class SquareCalculatorVisitor : public ShapeVisitor {
public:
    void visitCircle(Circle *circle) override {
        _square += circle->getRadius() * pow(M_PI, 2);
    }

    void visitRectangle(Rectangle *rectangle) override {
        _square += rectangle->getWidth() * rectangle->getHeight();
    }

    void visitSquare(Square *square) override {
        _square += pow(square->getSideWidth(), 2);
    }

    int getSquare() const {
        return _square;
    }

private:
    int _square = 0;
};

class PerimeterCalculatorVisitor : public ShapeVisitor {
public:
    void visitCircle(Circle *circle) override {
        _perimeter += 2 * circle->getRadius() * M_PI;
    }

    void visitRectangle(Rectangle *rectangle) override {
        _perimeter += 2 * (rectangle->getWidth() + rectangle->getHeight());
    }

    void visitSquare(Square *square) override {
        _perimeter += 4 * square->getSideWidth();
    }

    int getPerimeter() const {
        return _perimeter;
    }

private:
    int _perimeter = 0;
};

int main(int, char *[]) {
    std::vector<Shape*> shapes = {
            new Circle(10), new Rectangle(2, 5), new Square(7)
    };

    auto squareCalculatorVisitor = new SquareCalculatorVisitor();
    auto perimeterCalculatorVisitor = new PerimeterCalculatorVisitor();

    std::for_each(shapes.begin(), shapes.end(), [&](auto& s) {
        s->accept(squareCalculatorVisitor);
        s->accept(perimeterCalculatorVisitor);
    });

    std::cout << "Square: " << squareCalculatorVisitor->getSquare() << std::endl;
    std::cout << "Perimeter: " << perimeterCalculatorVisitor->getPerimeter() << std::endl;

    std::for_each(shapes.begin(), shapes.end(), [&](auto& s) {
        delete s;
        s = nullptr;
    });
    delete squareCalculatorVisitor;
    delete perimeterCalculatorVisitor;

    return 0;
}