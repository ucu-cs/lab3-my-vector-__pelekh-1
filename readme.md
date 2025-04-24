# Lab work **3**: **Custom Vector, Array & Smart Pointers**

Authors (team): Anastasiia Pelekh [@drakonchyk](https://github.com/drakonchyk)  

---

## Prerequisites

| Tool / Library | Minimum version | Why it’s needed |
|----------------|-----------------|-----------------|
| **C++ compiler** | GCC ≥ 11 / Clang ≥ 14 / MSVC ≥ 19.31 | C++20 language features |
| **CMake** | 3.20 | Cross-platform build system |
| **Make / Ninja** | — | Backend for CMake (choose one) |
| **GoogleTest** | v1.14 (fetched automatically) | Unit-testing framework |
| (Optional) **Valgrind** / **LLVM ASan/UBSan** | latest | Memory-error & UB detection |
| (Optional) **PVS-Studio** | 7.x | Static analysis |

---

### Compilation

```bash
git clone https://github.com/ucu-cs/lab3-my-vector-__pelekh-1.git
cmake -S . -B build              # configure
cmake --build build -j           # compile library, example & tests
```

> **Tip:** during development enable sanitizers  
> `cmake -S . -B build -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -g"`  

### Installation

No global install is required — the project builds a header-only
library. If you still prefer `make install`:

```bash
cmake --install build --prefix $HOME/.local
```

### Usage

```cpp
#include "my_vector.hpp"
#include "my_array.hpp"
#include "my_smart_ptrs.hpp"

int main() {
    my_vector<int> v = {1, 2, 3};
    v.push_back(4);

    my_array<std::string, 3> a{{"one", "two", "three"}};

    my_unique_ptr<int> up(new int(42));
    my_shared_ptr<int> sp1(new int(7));
    my_shared_ptr<int> sp2 = sp1;

    std::cout << v.back()          // → 4
              << " " << a[0]       // → one
              << " " << *up        // → 42
              << " " << sp1.use_count(); // → 2
}
```

Build & run the example:

```bash
cmake --build build --target example
./build/example
```

Run unit tests:

```bash
ctest --test-dir build/tests --output-on-failure
```

GENERAL BUILD
```bash
mkdir build
cd build
cmake .. && make
./example
./tests/tests
```
---

### Important!

* **Header-only**: include paths are enough — no linking against
  a compiled library.
* Smart-pointer implementation is minimal and **not thread-safe beyond the
  demonstrated use-case** (atomic ref-counts only).
* `my_vector` provides *strong exception-safety* for all mutating ops
  but is ~10 % slower than `std::vector` in push-back micro-benchmarks
  due to manual memory management.

---

### Results

* Implemented from scratch:
  * `my_vector<T>` — dynamic array with iterator support
  * `my_array<T,N>` — fixed-size aggregate array
  * `my_unique_ptr`, `my_shared_ptr`, `my_weak_ptr`
* GoogleTests cover fundamental, complex and nested types.
* Benchmarks show ~1.8× speed-up over naive realloc-every-push
  strategies; still within 5-15 % of libstdc++ `std::vector`.

What I learned:

* Low-level manual allocation (`operator new[]` / placement new).
* Exception-safe move/copy semantics & RAII.
* Intricacies of reference counting and weak ownership.
* Writing allocator-agnostic, STL-compatible iterators.

---

# Additional tasks

* pointers.

---