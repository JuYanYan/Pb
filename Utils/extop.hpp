/*
 | Pb
 | 文件名称: extop.hpp
 | 文件作用: 拓展的操作符
 | 创建日期: 2021-08-14
 | 更新日期: 2021-08-17
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined (_INCLUDE_PB_UTILS_EXT_OP_HPP_)
#define _INCLUDE_PB_UTILS_EXT_OP_HPP_        1
#include "extconvert.hpp"
#include "resultconvert.hpp"
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
    static inline Combinator<Tstr, Tres> Digit()
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
    static inline Combinator<Tstr, Tres> BinDigit()
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
    static inline Combinator<Tstr, Tres> OctDigit()
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
    static inline Combinator<Tstr, Tres> HexDigit()
    {
        return CondTerm<Tstr, Tres>([](const Tres &ch) -> bool
        {
            return (ch >= _CH('0') && ch <= _CH('9'))
                || (ch >= _CH('a') && ch <= _CH('f'))
                || (ch >= _CH('A') && ch <= _CH('F'));
        });
    }
    // 识别十进制整数的分析器
    // 十进制整数是'0'-'9'之间的任意字符组成的:
    // [0-9]+
    template<
        typename Tstr,
        typename Tres = uint64_t>
    static inline Combinator<Tstr, Tres> Integer()
    {
        return Digit<Tstr>() * N(1, Infinity) >> ToInteger<Tres>(static_cast<Tres>(10));
    }
    // 识别八进制整数的分析器
    // 八进制整数是以0开头,'0'-'7'之间的任意字符组成的:
    // 0[0-7]+
    template<
        typename Tstr,
        typename Tres = uint64_t>
    static inline Combinator<Tstr, Tres> OctInteger()
    {
        const auto body = OctDigit<Tstr>() * N(1, Infinity);
        const auto value = body >> ToInteger<Tres>(static_cast<Tres>(8));
        //
        return (Term<Tstr>(_CH('0')) + value) >> Right<unichar, Tres>;
    }
    // 识别十六进制整数的分析器
    // 十六进制整数是0x开头,'0'-'9','a'-'f','A'-'F'之间的任意字符组成的:
    // 0x([0-9]|[a-f]|[A-F])+
    template<
        typename Tstr,
        typename Tres = uint64_t>
    static inline Combinator<Tstr, Tres> HexInteger()
    {
        const auto body = HexDigit<Tstr>() * N(1, Infinity);
        const auto value = body >> ToInteger<Tres>(static_cast<Tres>(16));
        //
        return (Term<Tstr>(_CH('0')) + Term<Tstr>(_CH('x')) + value) 
            >> Third<unichar, unichar, Tres>;
    }
    // 识别一个标识符的分析器
    // 一个标识符是以任意一个字母或者下划线或者符号'$'开头, 后跟数字或者字母的东东
    template<
        typename Tstr,
        typename Tres = unistring>
    static inline Combinator<Tstr, Tres> Identifier()
    {
        // clang-format off
        // 标识符开始的字符
        auto begch = Term<Tstr>(_CH('$'))
                   | Term<Tstr>(_CH('_'))
                   | InRange<Tstr>(_CH('a'), _CH('z'))
                   | InRange<Tstr>(_CH('A'), _CH('Z'));
        // 标识符重复k次的字符
        auto bdych = Term<Tstr>(_CH('_'))
                   | InRange<Tstr>(_CH('a'), _CH('z'))
                   | InRange<Tstr>(_CH('A'), _CH('Z'))
                   | InRange<Tstr>(_CH('0'), _CH('9'));
        // 标识符
        auto ident = (begch + bdych * N(0, Infinity))
                >> [](unichar ch, const std::vector<unichar> &name) -> Result<Tres>
                {
                    Tres res;
                    res.push_back(ch);
                    for (const auto &i : name)
                    {
                        res.push_back(i);
                    }
                    //
                    return Success<Tres>(res);
                };
        // clang-format on
        return ident;
    }
}
#endif // !_INCLUDE_PB_UTILS_EXT_OP_HPP_
