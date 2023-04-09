// Паттерн Команда (Command)

#include <iostream>
#include <memory>

/// Класс Receiver
class TV {
public:
    void on() {
        std::cout << "TV is on" << std::endl;
    }

    void off() {
        std::cout << "TV is off" << std::endl;
    }

    void mute() {
        std::cout << "TV is mute" << std::endl;
    }

    void unmute() {
        std::cout << "TV is unmute" << std::endl;
    }
};

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class TvOnCommand : public ICommand {
public:
    TvOnCommand(const std::shared_ptr<TV>& tv) : _tv(tv) {}

    void execute() override {
        _tv->on();
    }

    void undo() override {
        _tv->off();
    }
private:
    std::shared_ptr<TV> _tv;
};

class TvMuteCommand : public ICommand {
public:
    TvMuteCommand(const std::shared_ptr<TV>& tv) : _tv(tv) {}

    void execute() override {
        _tv->mute();
    }

    void undo() override {
        _tv->unmute();
    }
private:
    std::shared_ptr<TV> _tv;
};

// Класс Invoker
class RemoteController {
public:
    RemoteController(std::shared_ptr<ICommand> tvOnCommand, std::shared_ptr<ICommand> tvMuteCommand)
        : _tvOnCommand(tvOnCommand), _tvMuteCommand(tvMuteCommand) {}

    void clickOnButton() {
        _tvOnCommand->execute();
    }

    void clickMuteButton() {
        _tvMuteCommand->execute();
    }

private:
    std::shared_ptr<ICommand> _tvOnCommand;
    std::shared_ptr<ICommand> _tvMuteCommand;
};

int main(int, char *[]) {

    auto tv = std::make_shared<TV>();
    auto tvOnCommand = std::make_shared<TvOnCommand>(tv);
    auto tvMuteCommand = std::make_shared<TvMuteCommand>(tv);

    auto remoteController = std::make_unique<RemoteController>(tvOnCommand, tvMuteCommand);
    remoteController->clickOnButton();
    remoteController->clickMuteButton();

    return 0;
}