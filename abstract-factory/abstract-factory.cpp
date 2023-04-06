// Паттерн Абстрактная фабрика (Abstract factory)

#include <string>

// Абстрактный продукт, производимый фабрикой
class MemoryInfoExtractor {
public:
    enum class MemoryType {
        free = 0,
        available,
        used
    };

    virtual ~MemoryInfoExtractor() = default;
    virtual std::size_t extract(MemoryType memoryType) = 0;
};

// Абстрактный продукт, производимый фабрикой
class OsInfoExtractor {
public:
    struct OsInfo {
        std::string name;
        std::string version;
        std::string bitDepth;
        // ...
    };

    virtual ~OsInfoExtractor() = default;
    virtual OsInfo extract() = 0;
};

// Extractor для извлечения информации о памяти в Linux системе
class LinuxMemoryInfoExtractor : public MemoryInfoExtractor {
public:
    std::size_t extract(MemoryType memoryType) override {
        // extract some information from /proc/meminfo
        return 1e6;
    }
};

// Extractor для извлечения информации о памяти в Windows системе
class WindowsMemoryInfoExtractor : public MemoryInfoExtractor {
public:
    std::size_t extract(MemoryType memoryType) override {
        // extract information about memory
        return 1e6;
    }
};

// Extractor для извлечения информации об OS в Linux системе
class LinuxOsInfoExtractor : public OsInfoExtractor {
public:
    OsInfo extract() override {
        // extract info about os
        return OsInfo();
    }
};

// Extractor для извлечения информации об OS в Windows системе
class WindowsOsInfoExtractor : public OsInfoExtractor {
public:
    OsInfo extract() override {
        // extract info about os
        return OsInfo();
    }
};

// Абстрактная фабрика
class ExtractorsFactory {
public:
    virtual ~ExtractorsFactory() = default;

    virtual MemoryInfoExtractor* buildMemoryInfoExtractor() = 0;
    virtual OsInfoExtractor* buildOsInfoExtractor() = 0;
    // ...
};

// Конкретная фабрика, которая создает Extractor-ы специфичные для Linux системы
class LinuxExtractorsFactory : public ExtractorsFactory {
public:
    MemoryInfoExtractor* buildMemoryInfoExtractor() override {
        return new LinuxMemoryInfoExtractor();
    }

    OsInfoExtractor* buildOsInfoExtractor() override {
        return new LinuxOsInfoExtractor();
    }
};

// Конкретная фабрика, которая создает Extractor-ы специфичные для Windows системы
class WindowsExtractorsFactory : public ExtractorsFactory {
public:
    MemoryInfoExtractor* buildMemoryInfoExtractor() override {
        return new WindowsMemoryInfoExtractor();
    }

    OsInfoExtractor* buildOsInfoExtractor() override {
        return new WindowsOsInfoExtractor();
    }
};

int main(int, char *[]) {

    // determine in runtime that is Linux OS
    auto extractorsFactory = new LinuxExtractorsFactory();

    auto memInfoExtractor = extractorsFactory->buildMemoryInfoExtractor();
    auto osInfoExtractor = extractorsFactory->buildOsInfoExtractor();

    auto usedMemory = memInfoExtractor->extract(MemoryInfoExtractor::MemoryType::used);
    auto osInfo = osInfoExtractor->extract();

    delete memInfoExtractor;
    delete osInfoExtractor;
    delete extractorsFactory;

    return 0;
}