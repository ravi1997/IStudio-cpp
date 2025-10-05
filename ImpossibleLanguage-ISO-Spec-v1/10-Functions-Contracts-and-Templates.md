# 10 — Functions, Contracts, Decorators, and Templates (Normative)

## 10.1 Functions with Enhanced Features
```
func-decl = [ "export" ] [ decorator-list ] [ "static" ] "function" identifier "(" [ param-list ] ")" [ ":" type ] [ contract-clauses ] block ;
param-list = param { "," param } ;
param      = [ "const" ] [ "ref" | "owned" | "borrowed" ] [ type ] identifier [ "?" ] [ "=" expression ] ;
decorator-list = { decorator } ;
decorator  = "@" identifier [ "(" [ argument-list ] ")" ] ;
```
- Parameters may have default values; evaluation occurs at callsite.
- Parameters can specify ownership semantics: `owned`, `borrowed`, or `ref`.
- Parameters can be marked as nullable with `?`.
- Methods are functions declared inside classes.
- Dart-style optional parameters with default values are supported.

**Valid**
```ipl
function greet(string name = "world") : void { print("Hello " + name); }  // optional param
function processList(owned<list<number>> data) { /* takes ownership */ }  // ownership semantics
function logData(borrowed<string> msg) { print(msg); }  // borrow semantics
```
**Invalid**
```ipl
function f(a b) {} // missing comma
```

## 10.2 Enhanced Decorators
Decorators support Python-style meta-programming capabilities. Decorators can be stacked and accept parameters.

**Valid**
```ipl
@memoize
@performanceLog
@validate(argName="n", min=0)
function fib(number n) { 
    if (n < 2) return n; 
    return fib(n-1) + fib(n-2); 
}

// Decorator that transforms function behavior
@transactional  // runs function in a database transaction
function updateUserData(User user) {
    // changes are committed if no exception, rolled back otherwise
}
```

## 10.3 Enhanced Contracts
```
contract-func = [ "export" ] [ decorator-list ] "contract" "function" identifier "(" [ param-list ] ")" [ ":" type ] [ contract-body ] ;
contract-body = "{" { contract-clause } stmt* "}" ;
contract-clause = "require" expression [ "error" string ] ";" | 
                  "ensure" expression [ "error" string ] ";" |
                  "invariant" expression [ "error" string ] ";" ;
```
- `require` conditions are checked before body execution (preconditions).
- `ensure` conditions are checked after body execution (postconditions), may reference `result` (the returned value) and `old(expr)` (value before function call).
- `invariant` conditions define properties that must hold for the function's execution context.

**Valid**
```ipl
contract function divide(number a, number b) : number {
  require b != 0 error "Division by zero";
  require b > 0 error "Expected positive divisor";
  ensure result * b == a error "Result verification failed";
  ensure result <= a || b < 1 error "Result exceeds bounds unexpectedly";
  return a / b;
}

// Class invariant example
class BankAccount {
  number balance = 0;
  
  contract function withdraw(number amount) {
    require amount > 0 error "Withdrawal amount must be positive";
    require amount <= balance error "Insufficient funds";
    invariant balance >= 0 error "Balance cannot be negative";
    // ... implementation
  }
}
```
**Invalid**
```ipl
contract function f() { ensure x > 0; } // 'x' not in scope; result unspecified
```

## 10.4 Templates with Concepts
```
template-decl = "template" "<" type-params ">" declaration ;
type-params   = type-param { "," type-param } ;
type-param    = identifier [ ":" constraint ] ;
constraint    = identifier | concept-constraint ;
concept-constraint = identifier "(" constraint-expr ")" ;  // concept with additional constraints
```
- Templates support C++20-style concepts for constraining generic parameters.
- Compile‑time functions are declared with `constexpr` and are evaluated during translation when all inputs are constant.

**Valid**
```ipl
// Basic template
template <T: number>
function add(T a, T b) { return a + b; }

// Template constrained by concept
concept Addable = Arithmetic;
template <T: Addable>
function sum(list<T> values) : T {
    let result = values[0];
    for each val in values[1:] {
        result = result + val;
    }
    return result;
}

// Compile-time functions
constexpr function factorial(number n) : number {
    if (n <= 1) return 1;
    return n * factorial(n-1);
}
const number FACT_5 = factorial(5);

// Template with multiple constraints
template <T: Comparable and Copyable>
function max2(T a, T b) { return (a > b) ? a : b; }
```
**Invalid**
```ipl
constexpr function r() { return read(); } // not evaluable at compile-time
template <T: number> function f() {} where T : string; // conflicting constraints
```

## 10.5 Higher-Order Functions and Closures
IPL supports functions as first-class values with closure semantics.

**Valid**
```ipl
// Function type: (number, number) -> number
let adder = function(a, b) { return a + b; };
let result = adder(5, 3);  // result = 8

// Closure capturing outer scope
function makeAdder(number base) {
    return function(number n) { return base + n; };
}
let add10 = makeAdder(10);
let result = add10(5);  // result = 15
```

## 10.6 Function Overloading and Specialization
IPL supports function overloading based on parameter types, including ownership semantics.

**Valid**
```ipl
// Overloaded functions
function process(string data) { print("Processing string: " + data); }
function process(number data) { print("Processing number: " + data); }
function process(owned<list<number>> data) { print("Processing owned list"); }
function process(borrowed<list<number>> data) { print("Processing borrowed list"); }

// The correct overload is selected based on the argument's type and ownership
```
