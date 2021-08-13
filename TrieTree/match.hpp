/*
 | Pb
 | 文件名称: match.hpp
 | 文件作用: 字典树匹配(扁平的字典树)
 | 创建日期: 2021-08-10
 | 更新日期: 2021-08-11
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#if not defined (_INCLUDE_PB_TRIETREE_MATCH_HPP_)
#define _INCLUDE_PB_TRIETREE_MATCH_HPP_        1
#include "trietree.hpp"
#include "../Type/ref.hpp"
namespace Pb
{
    template<
        typename _Tdat>
    class FlatTrieTree final
    {
    public:
        using Tdat = _Tdat;
        using Ttree = TrieTree<Tdat>;
        // 扁平树结点
        struct Node
        {
            // 该结点需要匹配的字符
            unichar ch;
            // 数据, 可能没有, 表示这个结点还不表示一个单词
            std::optional<Tdat> data;
            // 子结点数目
            size_t childnum;
            // 下个孩子的起始索引
            size_t childbeg;
        };
        // 匹配器
        class Matcher final
        {
        public:
            Matcher(const std::vector<Node> &_tree) : tree(_tree), index(0), matchcnt(0), matchword()
            {
                // nothing to do...
            }
            Matcher(const Matcher &) = delete;
            Matcher& operator=(const Matcher &) = delete;
            ~Matcher() = default;
            // 复位匹配的状态记录
            void Reset() noexcept
            {
                index = 0;
                matchcnt = 0;
                matchword.clear();
            }
            // 前进一步, 尝试匹配字符ch
            // 返回值为tuple(可以继续前进: bool, 完成了一个单词: optional<data>)
            // 考虑到Matcher类的使用方式, 该方法不返回新的Matcher对象, 而是修改对象本身
            std::tuple<bool, std::optional<Tdat>> Step(const unichar ch)
            {
                const auto &curnode = tree[index];          // 当前树结点
                // 根结点需要准备一下起始条件
                if (index == 0)
                {
                    index += 1;
                    matchcnt = curnode.childnum;
                }
                // 尝试在matchword里面寻找合适的字符
                std::optional<Tdat> data = std::nullopt;
                //
                for (size_t i = 0; i < matchcnt; i += 1)
                {
                    size_t idx = i + index;                 // 当前处理的结点偏移量
                    if (tree[idx].ch == ch)                 // 找到了合适的结点
                    {
                        matchword.push_back(ch);
                        data = tree[idx].data;
                        matchcnt = tree[idx].childnum;
                        index = idx + tree[idx].childbeg;   // 指向该结点的孩子们
                        goto succ;
                    }
                }
                goto stop;
            stop:
                // 没有完成单词, 匹配失败
                return { false, std::nullopt };
            succ:
                // 匹配成功
                return { true, data };
            }
            // 取得当前路径的单词
            inline const unistring& GetWord() const noexcept
            {
                return matchword;
            }
            // 判断一个单词是否在树中
            bool HasWord(const unichar *word, const size_t wordlen) noexcept
            {
                Reset();
                //
                bool res = true;
                for (size_t i = 0; i < wordlen; i += 1)
                {
                    auto [cont, complete] = Step(word[i]);
                    res = complete.has_value();
                    if (!cont)
                    {
                        break;
                    }
                }
                //
                Reset();
                //
                return res;
            }
            // 判断一个单词是否在树中
            inline bool HasWord(const unistring &word) noexcept
            {
                return HasWord(word.c_str(), word.length());
            }
        private:
            // 树
            const std::vector<Node> &tree;
            // 当前操作的结点的索引
            size_t index;
            // 需要查找的结点数目
            size_t matchcnt;
            // 记录的单词
            unistring matchword;
        };
        // 从Trie树构建
        FlatTrieTree(const Ttree& trietree)
        {
            tree.reserve(24);
            // BFS
            std::queue<std::tuple<size_t, AutoRef<Ttree::Node>>> q;
            // 根结点
            q.emplace(0, trietree.GetRoot());
            //
            // 放入的结点计数, 当前只有一个根节点
            size_t appendcnt = 1;
            while (!q.empty())
            {
                // 从tree[from]这个父结点过来的
                // 离第一个孩子的距离是dist
                const auto [from, node] = q.front();
                // 处理当前结点
                tree.emplace_back(Node {
                    .ch = node->curchar,
                    .data = node->data,
                    .childnum = node->childnum,
                    .childbeg = 0,                      // 孩子结点开始的位置由后面处理到孩子时修改
                });
                // 放入待处理的结点
                for (const auto &child : node->child)
                {
                    if (child)
                    {
                        // 当前结点的下标
                        const size_t curidx = appendcnt;
                        //
                        if (tree[from].childbeg == 0)   // 表示当前位置是child的第一个子结点
                        {
                            tree[from].childbeg = curidx - from;
                        }
                        appendcnt += 1;
                        q.emplace(curidx, child);
                    }
                }
                //
                q.pop();
            }
        }
        FlatTrieTree(const FlatTrieTree& _b) : tree(_b.tree)
        {
            // nothing to do...
        }
        FlatTrieTree& operator=(const FlatTrieTree& _b)
        {
            tree = _b.tree;
        }
        // 返回一个渐进式的匹配器
        inline Matcher BeginMatch() const
        {
            return Matcher(tree);
        }
    private:
        // 字典树
        // 例如, 以()表示一个结点, []表示一个表示单词末尾的结点的话:   
        //   ROOT+
        //       |
        //       +---(S)-----(I)--+--[N]
        //       |                |
        //       |                +--[Q]
        //       |
        //       +---(L)-----[N]
        // 上面的字典树存放了SIN, SIQ, LN三个单词, 会被存储为这种的方式:
        //              +------>------+
        //              |             |
        //              |      +------>------+--->--+
        //              |      |      |      |      |
        // {R}   (S)   (L)    (I)    [N]    [N]    [Q]
        //  |     |     |      |
        //  |     +----->------+
        //  |    /|\    |
        //  +-----+-----+
        //  0     1     2     3      4      5      6
        // 孩子结点的字符值按照从小到大的顺序存放, 方便查找(上面
        // 的一些位置反了, 但是懒得改了..好不容易画出来的).
        // 根结点是tree[0], 不匹配任何字符
        std::vector<Node> tree;
    };
}
#endif // !_INCLUDE_PB_TRIETREE_MATCH_HPP_
