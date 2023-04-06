// Паттерн Мост (bridge)

#include <vector>

// Интерфейс для классов реализации
class WeatherForecasterImpl {
public:
    /*
     * Получить прогноз погоды на день
     * @return набор пар, first - час, second - градус
     */
    virtual std::vector<std::pair<int, int>> getDailyForecast() = 0;
};

class InternetWeatherForecasterImpl : public WeatherForecasterImpl {
    std::vector<std::pair<int, int>> getDailyForecast() override {
        // TODO: extract information about weather from some internet service
        return {};
    }
};

class NeuralNetworkWeatherForecasterImpl : public WeatherForecasterImpl {
    std::vector<std::pair<int, int>> getDailyForecast() override {
        // TODO: request weather forecast from inner neural network
        return {};
    }
};

// Абстракция с которой взаимодействует клиент
class WeatherForecaster {
public:
    WeatherForecaster() {
        // TODO: some logic to determine implementation
        _impl = new InternetWeatherForecasterImpl();
    }

    int getDegreeNow() {
        auto dailyForecast = _impl->getDailyForecast();
        // TODO: find correspond degree in forecast
        return dailyForecast[0].second;
    }
protected:
    // for refined abstractions
    WeatherForecasterImpl* getForecasterImpl() {
        return _impl;
    }

private:
    WeatherForecasterImpl* _impl;
};

int main(int, char *[]) {
    auto forecaster = new WeatherForecaster();
    auto degree = forecaster->getDegreeNow();

    delete forecaster;

    return 0;
}