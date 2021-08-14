﻿/*
 | Pb
 | 文件名称: extop.hpp
 | 文件作用: 拓展的操作符
 | 创建日期: 2021-08-14
 | 更新日期: 2021-08-14
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined (_INCLUDE_PB_UTILS_EXT_OP_HPP_)
#define _INCLUDE_PB_UTILS_EXT_OP_HPP_        1
#include "../Combinator/operator.hpp"
namespace Pb
{
    // 十进制数的字符
    // TYPE<Tstr, Tres>:
    //  f :=
    //    () -> Combinator<Tstr, Tres>
    template<
        typename Tstr,
        typename Tres = unichar>
    static auto Digit()
    {
        return CondTerm<Tstr, Tres>([](const Tres &ch) -> bool
        {
            return ch >= _CH('0') && ch <= _CH('9');
        });
    }
    // 二进制数的字符
    // TYPE<Tstr, Tres>:
    //  f :=
    //    () -> Combinator<Tstr, Tres>
    template<
        typename Tstr,
        typename Tres = unichar>
    static auto BinDigit()
    {
        return CondTerm<Tstr, Tres>([](const Tres &ch) -> bool
        {
            return ch == _CH('0') || ch == _CH('1');
        });
    }
    // 八进制数的字符
    // TYPE<Tstr, Tres>:
    //  f :=
    //    () -> Combinator<Tstr, Tres>
    template<
        typename Tstr,
        typename Tres = unichar>
    static auto OctDigit()
    {
        return CondTerm<Tstr, Tres>([](const Tres &ch) -> bool
        {
            return ch >= _CH('0') && ch <= _CH('7');
        });
    }
    // 十六进制数的字符
    // TYPE<Tstr, Tres>:
    //  f :=
    //    () -> Combinator<Tstr, Tres>
    template<
        typename Tstr,
        typename Tres = unichar>
    static auto HexDigit()
    {
        return CondTerm<Tstr, Tres>([](const Tres &ch) -> bool
        {
            return (ch >= _CH('0') && ch <= _CH('9'))
                || (ch >= _CH('a') && ch <= _CH('f'))
                || (ch >= _CH('A') && ch <= _CH('F'));
        });
    }
}
#endif // !_INCLUDE_PB_UTILS_EXT_OP_HPP_
