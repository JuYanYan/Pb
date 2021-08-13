/*
 | Pb
 | 文件名称: str.hpp
 | 文件作用: 词法分析器(划分单个字符)
 | 创建日期: 2021-07-19
 | 更新日期: 2021-08-13
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined (_INCLUDE_PB_LEX_STRING_HPP_)
#define _INCLUDE_PB_LEX_STRING_HPP_        1
#include "../Type/result.hpp"
#include "../Combinator/operator.hpp"
namespace Pb
{
    class ConstString
    {
    public:
        // 输入的类型
        using Tinp = unistring;
        // 输出的类型
        using Tres = unichar;
        // this
        using Tthis = ConstString;
        // 
        //
        ConstString() : len(0), str(nullptr)
        {
            // nothing to do...
        }
        ConstString(const unistring &_str) : len(_str.length()), str(_str.c_str())
        {
            // nothing to do...
        }
        ConstString(const unichar *_str) : len(strlen(_str)), str(_str)
        {
            // nothing to do...
        }
        ConstString(const unichar *_str, size_t _len) : len(_len), str(_str)
        {
            // nothing to do...
        }
        ~ConstString() = default;
        ConstString(const ConstString &) = default;
        ConstString& operator=(const ConstString &) = default;
        // 取得当前单词
        Result<std::tuple<Tres, Tthis>> CurWord() const noexcept
        {
            using Tret = std::tuple<Tres, Tthis>;
            //
            if (*str == _CSTR('\0'))
            {
                return Error<Tret>(_CSTR("Input is empty."));
            }
            else {
                return Success<Tret>(std::make_tuple(*str, ConstString(str + 1, len - 1)));
            }
        }
        // 返回字符串
        inline const unichar* c_str() const noexcept
        {
            return str;
        }
    private:
        size_t len;
        const unichar* str;
    };

    // 单位项(char)
    // TYPE<Tstr, Tstr::Tword>:
    //  f :=
    //    Tstr::Tword -> Combinator<Tstr, Tstr::Tword>
    static inline auto operator""_t(unichar ch)
    {
        return Term<ConstString, ConstString::Tres>(ch);
    }
}
#endif // !_INCLUDE_PB_LEX_STRING_HPP_
