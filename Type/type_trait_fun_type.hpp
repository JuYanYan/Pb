/*
 | Pb
 | 文件名称: type_trait_fun_type.hpp
 | 文件作用: 函数类型的一些模板
 | 创建日期: 2021-07-19
 | 更新日期: 2021-07-19
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined(_INCLUDE_PB_TYPE_TRAIT_FUN_TYPE_HPP_)
#define _INCLUDE_PB_TYPE_TRAIT_FUN_TYPE_HPP_        1
namespace Pb
{
    // 参数
    template<
        typename Ta,
        typename ...Tothers>
    struct _FunParams
    {
        using Type =
            typename std::conditional<
                sizeof...(Tothers) == 0,
                Ta,
                std::tuple<Ta, Tothers...>>::type;
    };
    // 函数参数类型(tuple)
    template<
        typename ...Tparams>
    using FunctionParamType = typename _FunParams<Tparams...>::Type;
    // 存放函数的参数, 返回值等等的信息
    template<
        typename Ret, 
        typename ...Args>
    struct _TraitsBase
    {
        // 返回值类型
        using ReturnType = Ret;
        // 参数数目
        static constexpr size_t NArgs = sizeof...(Args);
        // 参数(元组)
        using ArgsTuple = typename _FunParams<Args...>::Type;
    };
    template<
        typename F>
    struct _Traits;
    //
    template<
        typename F>
    struct _Traits<std::reference_wrapper<F>> : public _Traits<F>
    {
        // Empty...
    };
    // 匹配一般的函数指针
    template<
        typename Ret, 
        typename ...Args>
    struct _Traits<Ret(*)(Args...)> : public _TraitsBase<Ret, Args...>
    {
        // Empty...
    };
    // lambda, std:bind等
    template<
        typename Ret, 
        typename Cl, 
        typename ...Args>
    struct _Traits<Ret(Cl::*)(Args...)> : public _TraitsBase<Ret, Args...>
    {
        // Empty...
    };
    // lambda, std:bind等(const)
    template<
        typename Ret, 
        typename Cl, 
        typename ...Args>
    struct _Traits<Ret(Cl::*)(Args...) const> : public _TraitsBase<Ret, Args...>
    {
        // Empty...
    };
    // 其余情况
    template<
        typename F>
    struct _Traits : public _Traits<decltype(&F::operator())>
    {
        // Empty...
    };
    // 匹配取得函数指针
    template<
        typename F>
    struct _FunTraits : public _Traits<std::decay_t<F>>
    {
        // Empty...
    };
    // 取得函数F的返回值类型
    template<
        typename F>
    using ReturnTypeOf = typename _FunTraits<F>::ReturnType;
    // 取得函数F的参数类型(tuple, 一个的时候就是本身)
    template<
        typename F>
    using ParamTypeOf = typename _FunTraits<F>::ArgsTuple;
}
#endif // !_INCLUDE_PB_TYPE_TRAIT_FUN_TYPE_HPP_
