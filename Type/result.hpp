/*
 | Pb
 | 文件名称: result.hpp
 | 文件作用: 结果类型
 | 创建日期: 2021-07-19
 | 更新日期: 2021-08-13
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined (_INCLUDE_PB_TYPE_RESULT_HPP_)
#define _INCLUDE_PB_TYPE_RESULT_HPP_        1
#include "sum_type.hpp"
namespace Pb
{
    // 结果标签
    enum class Label : int32_t
    {
        // none
        None,
        // 表示分析成功
        Success,
        // 表示分析出现错误, 但是不至于停止
        Error,
        // 表示分析出现了错误, 应该马上停下来
        Fatal,
    };
    // 结果类型
    template<
        typename _Tval>
    struct Result
    {
        using Tval = std::decay_t<_Tval>;           // (纯粹的传值)
        // 成功/失败?
        Label label;
        // 成功时的返回值
        Tval succ_val;
        // 失败时的值
        struct Failed
        {
            // 提示信息
            unistring msg;
        } failed_val;
    };
    // 构造一个成功的值
    template<
        typename Tval>
    inline Result<Tval> Success(const Tval &right = Tval())
    {
        return Result<Tval>
        {
            .label = Label::Success,
            .succ_val = right
        };
    }
    // 构造一个尝试失败时的返回值
    template<
        typename Tval>
    requires (
        // 必须有默认构造函数
        std::is_default_constructible<Tval>::value
    )
    inline Result<Tval> Error(const unistring &msg)
    {
        return Result<Tval>
        {
            .label = Label::Error,
            .failed_val = {
                .msg = msg
            }
        };
    }
    // 构造一个完全失败时的返回值
    template<
        typename Tval>
    requires (
        // 必须有默认构造函数
        std::is_default_constructible<Tval>::value
    )
    inline Result<Tval> Fatal(const unistring &msg)
    {
        return Result<Tval>
        {
            .label = Label::Fatal,
            .failed_val = {
                .msg = msg
            }
        };
    }
    // 构造一个失败时的返回值
    template<
        typename Tval,
        typename Tany>
    requires (
        // 必须有默认构造函数
        std::is_default_constructible<Tval>::value
    )
    inline Result<Tval> CopyError(const Result<Tany>& r)
    {
        return Result<Tval>
        {
            .label = r.label,
            .failed_val = {
                .msg = r.failed_val.msg
            }
        };
    }
}
#endif // !_INCLUDE_PB_TYPE_RESULT_HPP_
