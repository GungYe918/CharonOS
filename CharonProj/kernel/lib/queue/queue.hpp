#pragma once

#include <cstddef>
#include <array>

#include "../error/error.hpp"

// kQueue for interrupt management
template <typename T>
class kQueue {
    public:
        template <size_t N>

        kQueue(std::array<T, N>& buf);              // 배열을 사용하여 queue를 초기화하는 생성자, T는 배열의 요소 타입, N은 배열의 크기
        kQueue(T* buf, size_t size);                // 포인터를 사용하여 Queue를 초기화하는 생성자, 초기화 시 버퍼의 시작 주소 buf와 버퍼의 크기인 size 할당

        Error Push(const T& value);                 // Queue에 데이터를 추가
        Error Pop();                                // Queue에서 가장 처음에 넣은 값을 제거
        size_t Count() const;                       // Queue에 저장된 요소의 개수를 반환
        size_t Capacity() const;                    // Queue의 최대 용량을 반환
        const T& Front() const;                     // Queue에서 선두에 위치한 데이터의 값을 반환

    private:        
        T* data_;                                   // Queue의 데이터를 저장하는 배열을 지시하는 포인터
        size_t read_pos_, write_pos_, count_;
        /**
         * read_pos_ : Queue의 읽기 위치를 의미
         *  
         * write_pos_ : Queue의 쓰기 위치를 의미                         
         * 
         * count_ : Queue에 저장된 요소의 개수를 의미
         * 
        */
       
       const size_t capacity_;                      // Queue의 최대 용량을 의미
};
/**
 * @brief 인터럽트 관리에 사용되는 queue를 정의하는 kQueue 클래스
 * 
 * PLUS:
 *  인터럽트 관리에 Queue를 사용하는 이유:
 *      사용자의 어떠한 동작에 의하여 인터럽트가 발생하면 
 *      일단 이 인터럽트를 처리하기 전까지는 다른 인터럽트를 처리할 수 없다
 *      따라서 단기간에 많은 양의 인터럽트가 발생하게 되면 상대적으로 덜 중요한
 *      인터럽트의 처리를 포기할 수 밖에 없다. 이러한 문제를 해결하기 위해 
 *      인터럽트의 처리는 최대한 고속화되어야 한다.
 * 
 *      그리고 인터럽트 처리를 고속화 하기 위해 사용되는 방법이 Queue를 이용하는 것이다.
 *      Queue를 사용하면 인터럽트가 발생한 것을 기록만 하고 나중에 처리하는 것이 가능해지며, 
 *      인터럽트의 처리를 별도의 스레드나 태스크로 이동시킬 수 있어 작업의 과부하를 방지할 수 있다.
 * 
*/

template <typename T>
template <size_t N>
kQueue<T>::kQueue(std::array<T, N>& buf) : kQueue(buf.data(), N) {}
/**
 * @brief kQueue를 초기화하는 생성자
 * 
 * 이 생성자는 파라미터 2개를 받는 아래의 생성자에 처리를 이양한다. 
 * 이 생성자는 유일한 파라미터인 buf에서 배열의 시작 포인터와 요소 수를 받아 
 * 파라미터가 2개인 생성자에 전달한다.
*/

template <typename T>
kQueue<T>::kQueue(T* buf, size_t size)
    : data_{buf}, read_pos_{0}, write_pos_{0}, count_{0}, capacity_{size} {}
/**
 * @brief kQueue를 초기화하는 생성자
 * 
 * buf에는 Queue의 데이터를 저장하는 위치가 되는 배열을 지정, 
 * size에는 그 배열의 요소 수를 지정한다.
 * 
 * 이 생성자는 입력받은 2개의 파라미터를 사용해서 data_와 capacity를 초기화시킨다.
 * 다른 맴버 변수는 0으로 초기화한다.
*/



template <typename T>
Error kQueue<T>::Push(const T& value) {
    if (count_ == capacity_) {                  // 1)
        return MAKE_ERROR(Error::kFull);
    }

    data_[write_pos_] = value;                  // 2)
    ++count_;                                   // 3)
    ++write_pos_;                               // 4)

    if (write_pos_ == capacity_) {              // 5)
        write_pos_ = 0;
    }
    return MAKE_ERROR(Error::kSuccess);         // 6)
}

/**
 * @brief Queue에 데이터를 추가하는 함수
 * 
 * 동작방식:
 *  1) 현재 Queue의 용량이 최대 용량과 같다면 
 *     kFull 오류를 반환하고 함수를 종료한다.
 * 
 *  2) 
 * 
 *  3) 현재 Queue의 용량을 1 증가시킨다.
 * 
 *  4) 
 * 
 *  5) 
 * 
 *  6) kSuccess를 반환하고 함수를 종료한다.
 * 
*/



template <typename T>
Error kQueue<T>::Pop() {
    if (count_ == 0) {                          // 1)
        return MAKE_ERROR(Error::kEmpty);       // 2)
    }

    --count_;                                   // 3)
    ++read_pos_;                                // 4)

    if (read_pos_ == capacity_) {               // 5)
        read_pos_ = 0;
    }
    return MAKE_ERROR(Error::kSuccess);         // 6)
}

/**
 * @brief Queue에서 가장 처음 추가한 값을 제거하는 함수이다.
 * 
 * 동작방식:
 *  1)  
*/



template <typename T>
size_t kQueue<T>::Count() const {
    return count_;
}

template <typename T>
size_t kQueue<T>::Capacity() const {
    return capacity_;
}

template <typename T>
const T& kQueue<T>::Front() const {
    return data_[read_pos_];
}
