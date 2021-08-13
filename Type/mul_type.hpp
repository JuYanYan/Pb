/*
 | Pb
 | 文件名称: mul_type.hpp
 | 文件作用: 积类型
 | 创建日期: 2021-06-20
 | 更新日期: 2021-08-06
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined(_INCLUDE_PB_TYPE_MUL_TYPE_HPP_)
#define _INCLUDE_PB_TYPE_MUL_TYPE_HPP_        1
#include "type_trait_concept.hpp"
namespace Pb
{
    // 类型乘法(笛卡尔积)
    template<
        typename... Types>
    struct Product
    {
        using Tval = std::tuple<std::decay_t<Types>...>;
        // 数据
        Tval data;
        //
        Product() = default;
        //
        template<
            typename... TypeFronts,
            typename... TypeAfters>
        Product(const Product<TypeFronts...> &fr, const TypeAfters&... _data) noexcept :
            data(std::tuple_cat(fr.data, std::make_tuple(_data...)))
        {
            // nothing to do...
        }
        //
        Product(const Types&... _data) noexcept : data(_data...)
        {
            // nothing to do...
        }
        //
        Product(const Tval &_data) noexcept : data(_data)
        {
            // nothing to do...
        }
    };
    // 类型乘法
    // 给一个已经得到的结果添加项, 得到新结果
    template<
        typename Type, 
        typename... Types>
    struct _MakeProductType_Kernel;
    // 左乘:
    //  定义为tuple(a, b) * c == tuple(a, b, c)
    template<
        typename... TypesFront,
        typename... TypesAfter>
    struct _MakeProductType_Kernel<Product<TypesFront...>, TypesAfter...>
    {
        using Tval = Product<TypesFront..., TypesAfter...>;
    };
    // 右乘:
    //  定义为a * any_type = tuple(a, any_type)
    template<
        typename TypesFront,
        typename... TypesAfter>
    struct _MakeProductType_Kernel
    {
        using Tval = Product<TypesFront, TypesAfter...>;
    };
    // 构造乘法
    template<
        typename... Args>
    using MakeProductType = typename _MakeProductType_Kernel<Args...>::Tval;
    // 判断一个类型是否为积类型
    template<
        typename... Args>
    concept IsProductType = IsInstantOf<Product, Args...>;
}
#endif // !_INCLUDE_PB_TYPE_MUL_TYPE_HPP_
