/*
 | Pb
 | 文件名称: trietree.hpp
 | 文件作用: 字典树(前缀树)
 | 创建日期: 2021-08-09
 | 更新日期: 2021-08-10
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html
*/
#pragma once
#if not defined (_INCLUDE_PB_TRIETREE_TRIETREE_HPP_)
#define _INCLUDE_PB_TRIETREE_TRIETREE_HPP_        1
#include "../Type/ref.hpp"
namespace Pb
{
    // TrieTree(指针版本)
    // 字典树只能拿来处理字符串
    template<
        typename _Tdat>
    class TrieTree final
    {
    public:
        using Tdat = _Tdat;
        // Trie树结点
        struct Node
        {
            // 该结点代表的字符
            unichar curchar;
            // 数据, 如果为空表示该结点不代表任何单词
            std::optional<Tdat> data;
            // 孩子
            AutoRef<Node> child[std::numeric_limits<unichar>::max()];
            // 有效的孩子结点个数
            size_t childnum;
        };
        //
        TrieTree() : root(MakeRef<Node>(Node {}))
        {
            // nothing to do...
        }
        // 浅拷贝
        TrieTree(const TrieTree &_b) : root(_b.root)
        {
            // nothing to do...
        }
        // 浅拷贝
        TrieTree& operator=(const TrieTree &_b)
        {
            root = _b.root;
        }
        // 默认析构
        ~TrieTree() = default;
        // 添加一个单词, 携带数据是data
        TrieTree& AppendWord(const unichar *word, const size_t wordlen, const Tdat &data)
        {
            AutoRef<Node> curroot = root;
            //
            if (wordlen > 0)
            {
                for (size_t i = 0; i < wordlen; i += 1)
                {
                    const unichar curword = word[i];
                    //
                    if (!curroot->child[curword])
                    {
                        curroot->child[curword] = MakeRef<Node>(Node {
                            .curchar = curword,
                            .data = std::nullopt,
                            .child = { 0 },
                            .childnum = 0
                        });

                        curroot->childnum += 1;
                    }
                    //
                    curroot = curroot->child[curword];
                }
                // 现在, curroot是最终的位置, 存放数据
                curroot->data = data;
            }
            //
            return *this;
        }
        // 添加一个单词, 携带数据是data
        inline TrieTree& AppendWord(const unistring &word, const Tdat &data)
        {
            return AppendWord(word.c_str(), word.length(), data);
        }
        // 查找一个在字典树中的值
        // 如果单词为空, 那么会认为它不在字典树里面(因为空单词没有存放任何数据)
        std::optional<Tdat> Find(const unichar *word, const size_t wordlen) const noexcept
        {
            AutoRef<Node> curroot = root;
            //
            if (wordlen > 0)
            {
                bool res = true;
                //
                for (size_t i = 0; i < wordlen; i += 1)
                {
                    const unichar curword = word[i];
                    //
                    if (!curroot->child[curword])
                    {
                        res = false;
                        break;
                    }
                    //
                    curroot = curroot->child[curword];
                }
                //
                if (res)
                {
                    //
                    return curroot->data;
                }
            }
            return std::nullopt;
        }
        // 查找一个在字典树中的值
        // 如果单词为空, 那么会认为它不在字典树里面(因为空单词没有存放任何数据)
        inline std::optional<Tdat> Find(const unistring &word) const noexcept
        {
            return Find(word.c_str(), word.length());
        }
        // 判断一个单词是否在树中
        inline bool HasWord(const unichar *word, const size_t wordlen) const noexcept
        {
            return Find(word, wordlen).has_value();
        }
        // 判断一个单词是否在树中
        inline bool HasWord(const unistring &word) const noexcept
        {
            return Find(word.c_str(), word.length()).has_value();
        }
        // 取得根结点
        inline const AutoRef<Node>& GetRoot() const noexcept
        {
            return root;
        } 
    private:
        // 字典树的根结点
        AutoRef<Node> root;
    };
}
#endif // !_INCLUDE_PLOTTING_EXPR_PARSER_TRIETREE_TRIETREE_HPP_
