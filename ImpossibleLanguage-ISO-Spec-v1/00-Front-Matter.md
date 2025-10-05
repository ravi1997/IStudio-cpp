# 00 — Front Matter (Normative)

**Standard name:** Impossible Programming Language (IPL)  
**Short name:** IPL  
**Version:** 1.0-draft  
**Status:** Draft for review  
**Editors:** IPL Working Group  
**License:** Permissive, for implementation and teaching.


The keywords **shall**, **must**, **should**, **may**, and **can** in this document are to be interpreted as described in ISO/IEC Directives, Part 2. 
- **shall / must**: indicates a requirement strictly to be followed
- **should**: indicates a recommendation
- **may / can**: indicates permission or possibility


## 0.1 Notational Conventions

- Grammar productions use **ISO/IEC 14977 EBNF**.
- Code examples use the `.ipl` language fence for clarity.
- *Informative* notes are rendered as **Note:** paragraphs.
- Examples are labeled **Valid** or **Invalid** and include a rationale.

## 0.2 Language Feature Categories

This specification incorporates concepts from multiple programming paradigms:
- **C++-style OOP**: Classes, inheritance, operator overloading, templates, concepts, contracts
- **Python-style features**: Meta-classes, decorators, dynamic features
- **Java-style features**: Inheritance hierarchy, exception handling, interfaces
- **Dart-style features**: Enhanced nullability (null safety), const/final semantics, optional parameters
- **Rust-style features**: Memory safety, ownership, lifetimes
- **MATLAB-style features**: Matrix/array operations, vectorization
