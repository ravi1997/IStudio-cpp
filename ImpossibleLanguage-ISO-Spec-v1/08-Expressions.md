# 08 — Expressions (Normative)

## 8.1 Grammar
```
expression     = assignment ;
assignment     = conditional | unary-assign | ownership-transfer ;
unary-assign   = identifier "=" expression ;
ownership-transfer = "move" expression | "ref" expression | "&" expression | "*" expression ;
conditional    = logic-or [ "?" expression ":" expression ] ;
logic-or       = logic-and { "or" logic-and } ;
logic-and      = equality { "and" equality } ;
equality       = relational { ("==" | "!=") relational } ;
relational     = additive { ("<" | "<=" | ">" | ">=") additive } ;
additive       = multiplicative { ("+" | "-") multiplicative } ;
multiplicative = exponent { ("*" | "/" | "%" | "**") exponent } ;  // ** for matrix multiply
exponent       = unary { "^" unary } ;
unary          = [ "!" | "-" | "+" | "&" | "*" ] unary | postfix ; // & for borrow, * for dereference
postfix        = primary { call | index | member | access } ;
call           = "(" [ argument-list ] ")" ;
index          = "[" expression "]" ;  // for arrays/lists
matrix-index   = "[" expression "," expression "]" ;  // for matrices
member         = "." identifier ;
access         = "." identifier [ "(" [ argument-list ] ")" ] ;  // method call or field access
primary        = identifier | literal | "(" expression ")" | "new" type "(" [ argument-list ] ")" | 
                 "[" [ expression { "," expression } "]" |     // array literal
                 "[" [ "[" expression { "," expression } "]" { "," "[" expression { "," expression } "]" } "]" ;  // matrix literal
argument-list  = expression { "," expression } ;
```

## 8.2 Evaluation Order
- Function call arguments evaluate left‑to‑right.
- Member and index receivers evaluate before arguments.
- Short‑circuiting applies to `and`/`or`.
- Ownership transfers are evaluated before use to ensure safety.

## 8.3 Advanced Expressions

### Ownership and Borrowing Expressions
IPL supports Rust-style ownership and borrowing:
- `&x` creates a borrowed reference to `x`
- `*x` dereferences a reference `x`
- `move x` transfers ownership of `x`

**Valid**
```ipl
let ownedList = [1, 2, 3];        // owned value
let borrowedRef = &ownedList;     // borrowed reference
let length = borrowedRef.length;  // access through reference
let transferred = move ownedList; // ownership transferred
```

### Matrix Operations
Enhanced matrix expressions for MATLAB-style operations:

**Valid**
```ipl
let A = [[1, 2], [3, 4]];
let B = [[5, 6], [7, 8]];
let C = A ** B;        // matrix multiplication
let D = A .* B;        // element-wise multiplication
let E = A + B;         // element-wise addition
let F = A[0,1];        // matrix element access
```

### Tuple Expressions
Support for tuple destructuring and access:

**Valid**
```ipl
let pair = (10, "hello");     // tuple literal
let (num, text) = pair;       // destructuring
let first = pair.0;           // tuple element access
```

### Enhanced Member Access
IPL supports safe navigation for nullable types:

**Valid**
```ipl
let person? = getPerson();    // nullable type
let age = person?.age;        // safe navigation - returns null if person is null
let result = person?.getName()?.toUpperCase();  // chained safe navigation
```

## 8.4 Examples

**Valid**
```ipl
result = a * (b + c);
name = person.first.toUpper();
A[2,3] = 7;                    // matrix indexer (overloaded)
area = PI * r ^ 2;
let matrixResult = M1 ** M2;   // matrix multiplication
let borrowedVal = &someValue;  // borrow expression
let dereferenced = *borrowedVal; // dereference
```

**Invalid (requires diagnostic)**
```ipl
3 = x;             // assignment target must be assignable
f(,x);             // missing argument
a + ;              // missing right operand
*borrowedRef;      // dereferencing a borrowed reference without proper lifetime
let used = someOwnedValue;
let duplicate = someOwnedValue; // error: value already moved
```

## 8.5 Matrix Expressions
Implementations shall support elementwise `+ - * /` and matrix multiply `**` as well as MATLAB-style operations.

**Valid**
```ipl
C = A ** B;        // matrix multiply
D = A .* B;        // element-wise multiply
E = A * 2;         // scalar broadcast
F = A[1:3, 2:4];   // matrix slicing
```
**Invalid**
```ipl
E = A ** 3;        // '**' expects matrix operands
```
