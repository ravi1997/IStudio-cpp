# 17 — Examples: Valid and Invalid (Informative but Comprehensive)

## 17.1 Variables, Ownership, and Constants
**Valid**
```ipl
let number age = 25;
let message = "Hello"; // inferred type string

// Dart-style const and final
const number PI = 3.14159;           // compile-time constant
final string name = readName();      // runtime constant

// Rust-style ownership
let ownedList = [1, 2, 3];           // owned value
let borrowedRef = &ownedList;        // borrowed reference
let transferred = move ownedList;    // ownership transferred
```
**Invalid**
```ipl
let age number = 25;                 // order is type then identifier
5 = age;                             // LHS not assignable
const x = read();                    // const must be compile-time evaluable, use final instead
let used = someOwnedValue;
let duplicate = someOwnedValue;      // error: value already moved, ownership already transferred
```

## 17.2 Enhanced Control Flow
**Valid**
```ipl
// Traditional control flow
if (age >= 18) { print("adult"); } otherwise { print("minor"); }
repeat { tries = tries + 1; } until (tries == 3);

// Pattern matching
match (result) {
  case Success(value): { print("Success: " + value); }
  case Error(msg): { print("Error: " + msg); }
  default: { print("Unknown result"); }
}

// Enhanced for loops
for (let i = 0; i < 10; i = i + 1) { print(i); }
for each x in [1,2,3] { sum = sum + x; }
for each (key, value) in dict { print(key + " = " + value); }

// Labeled break and continue
outer: for (let i = 0; i < 10; i = i + 1) {
  for (let j = 0; j < 10; j = j + 1) {
    if (condition) {
      break outer;  // break out of outer loop
    }
  }
}
```
**Invalid**
```ipl
if age >= 18 { }                    // missing parentheses
repeat i = i + 1; until (i == 3);  // body must be a block
for each (x,y) in list { }         // list doesn't have key-value pairs
match (x) { }                      // match must have at least one case
```

## 17.3 Functions with Advanced Features
**Valid**
```ipl
// Function with optional parameters (Dart style)
function greet(string name = "world", number times = 1) : void {
    for (let i = 0; i < times; i = i + 1) {
        print("Hello " + name);
    }
}

// Enhanced contracts with error messages
contract function safeDiv(number a, number b) : number {
  require b != 0 error "Division by zero is not allowed";
  require b > 0 error "Expected positive divisor";
  ensure result * b == a error "Mathematical verification failed";
  return a / b;
}

// Template with concepts
concept Addable = Arithmetic;
template <T: Addable>
function sum(list<T> values) : T {
    let result = values[0];
    for each val in values[1:] {
        result = result + val;
    }
    return result;
}

// Higher-order functions
let adder = function(a, b) { return a + b; };
let numbers = [1, 2, 3, 4];
let doubled = numbers.map(x -> x * 2);

// Decorators
@memoize
@performanceLog
function expensiveCalc(number n) { /* implementation */ }
```
**Invalid**
```ipl
function f(a b) { }                    // missing comma
contract function g() { ensure x > 0; } // x not in scope; result unspecified
template <T: number> function f() {} where T : string; // conflicting constraints
let fn = function(x, x) { return x; }; // duplicate parameter names
```

## 17.4 Classes, Interfaces, Traits, and Memory Safety
**Valid**
```ipl
// Interface declaration
interface Drawable {
    void draw();
    number area();
}

// Trait declaration
trait Loggable {
    function log() {
        print("Object of type " + typeof(self).name() + " was used");
    }
}

// Enhanced class with multiple inheritance support
@dataClass
class Point implements Drawable, Loggable {
    public number x = 0;
    public number y = 0;
    
    function new(number x, number y) { 
        this.x = x; 
        this.y = y; 
    }
    
    function draw() { 
        print("Drawing point at (" + x + ", " + y + ")"); 
    }
    
    function area() : number { 
        return 0;  // points have no area
    }
    
    // Destructor for deterministic cleanup
    function delete() {
        print("Point at (" + x + ", " + y + ") is being destroyed");
    }
}

// Class with operator overloading and ownership
class SafeList<T> {
    owned<list<T>> data;
    
    function new() {
        this.data = move(createEmptyList());
    }
    
    function push(T item) {
        this.data.add(item);
    }
    
    function getBorrowed() : borrowed<list<T>> {
        return &this.data;
    }
    
    operator "+"(SafeList<T> a, SafeList<T> b) : SafeList<T> {
        let result = new SafeList<T>();
        for each item in a.data { result.push(item); }
        for each item in b.data { result.push(item); }
        return result;
    }
}

// Using the safe list
let list1 = new SafeList<number>();
list1.push(1);
list1.push(2);
let list2 = new SafeList<number>();
list2.push(3);
list2.push(4);
let combined = list1 + list2;  // uses overloaded operator
```
**Invalid**
```ipl
operator "++"(Point a) { }                    // pre/post increment not defined by the language
let ownedValue = createValue();
useValue(ownedValue);
useValue(ownedValue);                         // error: value already moved
class Bad
  let x;                                      // missing braces
class Derived inherits Base1, Base2 { }       // multiple inheritance not supported for classes (only interfaces)
```

## 17.5 Generics, Concepts, and Templates
**Valid**
```ipl
// Basic template
template <T: number>
function add(T a, T b) { return a + b; }

// Template constrained by concept
concept Comparable = LessThanComparable and EqualComparable;
template <T: Comparable>
function max2(T a, T b) { return (a > b) ? a : b; }

// Template with multiple constraints
template <T: Comparable and Copyable>
function sort(list<T> items) : list<T> {
    // implementation
}

// Meta-class example
meta class TypeInfo {
    string name();
    list<MethodInfo> methods();
    list<FieldInfo> fields();
}

let typeInfo = typeof(Point);  // returns TypeInfo for Point
```
**Invalid**
```ipl
template <> function f() {}                   // empty parameter list not permitted
template <T: number> function f() {} where T : string; // conflicting constraints
let result = add("hello", "world");          // template constraint violation
```

## 17.6 Matrix Operations (MATLAB Style)
**Valid**
```ipl
// Matrix operations
let A = [[1, 2], [3, 4]];
let B = [[5, 6], [7, 8]];
let C = A ** B;        // matrix multiplication
let D = A .* B;        // element-wise multiplication
let E = A + B;         // element-wise addition
let F = A[0,1];        // matrix element access
let G = A[0:1, 0:1];   // matrix slicing (first row, first column)

// Vector operations
let v1 = [1, 2, 3];
let v2 = [4, 5, 6];
let dotProduct = v1 «dot» v2;  // custom operator
```

## 17.7 Error Handling
**Valid**
```ipl
// Exception handling
try { 
    riskyOperation(); 
} catch (TypeError e) { 
    handleError(e); 
} catch (any e) { 
    handleGenericError(e); 
} finally { 
    cleanup(); 
}

// Result type for functional error handling
function divide(number a, number b) : Result<number, string> {
    if (b == 0) {
        return Result::error("Division by zero");
    }
    return Result::ok(a / b);
}

let result = divide(10, 2);
match (result) {
    case Result::ok(value): { print("Result: " + value); }
    case Result::error(msg): { print("Error: " + msg); }
}

// Optional type for potential absence
function findUser(string id) : Optional<User> {
    if (userExists(id)) {
        return Optional::some(getUser(id));
    }
    return Optional::none();
}

let userOpt = findUser("123");
if (userOpt.isPresent()) {
    print("Found: " + userOpt.get().name);
} else {
    print("User not found");
}
```
**Invalid**
```ipl
try { } catch (e) { } catch (f) { }        // catch blocks must be ordered from specific to general
throw;                                     // requires expression to throw
let value = someOptionalValue.get();       // potentially unsafe - should check isPresent() first
```
