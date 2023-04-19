// Паттерн Заместитель (Proxy)

#include <iostream>
#include <map>
#include <algorithm>
#include <stdint.h>

/**
 * Продемонстировать работу паттерна можно на примере решения следующей задачи:
 *
 * Спроектировать 2-мерную разреженную бесконечную матрицу, заполненную значениями по
 * умолчанию. Матрица должна хранить только занятые элементы - значения которых хотя бы раз
 * присваивались. Присвоение в ячейку значения по умолчанию освобождает ячейку.
 * При чтении элемента из свободной ячейки возвращать значение по умолчанию.
 *
 * Класс Matrix является proxy классом, который контролирует доступ к структуре map, для решения
 * поставленной задачи.
 * Классы MatrixRowProxy и MatrixValueProxy эмулируют доступ, соответственно к строке матрицы
 * и к конкретному значению матрицы.
 */

template<typename ItemType, ItemType defaultValue>
class Matrix {
private:
    using StorageType = std::map<std::pair<uint32_t, uint32_t>, ItemType>;
public:

    class MatrixValueProxy {
    public:
        operator ItemType();
        void operator=(const ItemType& value);
        ItemType get() const;

        MatrixValueProxy() = delete;

        MatrixValueProxy(const MatrixValueProxy& mrp) = delete;
        MatrixValueProxy& operator=(const MatrixValueProxy& mrp) = delete;

        MatrixValueProxy(MatrixValueProxy&& mrp) = delete;
        MatrixValueProxy& operator=(MatrixValueProxy&& mrp) = delete;
    private:
        friend class Matrix<ItemType, defaultValue>;
        friend class MatrixRowProxy;

        MatrixValueProxy(Matrix<ItemType, defaultValue>& parent, uint32_t row, uint32_t column);

        Matrix<ItemType, defaultValue>& _parent;
        uint32_t _row;
        uint32_t _column;
    };

    class MatrixRowProxy {
    public:
        MatrixValueProxy operator[](uint32_t column);

        MatrixRowProxy() = delete;

        MatrixRowProxy(const MatrixRowProxy& mrp) = delete;
        MatrixRowProxy& operator=(const MatrixRowProxy& mrp) = delete;

        MatrixRowProxy(MatrixRowProxy&& mrp) = delete;
        MatrixRowProxy& operator=(MatrixRowProxy&& mrp) = delete;
    private:
        friend class Matrix<ItemType, defaultValue>;

        MatrixRowProxy(Matrix<ItemType, defaultValue>& parent, uint32_t row);

        Matrix<ItemType, defaultValue>& _parent;
        uint32_t _row;
    };

public:

    Matrix() = default;
    ~Matrix() = default;

    MatrixRowProxy operator[](uint32_t row);

    std::size_t size() const;
    bool empty() const;
private:
    ItemType get(uint32_t row, uint32_t column);
    void set(uint32_t row, uint32_t column, const ItemType& value);

    StorageType _innerStorage;
};

/// Matrix implementation

template<typename ItemType, ItemType defaultValue>
typename Matrix<ItemType, defaultValue>::MatrixRowProxy Matrix<ItemType, defaultValue>::operator[](uint32_t row) {
    return MatrixRowProxy(*this, row);
}

template<typename ItemType, ItemType defaultValue>
void Matrix<ItemType, defaultValue>::set(uint32_t row, uint32_t column, const ItemType& value) {
    if (value == defaultValue) {
        if (!_innerStorage.count({row, column})) return;
        _innerStorage.erase({row, column});
    }
    else {
        _innerStorage[{row, column}] = value;
    }
}

template<typename ItemType, ItemType defaultValue>
ItemType Matrix<ItemType, defaultValue>::get(uint32_t row, uint32_t column) {
    if (!_innerStorage.count({row, column})) return defaultValue;
    return _innerStorage[{row, column}];
}

template<typename ItemType, ItemType defaultValue>
std::size_t Matrix<ItemType, defaultValue>::size() const {
    return _innerStorage.size();
}

template<typename ItemType, ItemType defaultValue>
bool Matrix<ItemType, defaultValue>::empty() const {
    return size() == 0;
}

/// MatrixRowProxy implementation

template<typename ItemType, ItemType defaultValue>
Matrix<ItemType, defaultValue>::MatrixRowProxy::MatrixRowProxy(
        Matrix<ItemType, defaultValue>& parent, uint32_t row)
        : _parent(parent), _row(row) {}

template<typename ItemType, ItemType defaultValue>
typename Matrix<ItemType, defaultValue>::MatrixValueProxy
Matrix<ItemType, defaultValue>::MatrixRowProxy::operator[](uint32_t column) {
    return MatrixValueProxy(_parent, _row, column);
}

/// MatrixValueProxy implementation

template<typename ItemType, ItemType defaultValue>
Matrix<ItemType, defaultValue>::MatrixValueProxy::MatrixValueProxy(
        Matrix<ItemType, defaultValue>& parent, uint32_t row, uint32_t column)
        : _parent(parent), _row(row), _column(column) {}

template<typename ItemType, ItemType defaultValue>
Matrix<ItemType, defaultValue>::MatrixValueProxy::operator ItemType() {
    return get();
}

template<typename ItemType, ItemType defaultValue>
void Matrix<ItemType, defaultValue>::MatrixValueProxy::operator=(const ItemType& value) {
    _parent.set(_row, _column, value);
}

template<typename ItemType, ItemType defaultValue>
ItemType Matrix<ItemType, defaultValue>::MatrixValueProxy::get() const {
    return _parent.get(_row, _column);
}

int main(int, char *[]) {
    Matrix<int, -1> matrix;

    std::cout << "Size: " << matrix.size() << std::endl;
    std::cout << "Expected default value: " << matrix[0][0] << std::endl;
    std::cout << "Expected default value: " << matrix[100][100] << std::endl;
    std::cout << "Expected default value: " << matrix[1000000][1000000] << std::endl;
    std::cout << "Size: " << matrix.size() << std::endl;

    matrix[200][500] = 10;
    matrix[500][200] = 10;
    matrix[500][500] = 10;

    std::cout << "Size: " << matrix.size() << std::endl;

    matrix[500][500] = -1;

    std::cout << "Size: " << matrix.size() << std::endl;

    return 0;
}