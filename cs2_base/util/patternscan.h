#include <stdint.h>
#include <string.h>

namespace Scanner {
    
    inline uintptr_t scanPattern(uint8_t *base, const size_t size, char* pattern, char* mask) {
        const size_t masksize = strlen(mask);

        for(size_t i = 0; i < size - masksize; i++) {
            for (size_t j = 0; j < masksize; j++) {
                if (mask[j] != '?' && *reinterpret_cast<uint8_t*>(base + i + j) != static_cast<uint8_t>(pattern[j])) break;

                if (j == masksize - 1) return reinterpret_cast<uintptr_t>(base) + i;
            }
        }
        return 0;
    }
}