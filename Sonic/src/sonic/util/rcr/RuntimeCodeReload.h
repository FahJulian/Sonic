#pragma once

#include <utility>

#if !defined(SN_DLL_EXPORT)
    #include "DllReloader.h"
#endif

#if defined(SN_DLL_EXPORT)

    #define SN_RCR_API __declspec(dllexport)
    #define SN_RCR_FUNCTION(returnType, name, arguments, argumentTypes, argumensForwarded, ...) __declspec(dllexport) returnType name##arguments
    #define SN_RCR_MEMBER_FUNCTION(returnType, name, arguments, argumentTypes, argumentsForwarded, ...) __declspec(dllexport) returnType name##arguments

#else

    #if !defined(SN_RCR_NAMESPACE)
        #define SR_RCR_NAMESPACE
    #endif

    #if !defined(SN_RCR_DLL_PATH)
        #define SN_RCR_DLL_PATH ""
    #endif

    #define SN_RCR_FUNCTION(returnType, name, arguments, argumentTypes, argumensForwarded, ...) \
        returnType name##arguments \
        { \
            static returnType(*functionPointer)(argumentTypes) = sonic::DllReloader::registerFunctionPointer(SN_RCR_DLL_PATH, &functionPointer, \
                sonic::DllReloader::getTypeNames<returnType>(), SN_RCR_NAMESPACE"::", #name, sonic::DllReloader::getTypeNames<__VA_ARGS__>(), sonic::DllReloader::getTypeNames<argumentTypes>(true)); \
	        \
            return (*functionPointer)##argumensForwarded; \
        }

    #define SN_RCR_MEMBER_FUNCTION(returnType, name, arguments, argumentTypes, argumensForwarded, ...) \
        returnType name##arguments \
        { \
            static returnType(std::remove_reference<decltype(*this)>::type::*memberFunctionPointer)(argumentTypes) = sonic::DllReloader::registerMemberFunctionPointer( \
                SN_RCR_DLL_PATH, &memberFunctionPointer, sonic::DllReloader::getTypeNames<returnType>(), #name, sonic::DllReloader::getTypeNames<__VA_ARGS__>(), sonic::DllReloader::getTypeNames<argumentTypes>(true)); \
	        \
            return (this->*memberFunctionPointer)##argumensForwarded; \
        }

#endif


#define SN_NEXT_TYPE ,

#define SN_RCR_FUNCTION_0_ARGS(returnType, name, ...) \
        SN_RCR_FUNCTION(returnType, name, (), , (), __VA_ARGS__)

#define SN_RCR_FUNCTION_1_ARGS(returnType, name, arg0Type, arg0Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name), arg0Type, (std::forward<arg0Type>(arg0Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_2_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name), arg0Type SN_NEXT_TYPE arg1Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_3_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_4_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_5_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_6_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_7_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_8_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_9_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_10_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_11_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_12_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_13_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_14_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_15_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_16_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_17_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_18_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_19_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_20_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_21_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE \
        arg19Type SN_NEXT_TYPE arg20Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_22_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE \
        arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_23_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE \
        arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_24_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE \
        arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_25_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE \
        arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_26_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE \
        arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE \
        arg24Type SN_NEXT_TYPE arg25Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_27_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE \
        arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE \
        arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_28_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE \
        arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE \
        arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_29_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE \
        arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE \
        arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_30_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE \
        arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE \
        arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_31_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE \
        arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE \
        arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE \
        arg29Type SN_NEXT_TYPE arg30Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_32_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE \
        arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE \
        arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE \
        arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_33_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE \
        arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE \
        arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE \
        arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_34_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE \
        arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE \
        arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE \
        arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_35_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE \
        arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE \
        arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE \
        arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_36_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE \
        arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE \
        arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE \
        arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE \
        arg34Type SN_NEXT_TYPE arg35Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), \
        std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_37_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE \
        arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE \
        arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE \
        arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE \
        arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), \
        std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_38_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE \
        arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE \
        arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE \
        arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE \
        arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), \
        std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_39_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE \
        arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE \
        arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE \
        arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE \
        arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), \
        std::forward<arg38Type>(arg38Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_40_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE \
        arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE \
        arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE \
        arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE \
        arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), \
        std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_41_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE \
        arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE \
        arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE \
        arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE \
        arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE \
        arg39Type SN_NEXT_TYPE arg40Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), \
        std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), \
        std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_42_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE \
        arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE \
        arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE \
        arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE \
        arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE \
        arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE arg41Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), \
        std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), \
        std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_43_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE \
        arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE \
        arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE \
        arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE \
        arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE \
        arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE arg41Type SN_NEXT_TYPE arg42Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), \
        std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), \
        std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_44_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE \
        arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE \
        arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE \
        arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE \
        arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE \
        arg41Type SN_NEXT_TYPE arg42Type SN_NEXT_TYPE arg43Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), \
        std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), \
        std::forward<arg43Type>(arg43Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_45_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE \
        arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE \
        arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE \
        arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE \
        arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE \
        arg40Type SN_NEXT_TYPE arg41Type SN_NEXT_TYPE arg42Type SN_NEXT_TYPE arg43Type SN_NEXT_TYPE arg44Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), \
        std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), \
        std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_46_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE \
        arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE \
        arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE \
        arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE \
        arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE \
        arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE arg41Type SN_NEXT_TYPE arg42Type SN_NEXT_TYPE arg43Type SN_NEXT_TYPE \
        arg44Type SN_NEXT_TYPE arg45Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), \
        std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), \
        std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), \
        std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_47_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE \
        arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE \
        arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE \
        arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE \
        arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE \
        arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE arg41Type SN_NEXT_TYPE arg42Type SN_NEXT_TYPE \
        arg43Type SN_NEXT_TYPE arg44Type SN_NEXT_TYPE arg45Type SN_NEXT_TYPE arg46Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), \
        std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), \
        std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), \
        std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name), std::forward<arg46Type>(arg46Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_48_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, arg47Type, arg47Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name, arg47Type arg47Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE \
        arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE \
        arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE \
        arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE \
        arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE \
        arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE arg41Type SN_NEXT_TYPE \
        arg42Type SN_NEXT_TYPE arg43Type SN_NEXT_TYPE arg44Type SN_NEXT_TYPE arg45Type SN_NEXT_TYPE arg46Type SN_NEXT_TYPE arg47Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), \
        std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), \
        std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), \
        std::forward<arg45Type>(arg45Name), std::forward<arg46Type>(arg46Name), std::forward<arg47Type>(arg47Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_49_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, arg47Type, arg47Name, \
    arg48Type, arg48Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name, arg47Type arg47Name, arg48Type arg48Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE \
        arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE \
        arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE \
        arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE \
        arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE \
        arg41Type SN_NEXT_TYPE arg42Type SN_NEXT_TYPE arg43Type SN_NEXT_TYPE arg44Type SN_NEXT_TYPE arg45Type SN_NEXT_TYPE \
        arg46Type SN_NEXT_TYPE arg47Type SN_NEXT_TYPE arg48Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), \
        std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), \
        std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name), std::forward<arg46Type>(arg46Name), std::forward<arg47Type>(arg47Name), \
        std::forward<arg48Type>(arg48Name)), __VA_ARGS__)

#define SN_RCR_FUNCTION_50_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, arg47Type, arg47Name, \
    arg48Type, arg48Name, arg49Type, arg49Name, ...) \
        SN_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name, arg47Type arg47Name, arg48Type arg48Name, arg49Type arg49Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE \
        arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE \
        arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE \
        arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE \
        arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE \
        arg40Type SN_NEXT_TYPE arg41Type SN_NEXT_TYPE arg42Type SN_NEXT_TYPE arg43Type SN_NEXT_TYPE arg44Type SN_NEXT_TYPE \
        arg45Type SN_NEXT_TYPE arg46Type SN_NEXT_TYPE arg47Type SN_NEXT_TYPE arg48Type SN_NEXT_TYPE arg49Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), \
        std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), \
        std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name), \
        std::forward<arg46Type>(arg46Name), std::forward<arg47Type>(arg47Name), std::forward<arg48Type>(arg48Name), std::forward<arg49Type>(arg49Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_0_ARGS(returnType, name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (), , (), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_1_ARGS(returnType, name, arg0Type, arg0Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name), arg0Type, (std::forward<arg0Type>(arg0Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_2_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name), arg0Type SN_NEXT_TYPE arg1Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_3_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_4_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_5_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_6_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_7_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_8_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_9_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_10_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_11_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_12_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_13_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_14_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_15_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_16_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_17_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_18_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_19_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_20_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_21_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE \
        arg19Type SN_NEXT_TYPE arg20Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_22_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE \
        arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_23_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE \
        arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_24_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE \
        arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_25_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE \
        arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_26_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE \
        arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE \
        arg24Type SN_NEXT_TYPE arg25Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_27_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE \
        arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE \
        arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_28_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE \
        arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE \
        arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_29_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE \
        arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE \
        arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_30_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE \
        arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE \
        arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_31_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE \
        arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE \
        arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE \
        arg29Type SN_NEXT_TYPE arg30Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_32_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE \
        arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE \
        arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE \
        arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_33_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE \
        arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE \
        arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE \
        arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_34_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE \
        arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE \
        arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE \
        arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_35_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE \
        arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE \
        arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE \
        arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_36_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE \
        arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE \
        arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE \
        arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE \
        arg34Type SN_NEXT_TYPE arg35Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), \
        std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_37_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE \
        arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE \
        arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE \
        arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE \
        arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), \
        std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_38_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE \
        arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE \
        arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE \
        arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE \
        arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), \
        std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_39_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE \
        arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE \
        arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE \
        arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE \
        arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), \
        std::forward<arg38Type>(arg38Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_40_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE \
        arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE \
        arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE \
        arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE \
        arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), \
        std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_41_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE \
        arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE \
        arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE \
        arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE \
        arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE \
        arg39Type SN_NEXT_TYPE arg40Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), \
        std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), \
        std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_42_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE \
        arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE \
        arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE \
        arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE \
        arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE \
        arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE arg41Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), \
        std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), \
        std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_43_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE \
        arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE \
        arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE \
        arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE \
        arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE \
        arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE arg41Type SN_NEXT_TYPE arg42Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), \
        std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), \
        std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_44_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE \
        arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE \
        arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE \
        arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE \
        arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE \
        arg41Type SN_NEXT_TYPE arg42Type SN_NEXT_TYPE arg43Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), \
        std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), \
        std::forward<arg43Type>(arg43Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_45_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE \
        arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE \
        arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE \
        arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE \
        arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE \
        arg40Type SN_NEXT_TYPE arg41Type SN_NEXT_TYPE arg42Type SN_NEXT_TYPE arg43Type SN_NEXT_TYPE arg44Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), \
        std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), \
        std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_46_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE \
        arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE \
        arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE \
        arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE \
        arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE \
        arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE \
        arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE \
        arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE \
        arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE arg41Type SN_NEXT_TYPE arg42Type SN_NEXT_TYPE arg43Type SN_NEXT_TYPE \
        arg44Type SN_NEXT_TYPE arg45Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), \
        std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), \
        std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), \
        std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_47_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE \
        arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE \
        arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE \
        arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE \
        arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE \
        arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE \
        arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE \
        arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE \
        arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE arg41Type SN_NEXT_TYPE arg42Type SN_NEXT_TYPE \
        arg43Type SN_NEXT_TYPE arg44Type SN_NEXT_TYPE arg45Type SN_NEXT_TYPE arg46Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), \
        std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), \
        std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), \
        std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name), std::forward<arg46Type>(arg46Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_48_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, arg47Type, arg47Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name, arg47Type arg47Name), arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE \
        arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE \
        arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE \
        arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE \
        arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE \
        arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE \
        arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE \
        arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE \
        arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE arg41Type SN_NEXT_TYPE \
        arg42Type SN_NEXT_TYPE arg43Type SN_NEXT_TYPE arg44Type SN_NEXT_TYPE arg45Type SN_NEXT_TYPE arg46Type SN_NEXT_TYPE arg47Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), \
        std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), \
        std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), \
        std::forward<arg45Type>(arg45Name), std::forward<arg46Type>(arg46Name), std::forward<arg47Type>(arg47Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_49_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, arg47Type, arg47Name, \
    arg48Type, arg48Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name, arg47Type arg47Name, arg48Type arg48Name), arg0Type SN_NEXT_TYPE \
        arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE arg5Type SN_NEXT_TYPE \
        arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE arg10Type SN_NEXT_TYPE \
        arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE arg15Type SN_NEXT_TYPE \
        arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE arg20Type SN_NEXT_TYPE \
        arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE arg25Type SN_NEXT_TYPE \
        arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE arg30Type SN_NEXT_TYPE \
        arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE arg35Type SN_NEXT_TYPE \
        arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE arg40Type SN_NEXT_TYPE \
        arg41Type SN_NEXT_TYPE arg42Type SN_NEXT_TYPE arg43Type SN_NEXT_TYPE arg44Type SN_NEXT_TYPE arg45Type SN_NEXT_TYPE \
        arg46Type SN_NEXT_TYPE arg47Type SN_NEXT_TYPE arg48Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), \
        std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), \
        std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name), std::forward<arg46Type>(arg46Name), std::forward<arg47Type>(arg47Name), \
        std::forward<arg48Type>(arg48Name)), __VA_ARGS__)

#define SN_RCR_MEMBER_FUNCTION_50_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, arg47Type, arg47Name, \
    arg48Type, arg48Name, arg49Type, arg49Name, ...) \
        SN_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name, arg47Type arg47Name, arg48Type arg48Name, arg49Type arg49Name), \
        arg0Type SN_NEXT_TYPE arg1Type SN_NEXT_TYPE arg2Type SN_NEXT_TYPE arg3Type SN_NEXT_TYPE arg4Type SN_NEXT_TYPE \
        arg5Type SN_NEXT_TYPE arg6Type SN_NEXT_TYPE arg7Type SN_NEXT_TYPE arg8Type SN_NEXT_TYPE arg9Type SN_NEXT_TYPE \
        arg10Type SN_NEXT_TYPE arg11Type SN_NEXT_TYPE arg12Type SN_NEXT_TYPE arg13Type SN_NEXT_TYPE arg14Type SN_NEXT_TYPE \
        arg15Type SN_NEXT_TYPE arg16Type SN_NEXT_TYPE arg17Type SN_NEXT_TYPE arg18Type SN_NEXT_TYPE arg19Type SN_NEXT_TYPE \
        arg20Type SN_NEXT_TYPE arg21Type SN_NEXT_TYPE arg22Type SN_NEXT_TYPE arg23Type SN_NEXT_TYPE arg24Type SN_NEXT_TYPE \
        arg25Type SN_NEXT_TYPE arg26Type SN_NEXT_TYPE arg27Type SN_NEXT_TYPE arg28Type SN_NEXT_TYPE arg29Type SN_NEXT_TYPE \
        arg30Type SN_NEXT_TYPE arg31Type SN_NEXT_TYPE arg32Type SN_NEXT_TYPE arg33Type SN_NEXT_TYPE arg34Type SN_NEXT_TYPE \
        arg35Type SN_NEXT_TYPE arg36Type SN_NEXT_TYPE arg37Type SN_NEXT_TYPE arg38Type SN_NEXT_TYPE arg39Type SN_NEXT_TYPE \
        arg40Type SN_NEXT_TYPE arg41Type SN_NEXT_TYPE arg42Type SN_NEXT_TYPE arg43Type SN_NEXT_TYPE arg44Type SN_NEXT_TYPE \
        arg45Type SN_NEXT_TYPE arg46Type SN_NEXT_TYPE arg47Type SN_NEXT_TYPE arg48Type SN_NEXT_TYPE arg49Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), \
        std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), \
        std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name), \
        std::forward<arg46Type>(arg46Name), std::forward<arg47Type>(arg47Name), std::forward<arg48Type>(arg48Name), std::forward<arg49Type>(arg49Name)), __VA_ARGS__)


