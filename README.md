# 🚀 CCBasicCore

**CCBasicCore** is the core library of **CCUtilsCommons**, designed to provide **lightweight data structures** and **basic algorithms** in pure C — portable across any platform.  
Think of it as a **minimalist STL for C**.

---

## 📦 Features

### ✅ CCVector

A simple yet efficient dynamic array (vector) implemented in C.
Supports common operations such as push, pop, get, set, resize, and clear.

Features:

- Dynamically resizable array for efficient memory usage.
- Type-agnostic, works with any data type.
- Easy element access with get and set.
- Optional deep-copy / custom free support through CCSpecialDefinedPack.

### ✅ CCList
A robust circular doubly-linked list implemented in C.
Supports common operations such as push front/back, pop front/back, insert, erase, iterate, and size/empty checks.

Features:
- Circular doubly-linked structure ensures efficient traversal in both directions.
- Custom element handling via CCSpecialDefinedPack for deep copy, free, and compare functions.
- Safe edge handling for empty and single-node lists.
- High-performance under stress testing with millions of elements.
- Flexible iteration through CCBasicCore_CCListRunIterate.

### ✅ CCStack (Abstract Stack Interface)

An object-oriented-style abstract stack interface implemented in C (a “base class” + vtable). Lets you switch implementations at runtime between CCVectorStack and CCListStack while exposing a single, stable API (push/pop/top/empty/size/destroy).

Features

- Abstraction & polymorphism: Vtable-based design hides implementation details behind a single CCStack type — callers use the same functions regardless of backing implementation.

- Fixed element size per instance: Each stack is created with a fixed elem_size; a stack instance holds exactly one element type.

- Pluggable element handling: Optional callbacks (like a CCSpecialDefinedPack) support deep-copy, free, and compare semantics for complex element types.

Performance semantics:

- CCVectorStack — contiguous storage, amortized O(1) push, O(1) pop/top, good cache locality.

- CCListStack — doubly-linked list, O(1) push/pop/top, avoids reallocation overhead.

- Robust edge handling: Well-defined behavior for empty and single-element stacks (typically pop/top → NULL).

- Testability & stress-ready: Designed for easy unit tests and stress tests (millions of ops) to validate correctness and memory behavior.

- Thread-safety optional: Implementations are not thread-safe by default; add external locking or instrument the implementations if concurrent access is required.

### ✅ CCQueue

A flexible and efficient queue abstraction implemented in C.
Supports multiple backends (CCVectorQueue, CCListQueue) with a unified interface.

Features:

- Unified interface: Switch seamlessly between vector-based and list-based implementations.

- Core operations: push (enqueue), pop (dequeue), front, size, and empty checks.

- Safe edge handling: Returns NULL when popping from an empty queue.

- High-performance under stress: Validated with hundreds of thousands of elements.

- Extensible design: New queue implementations can be plugged in without changing client code.

### ✅ CCDeque

A flexible double-ended queue (deque) implementation in C.
Supports efficient insertion and removal at both ends, with multiple backends (vector-based and list-based).

✨ Features

- Unified interface: Operations are accessed via CCDeque_Interface_t, allowing different internal implementations.

- Double-ended operations:
push_front, pop_front
push_back, pop_back
Front/Back accessors: Retrieve elements at either end in O(1).

- Multiple backends:

Vector-based deque (CCVectorDeque) — contiguous memory, fast random access.

List-based deque (CCListDeque) — doubly-linked structure, stable pointers.

- Custom element handling via CCSpecialDefinedPack:

- Deep copy (copier)

- Resource cleanup (freer)

- Comparison (compare)

- Safe edge handling: Robust behavior for empty or single-element deques.

- Cross-platform C API: Designed for embedded and desktop C projects.

### ✅ CCMap
A balanced binary search tree (BST / Treap) based associative container in C.
Supports efficient key–value storage with ordered iteration.

✨ Features

- Unified map interface: Provides insertion, lookup, update, and erase operations via CCMap API.

- Ordered storage: Keys are stored in sorted order, enabling range queries and ordered traversal.

Two backends (controlled by USE_TREAP_ACC macro):

- BST-based map — simple binary search tree (may degrade to O(n)).

- Treap-based map — randomized balanced BST using heap priority, ensures near O(log n).

Key–value management via CCSpecialDefinedPack:

- Deep copy (copier)

- Resource cleanup (freer)

- Key comparison (compare)

Deletion support:

- BST: classic 3-case erase (leaf, single child, two children).

- Treap: priority-based rotations to maintain balance.

Cross-platform C API:
Designed for embedded systems and desktop C projects, minimal dependencies.

### ✅ CCUnorderedMap
A hash table–based associative container in C.
Supports efficient average O(1) key–value access without ordering guarantees.

✨ Features

- Hash table backend: Uses chaining (linked lists) for collision handling.

- Key–value operations: Insert, find, update, and erase in expected O(1).

- Unordered storage: No guarantee of key ordering; optimized for fast lookups.

- Configurable hash and equality:
    - Custom hash function pointer.
    - Custom equality comparator.

Flexible memory management via CCSpecialDefinedPack:

- Deep copy (copier)

- Resource cleanup (freer)

- Key comparison (compare)

- Robust resizing: Table automatically expands to maintain load factor for performance.

- Cross-platform C API: Lightweight and portable, suitable for embedded and desktop projects.

### ✅ CCString
A lightweight and efficient C string library with optional small string optimization (SSO).

✨ Features

- Small String Optimization (SSO): For strings shorter than 15 characters, store directly in the stack to avoid heap allocation overhead.

- Dynamic Memory Management: For longer strings, automatically allocate memory on the heap, supporting pre-allocation and shrinking.

- Comprehensive API:

    - Create and free strings.

    - Append, insert, erase, and get substrings.

    - Compare strings and find substrings.

    - Iterate over characters with visit and process functions.

    - Clear and check string size and emptiness.

- Custom Iterators:

    - VisitStringIterator: Read-only character access.

    - AssignedStringIterator: Read-write character access.

- Edge Case Handling:

    - Supports empty strings, null characters, and large strings.

    - Handles strings with embedded null characters gracefully.

- Cross-Platform C API: Lightweight and portable, suitable for embedded and desktop projects.


### ✅ CCSet
A lightweight and efficient ordered set implemented in C using a binary search tree.

✨ Features

- Ordered Storage: Keys are stored in sorted order based on user-provided comparator.

- Unique Keys: Each key is stored at most once.

- Comprehensive API:

- Create and destroy sets.

- Insert, erase, and query keys.

- Get current size.

Flexible Key Operations:

- User-provided copier, freer, and comparator (CCSpecialDefinedPack).

- Edge Case Handling:

    - Duplicate insertions are ignored.

    - Safe calls with NULL set.

    - Supports extreme integer ranges and custom structs.

- Cross-Platform C API: Portable for embedded and desktop projects.

### ✅ CCUnorderedSet
A high-performance hash set implemented in C with separate chaining.

✨ Features

- Hash-Based Lookup: Average O(1) insert, erase, and query operations.

- Configurable Load Factor: Supports automatic rehashing to maintain performance.

- Comprehensive API:

    - Create and destroy hash sets.

    - Insert, erase, and query keys.

    - Get current size.

- Flexible Key Operations:

    - User-provided copier, freer, and comparator (CCSpecialDefinedPack).

    - Custom hash function supplied by user.

- Edge Case Handling:

    - Duplicate insertions are ignored.

    - Safe calls with NULL set.

- Handles large key ranges and high-volume data gracefully.

- Cross-Platform C API: Lightweight and portable, suitable for embedded and desktop use.



---

## ⚡ Benchmarks

All Tests for Data Structures lies on the test/ folder.
you can run the build and invoke the executation of tests
to see the performance in your machines

---

## 🔮 Roadmap
* [√] CCVector — A simple yet efficient dynamic array (vector) implemented in C. Supports common operations such as push, pop, get, set, resize, and clear.

* [√] CCList — A robust circular doubly-linked list implemented in C. Supports common operations such as push front/back, pop front/back, insert, erase, iterate, and size/empty checks.

* [√] CCStack — An object-oriented-style abstract stack interface implemented in C. Lets you switch implementations at runtime between CCVectorStack and CCListStack while exposing a single, stable API (push/pop/top/empty/size/destroy).

* [√] CCQueue — A flexible and efficient queue abstraction implemented in C. Supports multiple backends (CCVectorQueue, CCListQueue) with a unified interface.

* [√] CCDeque — A flexible double-ended queue (deque) implementation in C. Supports efficient insertion and removal at both ends, with multiple backends (vector-based and list-based).

* [√] CCMap — A balanced binary search tree (BST / Treap) based associative container in C. Supports efficient key–value storage with ordered iteration.

* [√] CCUnorderedMap — A hash table–based associative container in C. Supports efficient average O(1) key–value access without ordering guarantees.

* [√] CCString — A lightweight and efficient C string library with optional small string optimization (SSO).

* [√] CCUnorderedMap — A hash table–based associative container in C.
Supports efficient average O(1) key–value access, dynamic resizing, and customizable hash/equality functions.

* [√] CCUnorderedSet — A hash table–based set container in C.
Provides fast average O(1) element insertion, lookup, and deletion, ensuring uniqueness without ordering guarantees.
---

## 💡 Philosophy

* **Minimal**: Keep the API surface simple.
* **Portable**: Works on embedded, desktop, and server.
* **Performant**: Almost no overhead compared to raw arrays.

---

## 📜 License

MIT License. Free to use, modify, and share.

