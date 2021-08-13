/*
 | Pb
 | 文件名称: combinator.hpp
 | 文件作用: 组合子
 | 创建日期: 2021-07-19
 | 更新日期: 2021-08-13
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.

 语法分析器函数定义为
 f<Tstr, Tres> :=
   Tstr -> (Tres, Tstr)
     
 Combinator<Tres>为f<Tres>的包装器
 它完成两个任务:
   1. 将分析器函数包装为一个函数指针备用
   2. CPS变换
 
 组合子加法定义为:
   Combinator<T1> + Combinator<T2> == Combinator<T1 * T2>
   Combinator<T1> + Empty == Combinator<T1>
   
 组合子或定义为:
   Combinator<T1> | Combinator<T2> == Combinator<T1 or T2>
   Combinator<T1> | Nothing == Combinator<T1>

 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#if not defined (_INCLUDE_PB_COMBINATOR_COMBINATOR_HPP_)
#define _INCLUDE_PB_COMBINATOR_COMBINATOR_HPP_        1
#include "../Type/ref.hpp"
#include "../Type/result.hpp"
#include "../../../Utils/type_trait_fun_type.hpp"
namespace Pb
{
    template<
        typename _Tstr,
        typename _Tres>
    requires(requires(const _Tstr &str)
    {
        str.CurWord();                          // 要求有取得单词的方法
    })
    class Combinator final
    {
    public:
        // 处理的字串类型
        using Tstr = _Tstr;
        // 语法分析器结果类型
        using Tres = _Tres;
        // Continuation函数类型
        // Tcontinuation<...> := Tparserfun<...> -> bool
        //                    := (Result<Tres>, Tstr) -> bool
        using Tcontinuation = std::function<bool(Result<Tres>, Tstr)>;
        // 语法分析器实现函数类型
        // Tparserfun<...> := Parser<Tstr, Tres>
        //                 := (Tstr, Tcont) -> bool
        using Tparserfun = std::function<bool(Tstr, Tcontinuation)>;
        // 
        // 
        template<
            typename _Tparser>
        Combinator(_Tparser f) : parser(MakeRef<Tparserfun>(f))
        {
            // nothing to do...
        }
        Combinator(const Combinator<Tstr, Tres> &_b) noexcept : parser(_b.parser)
        {
            // nothing to do...
        }
        // 拷贝, 不修改continuation
        Combinator& operator=(const Combinator<Tstr, Tres> &_b)
        {
            *parser = *_b.parser;
            return *this;
        }
        //
        ~Combinator() = default;
        // 执行parser
        template<
            typename Tcont>
        bool Parse(const Tstr &str, Tcont cont) const
        {
            return (*parser)(str, cont);
        }
        // 与b交换
        inline void Swap(Combinator &_b) noexcept
        {
            parser.swap(_b.parser);
        }
    private:
        // 语法分析器函数
        AutoRef<Tparserfun> parser;
    };
    // 构造组合子
    // Tparser:
    //   f :=
    //     (Tstr, Tcont) -> Result<Tres>
    //       := (Tstr -> (Result<Tres>, Tstr))
    template<
        typename Tparser>
    static inline auto MakeCombinator(Tparser fparser)
    {
        // bool
        using Tparam = Utils::ParamTypeOf<Tparser>;
        // Tstr
        using Tstr = typename std::tuple_element<0, Tparam>::type;
        // Tcont:
        //    f :=
        //      (Result<Tres>, Tstr) -> bool
        using Tcont = typename std::tuple_element<1, Tparam>::type;
        // Tres
        using Tres = typename std::tuple_element<0, Utils::ParamTypeOf<Tcont>>::type::Tval;
        //
        return Combinator<Tstr, Tres>(fparser);
    }
}
#endif // !_INCLUDE_PB_COMBINATOR_COMBINATOR_HPP_
