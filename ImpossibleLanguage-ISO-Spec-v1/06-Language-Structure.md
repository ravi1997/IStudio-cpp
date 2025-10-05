# 06 — Language Structure (Normative)

## 6.1 Translation Units and Modules
A **module** is a translation unit that may export declarations for import by other modules.

```
module-decl = "module" identifier [ "." identifier ] ";" ;
import-decl = "import" identifier { "." identifier } [ "as" identifier ] ";" ;
export-decl = "export" declaration ;
```

**Valid**
```ipl
module math.linear;
import core.io;
import core.collections as coll;  // alias for imports
export function dot(list<number> a, list<number> b) { return sum(a * b); }
```
**Invalid**
```ipl
module 3d.graphics;    // identifier cannot start with digit
import ;               // missing module path
```

## 6.2 Declarations
Declarations introduce names. A declaration may be exported if it appears with `export` at the top level of a module.

```
declaration   = var-decl | const-decl | final-decl | func-decl | class-decl | interface-decl | trait-decl | meta-class-decl | operator-decl | template-decl | concept-decl ;
var-decl      = "let" [ type ] identifier "=" expression ";" ;
const-decl    = "const" [ type ] identifier "=" constexpr-expression ";" ;
final-decl    = "final" [ type ] identifier "=" expression ";" ;
```

**Valid**
```ipl
let number count = 0;
const number PI = 3.14159;      // compile-time constant
final string name = readName(); // runtime constant
```
**Invalid (requires diagnostic)**
```ipl
let 5 = x;       // literal on LHS
const x = read(); // not constexpr for const, use final instead
```

## 6.3 Interfaces and Traits
IPL supports multiple inheritance through interfaces and traits.

```
interface-decl = [ "export" ] "interface" identifier interface-body ;
interface-body = "{" { interface-method } "}" ;
interface-method = [ "static" ] [ type ] identifier "(" [ param-list ] ")" ";" ;

trait-decl = [ "export" ] "trait" identifier trait-body ;
trait-body = "{" { trait-method } "}" ;
trait-method = [ "static" ] [ type ] identifier "(" [ param-list ] ")" [ block ] ";" ;
```

**Valid**
```ipl
interface Drawable {
  void draw();
  number area();
}

trait Serializable {
  string serialize();
  Self deserialize(string data);
}
```

## 6.4 Meta-Classes
Meta-classes enable reflection and meta-programming.

```
meta-class-decl = [ "export" ] "meta" "class" identifier ":" "type" class-body ;
```

**Valid**
```ipl
meta class TypeMeta : type {
  string name();
  list<string> methods();
  list<string> fields();
}
```

## 6.5 Visibility and Access Control
Classes and members **may** be declared `public`, `private`, or `protected`. Top‑level module exports control inter‑module visibility. Additionally, IPL supports `internal` visibility for module-level access.

## 6.6 Name Lookup and Shadowing
- Unqualified lookup searches the innermost scope outward.
- Redeclaration in the same scope **shall** diagnose unless identical and intended as a forward declaration.
- Interface and trait methods participate in overload resolution.
