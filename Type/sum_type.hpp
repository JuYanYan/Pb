/*
 | Pb
 | 文件名称: sum_type.hpp
 | 文件作用: 类型加法(和类型)
 | 创建日期: 2021-06-20
 | 更新日期: 2021-08-06
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined(_INCLUDE_PB_TYPE_SUM_TYPE_HPP_)
#define _INCLUDE_PB_TYPE_SUM_TYPE_HPP_       1
#include "../../../Utils/type_trait_concept.hpp"
namespace Pb
{
    // 类型加法
    // A + B == A or B
    template<
        typename Type>
    struct _SumTypeBase
    {
    };
    template<
        typename... Types>
    struct SumType : public _SumTypeBase<Types>...
    {
        std::any data;
    };
    template<
        typename Type,
        typename SubType>
    struct _MakeSum_Kernel;
    template<
        typename _Type,
        typename _SubType>
    struct _MakeSumType_Kernel
    {
        using Type = std::decay_t<_Type>;
        using SubType = std::decay_t<_SubType>;
        // 值类型
        using Tval = typename std::conditional<
            std::same_as<Type, SubType>,
            Type,
            SumType<Type, SubType>>::type;
    };
    template<
        typename... Tys,
        typename _SubType>
    struct _MakeSumType_Kernel<SumType<Tys...>, _SubType>
    {
        using SubType = std::decay_t<_SubType>;
        using AddType = SumType<Tys...>;
        // 值类型
        using Tval = typename std::conditional<
            std::is_base_of<_SumTypeBase<SubType>, AddType>::value,
            AddType,
            SumType<Tys..., SubType>>::type;
    };
    // 构造加法(和类型)
    template<
        typename... Args>
    using MakeSumType = typename _MakeSumType_Kernel<Args...>::Tval;
    // 判断一个类型是否为和类型
    template<
        typename... Args>
    concept IsSumType = Utils::IsInstantOf<SumType, Args...>;
}
#endif // !_INCLUDE_PB_TYPE_SUM_TYPE_HPP_

