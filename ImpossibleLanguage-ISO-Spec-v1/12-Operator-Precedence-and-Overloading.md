# 12 — Operator Precedence, Associativity, and Overloading (Normative)

## 12.1 Precedence Table
From highest to lowest (operators of the same row have equal precedence):
1.  Postfix: `() [] .`  
2.  Unary: `+ - ! & *`  (unary plus, unary minus, logical NOT, address-of, dereference)  
3.  Exponent: `^`  
4.  Multiplicative: `* / % ** .* ./`  (** matrix multiply, .* element-wise multiply, ./ element-wise divide)  
5.  Additive: `+ -`  
6.  Shift: `<< >>`  
7.  Relational: `< <= > >=`  
8.  Equality: `== !=`  
9.  Bitwise AND: `&`
10. Bitwise XOR: `^`
11. Bitwise OR: `|`
12. Logical AND: `and`  
13. Logical OR: `or`  
14. Conditional: `? :`  
15. Assignment: `= += -= *= /= %= ^= &= |= <<= >>=`
16. Throw: `throw`
17. Lambda: `=>`

Associativity: postfix L→R, unary R→L, exponent R→L, others L→R, assignment R→L, conditional R→L, lambda R→L.

## 12.2 Custom Operator Classes
Custom operators **shall** be assigned to one of the above precedence levels via declaration:
```
precedence-decl = "operator" "precedence" operator-symbol "at" level [ "associativity" ( "left" | "right" | "none" ) ] ";" ;
level           = integer in [1,17] ;
```

**Valid**
```ipl
operator precedence «dot» at 4;
operator precedence «outer» at 3 associativity left;
```
**Invalid**
```ipl
operator precedence «dot» at 0; // out of range
operator precedence «dot» at 18; // out of range
```

## 12.3 Advanced Operator Overloading
IPL supports comprehensive operator overloading including assignment operators, comparison chains, and matrix operations.

### Assignment Operators
All assignment operators can be overloaded:
```
operator "=" (Self this, T other) : void
operator "+=" (Self this, T other) : Self
operator "-=" (Self this, T other) : Self
// etc. for *=, /=, %=, ^=, &=, |=, <<=, >>=
```

**Valid**
```ipl
class Counter {
    number value = 0;
    
    function new(number initial) { this.value = initial; }
    
    operator "+" (Counter a, Counter b) : Counter {
        return new Counter(a.value + b.value);
    }
    
    operator "+=" (Counter this, number amount) : Counter {
        this.value = this.value + amount;
        return this;
    }
    
    operator "==" (Counter a, Counter b) : bool {
        return a.value == b.value;
    }
    
    operator "<" (Counter a, Counter b) : bool {
        return a.value < b.value;
    }
}
```

### Matrix and Vector Operators
Support for MATLAB-style mathematical operations:

**Valid**
```ipl
class Matrix {
    // ... matrix implementation
    
    operator "*" (Matrix A, Matrix B) : Matrix {  // matrix multiplication
        // implementation
    }
    
    operator ".*" (Matrix A, Matrix B) : Matrix {  // element-wise multiplication
        // implementation
    }
    
    operator "+" (Matrix A, Matrix B) : Matrix {  // element-wise addition
        // implementation
    }
    
    operator "*" (Matrix A, number s) : Matrix {  // scalar multiplication
        // implementation
    }
    
    operator "[]" (Matrix m, number row, number col) : number {  // element access
        // implementation
    }
}
```

### Comparison Chaining
IPL supports chained comparisons as in Python:

**Valid**
```ipl
let a = 1, b = 2, c = 3;
let result = a < b < c;  // equivalent to (a < b) and (b < c)
```

### Ownership Operators
Memory-safe operators for Rust-style ownership:

**Valid**
```ipl
class OwnedResource {
    // ... resource implementation
    
    operator "move" (OwnedResource r) : owned<OwnedResource> {
        // transfer ownership
    }
    
    operator "&" (OwnedResource r) : borrowed<OwnedResource> {
        // create borrowed reference
    }
    
    operator "clone" (OwnedResource r) : OwnedResource {
        // create a copy
    }
}
```

## 12.4 Operator Resolution with Concepts
When resolving overloaded operators, the system considers concept constraints:

**Valid**
```ipl
concept Arithmetic = Addable and Subtractable and Multipliable;
concept Ordered = Comparable and LessThanComparable;

// Only works with Arithmetic types
template <T: Arithmetic>
operator "+" (T a, T b) : T { return a.add(b); }

// Only works with Ordered types
template <T: Ordered>
operator "<" (T a, T b) : bool { return a.lessThan(b); }
```

## 12.5 Overload Resolution
- Candidate functions are selected by name/operator, arity, and concept constraints.
- Best viable function chosen by exact match > convertible > user‑defined conversion (if provided).
- When multiple candidates are equally good, the most constrained (concept-wise) is chosen.
- Ambiguity is ill‑formed (requires diagnostic).
