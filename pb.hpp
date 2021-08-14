/*
 | Pb
 | 文件名称: pb.hpp
 | 文件作用: 总体需要的头文件
 | 创建日期: 2021-08-13
 | 更新日期: 2021-08-14
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined (_INCLUDE_PB_HPP_)
#define _INCLUDE_PB_HPP_        1
//
#include <any>
#include <tuple>
#include <stack>
#include <vector>
#include <optional>
#include <stdint.h>
#include <assert.h>
#include <functional>
#include <type_traits>
// 一些需要的类型
namespace Pb
{
    #define _CSTR(x)         x
    #define _CH(x)           x
    // 浮点数类型
    using float32_t = float;
    using float64_t = double;
    // tstring
    using unichar = char;
    using unistring = std::string;
    using unifstream = std::ifstream;
    using unistringstream = std::stringstream;
}
//
#include "Utils/str.hpp"
#include "Utils/extop.hpp"
#include "Utils/extconvert.hpp"
#include "TrieTree/trietree.hpp"
#include "Utils/resultconvert.hpp"
#include "Combinator/operator.hpp"
#include "Combinator/combinator.hpp"
//
#undef _CSTR
#undef _CH
#endif // !_INCLUDE_PB_HPP_
