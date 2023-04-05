// Паттерн Декоратор (Decorator)

#include <string>
#include <fstream>
#include <memory>

class Stream {
public:
    virtual void write(const char* data, const std::size_t size) = 0;
    virtual std::size_t read(char* data, const std::size_t size) = 0;
};

class FileStream : public Stream {
public:
    FileStream(const std::string& path) {
        _stream.open(path);
    }

    void write(const char* data, const std::size_t size) override {
        _stream.write(data, size);
    }

    std::size_t read(char* data, const std::size_t size) {
        _stream.read(data, size);
        return _stream.gcount();
    }
private:
    std::fstream _stream;
};

// Класс EncryptedStream является декоратором класса Stream
class EncryptedStream : public Stream {
public:
    EncryptedStream(std::unique_ptr<Stream> stream)
        : _stream(std::move(stream)) {}

    void write(const char* data, const std::size_t size) {
        // TODO: выполнить шифрование данных
        _stream->write(data, size);
    }

    std::size_t read(char* data, const std::size_t size) {
        char* encryptedData = new char[size];
        _stream->read(encryptedData, size);
        // TODO: расшифровать encryptedData и записать в data, вернуть размер расшифрованных данных
        return size;
    }

private:
    std::unique_ptr<Stream> _stream;
};


int main(int, char *[]) {
    auto stream = std::make_unique<EncryptedStream>(std::make_unique<FileStream>("path to file"));

    const std::size_t size = 1e5;
    char* data = new char[size];
    // fill data

    stream->write(data, size);

    const std::size_t otherSize = 1e3;
    char* otherData = new char[otherSize];

    stream->read(otherData, otherSize);
    // process data

    delete[] data;
    delete[] otherData;

    return 0;
}