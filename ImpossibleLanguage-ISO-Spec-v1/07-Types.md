# 07 — Types (Normative)

## 7.1 Built‑in Types
`number`, `string`, `bool`, `void`, `bytes`, `matrix<number>`, `list<T>`, `dict<K,V>`, `set<T>`, `tuple<T1, T2, ...>`

## 7.2 Nullable Types and Safety
Appending `?` forms a nullable type: `number?`, `string?`. Only `null` is assignable to the null state. IPL implements strict null safety with compile-time checks.

```
nullable-type = type "?" ;
```

**Valid**
```ipl
let number? maybeAge = null;
maybeAge = 37;
if (maybeAge != null) {
    let definiteAge = maybeAge;  // safe to use without check
}
```
**Invalid**
```ipl
let number? x = "no";    // type mismatch
let definiteAge = maybeAge;  // compile error: possible null dereference
```

## 7.3 Const and Final Types
IPL distinguishes between compile-time constants (`const`) and runtime constants (`final`).

- `const`: Compile-time constants that must be initialized with compile-time expressions
- `final`: Runtime constants that can be initialized once at runtime

**Valid**
```ipl
const number PI = 3.14159;           // compile-time constant
final string userName = readInput(); // runtime constant
```

## 7.4 Type Inference
Local `let` declarations **may** omit the type; it will be inferred from the initializer. IPL supports advanced type inference including flow-sensitive analysis for nullability.

**Valid**
```ipl
let x = 5;           // x: number
let msg = "ok";      // msg: string
let matrix = [[1, 2], [3, 4]];  // inferred as matrix<number>
```
**Invalid**
```ipl
let y;               // no initializer => cannot infer
```

## 7.5 Generics with Concepts
Generic forms use angle brackets. IPL supports concepts - named sets of constraints on generic parameters.

```
list<T>, dict<K,V>, matrix<T> where T : constraint
concept-decl = "concept" identifier "=" constraint-expression ;
constraint-expression = type-constraint { "and" type-constraint } ;
type-constraint = "Copyable" | "Comparable" | "Arithmetic" | interface-identifier ;
```

**Valid**
```ipl
concept Addable = Arithmetic;
function add<T>(T a, T b) where T : Addable { return a + b; }

concept Drawable = Drawable;  // constraint that type implements Drawable interface
function drawAll<T>(list<T> items) where T : Drawable {
    for each item in items { item.draw(); }
}
```
**Invalid**
```ipl
function add<T>(T a, T b) where T : string { return a + b; } // '+' not defined for string unless overloaded
```

## 7.6 Interfaces and Subtyping
IPL supports interface-based inheritance enabling polymorphism.

**Valid**
```ipl
interface Shape {
    number area();
    number perimeter();
}

class Circle implements Shape {
    number radius;
    function area() : number { return PI * radius * radius; }
    function perimeter() : number { return 2 * PI * radius; }
}
```

## 7.7 Traits
Traits provide a way to share code between classes without using inheritance.

```ipl
trait Loggable {
    string className() { return "Object"; }
    void log() { print("Logging " + className()); }
}

class Point {
    number x, y;
    inherits Loggable;  // mixin trait
}
```

## 7.8 Meta-Types
Meta-classes enable type introspection and meta-programming.

```ipl
meta class TypeInfo {
    string name();
    list<MethodInfo> methods();
    list<FieldInfo> fields();
}

let typeInfo = typeof(Point);  // returns TypeInfo for Point
```

## 7.9 Memory Safety Types
IPL supports ownership and borrowing concepts for memory safety.

- `owned<T>`: Exclusive ownership of a value
- `borrowed<T>`: Temporary access to a value without ownership
- `ref<T>`: Reference to a value (with lifetime annotation)

**Valid**
```ipl
function processOwned(owned<List<number>> list) {
    // takes ownership, list will be moved
}

function processBorrowed(borrowed<List<number>> list) {
    // borrows list, doesn't take ownership
    let len = list.length;  // safe to access
}
```

## 7.10 Subtyping and Conversion
- Structural subtyping is **not** supported; subtyping is nominal via `inherits` and `implements`.
- Implicit conversion is restricted to safe, non‑lossy cases (e.g., int → number if provided).
- All other conversions are explicit via `as` operator (Clause 12).
- Interface implementation establishes subtyping relationships.
