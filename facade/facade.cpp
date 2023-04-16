// Паттерн Фасад (facade)

#include <vector>
#include <string>
#include <functional>

enum Parameter {
    Loudness,
    Momentary,
    Range,
    TruePeak
    // ...
};

class LoudnessResult {};

class FileDecoder {
public:
    FileDecoder(const std::string& file) {}

    void setNewDataCallback(std::function<void(float*, std::size_t)> newDataCallback) {
        _newDataCallback = newDataCallback;
    }

    void start() {
        // TODO: decode file
    }

    void stop() {
        // TODO: stop decoding
    }

private:
    std::function<void(float*, std::size_t)> _newDataCallback;
};

class Ebur128Meter {
public:
    Ebur128Meter(const std::vector<Parameter>& parameters) {
        // TODO: init
    }

    void process(float*, std::size_t) {
        // TODO: calculate
    }

    std::vector<LoudnessResult> getResults() {
        return {};
    }
};

// Фасад, предоставляет простой интерфейс для вычисления параметров громкости файла
class LoudnessAnalyzer {
public:

    LoudnessAnalyzer(const std::string& file, const std::vector<Parameter>& parameters)
        : _decoder(file),
          _meter(parameters) {
        _decoder.setNewDataCallback([&](float* data, std::size_t size) {
            processNewData(data, size);
        });
    }

    void start() {
        _decoder.start();
    }

    void stop() {
        _decoder.stop();
    }

    std::vector<LoudnessResult> getResults() {
        return _meter.getResults();
    }

private:

    void processNewData(float* data, std::size_t size) {
        _meter.process(data, size);
    }

    FileDecoder _decoder;
    Ebur128Meter _meter;
};

int main(int, char *[]) {
    LoudnessAnalyzer analyzer("path to file", {Parameter::Loudness, Parameter::Momentary});
    analyzer.start();
    auto results = analyzer.getResults();

    return 0;
}