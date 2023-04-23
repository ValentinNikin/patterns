// Паттерн Интерпретатор (interpreter)

#include <iostream>
#include <string.h>
#include <map>
#include <memory>

namespace roman_numerals {

//    romanNumeral ::= {thousands} {hundreds} {tens} {ones}
//    thousands,hundreds,tens,ones ::= nine | four | {five} {one} {one} {one}
//    nine ::= "CM" | "XC" | "IX"
//    four ::= "CD" | "XL" | "IV"
//    five ::= 'D' | 'L' | 'V'
//    one  ::= 'M' | 'C' | 'X' | 'I'

    class Thousand;
    class Hundred;
    class Ten;
    class One;

    class RNInterpreter {
    public:
        int interpret(char* str);

    protected:
        virtual void interpret(char* input, int& total) {
            // for internal use
            int index = 0;
            if (strncmp(input, nine(), 2) == 0) {
                total += 9 * multiplier();
                index = 2;
            } else if (strncmp(input, four(), 2) == 0) {
                total += 4 * multiplier();
                index = 2;
            } else {
                if (input[0] == five()) {
                    total += 5 * multiplier();
                    index = 1;
                } else {
                    index = 0;
                }
                for (int end = index + 3; index < end; index++) {
                    if (input[index] == one())
                        total += 1 * multiplier();
                    else
                        break;
                }
            }
            strcpy(input, &(input[index]));
        } // remove leading chars processed

        // cannot be pure virtual because client asks for instance
        virtual char one() {}
        virtual char* four() {}
        virtual char five() {}
        virtual char* nine() {}
        virtual int multiplier() {}
    };

    class Thousand : public RNInterpreter {
    public:

    protected:
        char one() override { return 'M'; }
        char* four() override { return ""; }
        char five() override { return '\0'; }
        char* nine() override { return ""; }
        int multiplier() override { return 1000; }
    };

    class Hundred : public RNInterpreter {
    public:

    protected:
        char one() override { return 'C'; }
        char* four() override { return "CD"; }
        char five() override { return 'D'; }
        char* nine() override { return "CM"; }
        int multiplier() override { return 100; }
    };

    class Ten : public RNInterpreter {
    public:

    protected:
        char one() override { return 'X'; }
        char* four() override { return "XL"; }
        char five() override { return 'L'; }
        char* nine() override  { return "XC"; }
        int multiplier() override { return 10; }
    };

    class One : public RNInterpreter {
    public:

    protected:
        char one() { return 'I'; }
        char* four() { return "IV"; }
        char five() { return 'V'; }
        char* nine() { return "IX"; }
        int multiplier() { return 1; }
    };

    int RNInterpreter::interpret(char* input) {
        Thousand thousands;
        Hundred hundreds;
        Ten tens;
        One ones;

        int total = 0;
        thousands.interpret(input, total);
        hundreds.interpret(input, total);
        tens.interpret(input, total);
        ones.interpret(input, total);

        if (strcmp(input, ""))
            // if input was invalid, return 0
            return 0;
        return total;
    }

    void example() {
        RNInterpreter interpreter;
        char input[20];
        std::cout << "Enter Roman Numeral: ";
        while (std::cin >> input) {
            std::cout << "   interpretation is " << interpreter.interpret(input) << std::endl;
            std::cout << "Enter Roman Numeral: ";
        }
    }
} // namespace roman_numerals

namespace classic {

//    IExpression ::= NumberExpression | Constant | AddExpression | SubtractExpression
//    AddExpression ::= IExpression + IExpression
//    SubtractExpression ::= IExpression - IExpression
//    NumberExpression ::= [A-Z,a-z]+
//    Constant ::= [1-9]+

    class Context {
    public:
        int getVariable(const std::string& name) {
            if (_variables.find(name) == _variables.end()) {
                throw std::runtime_error("Value for variable " + name + " not found");
            }
            return _variables[name];
        }

        void setVariable(const std::string& name, int value) {
            _variables[name] = value;
        }
    private:
        std::map<std::string, int> _variables;
    };

    // интерфейс интерпретатора
    class IExpression {
    public:
        virtual ~IExpression() = default;
        virtual int interpret(Context* context) = 0;
    };

    // терминальное выражение
    class NumberExpression : public IExpression {
    public:

        NumberExpression(const std::string& name)
            : _name(name) {}

        int interpret(Context* context) override {
            return context->getVariable(_name);
        }
    private:
        std::string _name; // имя переменной
    };

    // нетерминальное выражение для сложения
    class AddExpression : public IExpression {
    public:

        AddExpression(std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right)
            : _left(std::move(left)), _right(std::move(right)) {}

        int interpret(Context* context) override {
            return _left->interpret(context) + _right->interpret(context);
        }

    private:
        std::unique_ptr<IExpression> _left;
        std::unique_ptr<IExpression> _right;
    };

    // нетерминальное выражение для вычитания
    class SubtractExpression : public IExpression {
    public:

        SubtractExpression(std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right)
            : _left(std::move(left)), _right(std::move(right)) {}

        int interpret(Context* context) override {
            return _left->interpret(context) - _right->interpret(context);
        }

    private:
        std::unique_ptr<IExpression> _left;
        std::unique_ptr<IExpression> _right;
    };

    void example() {
        auto context = std::make_unique<Context>();
        context->setVariable("x", 3);
        context->setVariable("y", 2);
        context->setVariable("z", 1);

        auto expression = std::make_unique<SubtractExpression>(
                std::make_unique<AddExpression>(
                    std::make_unique<NumberExpression>("x"),
                    std::make_unique<NumberExpression>("y")
                ),
                std::make_unique<NumberExpression>("z")
            );

        auto result = expression->interpret(context.get());

        std::cout << "Result: " << result << std::endl;
    }
}

int main(int, char *[]) {
    classic::example();
    roman_numerals::example();

    return 0;
}