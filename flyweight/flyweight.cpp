// Паттерн Flyweight (приспособленец)

#include <vector>
#include <map>
#include <string>
#include <algorithm>

// Абстрактный класс цифрового фильтра - интерфейс приспособленца
class Filter {
public:
    virtual void apply(uint8_t* data, std::size_t size, std::vector<double>& state) = 0;
};

// Класс БИХ-фильтра - конкретный приспособленец
class IIRFilter : public Filter {
public:
    IIRFilter(std::vector<double> a, std::vector<double> b)
        : _a(a), _b(b) {}

    void apply(uint8_t* data, std::size_t size, std::vector<double>& state) override {
        // TODO: implementation of filter algorithm
    }

private:
    std::vector<double> _a;
    std::vector<double> _b;
};

// Класс КИХ-фильтра - конкретный приспособленец
class FIRFilter : public Filter {
public:
    FIRFilter(std::vector<double> kernel)
            : _kernel(kernel) {}

    void apply(uint8_t* data, std::size_t size, std::vector<double>& state) override {
        // TODO: implementation of filter algorithm
    }

private:
    std::vector<double> _kernel;
};

// Фабрика фильтров - фабрика приспособленцев
class FiltersFactory {
public:
    Filter* get(const std::string& key) {
        auto existFilter = _filters.find(key);
        if (existFilter != _filters.end()) {
            return existFilter->second;
        }

        if (key == "IIR_LOW_PASS") {
            _filters[key] = new IIRFilter({1, 2, 3}, {4, 5, 6});
        }
        else if (key == "FIR_HALF_BAND") {
            _filters[key] = new FIRFilter({ 1./4., 1./2., 1./4. });
        }
        // TODO: other filters

        return _filters[key];
    }

    ~FiltersFactory() {
        std::for_each(_filters.cbegin(), _filters.cend(), [](auto& f) {
            delete f.second;
        });
    }
private:
    std::map<std::string, Filter*> _filters;
};

int main(int, char *[]) {
    // Возможно не самый лучший пример, так как фильтров может быть не так много,
    // но в целом он соответствует структуре классов и идее разделения внутреннего и внешнего состояния,
    // используемой паттерном Flyweight.
    auto factory = new FiltersFactory();
    auto filter = factory->get("IIR_LOW_PASS");

    std::size_t bufferSize = 1000;
    auto buffer = new uint8_t[bufferSize];
    std::vector<double> state(3, 0.0);

    filter->apply(buffer, bufferSize, state);

    delete factory;
    delete[] buffer;

    return 0;
}