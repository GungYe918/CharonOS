/**
 * @file memory_manager.hpp
 * 
 * 메모리를 관리하는 memory_manager를 정의한다.
*/

#pragma once 

#include <array>
#include <limits>

#include "../../error/error.hpp"

namespace {
    constexpr unsigned long long operator""_KiB(unsigned long long kib) {
        return kib * 1024;
    }

    constexpr unsigned long long operator""_MiB(unsigned long long mib) {
        return mib * 1024_KiB;
    }

    constexpr unsigned long long operator""_GiB(unsigned long long gib) {
        return gib * 1024_MiB;
    }
}

static const auto kBytesPerFrame(4_KiB);

class FrameID {
    public:
        explicit FrameID(size_t id) : id_{id} {}
        size_t ID() const {  return id_;  }
        void* Frame() const {  return reinterpret_cast<void*>(id_ * kBytesPerFrame);  }

    private:
        size_t id_;
};

static const FrameID kNullFrame(std::numeric_limits<size_t>::max());

/**
 * @brief 비트맵 배열을 사용하여 프레임(물리 주소 상의 구역)단위로 메모리를 관리하는 클래스
 * 
 * 1비트를 1프레임에 대응시켜, 비트맵으로 빈 프레임을 관리한다.
 * 배열 alloc_map의 각 비트가 프레임에 대응하며, 0이면 빈 공간, 1이면 사용중인 공간이다.
 * alloc_map[n]의 m번째 비트가 대응하는 물리 주소는 아래와 같은 식으로 구한다.
 * 
 * EX:
 *  kFrameBytes * (n * kBitsPerMapLine + m) 
 * 
*/

class BitmapMemoryManager {
    public:
        /*  이 메모리 관리 클래스가 처리할 수 있는 최대 물리 메모리 양(Bytes 단위)  */
        static const auto kMaxPhysicalMemoryBytes{128_GiB};

        /*  kMaxPhysicalMemoryBytes까지의 물리 메모리를 처리하기 위해 필요한 프레임 수  */
        static const auto kFrameCount{kMaxPhysicalMemoryBytes / kBytesPerFrame};


        /*  비트맵 배열 요소 형식  */
        using MapLineType = unsigned long;

        /*  비트맵 배열 하나의 요소의 비트 수 == 프레임 수  */
        static const size_t kBitsPerMapLine{8 * sizeof(MapLineType)};

        BitmapMemoryManager();


        /*  요청된 프레임 수의 공간을 할당하고 첫 번째 프레임 ID를 반환한다.  */
        ValueWithError<FrameID> Allocate(size_t num_frames);
        /*  오류 발생 시 에러를 반환  */
        Error Free(FrameID start_frame, size_t num_frames);
        /*  할당된 영역을 표시한다  */
        void MarkAllocated(FrameID start_frame, size_t num_frames);


        void SetMemoryRange(FrameID range_begin, FrameID range_end);
        /**
         * @brief 메모리 메니저에서 다루는 메모리 범위를 설정한다.
         * 
         * 이 호출 이후 할당된 메모리는 설정된 범위 내에서만 수행된다.
         * 
         * @param range_begin_ 메모리 범위의 시작점
         * @param range_end_   메모리 범위의 끝점, 마지막 프레임 다음의 프레임
         * 
        */

        private:
            std::array<MapLineType, kFrameCount / kBitsPerMapLine> alloc_map_;

            /*  메모리 메니저에서 다루는 메모리 범위의 시작점  */
            FrameID range_begin_;
            /*  메모리 메니저에서 다루는 메모리 범위의 끝점(최종 프레임 다음의 프레임)  */
            FrameID range_end_;

            bool GetBit(FrameID frame) const;
            void SetBit(FrameID frame, bool allocated);
};