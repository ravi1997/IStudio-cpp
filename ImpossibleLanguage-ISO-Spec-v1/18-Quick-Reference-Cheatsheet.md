# 18 — Quick Reference Cheatsheet (Informative)

## Declarations
```ipl
let <type?> <name> = <expr>;                    // mutable variable
const <type> <NAME> = <constexpr-expr>;        // compile-time constant
final <type?> <name> = <expr>;                  // runtime constant
function <name>([params]) [: type] { ... }      // function
contract function <name>([params]) [: type] {   // function with contracts
  require <condition>;
  ensure <condition>;
  ...
}
@decorator                                      // function with decorators
function <name>([params]) [: type] { ... }
class Name [inherits Base] [implements Interface] { ... }  // class with inheritance
interface Name { method(); ... }                // interface
trait Name { method() { ... } ... }             // trait
meta class Name : type { ... }                  // meta-class
operator "+"(T a, T b) [: T] { ... }            // operator overload
template <T: constraint> declaration            // generic template
concept Name = constraint-expression;           // concept definition
```

## Variables and Ownership
```ipl
let ownedValue = createValue();                 // owned value
let borrowedRef = &ownedValue;                  // borrowed reference
let transferred = move ownedValue;              // ownership transfer
let fn = (x, y) -> x + y;                      // lambda expression
```

## Control Flow
```ipl
if (cond) { ... } otherwise { ... }             // conditional
match (value) {                                 // pattern matching
  case Pattern1: { ... }
  case Pattern2: { ... }
  default: { ... }
}
while (cond) { ... }
repeat { ... } until (cond);
for (init; cond; update) { ... }                // C-style for loop
for each x in xs { ... }                        // for-each
for each (k, v) in dict { ... }                 // key-value iteration
try { ... } catch (e) { ... } finally { ... }   // exception handling
try { ... } catch (TypeError e) { ... }         // typed exceptions
unsafe { ... }                                  // unsafe block
```

## Types
- Primitives: `number`, `string`, `bool`, `void`, `bytes`
- Nullable: `T?`
- Collections: `list<T>`, `dict<K,V>`, `set<T>`, `matrix<number>`, `tuple<T1, T2, ...>`
- Constants: `const` (compile-time), `final` (runtime)
- Memory-safe: `owned<T>`, `borrowed<T>`, `ref<T>`
- Functional: `Result<T, E>`, `Optional<T>`
- Other: `any`, `Self`

## Operators
- Arithmetic: `+ - * / % ** ^` (`**` for matrix multiply, `^` for exponent)
- Assignment: `= += -= *= /= %= ^= &= |= <<= >>=`
- Logical: `and or !`
- Comparison: `== != < <= > >=`
- Membership: `in` (for collections)
- Access: `[] . ?.`
- Element-wise: `.* ./ .^` (for matrices)
- Ownership: `&` (borrow), `*` (dereference), `move`
- Custom: `«operatorName»` (enclosed in unicode angle brackets)

## Functions
```ipl
function name(type param = defaultValue)        // with optional params
function process(owned<T> data)                 // ownership semantics
function log(borrowed<T> data)                  // borrowed parameter
@decorator1 @decorator2                         // multiple decorators
function name() { ... }
template <T: constraint> function name(T x)     // generic function
```

## Classes and OOP
```ipl
class Name {
  field = value;                                // field with default
  function new(params) { ... }                  // constructor
  function delete() { ... }                     // destructor
  function method() { ... }                     // method
  static field = value;                         // static field
  static function staticMethod() { ... }        // static method
}
class Derived inherits Base implements Interface { ... }
operator "+"(Self a, Self b) : Self { ... }     // operator overload in class
```

## Standard Library Modules
- `core.io`: `print()`, `readLine()`, file operations
- `core.math`: `PI`, `sqrt()`, `sin()`, matrix operations
- `core.collections`: `list<T>`, `dict<K,V>`, `set<T>`, functional operations
- `core.memory`: `owned<T>`, `borrowed<T>`, `Box<T>`, `Rc<T>`
- `core.functional`: higher-order functions, function composition
- `core.meta`: reflection, `typeof()`, annotations
- `core.concurrency`: async/await, channels, tasks
- `core.result`: `Result<T, E>`, `Optional<T>`, functional error handling
- `core.traits`: trait system support

## Null Safety
```ipl
let value? : number? = null;                    // nullable type
if (value != null) {                            // null check
  let safeValue = value;                        // safe to use
}
let chained = obj?.prop?.method();              // safe navigation
```

## Pattern Matching
```ipl
match (expr) {
  case literal: { ... }
  case identifier: { ... }                      // binds value to identifier
  case constructor(arg1, arg2): { ... }
  case (a, b, c): { ... }                      // tuple destructuring
  default: { ... }
}
```
