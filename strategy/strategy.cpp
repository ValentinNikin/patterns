// Паттерн Стратегия (Strategy)

#include <iostream>
#include <memory>
#include <queue>
#include <mutex>

/**
 * Реализация через определение интерфейсов классов Strategy и Context.
 */
namespace interface_implementation {

    class ISynchronizer {
    public:
        virtual ~ISynchronizer() = default;

        virtual void lock() = 0;
        virtual void unlock() = 0;
    };

    // Конкретные стратегии реализуют различные вариации алгоритма.
    class MutexThreadSafeSynchronizer : public ISynchronizer {
    public:
        void lock() override {
            std::cout << "MutexThreadSafeSynchronizer::lock()" << std::endl;
            _mutex.lock();
        }

        void unlock() override {
            _mutex.unlock();
            std::cout << "MutexThreadSafeSynchronizer::unlock()" << std::endl;
        }

    private:
        std::mutex _mutex;
    };

    class SingleThreadSafeSynchronizer : public ISynchronizer {
    public:
        void lock() override {
            std::cout << "SingleThreadSafeSynchronizer::lock()" << std::endl;
            // do nothing here
        }

        void unlock() override {
            std::cout << "SingleThreadSafeSynchronizer::unlock()" << std::endl;
            // do nothing here
        }
    };

    // Контекст - класс, выполняющий операции с помощью стратегий.
    template<typename T>
    struct Queue {
    public:
        // Клиент должен иметь возможность заменить стратегию на лету.
        Queue(std::unique_ptr<ISynchronizer> synchronizer)
                : _synchronizer{std::move(synchronizer)} {}

        void push(const T &value) {
            // Во время выполнения программы контекст получает вызовы от клиента и делегирует их объекту конкретной стратегии.
            _synchronizer->lock();

            _impl.push(value);

            _synchronizer->unlock();
        }

    private:
        std::queue <T> _impl;
        // Контекст хранит указатель на объект конкретной стратегии, работая с ним через общий интерфейс стратегий.
        std::unique_ptr<ISynchronizer> _synchronizer;
    };

    void example() {
        std::cout << "interface_implementation::example:" << std::endl;

        // Клиент должен создать объект конкретной стратегии и передать его в конструктор контекста.
        Queue<int> queue(std::make_unique<MutexThreadSafeSynchronizer>());

        queue.push(42);
        queue.push(50);

        std::cout << std::endl << std::endl;
    }
}

/**
 * Передача стратегии как параметра шаблона
 */
namespace templates_implementation {

    class MutexThreadSafeSynchronizer {
    public:
        void lock() {
            std::cout << "MutexThreadSafeSynchronizer::lock()" << std::endl;
            _mutex.lock();
        }

        void unlock() {
            _mutex.unlock();
            std::cout << "MutexThreadSafeSynchronizer::unlock()" << std::endl;
        }

    private:
        std::mutex _mutex;
    };

    class SingleThreadSafeSynchronizer {
    public:
        void lock() {
            std::cout << "SingleThreadSafeSynchronizer::lock()" << std::endl;
            // do nothing here
        }

        void unlock() {
            std::cout << "SingleThreadSafeSynchronizer::unlock()" << std::endl;
            // do nothing here
        }
    };

    template<typename T, typename Synchronizer = SingleThreadSafeSynchronizer>
    class Queue {
    public:
        void push(const T& value) {
            _synchronizer.lock();

            _impl.push(value);

            _synchronizer.unlock();
        }
    private:
        std::queue<T> _impl;
        Synchronizer _synchronizer;
    };

    void example() {
        std::cout << "templates_implementation::example:" << std::endl;

        // Передаем конкретные стратегии как параметр шаблона
        Queue<int, SingleThreadSafeSynchronizer> noThreadSafeQueue;

        noThreadSafeQueue.push(42);
        noThreadSafeQueue.push(24);

        Queue<int, MutexThreadSafeSynchronizer> mutexBasedQueue;

        mutexBasedQueue.push(42);
        mutexBasedQueue.push(24);

        std::cout << std::endl << std::endl;
    }
}

int main(int, char *[]) {
    interface_implementation::example();
    templates_implementation::example();

    return 0;
}