/*
 | Pb
 | 文件名称: extconvert.hpp
 | 文件作用: 常用的数据转换函数
 | 创建日期: 2021-08-14
 | 更新日期: 2021-08-14
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined (_INCLUDE_PB_UTILS_EXT_CONVERT_HPP_)
#define _INCLUDE_PB_UTILS_EXT_CONVERT_HPP_        1
#include "../Type/result.hpp"
namespace Pb
{
    // 将字符转换为index进制的整数
    // 字符必须为[0-9]|[a-z]|[A-Z], 该函数不做检查
    template<
        typename Tint,
        typename Tch = unichar>
    static inline auto ToInteger(Tint index)
    {
        return [=](const std::vector<Tch> &list) -> Result<Tint>
        {
            Tint res = 0;
            for (const Tch &i : list)
            {
                const Tint val = i >= '0' && i <= '9' ? static_cast<Tint>(i - '0')
                               : i >= 'a' && i <= 'z' ? static_cast<Tint>(i - 'a' + 10)
                               : i >= 'a' && i <= 'z' ? static_cast<Tint>(i - 'A' + 10)
                               : 0;
                const Tint tmp = res;
                res = res * index + val;
                // 检查运算溢出
                if (res < tmp)
                {
                    return Fatal<Tint>("The integer constant is too large.");
                }
            }
            return Success<Tint>(res);
        };
    }
    // 将字符转换为字符串
    template<
        typename Tstr,
        typename Tch = unichar>
    static inline auto ToString()
    {
        return [=](const std::vector<Tch> &list) -> Result<Tstr>
        {
            Tstr res;
            res.reserve(list.size() + 1);
            //
            for (const auto &i : list)
            {
                res.push_back(i);
            }
            //
            return Success<Tstr>(res);
        };
    }
}
#endif // !_INCLUDE_PB_UTILS_EXT_CONVERT_HPP_
