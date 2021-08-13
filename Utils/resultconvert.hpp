/*
 | Pb
 | 文件名称: token.hpp
 | 文件作用: 单词
 | 创建日期: 2021-07-31
 | 更新日期: 2021-08-06
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#if not defined (_INCLUDE_PB_UTILS_RESULTCONVERT_HPP_)
#define _INCLUDE_PB_UTILS_RESULTCONVERT_HPP_        1
#include "../Type/result.hpp"
namespace Pb
{
    // 倆个值留第一个
    template<
        typename Ta,
        typename Tb = Ta>
    static constexpr Result<Ta> Left(const Ta& a, const Tb&)
    {
        return Success<Ta>(a);
    }
    // 倆个值留第二个
    template<
        typename Ta,
        typename Tb = Ta>
    static constexpr Result<Tb> Right(const Ta&, const Tb& b)
    {
        return Success<Tb>(b);
    }
    // 三个值留第一个
    template<
        typename Ta,
        typename Tb = Ta,
        typename Tc = Tb>
    static constexpr Result<Ta> First(const Ta& a, const Tb&, const Tc&)
    {
        return Success<Ta>(a);
    }
    // 三个值留第二个
    template<
        typename Ta,
        typename Tb = Ta,
        typename Tc = Tb>
    static constexpr Result<Tb> Second(const Ta&, const Tb& b, const Tc&)
    {
        return Success<Tb>(b);
    }
    // 三个值留第三个
    template<
        typename Ta,
        typename Tb = Ta,
        typename Tc = Tb>
    static constexpr Result<Tc> Third(const Ta&, const Tb&, const Tc& c)
    {
        return Success<Tc>(c);
    }
}
#endif // !_INCLUDE_PB_UTILS_RESULTCONVERT_HPP_
