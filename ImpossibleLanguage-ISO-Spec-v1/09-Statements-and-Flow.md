# 09 — Statements and Control Flow (Normative)

## 9.1 Grammar
```
statement     = block | var-decl | const-decl | final-decl | expr-stmt | if-stmt | match-stmt | while-stmt | repeat-stmt | for-stmt | for-each-stmt
              | return-stmt | break-stmt | continue-stmt | try-stmt | throw-stmt | unsafe-stmt | owned-stmt ;
block         = "{" { statement } "}" ;
expr-stmt     = expression ";" ;
if-stmt       = "if" "(" expression ")" block [ "otherwise" block ] ;
match-stmt    = "match" "(" expression ")" "{" { "case" pattern ":" block } [ "default" ":" block ] "}" ;
while-stmt    = "while" "(" expression ")" block ;
repeat-stmt   = "repeat" block "until" "(" expression ")" ";" ;
for-stmt      = "for" "(" [ for-init ] ";" [ expression ] ";" [ for-update ] ")" block ;
for-each-stmt = "for" "each" identifier "in" expression block ;
for-each-kv-stmt = "for" "each" "(" identifier "," identifier ")" "in" expression block ;  // for key-value iteration
return-stmt   = "return" [ expression ] ";" ;
break-stmt    = "break" [ identifier ] ";" ;  // labeled break
continue-stmt = "continue" [ identifier ] ";" ;  // labeled continue
try-stmt      = "try" block "catch" "(" [ type ] identifier ")" block [ "finally" block ] ;
throw-stmt    = "throw" expression ";" ;
unsafe-stmt   = "unsafe" block ;  // for memory-unsafe operations
owned-stmt    = "owned" "(" identifier ")" block ;  // for moving owned values into block

for-init      = var-decl | expr-stmt ;
for-update    = expression ;
pattern       = literal | identifier | "_" | "(" pattern { "," pattern } ")" | constructor "(" [ pattern { "," pattern } ] ")" ;
```

## 9.2 Semantics

### If and Match Statements
- The condition in `if` shall be of type `bool` after implicit truthiness rules (no implicit conversion from number).
- `match` provides pattern matching similar to Rust/ML languages.

**Valid**
```ipl
if (isReady()) { start(); } otherwise { wait(); }

match (result) {
  case Success(value): { print("Success: " + value); }
  case Error(msg): { print("Error: " + msg); }
  default: { print("Unknown result"); }
}
```
**Invalid**
```ipl
if (1) { } // requires diagnostic: non-boolean condition
```

### Repeat‑Until
- The body executes at least once; loop terminates when the condition becomes `true`.

**Valid**
```ipl
repeat { i = i + 1; } until (i >= 10);
```
**Invalid**
```ipl
repeat i = i + 1; until (i >= 10); // body must be a block
```

### For and For-Each
- Traditional C-style for loops and foreach iteration.
- For-Each can iterate over key-value pairs in dictionaries.

**Valid**
```ipl
for (let i = 0; i < 10; i = i + 1) { print(i); }
for each x in [1,2,3] { sum = sum + x; }
for each (key, value) in dict { print(key + " = " + value); }
```
**Invalid**
```ipl
for each (x,y) in list { } // list doesn't have key-value pairs
```

### Exception Handling with Type Specifications
IPL allows catching specific exception types:

**Valid**
```ipl
try {
  riskyOperation();
} catch (TypeError e) {
  handleTypeError(e);
} catch (any e) {  // catch any other exception type
  handleGenericError(e);
} finally {
  cleanup();
}
```

### Unsafe Blocks
For memory-unsafe operations that bypass ownership checks:

**Valid**
```ipl
unsafe {
  // Operations that might violate memory safety
  let ptr = getAddress(someValue);
  *ptr = newValue;  // Direct memory manipulation
}
```

### Labeled Break and Continue
For nested loops:

**Valid**
```ipl
outer: for (let i = 0; i < 10; i = i + 1) {
  for (let j = 0; j < 10; j = j + 1) {
    if (condition) {
      break outer;  // break out of outer loop
    }
  }
}
```

### Ownership Statements
For transferring ownership into blocks:

**Valid**
```ipl
let ownedList = createExpensiveList();
owned(ownedList) {  // move ownership of ownedList into this block
  processList(ownedList);  // ownedList is now owned by this block
}  // ownedList is destroyed at end of block
```
