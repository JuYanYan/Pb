/*
 | Pb
 | 文件名称: pbdemo.cpp
 | 文件作用: 例子程序
 | 创建日期: 2021-08-13
 | 更新日期: 2021-08-13
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html

 Compile & Link command:

   clang pbdemo.cpp --std=c++20 -o pbdemo
*/
#include "pb.hpp"
#include <format>
#include <iostream>
#include <assert.h>
//
int main(int argc, char **argv)
{
    using namespace Pb;
    //
    if (argc != 2)
    {
        std::cout << "pbdemo [Args...]"       << std::endl
                  << "Args:"                  << std::endl
                  << "    a decimal integer." << std::endl;
        return EXIT_FAILURE;
    }
    // parser
    // clang-format off
    auto numbers = InRange<ConstString>('0', '9') * N(1, Infinity) 
                >> ToInteger<uint64_t>(10);
    // clang-format on
    // 输入
    ConstString str = argv[1];
    // 执行分析
    std::cout << std::format(">> {}", str.c_str()) << std::endl;
    auto [res, nextstr] = numbers(str);
    if (res.label == Label::Success)
    {
        std::cout << std::format(">> Success.\n   result: {}\n   then: '{}'", res.succ_val, nextstr.c_str()) << std::endl;
    }
    else {
        std::cout << std::format(">> Error.\n   {}", res.failed_val.msg) << std::endl;
    }
    std::cout << std::format(">> {}", res.label == Label::Success) << std::endl;
    //
    return EXIT_SUCCESS;
}
