#include <cstdint>
#include <iostream>

void compute_f_mmx(const int8_t* a, const int8_t* b, const int8_t* c,
                   const int16_t* d, int16_t* f) {
    asm volatile(
        "pxor %%mm7, %%mm7\n\t"

        "movq (%[a]), %%mm0\n\t"
        "movq %%mm0, %%mm4\n\t"
        "punpcklbw %%mm7, %%mm0\n\t"
        "punpckhbw %%mm7, %%mm4\n\t"
        "psllw $8, %%mm0\n\t"
        "psraw $8, %%mm0\n\t"
        "psllw $8, %%mm4\n\t"
        "psraw $8, %%mm4\n\t"

        "movq (%[b]), %%mm1\n\t"
        "movq %%mm1, %%mm5\n\t"
        "punpcklbw %%mm7, %%mm1\n\t"
        "punpckhbw %%mm7, %%mm5\n\t"
        "psllw $8, %%mm1\n\t"
        "psraw $8, %%mm1\n\t"
        "psllw $8, %%mm5\n\t"
        "psraw $8, %%mm5\n\t"

        "movq (%[c]), %%mm2\n\t"
        "movq %%mm2, %%mm6\n\t"
        "punpcklbw %%mm7, %%mm2\n\t"
        "punpckhbw %%mm7, %%mm6\n\t"
        "psllw $8, %%mm2\n\t"
        "psraw $8, %%mm2\n\t"
        "psllw $8, %%mm6\n\t"
        "psraw $8, %%mm6\n\t"

        "pmullw %%mm2, %%mm1\n\t"
        "pmullw %%mm6, %%mm5\n\t"

        "paddw %%mm0, %%mm1\n\t"
        "paddw %%mm4, %%mm5\n\t"

        "movq (%[d]), %%mm3\n\t"

        "psubw %%mm3, %%mm1\n\t"
        "movq 8(%[d]), %%mm3\n\t"
        "psubw %%mm3, %%mm5\n\t"

        "movq %%mm1, (%[f])\n\t"
        "movq %%mm5, 8(%[f])\n\t"

        "emms\n\t"
        :
        : [a] "r"(a), [b] "r"(b), [c] "r"(c), [d] "r"(d), [f] "r"(f)
        : "memory", "mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7");
}

void compute_f(const int8_t* a, const int8_t* b, const int8_t* c,
               const int16_t* d, int16_t* f) {
    for (auto i = 0; i < 8; ++i) {
        f[i] = a[i] + (b[i] * c[i]) - d[i];
    }
}

template <typename T>
void print_array(const T* arr, const int len, std::string name = "\0") {
    if (name != "\0") {
        std::cout << name << ": ";
    }

    for (auto i = 0; i < len; ++i) {
        std::cout << arr[i] << " ";
    }

    std::cout << "\n";
}

void print_array(const int8_t* arr, const int len, std::string name = "\0") {
    if (name != "\0") {
        std::cout << name << ": ";
    }

    for (auto i = 0; i < len; ++i) {
        std::cout << (int)arr[i] << " ";
    }

    std::cout << "\n";
}

int main() {
    int8_t a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int8_t b[8] = {-8, -7, -5, -4, 4, 3, 2, 1};
    int8_t c[8] = {1, 1, 2, 2, 3, 3, 4, 4};
    int16_t d[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int16_t f[8] = {};
    int16_t f_test[8] = {};

    print_array(a, 8, "a");
    print_array(b, 8, "b");
    print_array(c, 8, "c");
    print_array(d, 8, "d");

    compute_f_mmx(a, b, c, d, f);
    compute_f(a, b, c, d, f_test);

    print_array(f, 8, "f");
    print_array(f_test, 8, "f_test");

    return 0;
}
