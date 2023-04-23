// Паттерн Наблюдатель (observer)

#include <iostream>
#include <list>
#include <algorithm>

class Subject;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(Subject* subject) = 0;
};

class Subject {
public:
    virtual ~Subject() = default;

    void attach(Observer* observer) {
        _observers.push_back(observer);
    }

    void detach(Observer* observer) {
        _observers.remove(observer);
    }

    void notify() {
        std::for_each(_observers.begin(), _observers.end(), [this](auto& o) {
            o->update(this);
        });
    }

private:
    std::list<Observer*> _observers;
};

// Concrete subject
class Timer : public Subject {
public:
    void run() {
        for (int i = 0; i < 10; i++) {
            _second++;
            notify();
        }
    }

    int getHour() { return _hour; }
    int getMinute() { return _minute; }
    int getSecond() { return _second; }

private:
    int _hour = 0;
    int _minute = 0;
    int _second = 0;
};

// Concrete observer
class Clock : public Observer {
public:
    Clock(Timer* subject)
        : _subject(subject) {
        _subject->attach(this);
    }

    void update(Subject* subject) override {
        if (subject == _subject) {
            std::cout << _subject->getHour() << ":" << _subject->getMinute() << ":" << _subject->getSecond() << std::endl;
        }
    }

private:
    Timer* _subject;
};

int main(int, char *[]) {
    auto timer = new Timer();
    auto clock = new Clock(timer);

    timer->run();
    timer->detach(clock);

    delete clock;
    delete timer;

    return 0;
}