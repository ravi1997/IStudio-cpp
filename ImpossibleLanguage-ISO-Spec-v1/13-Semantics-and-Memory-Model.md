# 13 — Semantics and Memory Model (Normative)

## 13.1 Execution Model
- IPL is a **statically typed, ahead‑of‑time compiled** language; interpreters **may** exist.
- Evaluation is strict (eager).
- IPL supports both garbage-collected and deterministic memory management models.

## 13.2 Object Model
- Primitives are value types; objects (class instances, collections) are reference types.
- `this` refers to the current instance; `super` refers to the base class.
- IPL supports both class-based and prototype-based inheritance through meta-classes.

## 13.3 Memory Management and Ownership
IPL implements Rust-style ownership semantics for memory safety:

### 13.3.1 Ownership Rules
1. Each value has a single owner at a time
2. When the owner goes out of scope, the value is dropped
3. You can transfer ownership (move) between variables
4. You can have multiple immutable references OR one mutable reference to a value

### 13.3.2 Ownership Transfers
- `move x` transfers ownership of `x` to another variable
- When a value is moved, the original variable becomes invalid
- Functions can take ownership of parameters marked as `owned<T>`

**Valid**
```ipl
function processList(owned<list<number>> data) {
    // 'data' is now owned by this function
    // it will be dropped when the function ends
}

let myList = [1, 2, 3];
processList(move myList);  // transfer ownership
// myList is no longer accessible here
```

### 13.3.3 Borrowing
- `&x` creates an immutable borrowed reference to `x`
- `&mut x` creates a mutable borrowed reference to `x` (syntax may vary)
- Borrowing follows Rust's borrowing rules: either multiple immutable references OR one mutable reference

**Valid**
```ipl
function readList(borrowed<list<number>> data) {
    // 'data' is borrowed, not owned
    print("Length: " + data.length);
    // 'data' is returned to original owner when function ends
}

let myList = [1, 2, 3];
readList(&myList);  // borrow myList immutably
// myList is still accessible here
```

### 13.3.4 Lifetimes and Scopes
- Variables are valid from their declaration until the end of their lexical scope
- Owned values are dropped when they go out of scope
- Borrowed values remain valid as long as the original owner is valid and not mutated
- Lifetime annotations may be required for complex borrowing scenarios

## 13.4 Lifetimes and Storage
- **Stack**: Local variables with value types and references
- **Heap**: Objects created with `new`, owned containers
- **Static**: Global and static variables with program lifetime
- IPL implementations **shall** provide either:
  - Automatic garbage collection with concurrent marking and sweeping
  - Deterministic destruction based on ownership and borrowing
  - A hybrid approach with precise control over memory management

## 13.5 Null Safety
IPL implements strict null safety:
- Types are non-nullable by default
- Nullable types are explicitly marked with `?` (e.g., `string?`)
- Use of nullable values requires explicit null checking or safe navigation (`?.`)
- No null pointer exceptions at runtime

## 13.6 Concurrency Model
- IPL supports message-passing concurrency based on the actor model
- Data races are prevented through ownership: only the owner of data can access it
- Channels provide safe communication between concurrent tasks
- The `async` and `await` keywords enable async/await style programming

**Valid**
```ipl
async function fetchData() : string {
    let result = await networkRequest("https://api.example.com");
    return result;
}

// Using async function
async function main() {
    let data = await fetchData();
    print(data);
}
```

## 13.7 Contracts and Verification
- Failure of a `require` or `ensure` **shall** raise a `ContractError` before/after body respectively
- Contracts can be enforced at compile-time using static analysis when possible
- Contracts may be disabled in optimized builds but must be available for debugging

## 13.8 Type System Semantics
- IPL implements nominal typing with structural compatibility for interfaces
- Concept-based constraints enable compile-time verification of generic requirements
- Variance rules apply to generic types: covariant for immutable access, contravariant for mutable access

## 13.9 Error Handling Model
- IPL follows a hybrid approach: exceptions for error conditions, optionals for expected absence of values
- The `Result<T, E>` type provides functional-style error handling
- Destructors (`delete` functions) are guaranteed to run for owned resources, enabling RAII patterns
