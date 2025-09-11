# 🚀 CCBasicCore

**CCBasicCore** is the core library of **CCUtilsCommons**, designed to provide **lightweight data structures** and **basic algorithms** in pure C — portable across any platform.  
Think of it as a **minimalist STL for C**.

---

## 📦 Features

### ✅ CCVector
A simple yet efficient **dynamic array (vector)** implemented in C.  
Supports common operations such as **push, pop, get, set, resize**, and **clear**.

---

## ⚡ Benchmarks

All benchmarks were run on a modern desktop CPU with `1,000,000` elements.

```shell
==== Basic Function Test ====
Size after 10 push: 10
0 0 0 0 0 0 133281 0 0 0 
Element[5] after set = 999
Size after 5 pop: 5
==== Basic Test Done ====

==== Stress Test ====
Push 1000000 elements: 0.014 sec
Pop  1000000 elements: 0.008 sec
Final size: 0
==== Stress Test Done ====

==== Performance Compare (Vector vs Raw Array) ====
Vector push 1000000 elements: 0.014 sec
Raw array write 1000000 elements: 0.011 sec
==== Performance Compare Done ====
````

👉 The performance of `CCVector` is **very close to raw arrays**, while providing **safety & convenience**.

---

## 🔮 Roadmap

* [ ] CCList (linked list)
* [ ] CCHashMap (hash table)
* [ ] CCString (string utility)
* [ ] More algorithms

---

## 💡 Philosophy

* **Minimal**: Keep the API surface simple.
* **Portable**: Works on embedded, desktop, and server.
* **Performant**: Almost no overhead compared to raw arrays.

---

## 🛠 Usage

```c
#include "ccvector.h"

CCVector vec;
ccvector_init(&vec);

for (int i = 0; i < 10; i++) {
    ccvector_push(&vec, i);
}

printf("Vector size: %zu\n", ccvector_size(&vec));
ccvector_free(&vec);
```

---

## 📜 License

MIT License. Free to use, modify, and share.

