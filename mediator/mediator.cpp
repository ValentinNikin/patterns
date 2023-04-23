// Паттер Посредник (mediator)

#include <iostream>
#include <memory>
#include <map>
#include <algorithm>

enum class ModuleType {
    Unknown = 0,
    Decoder,
    FrameProcessor,
    // ...
};

class AbstractMessage {};
class SomeMessage : public AbstractMessage {};

class ModulesContainer;

class AbstractModule {
public:
    AbstractModule(const std::shared_ptr<ModulesContainer>& container)
        : _container(container) {}
    virtual ~AbstractModule() = default;

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void receiveMessage(std::unique_ptr<AbstractMessage> message) = 0;

protected:
    std::shared_ptr<ModulesContainer> _container;
};

class DecoderModule : public AbstractModule {
public:
    DecoderModule(const std::shared_ptr<ModulesContainer>& container)
        : AbstractModule(container) {}

    void start() override;

    void stop() override {
        // some logic
    }

    void receiveMessage(std::unique_ptr<AbstractMessage> message) override {
        std::cout << "Decoder module received message" << std::endl;
    }
};

class FrameProcessorModule : public AbstractModule {
public:
    FrameProcessorModule(const std::shared_ptr<ModulesContainer>& container)
            : AbstractModule(container) {}

    void start() override;

    void stop() override {
        // some logic
    }

    void receiveMessage(std::unique_ptr<AbstractMessage> message) override {
        std::cout << "Frame processor module received message" << std::endl;
    }
};

// Класс - посредник
class ModulesContainer : public std::enable_shared_from_this<ModulesContainer> {
public:
    void init() {
        _modules[ModuleType::Decoder] = std::make_unique<DecoderModule>(shared_from_this());
        _modules[ModuleType::FrameProcessor] = std::make_unique<FrameProcessorModule>(shared_from_this());
    }

    void start() {
        std::for_each(_modules.cbegin(), _modules.cend(), [](auto& m) {
            m.second->start();
        });
    }

    void sendMessage(const ModuleType to, std::unique_ptr<AbstractMessage> message) {
        auto moduleIt = _modules.find(to);
        if (moduleIt == _modules.end()) {
            throw std::runtime_error("Module not found");
        }

        moduleIt->second->receiveMessage(std::move(message));
    }

private:
    std::map<ModuleType, std::unique_ptr<AbstractModule>> _modules;
};

void DecoderModule::start() {
    std::cout << "Decoder module started" << std::endl;
    std::cout << "Send message to FrameProcessor" << std::endl;
    _container->sendMessage(ModuleType::FrameProcessor, std::make_unique<SomeMessage>());
}

void FrameProcessorModule::start() {
    std::cout << "Frame processor module started" << std::endl;
    std::cout << "Send message to Decoder" << std::endl;
    _container->sendMessage(ModuleType::Decoder, std::make_unique<SomeMessage>());
}

int main(int, char *[]) {
    auto container = std::make_shared<ModulesContainer>();
    container->init();
    container->start();

    return 0;
}
