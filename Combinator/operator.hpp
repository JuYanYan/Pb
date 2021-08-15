/*
 | Pb
 | 文件名称: operator.hpp
 | 文件作用: 组合子运算
 | 创建日期: 2021-07-19
 | 更新日期: 2021-08-13
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.

 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#if not defined (_INCLUDE_PB_COMBINATOR_OPERATOR_HPP_)
#define _INCLUDE_PB_COMBINATOR_OPERATOR_HPP_        1
#include "combinator.hpp"
#include "../Type/sum_type.hpp"
#include "../Type/mul_type.hpp"
#include "../Type/list_type.hpp"
#include "../TrieTree/match.hpp"
#include "../TrieTree/trietree.hpp"
namespace Pb
{
    // 无穷大
    static constexpr int32_t Infinity = -1;
    // 重复k项的处理函数
    // 闭包描述子
    struct ClosureDescriptor
    {
        using Tval = int32_t;
        // 最少需要的重复次数
        int32_t Nmin;
        // 最大需要的重复次数
        int32_t Nmax;
    };
    // N
    static inline ClosureDescriptor N(int32_t Min, int32_t Max) noexcept
    {
        return ClosureDescriptor{ .Nmin = Min, .Nmax = Max };
    }
    // N
    static inline ClosureDescriptor operator""_n(unsigned long long ntimes) noexcept
    {
        const int32_t n = static_cast<int32_t>(ntimes);
        //
        return ClosureDescriptor{ .Nmin = n, .Nmax = n };
    }
    // 单位元
    // TYPE:
    //   f<Tstr, Tres> :=
    //       () -> Combinator<Tstr, Tres>
    template<
        typename Tstr,
        typename Tres = std::monostate>
    static inline Combinator<Tstr, Tres> Empty()
    {
        using Tcont = typename Combinator<Tstr, Tres>::Tcontinuation;
        //
        return MakeCombinator([](Tstr str, Tcont cont) -> bool
        {
            return cont(Success<Tres>(), str);
        });
    }
    // 什么也不接受的项
    // TYPE:
    //   f<Tstr, Tres> :=
    //       () -> Combinator<Tstr, Tres>
    template<
        typename Tstr,
        typename Tres = std::monostate>
    static inline Combinator<Tstr, Tres> Nothing()
    {
        using Tcont = typename Combinator<Tstr, Tres>::Tcontinuation;
        //
        return MakeCombinator([](Tstr str, Tcont cont) -> bool
        {
            return cont(
                Error<Tres>(_CSTR("Nothing is good, but Abelian group not.")),
                str
            );
        });
    }
    // 条件项
    // 接收一个字符, 与一个条件判断cond, 如果满足则吃掉字符, 进行下一步
    // TYPE<Tstr, Tres, Tfcond: Tres -> bool> :
    //  f :=
    //    (Tres -> bool) -> Combinator<Tstr, Tres>
    //
    template<
        typename Tstr,
        typename Tres,
        typename Tfcond>
    static Combinator<Tstr, Tres> CondTerm(Tfcond cond)
    {
        using Tcont = typename Combinator<Tstr, Tres>::Tcontinuation;
        //
        using Tlexres = Result<std::tuple<typename Tstr::Tres, Tstr>>;
        //
        return MakeCombinator([=](Tstr str, Tcont cont) -> bool
        {
            Tlexres r = str.CurWord();
            if (r.label == Label::Success)
            {
                auto [word, next] = r.succ_val;
                if (cond(word))
                {
                    return cont(Success<Tres>(word), next);
                }
                else {
                    return cont(Error<Tres>(_CSTR("'cond(word)' return false.")), str);
                }
            }
            else {
                return cont(CopyError<Tres>(r), str);
            }
        });
    }
    // 单位项
    // 它接收字符ch
    // TYPE<Tstr, Tres>:
    //  f :=
    //    Tres -> Combinator<Tstr, Tres>
    //
    template<
        typename Tstr,
        typename Tres>
    static inline Combinator<Tstr, Tres> Term(const Tres &need_ch)
    {
        return CondTerm<Tstr, Tres>([=](const Tres &ch) -> bool
        {
            return ch == need_ch;
        });
    }
    // 表示一个范围内的字符项
    // 它接收字符'a' ~ 'b'中的任意一个字符
    // TYPE<Tstr, Tres>:
    //  f :=
    //    (Tres, Tres) -> Combinator<Tstr, Tres>
    // 相当于:
    // InRange(a, b) := 
    //   a | (a + 1) | (a + 2) | ...... | (b - 1) | b
    template<
        typename Tstr,
        typename Tres>
    static inline Combinator<Tstr, Tres> InRange(const Tres &a, const Tres &b)
    {
        return CondTerm<Tstr, Tres>([=](const Tres &ch) -> bool
        {
            return ch >= a && ch <= b;
        });
    }
    // 表示接收一串一模一样的字符
    // 它不会记录该字符串的内容, 什么也不会返回, 但是会检查是否全部匹配目标串
    // TYPE<Tstr, Tres, Ttarget>:
    //   f :=
    //     Ttarget -> Combinator<Tstr, ()>
    template<
        typename Tstr,
        typename Ttarget>
    static auto StringTerm(const Ttarget &target)
    {
        // 最终得到的结果类型
        using Tretres = std::monostate;
        // 词法分析器结果
        using Tlexres = Result<std::tuple<typename Tstr::Tres, Tstr>>;
        // 词法分析器的输入类型
        using Tlexinp = Tstr;
        // Continuation
        using Tretcont = typename Combinator<Tstr, Tretres>::Tcontinuation;
        //
        return MakeCombinator([=](Tstr str, Tretcont cont) -> bool
        {
            bool result = true;
            Tlexinp curstr = str;
            //
            for (const auto &i : target)
            {
                Tlexres r = curstr.CurWord();
                if (r.label == Label::Success)
                {
                    auto [word, next] = r.succ_val;
                    curstr = next;
                    if (word != i)
                    {
                        result = false;
                        break;
                    }
                }
            }
            //
            if (result)
            {
                return cont(Success<Tretres>(Tretres{}), curstr);
            }
            else {
                return cont(Error<Tretres>(_CSTR("String is not match.")), curstr);
            }
        });
    }
    // 表示接受字典树中的某一项, 它会寻找最长匹配
    template<
        typename Tstr,
        typename Tres>
    static auto TrieTerm(const TrieTree<Tres> &trietree)
    {
        // 最终得到的结果类型
        using Tretres = Tres;
        // 词法分析器结果
        using Tlexres = Result<std::tuple<typename Tstr::Tres, Tstr>>;
        // 词法分析器的输入类型
        using Tlexinp = Tstr;
        // Continuation
        using Tretcont = typename Combinator<Tstr, Tretres>::Tcontinuation;
        //
        return MakeCombinator([=](Tstr str, Tretcont cont) -> bool
        {
            FlatTrieTree flat = trietree;
            auto match = flat.BeginMatch();
            Tlexinp curstr = str;
            std::stack<std::tuple<Tres, Tstr>> matchstack;
            //
            while (1)
            {
                Tlexres r = curstr.CurWord();
                if (r.label == Label::Success)
                {
                    auto [word, next] = r.succ_val;                    
                    curstr = next;
                    // 尝试匹配当前字符
                    auto [repa, data] = match.Step(word);
                    //
                    if (data.has_value())
                    {
                        matchstack.emplace(data.value(), next);
                    }
                    if (!repa)
                    {
                        // 停止尝试
                        break;
                    }
                }
                else {
                    // 停止尝试
                    break;
                }
            }
            //
            if (matchstack.size() > 0)
            {
                auto [data, nextstr] = matchstack.top();
                return cont(Success<Tres>(data), nextstr);
            }
            else {
                return cont(Error<Tres>(_CSTR("Cannot find a suitable item.")), str);
            }
        });
    }
    // 组合子加法
    // TYPE<Tstr, Tres1, Tres2>:
    //  f :=
    //    (Combinator<Tstr, Tres1>, Combinator<Tstr, Tres2>) -> Combinator<Tstr, Tres1 * Tres2>
    template<
        typename Tstr,
        typename Tres1,
        typename Tres2>
    static auto operator+(const Combinator<Tstr, Tres1> &a, const Combinator<Tstr, Tres2> &b)
    {
        // 最终得到的结果类型
        using Tretres = MakeProductType<Tres1, Tres2>;
        // Continuation
        using Tretcont = typename Combinator<Tstr, Tretres>::Tcontinuation;
        //
        return MakeCombinator([=](Tstr str, Tretcont cont) -> bool
        {
            return a.Parse(str, [=](const Result<Tres1>& res1, const Tstr &nextstr1) -> bool
            {
                if (res1.label == Label::Success)
                {
                    return b.Parse(nextstr1, [=](const Result<Tres2>& res2, const Tstr &nextstr2)
                    {
                        if (res2.label == Label::Success)
                        {
                            auto &&r = Tretres(res1.succ_val, res2.succ_val);
                            return cont(Success<Tretres>(std::move(r)), nextstr2);
                        }
                        else {
                            return cont(CopyError<Tretres>(res2), nextstr2);
                        }
                    });
                }
                else {
                    return cont(CopyError<Tretres>(res1), nextstr1);
                }
            });
        });
    }
    // 表示一个内容出现许多次(也可以是0次)
    // TYPE<Tstr, Tres, N<Min, Max>>:
    //  f :=
    //    (Combinator<Tstr, Tres>, N) -> Combinator<Tstr, List[Tres | len := [Min .. Max]]>
    template<
        typename Tstr,
        typename Tres>
    static auto operator*(const Combinator<Tstr, Tres> &meta, const ClosureDescriptor &closure)
    {
            // 最终得到的结果类型
        using Tretres = MakeListType<Tres>;
        // Continuation
        using Tretcont = typename Combinator<Tstr, Tretres>::Tcontinuation;
        // 计数器类型
        using Tcount = typename ClosureDescriptor::Tval;
        //
        return MakeCombinator([=](Tstr str, Tretcont cont) -> bool
        {
            Tstr curstr;
            Tretres reslist;
            Result<Tres> curres;
            // 预留空间给处理结果
            if (closure.Nmax != Infinity)
            {
                reslist.reserve(closure.Nmax);
            }
            else {
                reslist.reserve(std::min(closure.Nmin, static_cast<Tcount>(24)));
            }
            // 先是必须的部分
            curstr = str;
            //
            for (Tcount cnt = 0; cnt < closure.Nmin; cnt += 1)
            {
                const bool contloop = meta.Parse(curstr, [&](const Result<Tres> &res, const Tstr &nextstr)
                {
                    curres = res;
                    curstr = nextstr;
                    if (res.label == Label::Success)
                    {
                        reslist.emplace_back(res.succ_val);
                        return true;
                    }
                    else {
                        return false;
                    }
                });
                if (!contloop)
                {
                    goto fatal_then_return;
                }
            }
            // 接着是可选的部分
            // 上面一定会有Nmin次, 这里处理剩下的Nmax - Nmin次
            for (Tcount cnt = closure.Nmin; closure.Nmax == Infinity ? true : cnt < closure.Nmax; cnt += 1)
            {
                const bool contloop = meta.Parse(curstr, [&](const Result<Tres> &res, const Tstr &nextstr)
                {
                    if (res.label != Label::Success)
                    {
                        // 失败了就跳出循环, 但是不算错误(因为是尝试然后失败了)
                        return false;
                    }
                    else {
                        curstr = nextstr;
                        //
                        reslist.emplace_back(res.succ_val);
                        // 没有失败, 继续尝试
                        return true;
                    }
                });
                if (!contloop)
                {
                    goto success_end;
                }
            }
        success_end:
            return cont(Success<Tretres>(reslist), curstr);
        fatal_then_return:
            return cont(CopyError<Tretres>(curres), curstr);
        });
    }
    // 组合子或
    // TYPE<Tstr, Tres1, Tres2>:
    //  f :=
    //    (Combinator<Tstr, Tres1>, Combinator<Tstr, Tres2>) -> Combinator<Tstr, Tres1 * Tres2>
    // 它会先测试a, 然后测试b
    // 遇到Fatal会完全停下来
    template<
        typename Tstr,
        typename Tres1,
        typename Tres2>
    static auto operator|(const Combinator<Tstr, Tres1> &a, const Combinator<Tstr, Tres2> &b)
    {
        // 最终得到的结果类型
        using Tretres = MakeSumType<Tres1, Tres2>;
        // Continuation
        using Tretcont = typename Combinator<Tstr, Tretres>::Tcontinuation;
        //
        return MakeCombinator([=](Tstr str, Tretcont cont) -> bool
        {
            return a.Parse(str, [=](const Result<Tres1>& res, const Tstr &nextstr) -> bool
            {
                if (res.label == Label::Success && cont(Success<Tretres>(res.succ_val), nextstr))
                {
                    return true;
                }
                else {
                    return b.Parse(str, [=](const Result<Tres2>& res, const Tstr &nextstr)
                    {
                        if (res.label == Label::Success)
                        {
                            return cont(Success<Tretres>(Tretres(res.succ_val)), nextstr);
                        }
                        else {
                            return cont(CopyError<Tretres>(res), nextstr);
                        }
                    });
                }
            });
        });
    }
    // 组合子结果类型转换
    // TYPE<Tstr, Toldres, Tfun>:
    //  f :=
    //    (Combinator<Tstr, Toldres>, Toldres -> Tnewres) -> Combinator<Tstr, Tnewres>
    template<
        typename Tstr,
        typename Toldres,
        typename Tfun,
        typename Tnewres = typename ReturnTypeOf<Tfun>::Tval>
    requires (
        IsInstantOf<Result, ReturnTypeOf<Tfun>>
    )
    static auto operator>>(const Combinator<Tstr, Toldres> &a, Tfun convert)
    {
        using Tretcont = typename Combinator<Tstr, Tnewres>::Tcontinuation;
        //
        return MakeCombinator([=](Tstr str, Tretcont cont) -> bool
        {
            return a.Parse(str, [=](const Result<Toldres>& res, const Tstr &nextstr) -> bool
            {
                if (res.label == Label::Success)
                {
                    Result<Tnewres> tmp;
                    if constexpr (IsProductType<Toldres>)
                    {
                        tmp = std::apply(convert, res.succ_val.data);
                    }
                    else if constexpr (IsSumType<Toldres> || IsListType<Toldres>)
                    {
                        tmp = convert(res.succ_val.data);
                    }
                    else {
                        tmp = convert(res.succ_val);
                    }
                    //
                    if (tmp.label == Label::Success)
                    {
                        return cont(Success<Tnewres>(tmp.succ_val), nextstr);
                    }
                    else {
                        return cont(CopyError<Tnewres>(tmp), nextstr);
                    }
                }
                else {
                    return cont(CopyError<Tnewres>(res), nextstr);
                }
            });
        });
    }
    // 表示一个可选(出现0次或者1次)的值
    // TYPE<Tstr, Tres>:
    //  f :=
    //    Combinator<Tstr, Tres> -> Combinator<Tstr, optional<Tres>>
    template<
        typename Tstr,
        typename Tres>
    static auto Optional(const Combinator<Tstr, Tres> &a)
    {
        // 最终得到的结果类型
        using Tretres = std::optional<Tres>;
        // Continuation
        using Tretcont = typename Combinator<Tstr, Tretres>::Tcontinuation;
        //
        return MakeCombinator([=](Tstr str, Tretcont cont) -> bool
        {
            return a.Parse(str, [=](const Result<Tres> &res, const Tstr &nextstr)
            {
                if (res.label != Label::Success)
                {
                    // 失败了, 相当于尝试失败, 不返回false
                    return cont(Success<Tretres>(std::nullopt), nextstr);
                }
                else {
                    // 没有失败
                    return cont(Success<Tretres>(res.succ_val), nextstr);
                }
            });
        });
    }
}
#endif // !_INCLUDE_PB_COMBINATOR_OPERATOR_HPP_
