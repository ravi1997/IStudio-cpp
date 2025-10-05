# 14 — Errors, Exceptions, and Diagnostics (Normative)

## 14.1 Exception Hierarchy
IPL implements a comprehensive exception hierarchy inspired by Java and Rust:

```
Exception
├── SystemException
│   ├── MemoryException
│   │   ├── OutOfMemoryError
│   │   └── InvalidMemoryAccessError
│   ├── ThreadException
│   └── IOException
├── LogicException
│   ├── ArgumentException
│   │   ├── NullArgumentException
│   │   └── InvalidEnumArgumentException
│   ├── InvalidOperationException
│   └── ContractException
│       ├── PreconditionException
│       ├── PostconditionException
│       └── InvariantException
└── RuntimeException
    ├── NullReferenceError
    ├── IndexError
    ├── KeyError
    ├── TypeError
    ├── DivisionByZero
    ├── OwnershipViolationError
    └── BorrowingError
```

## 14.2 Exception Handling and Result Types
IPL supports both exception-based and functional error handling:

### Traditional Exception Handling
```ipl
try { 
    riskyOperation(); 
} catch (TypeError e) { 
    logTypeError(e); 
} catch (any e) {  // catch all other exceptions
    logGenericError(e); 
} finally { 
    cleanup(); 
}
```

### Result Type for Functional Error Handling
IPL provides `Result<T, E>` and `Optional<T>` types for functional error handling:

```ipl
// Result<T, E> type for operations that can fail
function divide(number a, number b) : Result<number, string> {
    if (b == 0) {
        return Result::error("Division by zero");
    }
    return Result::ok(a / b);
}

// Using the result
let result = divide(10, 2);
match (result) {
    case Result::ok(value): { print("Result: " + value); }
    case Result::error(msg): { print("Error: " + msg); }
}

// Optional<T> type for potential absence of value
function findUser(string id) : Optional<User> {
    if (userExists(id)) {
        return Optional::some(getUser(id));
    }
    return Optional::none();
}

// Using optional
let userOpt = findUser("123");
if (userOpt.isPresent()) {
    let user = userOpt.get();
    print("Found user: " + user.name);
} else {
    print("User not found");
}
```

**Valid**
```ipl
try { 
    risky(); 
} catch (Exception e) {  // catch base exception type
    log(e); 
} finally { 
    cleanup(); 
}

// Using multiple specific catch blocks
try {
    complexOperation();
} catch (FileNotFoundError e) {
    handleFileNotFound(e);
} catch (PermissionError e) {
    handlePermission(e);
} catch (Exception e) {  // general catch-all
    handleGeneralError(e);
}
```
**Invalid**
```ipl
throw; // requires expression to throw
try { } catch (e) { } catch (f) { }  // catch blocks must be ordered from specific to general
```

## 14.3 Ownership-Related Exceptions
New exception types for memory safety violations:

- `OwnershipViolationError`: Thrown when attempting to use a value after it has been moved
- `BorrowingError`: Thrown when borrowing rules are violated (e.g., mutable and immutable references to same value)

**Valid**
```ipl
try {
    let ownedList = createList();
    processList(move ownedList);  // transfer ownership
    print(ownedList.length);  // this would cause OwnershipViolationError
} catch (OwnershipViolationError e) {
    print("Attempted to use moved value: " + e.getMessage());
}
```

## 14.4 Diagnostics
An implementation **shall** report diagnostics for:
- Syntax errors.
- Type mismatches.
- Ambiguous overload resolution.
- Use of undefined names.
- Contract violations detected at compile time (constexpr contracts).
- Ownership violations at compile time when possible.
- Borrowing rule violations.
- Concept constraint violations.
- Unsafe operations in safe code contexts.
- Null dereference possibilities in safe code.
- Potential memory leaks in deterministic memory management contexts.

## 14.5 Diagnostic Quality
IPL implementations **should** provide:
- Precise source location information (file, line, column).
- Multi-line diagnostics with primary and secondary spans.
- Suggested fixes where possible.
- Candidate overload information for ambiguous resolution.
- Lifetime error explanations for borrowing violations.

**Valid**
```
Error [E0123]: Cannot borrow `list` as mutable more than once at a time
  --> example.ipl:15:5
   |
14 | let mut_ref1 = &mut list;
   |                  ------ first mutable borrow occurs here
15 | let mut_ref2 = &mut list;
   |     ^^^^^^^^           ---- second mutable borrow occurs here
   |
   = help: Consider using different variables or restructure the code
```
