# 11 — Classes, Objects, Operators, and Modules (Normative)

## 11.1 Enhanced Classes with Multiple Inheritance Support
```
class-decl = [ "export" ] [ decorator-list ] "class" identifier [ inheritance-spec ] class-body ;
inheritance-spec = ( "inherits" identifier { "," identifier } ) | ( "implements" identifier { "," identifier } ) | 
                   ( "inherits" identifier { "," identifier } "implements" identifier { "," identifier } ) ;
class-body = "{" { member-decl | interface-impl } "}" ;
member-decl = field | method | ctor | dtor | static-block | visibility ;
field       = [ visibility ] [ "static" ] [ "const" | "final" ] [ type ] identifier [ "=" expression ] ";" ;
method      = [ visibility ] [ "static" ] [ "virtual" | "override" | "final" ] "function" identifier "(" [ params ] ")" [ ":" type ] [ contract-clauses ] block ;
ctor        = [ visibility ] "function" "new" "(" [ params ] ")" [ ":" init-list ] block ;
dtor        = [ visibility ] "function" "delete" "(" ")" block ;  // destructor for deterministic cleanup
static-block = "static" "{" { statement } "}" ;
interface-impl = "function" identifier "(" [ params ] ")" [ ":" type ] block ;  // implementing interface method
init-list    = member-init { "," member-init } ;
member-init  = identifier "(" expression ")" ;
visibility   = "public" | "private" | "protected" | "internal" ;
decorator-list = { decorator } ;
decorator    = "@" identifier [ "(" [ argument-list ] ")" ] ;
```

**Valid**
```ipl
interface Drawable {
    void draw();
    number area();
}

interface Serializable {
    string serialize();
}

@dataClass
class Point implements Drawable, Serializable {
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
    
    function serialize() : string {
        return "(" + x + ", " + y + ")";
    }
    
    function move(number dx, number dy) { 
        x = x + dx; 
        y = y + dy; 
    }
    
    // Destructor for deterministic cleanup
    function delete() {
        print("Point at (" + x + ", " + y + ") is being destroyed");
    }
}

// Class with inheritance and initialization list
class ColoredPoint inherits Point {
    string color;
    
    function new(number x, number y, string color) : Point(x, y) {
        this.color = color;
    }
}
```
**Invalid**
```ipl
class Bad
  let x; // missing braces
```

## 11.2 Enhanced Operator Overloading
```
operator-decl = [ "export" ] [ decorator-list ] "operator" operator-symbol "(" param-list ")" [ ":" type ] block ;
operator-symbol = "+" | "-" | "*" | "/" | "%" | "^" | "==" | "!=" | "<" | "<=" | ">" | ">=" 
                | "[]" | "()" | "=" | "+=" | "-=" | "*=" | "/=" | "!" | "&" | "*" | "->" | "<<" | ">>"
                | custom-op ;
custom-op = "«" identifier "»" ; // unicode fences for custom ops
```
- Overloads must be unambiguous and respect arity.
- Additional operators for memory management: `&` (address-of), `*` (dereference).
- Assignment operators with compound assignment support.
- Precedence for custom operators is specified in Clause 12.

**Valid**
```ipl
class Vector {
    number x, y;
    
    function new(number x, number y) { this.x = x; this.y = y; }
    
    operator "+"(Vector a, Vector b) : Vector { 
        return new Vector(a.x + b.x, a.y + b.y); 
    }
    
    operator "*"(Vector a, number s) : Vector {  // scalar multiplication
        return new Vector(a.x * s, a.y * s); 
    }
    
    operator "[]"(number index) : number {  // element access
        if (index == 0) return x;
        else if (index == 1) return y;
        else throw IndexError("Vector index out of bounds");
    }
    
    operator "«cross»"(Vector a, Vector b) : number {  // custom operator
        return a.x * b.y - a.y * b.x; 
    }
    
    // Ownership operators
    operator "move"(Vector v) : owned<Vector> {  // transfer ownership
        return v;  // in real implementation, this would transfer ownership
    }
}

// Using the overloaded operators
let v1 = new Vector(1, 2);
let v2 = new Vector(3, 4);
let v3 = v1 + v2;           // uses operator "+"
let v4 = v1 * 5;            // uses operator "*"
let x_component = v1[0];    // uses operator "[]"
let result = v1 «cross» v2; // uses custom operator
```
**Invalid**
```ipl
operator "++"(Vector v) { } // cannot redefine built-in unary increment if not in language
```

## 11.3 Memory-Safe Classes with Ownership
IPL supports Rust-style ownership within classes for memory safety.

**Valid**
```ipl
class SafeList<T> {
    owned<list<T>> data;
    
    function new() {
        this.data = move(createEmptyList());  // transfer ownership
    }
    
    function push(T item) {
        this.data.add(item);
    }
    
    function getBorrowed() : borrowed<list<T>> {
        return &this.data;  // return borrowed reference
    }
    
    // When SafeList goes out of scope, its destructor runs and the owned data is freed
}
```

## 11.4 Meta-Classes and Reflection
Classes can be enhanced with meta-classes that provide runtime introspection capabilities.

**Valid**
```ipl
meta class Reflective {
    list<string> getFieldNames();
    list<string> getMethodNames();
    any getFieldValue(string fieldName);
    void setFieldValue(string fieldName, any value);
}

// A class that uses meta-programming
@reflective
class Person {
    string name;
    number age;
    
    function new(string name, number age) {
        this.name = name;
        this.age = age;
    }
    
    function greet() : string {
        return "Hello, I'm " + name;
    }
}

// Using reflection
let person = new Person("Alice", 30);
let meta = typeof(person);  // get meta-class instance
let fields = meta.getFieldNames();  // ["name", "age"]
```

## 11.5 Traits and Mixins
IPL supports trait-based composition for code reuse without deep inheritance hierarchies.

**Valid**
```ipl
trait Loggable {
    function log() {
        print("Object of type " + typeof(self).name() + " was used");
    }
}

trait Timestamped {
    number createdAt = getCurrentTime();
    
    function getAge() : number {
        return getCurrentTime() - this.createdAt;
    }
}

// Using traits
class DataProcessor implements Loggable, Timestamped {
    string data;
    
    function new(string data) {
        this.data = data;
        // Traits' fields are automatically included
    }
    
    function process() {
        this.log();  // from Loggable trait
        print("Processing for " + this.getAge() + " seconds");  // from Timestamped trait
    }
}
```

## 11.6 Modules and Packages
- `module` declares the module identity.
- `export` exposes declarations.
- `import` brings other modules into scope.
- Support for module aliases and selective imports.

**Valid**
```ipl
module graphics.shapes;

import core.collections as coll;  // alias for core.collections
import core.math { PI, sin, cos };  // selective import
import core.io;  // full import

export class Circle {
    number radius;
    
    function new(number radius) {
        this.radius = radius;
    }
    
    function area() : number {
        return PI * this.radius * this.radius;
    }
}

// Using modules
import graphics.shapes;
let circle = new Circle(5);
let area = circle.area();
```
