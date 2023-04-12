// Паттерн Фабричный метод (Factory Method)

#include <string>
#include <memory>
#include <vector>
#include <algorithm>

// Абстрактный продукт
class AbstractResult {
public:
    virtual ~AbstractResult() = default;
};

// Конкретная реализация продукта
class MemoryCheckResult : public AbstractResult {
public:
    // TODO: some logic
};

// Конкретная реализация продукта
class CpuCheckResult : public AbstractResult {
public:
    // TODO: some logic
};

// Абстрактная реализация Creator
class PerformanceChecker {
public:
    virtual ~PerformanceChecker() = default;
    // фабричный метод
    virtual std::unique_ptr<AbstractResult> measure() = 0;
};

// Конкретная реализация Creator
class MemoryPerformanceChecker : public PerformanceChecker {
public:
    std::unique_ptr<AbstractResult> measure() override {
        // TODO: make some calculations
        return std::make_unique<MemoryCheckResult>();
    }
};

// Конкретная реализация Creator
class CpuPerformanceChecker : public PerformanceChecker {
public:
    std::unique_ptr<AbstractResult> measure() override {
        // TODO: make some calculations
        return std::make_unique<CpuCheckResult>();
    }
};

int main(int, char *[]) {
    std::vector<std::unique_ptr<PerformanceChecker>> performanceCheckers;
    performanceCheckers.push_back(std::make_unique<MemoryPerformanceChecker>());
    performanceCheckers.push_back(std::make_unique<CpuPerformanceChecker>());

    std::vector<std::unique_ptr<AbstractResult>> results;

    std::transform(
            performanceCheckers.begin(), performanceCheckers.end(), std::back_inserter(results),
            [](const auto& checker) {
                // вызов фабричного метода
                return checker->measure();
            });

    return 0;
}