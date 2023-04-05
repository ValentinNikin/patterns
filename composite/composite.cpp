// Паттерн Компоновщик (Composite)

#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>

class FileSystemObject {
public:
    virtual ~FileSystemObject() = default;

    std::string name() { return _name; }

    virtual std::size_t size() = 0;

    // Метод addObject можно было бы объявить в классе Directory, но в этом случае
    // пришлось бы уметь различать File от Directory в runtime.
    virtual void addObject(FileSystemObject *) {
        throw std::runtime_error("Method is not implemented");
    }

protected:
    FileSystemObject(const std::string& name) : _name(name) {}
private:
    std::string _name;
};

class File : public FileSystemObject {
public:
    File(const std::string& name, const std::size_t size)
        : FileSystemObject(name), _size(size) {}

    std::size_t size() override {
        return _size;
    }
private:
    std::size_t _size;
};

class Directory : public FileSystemObject {
public:
    Directory(const std::string& name) : FileSystemObject(name) {}

    ~Directory() override {
        for (auto fso : _nestedObjects) {
            delete fso;
        }
    }

    std::size_t size() override {
        std::size_t total = 0;
        for (auto fso : _nestedObjects) {
            total += fso->size();
        }
        return total;
    }

    void addObject(FileSystemObject *fso) override {
        _nestedObjects.push_back(fso);
    }

private:
    std::vector<FileSystemObject *> _nestedObjects;
};

int main(int, char *[]) {
    auto* directory = new Directory("root");
    directory->addObject(new File("temp.txt", 200));
    directory->addObject(new File("film.avi", 1000));

    std::cout << directory->size() << std::endl;

    delete directory;

    return 0;
}