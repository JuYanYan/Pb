/*
 | Pb
 | 文件名称: list_type.hpp
 | 文件作用: 列表类型(List<x> := 1 + x * List<x>)
 | 创建日期: 2021-08-05
 | 更新日期: 2021-08-05
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined(_INCLUDE_PB_TYPE_LIST_TYPE_HPP_)
#define _INCLUDE_PB_TYPE_LIST_TYPE_HPP_       1
#include "../../../Utils/type_trait_concept.hpp"
namespace Pb
{
    // 列表类型(其实实现是vector)
    template<
        typename Type>
    struct ListType
    {
        using Tval = std::vector<Type>;
        //
        Tval data;
    };
    // 构建列表类型
    template<
        typename Targ>
    using MakeListType = typename ListType<Targ>::Tval;
    // 判断一个类型是否为和类型
    template<
        typename... Args>
    concept IsListType = Utils::IsInstantOf<ListType, Args...>;
}
#endif // !_INCLUDE_PB_TYPE_LIST_TYPE_HPP_

