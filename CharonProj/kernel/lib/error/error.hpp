#pragma once

#include <array>
#include <cstdio>


/* 이 클래스는 커널 작동 시 발생하는 상황들에 대한 오류 코드를 정의하고 
*  문자열로 매핑하기 위한 오류 클래스이다.
*/
class Error { 								
    public:
        enum Code {
            kSuccess, 					// 목표한 동작이 성공적으로 수행되었을 경우 반환
            kFull, 					// 버퍼 등의 자원이 가득 찼을 경우 반환
            kEmpty, 					// 버퍼 등의 자원이 비어있을 경우 반환
            kNoEnoughMemory, 				// 메모리 부족으로 동작을 수행할 수 없을 경우 반환
            kIndexOutOfRange, 				// 유효하지 않은 인덱스 범위를 참조했을 경우 반환
            kHostControllerNotHalted, 			// 호스트 컨트롤러가 정지되지 않았을 경우 반환
            kInvalidSlotID, 				// 유효하지 않은 슬롯 ID를 참조할 경우 반환
            kPortNotConnected, 				// 포트에 장치가 연결되지 않았을 경우 반환
            kInvalidEndpointNumber, 			// 유효하지 않은 엔드포인트 번호를 참조했을 경우 반환
            kTransferRingNotSet, 			// 전송 링이 설정되지 않았을 경우 반환
            kAlreadyAllocated, 				// 이미 특정 값이 할당된 상태에서 재할당을 시도할 시 반환
            kNotImplemented, 				// 아직 구현되지 않음에 대한 오류
            kInvalidDescriptor, 			// 유효하지 않은 디스크럽터를 참조했을 경우 반환
            kBufferTooSmall, 				// 버퍼가 너무 작을 경우 반환
            kUnknownDevice, 				// 미확인 디바이스가 연결되었을 경우 반환
            kNoCorrespondingSetupStage, 		// 대응하는 설정 단계가 없을 경우 반환
            kTransferFailed, 				// 전송이 실패할 경우 반환
            kInvalidPhase, 				// 유효하지 않은 단계
            kUnknownXHCISpeedID, 			// 알 수 없는 XHCI속도 ID
            kNoWaiter, 					// 대기열이 없을 경우 반환
            kLastOfCode, 				// 코드 목록의 마지막을 의미
        };

    private:
        static constexpr std::array code_name_{ 	// code_name_이라는 이름의 array선언 후 오류 코드들의 문자열을 초기
            "kSuccess", 			
            "kFull", 
            "kEmpty",
            "kNoEnoughMemory",
            "kIndexOutOfRange",
            "kHostControllerNotHalted",
            "kInvalidSlotID",
            "kPortNotConnected",
            "kInvalidEndpointNumber",
            "kTransferRingNotSet",
            "kAlreadyAllocated",
            "kNotImplemented",
            "kInvalidDescriptor",
            "kBufferTooSmall",
            "kUnknownDevice",
            "kNoCorrespondingSetupStage",
            "kTransferFailed",
            "kInvalidPhase",
            "kUnknownXHCISpeedID",
            "kNoWaiter",
        };

        static_assert(Error::Code::kLastOfCode == code_name_.size()); 		// Error::Code::kLastOfCode의 값이 code_name_의 크기와 같은지 확인 후 
								      		// 불일치 시 컴파일 오류 생성


    public: 
        Error(Code code, const char* file, int line) : code_{code}, line_{line}, file_{file} {} 	// 1) 	

        Code Cause() const { 										// 2)
            return this->code_; 							
        }

        operator bool() const { 									// 3)
            return this->code_ != kSuccess; 								
        }

        const char* Name() const { 									// 4)
            return code_name_[static_cast<int>(this->code_)]; 						// 5)
        }

        const char* File() const { 									// 6)
            return this->file_;
        }

        int Line() const { 										// 7)
            return this->line_;
        }
    
    private: 												// 8)
        Code code_; 
        int line_;
        const char* file_;
};

/*
 * 이 부분은 Error 클래스의 생성자와 맴버 함수를 정의한다.
 *
 * 동작방식: 
 * 	1) 이 코드는 Error 클래스의 생성자를 정의하는 코드로, 오류 코드('code'), 발생한 라인 번호('line'), 
 * 	   파일 이름('file')을 받아서 Error 객체를 생성한다.
 *
 * 	2) 이 함수는 오류가 발생한 객체의 오류 코드를 반환한다.
 *
 * 	3) 이 함수는 오류 객체가 성공적인 상태인지의 여부를 반환한다.
 *
 * 	4) 이 함수는 오류 코드의 이름을 반환한다.
 *
 * 	5) static_cast로 this->code_변수를 정수형으로 변환하고, 이 값은 code_name_의 인덱스 번호가 되어 
 * 	   code_name_배열에서 특정 인덱스 값에 해당하는 오류 코드의 이름을 가져온다. 
 * 	   이때 가져온 이름은 const char* 형태로 반환된다. const char* 형태로 반환하는 이유는 
 * 	   추가적인 메모리 할당을 피하고, 다양한 상황에 대응하여 문자열을 처리할 수 있기 때문이다.
 * 	
 * 	6) 이 함수는 오류가 발생한 파일의 이름을 반환한다.
 *
 * 	7) 이 함수는 오류가 발생한 라인 번호를 반환한다.
 *
 * 	8) 이 부분은 Error 클래스의 내부 상태를 저장하기 위해 선언되었다. 
 *
 * */


#define MAKE_ERROR(code) Error((code), __FILE__, __LINE__)

/*
 * 이 매크로는 오류 코드를 받고 Error 클래스의 객체를 생성하는 역할을 한다.
 *
 * 동작방식:
 * 	1. MAKE_ERROR(code) : 발생한 오류 코드를 인자로 받는다.
 * 	2. 매크로가 호출된 파일 이름(__FILE__), 라인 수(__LINE__)를 받아 Error 객체를 생성한다.
 * 	   * __FILE__은 오류가 발생한 파일의 이름을, __LINE__은 오류가 발생한 라인의 번호를 의미한다.
 * 	3. 오류가 발생한 파일 이름, 라인 번호를 포함한 Error 객체가 생성된다. 
 * 
 * */



//return value and error code
template <class T>
struct ValueWithError {
    T value; 			// 구조체 내부에 저장될 데이터의 타입을 나타낸다.
    Error error; 		// Error 클래스를 저장하는 error 변수이다.
};

/**
 * 사용예시:
 * 	if(some_condition) {
 * 		result = ValueWithError(Error::Code::kIndexOutOfRange);
 *
 * 	} else {
 * 		result = some_value;
 * 	}
 *
 */
