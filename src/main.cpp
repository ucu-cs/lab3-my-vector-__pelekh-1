#include <iostream>
#include "my_vector.h"
#include "my_array.h"
#include "my_unique_ptr.h"
#include "my_shared_ptr.h"

int main() {
    my_vector<int> v = {1,2,3};
    v.emplace_back(4);
    std::cout << "my_vector: ";
    for (auto x: v) std::cout << x << ' ';
    std::cout << '\n';

    my_array<std::string,3> a = {"one","two","three"};
    std::cout << "my_array: ";
    for (auto &s: a) std::cout << s << ' ';
    std::cout << '\n';

    my_unique_ptr<int> up(new int(42));
    std::cout << "unique_ptr value = " << *up << '\n';

    my_shared_ptr<int> sp1(new int(7));
    my_shared_ptr<int> sp2 = sp1;
    std::cout << "shared count = " << sp1.use_count() << '\n';
}
