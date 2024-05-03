/**
 * @file log.hpp
*/

#pragma once

enum LogLevel {
    kError  = 3,
    kWarn   = 4,
    kInfo   = 6,
    kDebug  = 7,
};

/**
 * @brief SetLogLevel함수 : 전역적인 로그의 우선 순위 임계값을 설정
 * 
 * 매개변수 : LogLevel level -> 설정하려는 로그의 임계값
 * 
 * 동작방식 : 이후의 'Log'호출에서 이 임계값보다 낮은 우선 순위의 로그는 기록 x
*/



void SetLogLevel(LogLevel level);


/**
 * @brief Log함수 : 주어진 [로그 우선 순위]로 로그를 기록
 * 
 * 매개변수[I1]  : LogLevel levl -> 로그 우선 순위
 * 매개변수[I2] : const char* format -> 로그 메세지의 형식 문자열 (printf 형식을 따름)
 * 매개변수[I3] : '...' -> 가변 인자 목록
 * 
 * 반환할값 : 로그 메세지가 성공적으로 기록될 경우 기록된 문자 수를 반환
 * 
 * 동작방식[I1] : 설정된 로그 임계값보다 높은 우선 순위의 로그만 기록
 * 동작방식[I2] : format과 가변 인자들을 사용하여 로그 메세지를 형성 & 기록
 * 동작방식[I3] : 로그는 설정된 임계값 이상의 우선 순위를 가진 경우만 기록, 나머지는 무시
 * 동작방식[IV] : 반환 값은 로그가 기록된 문자의 수
*/
int Log(LogLevel level, const char* format, ...);