#[[
  MIT License

  Copyright (C) 2022 by wangwenx190 (Yuhang Zhao)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
]]

if(_WINDOWS_TOOLCHAIN_INCLUDED)
    return() # Avoid transitive include.
endif()
set(_WINDOWS_TOOLCHAIN_INCLUDED 1)
if(NOT WIN32)
    return() # This toolchain is designed for Win32 platforms only.
endif()
if(NOT MSVC)
    return() # This toolchain is for MSVC/Clang-CL/Intel-CL only.
endif()
set(_arch "x86")
if(CMAKE_SIZEOF_VOID_P STREQUAL "8")
    set(_arch "x64")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(INTEL 1)
endif()
set(_c17_flag "/std:c17")
set(_cxx20_flag "/std:c++20")
if(CLANG)
    # Clang-CL uses the same parameter with MSVC.
elseif(INTEL)
    set(_c17_flag "/Qstd=c17")
    set(_cxx20_flag "/Qstd=c++20")
endif()
# Enable C17 standard.
string(REGEX REPLACE "[-|/]std:c[90|99|11|17|23]" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(APPEND CMAKE_C_FLAGS " ${_c17_flag} ")
# Enable C++20 standard.
string(REGEX REPLACE "[-|/]std:c++[98|11|14|17|20|23]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
string(APPEND CMAKE_CXX_FLAGS " ${_cxx20_flag} ")
# Enable exception handling for C++. Some of our dependencies need it.
string(REGEX REPLACE "[-|/]EHs-c-" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
string(APPEND CMAKE_CXX_FLAGS " /EHsc ") # C++ feature, don't apply to C flags.
if((NOT CLANG) AND (NOT INTEL) AND (_arch STREQUAL "x64"))
    # Make the exception handling overhead smaller, but only on x64.
    string(APPEND CMAKE_CXX_FLAGS " /d2FH4 ") # Introduced in VS2019.
endif()
# Enable runtime type information (RTTI) generation. We need it.
string(REGEX REPLACE "[-|/]GR-" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
string(APPEND CMAKE_CXX_FLAGS " /GR ") # C++ feature, don't apply to C flags.
# Change default code page to UTF-8 (65001) and suppress the copyright messages.
string(APPEND CMAKE_RC_FLAGS " /c65001 /nologo ")
set(_utf8_flag "/utf-8")
if(CLANG)
    # Clang-CL uses the same parameter with MSVC.
elseif(INTEL)
    set(_utf8_flag "/Qoption,cpp,--unicode_source_kind,UTF-8")
endif()
# Enable highest warning level.
string(REGEX REPLACE "[-|/]W[-|0|1|2|3|4]" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(REGEX REPLACE "[-|/]W[-|0|1|2|3|4]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
string(APPEND CMAKE_C_FLAGS " /W4 ")
string(APPEND CMAKE_CXX_FLAGS " /W4 ")
set(_common_flags "/await:strict /bigobj /MP /FS ${_utf8_flag} /permissive- /ZH:SHA_256")
set(_ltcg_compile_flag "/GL")
set(_ltcg_link_flag "/LTCG")
if(CLANG)
    set(_ltcg_compile_flag "-flto=thin") # Use ThinLTO to reduce compile time.
    set(_ltcg_link_flag "") # LLD doesn't need any parameters to enable LTO.
elseif(INTEL)
    set(_ltcg_compile_flag "/Qipo")
    set(_ltcg_link_flag "${_ltcg_compile_flag}")
endif()
set(_compiler_flags "/guard:cf /guard:ehcont ${_ltcg_compile_flag} /GA /GT /Gw /Gy /QIntel-jcc-erratum /Qspectre-load /Zc:inline")
set(_langs C CXX)
set(_build_types MINSIZEREL RELEASE RELWITHDEBINFO)
foreach(_lang ${_langs})
    foreach(_build_type ${_build_types})
        string(REGEX REPLACE "[-|/]Ob[0|1|2|3]" "" CMAKE_${_lang}_FLAGS_${_build_type} ${CMAKE_${_lang}_FLAGS_${_build_type}})
    endforeach()
    set(_params_common "${_common_flags}")
    if(_lang STREQUAL "CXX")
        string(APPEND _params_common " /Zc:char8_t,__cplusplus,externC,externConstexpr,hiddenFriend,lambda,preprocessor,referenceBinding,rvalueCast,strictStrings,ternary,throwingNew,trigraphs ")
    endif()
    string(APPEND CMAKE_${_lang}_FLAGS " ${_params_common} ")
    # Enable "Just My Code debugging" for debug builds.
    string(APPEND CMAKE_${_lang}_FLAGS_DEBUG " /JMC ")
    # Strangely, "/Ob2" produces smaller binary than "/Ob1".
    string(APPEND CMAKE_${_lang}_FLAGS_MINSIZEREL " ${_compiler_flags} /Ob2 ")
    # "/Ob3" was introduced in VS2019, but Clang-CL and Intel-CL can't recognize it.
    set(_ob3_flag "/Ob3")
    if(CLANG OR INTEL)
        set(_ob3_flag "/Ob2")
    endif()
    string(APPEND CMAKE_${_lang}_FLAGS_RELEASE " ${_compiler_flags} ${_ob3_flag} /Oi /Oy ")
    # Don't expand too aggressively if we plan to debug the code.
    string(APPEND CMAKE_${_lang}_FLAGS_RELWITHDEBINFO " ${_compiler_flags} /Ob1 ")
    set(_o1_flag "/O1")
    set(_o2_flag "/O2")
    set(_o3_flag "/O2") # MSVC doesn't support "/O3".
    if(CLANG)
        set(_o1_flag "/clang:-Oz") # Clang's own "-Oz" can produce much smaller binary than "/O1".
        set(_o3_flag "/clang:-O3")
    elseif(INTEL)
        set(_o3_flag "-O3")
    endif()
    set(_old_flag_regex "[-|/]O[d|0|1|2|3|fast]")
    string(REGEX REPLACE "${_old_flag_regex}" "${_o1_flag}" CMAKE_${_lang}_FLAGS_MINSIZEREL ${CMAKE_${_lang}_FLAGS_MINSIZEREL})
    string(REGEX REPLACE "${_old_flag_regex}" "${_o3_flag}" CMAKE_${_lang}_FLAGS_RELEASE ${CMAKE_${_lang}_FLAGS_RELEASE})
    # Don't optimize too aggressively if we want to debug the code.
    string(REGEX REPLACE "${_old_flag_regex}" "${_o2_flag}" CMAKE_${_lang}_FLAGS_RELWITHDEBINFO ${CMAKE_${_lang}_FLAGS_RELWITHDEBINFO})
endforeach()
set(_linker_flags "/CETCOMPAT /DYNAMICBASE /GUARD:CF /GUARD:EHCONT /LARGEADDRESSAWARE ${_ltcg_link_flag} /NXCOMPAT /OPT:REF /OPT:ICF /WX")
if(_arch STREQUAL "x64")
    # "/HIGHENTROPYVA" is only suitable for 64-bit architecture.
    string(APPEND _linker_flags " /HIGHENTROPYVA ")
endif()
set(_bin_types EXE SHARED)
foreach(_bin_type ${_bin_types})
    foreach(_build_type ${_build_types})
        set(_params "${_linker_flags}")
        if(_bin_type STREQUAL "EXE") # "/TSAWARE" is only for executables.
            string(APPEND _params " /TSAWARE ")
        endif()
        string(APPEND CMAKE_${_bin_type}_LINKER_FLAGS_${_build_type} " ${_params} ")
    endforeach()
endforeach()
