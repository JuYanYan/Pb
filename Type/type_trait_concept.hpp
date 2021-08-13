/*
 | Pb
 | 文件名称: type_trait_concept.hpp
 | 文件作用: type trait (concept)
 | 创建日期: 2021-07-19
 | 更新日期: 2021-07-20
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined(_INCLUDE_PB_TYPE_TRAIT_CONCEPT_HPP_)
#define _INCLUDE_PB_TYPE_TRAIT_CONCEPT_HPP_        1
#include "type_trait_fun_type.hpp"
namespace Pb
{
    //
    // 判断类型T是否为某个模板Template的特化
    //
    template<
        template<typename...> typename Template,
        typename T>
    struct _IsInstantOf : public std::false_type
    {
    };
    template<
        template<typename...> typename Template,
        typename... Types>
    struct _IsInstantOf<Template, Template<Types...>> : public std::true_type
    {
    };
    // 判断类型T是否为某个模板Template的特化
    template<
        template<typename...> typename Template,
        typename... Types>
    concept IsInstantOf = _IsInstantOf<Template, Types...>::value;
}
#endif // !_INCLUDE_PB_TYPE_TRAIT_CONCEPT_HPP_