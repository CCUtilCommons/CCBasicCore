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

---

## ⚡ Benchmarks

All Tests for Data Structures lies on the test/ folder.
you can run the build and invoke the executation of tests
to see the performance in your machines

---

## 🔮 Roadmap
* [√] CCVector (C Dynamic Array)
* [√] CCList (linked list)
* [ ] CCHashMap (hash table)
* [ ] CCString (string utility)
* [ ] More algorithms

---

## 💡 Philosophy

* **Minimal**: Keep the API surface simple.
* **Portable**: Works on embedded, desktop, and server.
* **Performant**: Almost no overhead compared to raw arrays.

---

## 📜 License

MIT License. Free to use, modify, and share.

