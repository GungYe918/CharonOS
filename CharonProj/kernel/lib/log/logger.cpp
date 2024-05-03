// include - local
#include "logger.hpp"
#include "../terminal/terminal.hpp"


// include - system
#include <cstddef>
#include <cstdio>


namespace {
    LogLevel log_level = kWarn;  			// 현재의 로그 레벨을 kWarn으로 설정
}

extern Terminal* terminal; 				// 터미널 변수를 설정

void SetLogLevel(LogLevel level) { 			// 로그 레벨을 설정하는 함수, 현재의 로그 레벨을 level인자의 값으로 설정
    log_level = level;
}

int Log(LogLevel level, const char* format, ...) { 	// 형식화된 로그 메세지를 받아 이를 터미널에 출력하는 함수이다. 
    if (level > log_level) {
        return 0;
    }

    //가변 인자 목록(va_list)타입 변수
    va_list ap;
    int result;
    char str[2048];


    /**
     * @brief vsprintf를 이용한 문자열 저징 맞 출력
     * 
     * 동작방식[I1] : 
     * 동작방식[I2] : 
     * 동작방식[I3] : 
     * 동작방식[IV] : 
     * 동작방식[V1] : 
    */


    va_start(ap, format);
    result = vsprintf(str, format, ap);
    va_end(ap);

    terminal->printString(str); 			// 터미널의 printString() 함수를 사용해서 터미널에 결과를 출력한다.
    return result; 					        // 문자열의 길이를 반환 
}
