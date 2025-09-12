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

