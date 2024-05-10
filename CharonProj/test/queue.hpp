#include "kernel/lib/error/error.hpp"

template <typename T>
class ArrayQueue {
    public:
        template <size_t N>
        ArrayQueue(std::array<T, N>&buf);
        ArrayQueue(T* buf, size_t size);

        Error Push(const T& value);
        Error Pop();

        size_t Count() const;
        const T& Front() const;

    private:
        T* data;

        size_t read_pos_, write_pos_, count_;
        /**
         * read_pos_ :
         * 
         * write_pos_ :
         * 
         * count_ : 
         * 
        */

        const size_t capacity_;
        
};