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
 *  1) 현재 Queue의 용량이 최대 용량과 같다면 => 용량이 가득 찼다면
 *     kFull 오류를 반환하고 함수를 종료한다.
 * 
 *  2) Queue의 쓰기 위치에 새로운 데이터를 저장한다. 
 * 
 *  3) 현재 Queue의 용량을 1 증가시킨다.
 * 
 *  4) 쓰기 위치를 다음으로 이동시킨다.
 * 
 *  5) 쓰기 위치가 용량을 초과하여 버퍼의 끝에 도달하였을 경우 
 *     쓰기 위치를 다시 0으로 초기화한다.
 * 
 *  6) kSuccess를 반환하고 함수를 종료한다.
 * 
 * 
 * PLUS:
 *  5)에서 쓰기 위치를 다시 0으로 초기화 시키는 이유:
 *      Queue 배열의 시작과 끝을 연결하기 위해서이다. 
 *      이러한 형태의 Queue 배열을 원형 버퍼라고 부른다.
 * 
 *      원형 버퍼는 순환 구조를 가지고 있기 때문에 효율성과
 *      메모리 관리 면에서 큰 이점이 있다.
 * 
 *      1. 메모리 효율성: 
 *          원형 버퍼는 고정된 크기의 메모리 블록으로 구현되고,
 *          메모리 재사용이 허용되기 때문에 동적 메모리 할당이
 *          필요하지 않다. 따라서 메모리 관리에 필요한 오버헤드를 
 *          줄일 수 있다.
 * 
 *      2. 삽입 및 삭제 연산의 속도 증가:
 *          원형 버퍼는 인덱스를 순환하며 연산을 수행하기 때문에 
 *          삽입 및 삭제 시간에 상수 시간(O(1))에 수행된다.
 *          이 때문에 효율적인 작업 처리가 가능해진다.
 * 
*/



template <typename T>
Error kQueue<T>::Pop() {
    if (count_ == 0) {                          // 1)
        return MAKE_ERROR(Error::kEmpty);       
    }

    --count_;                                   // 2)
    ++read_pos_;                                // 3)

    if (read_pos_ == capacity_) {               // 4)
        read_pos_ = 0;
    }
    return MAKE_ERROR(Error::kSuccess);         // 5)
}

/**
 * @brief Queue에서 가장 처음 추가한 값을 제거하는 함수이다.
 * 
 * 동작방식:
 *  1)  Queue가 비어있다면 Error::kEmpty를 반환하고 
 *      함수를 종료한다. 
 * 
 *  2)  Queue의 현재 용량을 1 감소시킨다. 
 * 
 *  3)  Queue의 읽기 위치를 1 증가시킨다.
 * 
 *  4)  read_pos를 1 증가시킨 값이 Queue의 최대 용량과 
 *      같다면 읽기 위치를 0으로 초기화한다. (원형 버퍼)
 * 
 *  5)  Error::kSuccess를 반환하고 함수를 종료한다.
 * 
 * 
 *  PLUS:
 *   Pop()은 진짜로 값에대한 제거를 실행하는가:
 *      이 파일에 구현된 Pop()은 실재로 값에 대한 제거를 
 *      수행하지 않는다. 그 이유는 이 Queue가 원형 버퍼로 
 *      구현되었기 때문에 굳이 값을 제거할 필요 없이 읽기 위치를 
 *      증가시키는 것 만으로도 제거한 값에 대한 접근을 방지할 수 
 *      있기 때문이다.
 * 
 *      실재로 Pop이 실행된 위치의 데이터는 원형 버퍼에서 제일 
 *      뒤로 이동한 것처럼 처리되고, 원형 버퍼가 한 번 순환하게 될 때
 *      Push함수에 의해 다른 값으로 대체되게 된다.
 *       
*/  



template <typename T>
size_t kQueue<T>::Count() const {
    return count_;
}
/**
 * @brief 현재 Queue의 용량을 반환한다.
*/


template <typename T>
size_t kQueue<T>::Capacity() const {
    return capacity_;
}
/**
 * @brief Queue의 최대 용량을 반환한다.
*/


template <typename T>
const T& kQueue<T>::Front() const {
    return data_[read_pos_];
}
/**
 * @brief 현재 Queue에서 가장 선두에 있는 데이터를 반환한다.
*/