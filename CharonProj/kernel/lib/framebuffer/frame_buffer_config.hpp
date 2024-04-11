 #pragma once

#include <stdint.h>

enum PixelFormat { 		// graphic.hpp에서 사용되는 픽셀 형식을 정의하는 열거형이다.
  kPixelRGBResv8BitPerColor, 	 
  kPixelBGRResv8BitPerColor, 	
};

/*
 * kPixelRGBResv8BitPerColor: 
 * 	RGB 순서로 8비트 색상을 정의한다.
 * 
 * kPixelBGRResv8BitPerColor:
 * 	BGR(Reverse) 형태로 8비트 색상을 정의한다.
 *
 * */


struct FrameBufferConfig {      // 1)프레임 버퍼 설정을 저정하는 열거형이다.
  uint8_t* frame_buffer;
  uint32_t pixels_per_scan_line;
  uint32_t horizontal_resolution;
  uint32_t vertical_resolution;
  enum PixelFormat pixel_format;
};

/*
 * frame_buffer:
 * 	프레임 버퍼를 지시하는 포인터이다.
 * 
 * pixels_per_scan_line:
 * 	스캔 라인 당 픽셀 수를 의미한다.
 *
 * horizontal_resolution:
 * 	수평 해상도를 의미한다.
 *
 * vertical_resolution:
 * 	수직 해상도를 의미한다.
 *
 * pixel_format:
 * 	픽셀 형식을 나타내는 열거형 변수이다.
 *
 * ============================================
 *
 * 1) 프레임 버퍼
 * 	그래픽 디스플레이 장치에서 사용자에게 이미지나 문자 등을 보여주기 위해 사용되는 메모리 영역을 의미한다.
 * 	이 메모리 영역에는 디스플레이에 표시될 이미지의 픽셀 영역이 정의된다.
 *
 * PLUS:
 * 	BGR형식으로도 픽셀을 정의하는 이유:
 * 		일반적으로 그래픽 라이브러리나 시스템은 RGB형식으로 픽셀을 정의하나, 
 * 		일부 라이브러리나 하드웨어, 시스템 등은 BGR형식으로도 픽셀을 정의한다.
 * 		따라서 두 가지 형식을 모두 고려해야 한다.
 * */
