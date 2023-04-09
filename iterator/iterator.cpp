// Паттерн Итератор (Iterator)

#include <iostream>
#include <vector>
#include <stdexcept>

namespace reference_implementation {

template<class Item>
class Iterator {
public:
    virtual ~Iterator() = default;

    virtual void reset() = 0;
    virtual void next() = 0;
    virtual bool isDone() const = 0;
    virtual Item currentItem() = 0;
};

template<class Item>
class IEnumerable {
public:
    virtual ~IEnumerable() = default;
    virtual Iterator<Item>* createIterator() = 0;
};

template<typename Item>
class ArrayIterator;

template<typename Item>
class Array : IEnumerable<Item> {
public:
    std::size_t size() const {
        return _innerArr.size();
    }

    Item& get(std::size_t ind) {
        return _innerArr[ind];
    }

    void add(const Item& item) {
        _innerArr.push_back(item);
    }

    Iterator<Item>* createIterator() override {
        return new ArrayIterator<Item>(this);
    }

private:
    std::vector<Item> _innerArr;
};

template<typename Item>
class ArrayIterator : public Iterator<Item> {
public:
    ArrayIterator(Array<Item>* array)
        : _array(array) {}

    void reset() override {
        _pos = 0;
    }

    void next() override {
        _pos++;
    }

    bool isDone() const override {
        return _pos >= _array->size();
    }

    Item currentItem() override {
        if (isDone()) {
            throw std::runtime_error("Iterator out of range");
        }

        return _array->get(_pos);
    }

private:
    Array<Item>* _array;
    std::size_t _pos = 0;
};

void example() {
    std::cout << "Reference implementation of iterator" << std::endl;

    Array<int>* arr = new Array<int>();
    arr->add(1);
    arr->add(2);
    arr->add(3);
    arr->add(4);
    arr->add(5);

    auto iterator = arr->createIterator();
    for (iterator->reset(); !iterator->isDone(); iterator->next()) {
        std::cout << iterator->currentItem() << " ";
    }
    std::cout << std::endl;

    delete iterator;
    delete arr;
}

}

namespace alternative_implementation {

template<typename T>
class LinkedList {
private:
    struct Node {
        T value;
        Node* next { nullptr };
        ~Node() {
            delete next;
        }
    };

    Node* _head { nullptr };
    Node* _tail { nullptr };
public:
    class Iterator {
    private:
        Node* _current;
    public:
        Iterator(Node* node) : _current(node) {}
        Iterator& operator++() {
            _current = _current->next;
            return *this;
        }
        bool operator==(Iterator other) const { return _current == other._current; }
        bool operator!=(Iterator other) const { return !(*this == other); }
        T operator*() { return _current->value; }
    };

    LinkedList() = default;
    ~LinkedList() {
        delete _head;
    }
    LinkedList(const LinkedList&) = delete;
    LinkedList(LinkedList&&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;
    LinkedList& operator=(LinkedList&&) = delete;

    void push_back(T value) {
        if (_tail) {
            _tail->next = new Node();
            _tail = _tail->next;
        } else {
            _head = new Node();
            _tail = _head;
        }
        _tail->value = value;
    }

    Iterator begin() {
        return { _head };
    }
    Iterator end() {
        return { nullptr };
    }
};

void example() {
    std::cout << "Alternative implementation of iterator" << std::endl;

    LinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    auto iter = list.begin();
    while(iter != list.end()) {
        std::cout << *iter << " ";
        ++iter;
    }
    std::cout << std::endl;

    for (auto i : list) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
}

namespace alternative_implementation_stl {

template<long FROM, long TO>
class Range {
public:
    class iterator {
        long num = FROM;
    public:
        iterator(long _num = 0) : num(_num) {}
        iterator& operator++() {
            num = (TO >= FROM) ? (num + 1) : (num - 1);
            return *this;
        }
        iterator operator++(int) {
            iterator retval = *this;
            ++(*this);
            return retval;
        }
        bool operator==(iterator other) const { return num == other.num; }
        bool operator!=(iterator other) const { return !(*this == other); }
        long operator*() { return num; }

        // iterator traits
        using difference_type = long;
        using value_type = long;
        using pointer = const long*;
        using reference = const long&;
        using iterator_category = std::forward_iterator_tag;
    };

    iterator begin() { return FROM; }
    iterator end() { return (TO >= FROM) ? (TO + 1) : (TO - 1); }
};

void example() {
    std::cout << "Alternative implementation of iterator with C++ STL" << std::endl;

    for(auto num: Range<1, 5>{}) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    for(auto num: Range<5, 1>{}) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

}

int main(int, char *[]) {
    reference_implementation::example();
    alternative_implementation::example();
    alternative_implementation_stl::example();

    return 0;
}