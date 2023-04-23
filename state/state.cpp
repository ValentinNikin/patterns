// Паттерн Состояние (state)

#include <memory>

class TCPOctetStream;
class TCPState;

// Context
class TCPConnection {
public:
    TCPConnection();

    void activeOpen();
    void passiveOpen();
    void close();
    void send();
    void acknowledge();
    void synchronize();

    void processOctet(TCPOctetStream*) {}

private:
    friend class TCPState;
    void changeState(TCPState* state) {
        _state = state;
    }
private:
    TCPState* _state;
};

// State
class TCPState {
public:
    virtual void transmit(TCPConnection*, TCPOctetStream*) {}
    virtual void activeOpen(TCPConnection*) {}
    virtual void passiveOpen(TCPConnection*) {}
    virtual void close(TCPConnection*) {}
    virtual void send(TCPConnection*) {}
    virtual void acknowledge(TCPConnection*) {}
    virtual void synchronize(TCPConnection*) {}
protected:
    void changeState(TCPConnection* connection, TCPState* state) {
        connection->changeState(state);
    }
};

// Concrete state
class TCPEstablished : public TCPState {
public:
    static TCPState* getInstance() {
        if (_state == nullptr) {
            _state = std::unique_ptr<TCPEstablished>(new TCPEstablished());
        }
        return _state.get();
    }

    void transmit(TCPConnection* connection, TCPOctetStream* stream) override;
    void close(TCPConnection* connection) override;
private:
    TCPEstablished() = default;
    static std::unique_ptr<TCPEstablished> _state;
};

// Concrete state
class TCPListen : public TCPState {
public:
    static TCPState* getInstance() {
        if (_state == nullptr) {
            _state = std::unique_ptr<TCPListen>(new TCPListen());
        }
        return _state.get();
    }

    void send(TCPConnection* connection) override;
private:
    TCPListen() = default;
    static std::unique_ptr<TCPListen> _state;
};

// Concrete state
class TCPClosed : public TCPState {
public:
    static TCPState* getInstance() {
        if (_state == nullptr) {
            _state = std::unique_ptr<TCPClosed>(new TCPClosed());
        }
        return _state.get();
    }

    void activeOpen(TCPConnection* connection) override;
    void passiveOpen(TCPConnection* connection) override;
private:
    TCPClosed() = default;
    static std::unique_ptr<TCPClosed> _state;
};

std::unique_ptr<TCPEstablished> TCPEstablished::_state;
std::unique_ptr<TCPListen> TCPListen::_state;
std::unique_ptr<TCPClosed> TCPClosed::_state;

TCPConnection::TCPConnection() {
    _state = TCPClosed::getInstance();
}

void TCPEstablished::transmit(TCPConnection* connection, TCPOctetStream* stream) {
    connection->processOctet(stream);
}

void TCPEstablished::close(TCPConnection* connection) {
    changeState(connection, TCPListen::getInstance());
}

void TCPListen::send(TCPConnection* connection) {
    changeState(connection, TCPEstablished::getInstance());
}

void TCPClosed::activeOpen(TCPConnection* connection) {
    changeState(connection, TCPEstablished::getInstance());
}

void TCPClosed::passiveOpen(TCPConnection* connection) {
    changeState(connection, TCPListen::getInstance());
}

void TCPConnection::activeOpen() {
    _state->activeOpen(this);
}

void TCPConnection::passiveOpen() {
    _state->passiveOpen(this);
}

void TCPConnection::close() {
    _state->close(this);
}

void TCPConnection::send() {
    _state->send(this);
}

void TCPConnection::acknowledge() {
    _state->acknowledge(this);
}

void TCPConnection::synchronize() {
    _state->synchronize(this);
}

int main(int, char *[]) {
    auto connection = new TCPConnection();
    connection->passiveOpen();
    connection->send();
    connection->close();

    delete connection;

    return 0;
}