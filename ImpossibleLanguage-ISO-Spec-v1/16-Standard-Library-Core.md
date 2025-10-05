# 16 — Standard Library Core (Normative)

This clause specifies a comprehensive, portable standard library incorporating features from multiple programming paradigms.

## 16.1 `core.io`
- `print(any) : void` — textual output.
- `readLine() : string?` — input line or `null` at EOF.
- `readFile(string path) : Result<string, IOError>` — read file contents.
- `writeFile(string path, string content) : Result<void, IOError>` — write file.
- `async readFileAsync(string path) : Future<Result<string, IOError>>` — async file reading.
- `console.inputStream() : Stream<string>` — input stream for event-based I/O.

## 16.2 `core.math` - Enhanced with MATLAB-style Operations
- Constants: `PI`, `E`, `TAU`, `INFINITY`, `NAN`.
- Basic Functions: `abs`, `sqrt`, `pow`, `sin`, `cos`, `tan`, `log`, `exp`.
- Matrix Functions: `zeros(rows,cols)`, `identity(n)`, `ones(rows,cols)`, `rand(rows,cols)`.
- Matrix Operations: `multiply(matrix A, matrix B)`, `transpose(matrix)`, `determinant(matrix)`, `inverse(matrix)`.
- Vector Operations: `dot(Vector a, Vector b)`, `cross(Vector a, Vector b)`, `magnitude(Vector v)`.

**Enhanced Example:**
```ipl
import core.math;
let matrixA = [[1, 2], [3, 4]];
let matrixB = [[5, 6], [7, 8]];
let result = multiply(matrixA, matrixB);  // matrix multiplication
let identity3 = identity(3);              // 3x3 identity matrix
```

## 16.3 `core.collections` - Enhanced with Functional and Memory-Safe Operations
- `list<T>` operations: `push`, `pop`, `map`, `filter`, `reduce`, `fold`, `forEach`.
- `dict<K,V>` operations: `get`, `set`, `has`, `remove`, `keys`, `values`, `entries`.
- `set<T>` operations: `add`, `remove`, `has`, `union`, `intersection`, `difference`.
- `vector<T>`: memory-safe, owned dynamic array with move semantics: `push`, `pop`, `get`, `set`.
- `slice<T>`: borrowed view of contiguous memory: `length`, `get`, `subslice`.

**Enhanced Example:**
```ipl
import core.collections;
let numbers = [1, 2, 3, 4, 5];
let squares = numbers.map(x -> x * x);
let evens = numbers.filter(x -> x % 2 == 0);
let sum = numbers.reduce(0, (acc, x) -> acc + x);

// Memory-safe vector operations
let ownedVec = Vector::new([1, 2, 3]);
processVector(move ownedVec);  // transfer ownership
// ownedVec is no longer accessible here
```

## 16.4 `core.memory` - Rust-Style Memory Management
- `owned<T>`: wrapper for owned values with deterministic destruction.
- `borrowed<T>`: wrapper for borrowed references.
- `ref<T>`: wrapper for shared references with reference counting.
- `Box<T>`: heap-allocated value with owned semantics.
- `Rc<T>`: reference-counted shared ownership.
- `Arc<T>`: atomic reference-counted shared ownership for concurrency.
- `drop(value)` : void — explicitly drop a value.

**Memory Management Example:**
```ipl
import core.memory;
let boxedNumber = Box::new(42);  // heap allocated
let sharedList = Rc::new([1, 2, 3]);  // reference counted
let clonedRef = sharedList.clone();  // increment reference count
// Both sharedList and clonedRef point to same data
```

## 16.5 `core.functional` - Higher-Order Functions and Functionals
- `function<T, R>(T -> R)` : function type.
- `compose<F, G>(F f, G g) : (T -> R)` — function composition.
- `partial<F>(F f, args...) : (remaining_args... -> R)` — partial application.
- `curry<F>(F f) : (T1 -> (T2 -> ... -> R))` — currying.
- `memoize<F>(F f) : F` — memoized function.

**Functional Example:**
```ipl
import core.functional;
let add = (number a, number b) -> a + b;
let add5 = partial(add, 5);  // partially applied function
let result = add5(10);  // result is 15
```

## 16.6 `core.meta` - Reflection and Meta-Programming
- `typeof(value) : TypeInfo` — get type information.
- `TypeInfo.name() : string` — get type name.
- `TypeInfo.methods() : list<MethodInfo>` — get methods.
- `TypeInfo.fields() : list<FieldInfo>` — get fields.
- `MethodInfo.invoke(object, args...) : any` — method invocation.
- `hasAnnotation(value, string annotation) : bool` — check for annotations.

**Meta-Programming Example:**
```ipl
import core.meta;
class Person {
    string name;
    number age;
    
    function new(string name, number age) {
        this.name = name;
        this.age = age;
    }
}

let person = new Person("Alice", 30);
let typeInfo = typeof(person);
print("Type name: " + typeInfo.name());
let fields = typeInfo.fields();
// Process fields via reflection
```

## 16.7 `core.concurrency` - Actor Model and Async Programming
- `spawn(function() -> any) : TaskId` — spawn concurrent task.
- `Channel<T>::new() : Channel<T>` — create communication channel.
- `channel.send(T value) : Result<void, ChannelError>` — send on channel.
- `channel.receive() : Result<T, ChannelError>` — receive from channel.
- `Future<T>` and `Promise<T>` for async programming.
- `async` and `await` support as keywords.

**Concurrency Example:**
```ipl
import core.concurrency;
async function fetchData(string url) : string {
    // async implementation
}

// Using async/await
async function processAll() {
    let data1 = await fetchData("url1");
    let data2 = await fetchData("url2");
    return combine(data1, data2);
}
```

## 16.8 `core.result` - Functional Error Handling
- `Result<T, E>`: enum with `Ok(T)` and `Error(E)` variants.
- `Optional<T>`: enum with `Some(T)` and `None` variants.
- `Result::ok(T value) : Result<T, E>` — create success result.
- `Result::error(E error) : Result<T, E>` — create error result.
- `Optional::some(T value) : Optional<T>` — create some optional.
- `Optional::none() : Optional<T>` — create none optional.
- `result.map(F fn) : Result<U, E>` — map success value.
- `result.andThen(F fn) : Result<U, E>` — flat map for success.

**Error Handling Example:**
```ipl
import core.result;
function safeDivide(number a, number b) : Result<number, string> {
    if (b == 0) {
        return Result::error("Division by zero");
    }
    return Result::ok(a / b);
}

let result = safeDivide(10, 2);
match (result) {
    case Result::ok(value): { print(value); }
    case Result::error(err): { print("Error: " + err); }
}
```

## 16.9 `core.traits` - Trait System Support
- `trait` definition support in the standard library.
- `impl Trait for Type` syntax for implementing traits.
- `dyn Trait` for trait objects (dynamic dispatch).

**Trait Example:**
```ipl
import core.traits;  // Not needed as traits are built-in
trait Drawable {
    function draw() : void;
}

class Circle implements Drawable {
    function draw() { print("Drawing a circle"); }
}

function render(dyn Drawable drawable) {  // dynamic dispatch
    drawable.draw();
}
```

**Examples (Valid)**
```ipl
import core.io;
import core.math;
import core.collections;
import core.result;

// Combined functionality example
let numbers = [1, 2, 3, 4, 5];
let result = numbers
    .filter(x -> x > 2)
    .map(x -> x * x)
    .reduce(0, (a, b) -> a + b);

print("Sum of squares: " + result);
```

**Invalid**
```ipl
print = 5; // attempting to assign to a function name
```
