/**
 * @file x86_descriptor.hpp
 * 
 * x86_descriptor를 정의한다.
*/

#pragma once 

enum class DescriptorType {

    /*  system segment & descriptor type  */
    kUpper8Bytes        = 0,
    kLDT                = 2,
    kTSSAvailable       = 9,
    kTSSBusy            = 11,
    kCallGate           = 12,
    kInterruptGate      = 14,
    kTrapGate           = 15,


    /*  code & data segment types  */
    kReadWrite          = 2,
    kExecuteRead        = 10,

};