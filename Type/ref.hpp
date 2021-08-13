/*
 | Pb
 | 文件名称: ref.hpp
 | 文件作用: 引用
 | 创建日期: 2021-05-05
 | 更新日期: 2021-08-13
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined (_INCLUDE_PB_TYPE_REF_HPP_)
#define _INCLUDE_PB_TYPE_REF_HPP_         1
namespace Pb
{
    // 引用(指针)
    template<typename T>
    using AutoRef = std::shared_ptr<T>;
    // 构造一个引用
    template<typename T, typename... Args>
    inline AutoRef<T> MakeRef(Args&& ...args)
    {
        return std::make_shared<T>(args...);
    }
}
#endif // !_INCLUDE_PB_TYPE_REF_HPP_
