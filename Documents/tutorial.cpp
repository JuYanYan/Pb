/*
 | Pb
 | 文件名称: tutorial.cpp
 | 文件作用: 例子程序(教程)
 | 创建日期: 2021-08-14
 | 更新日期: 2021-08-14
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
 It may be used under the terms of the GNU General Public License v2.
 For full terms see the file COPYING file or visit http://www.gnu.org/licenses/gpl-2.0.html

 Compile & Link command:

   clang tutorial.cpp --std=c++20 -o tutorial
*/
#include "../pb.hpp"
#include <format>
#include <iostream>
#include <assert.h>
//
struct Color
{
    uint8_t r, g, b;  
};
//
int main(int argc, char **argv)
{
    using namespace Pb;
    //
    if (argc != 2)
    {
        std::cout << "tutorial [Args...]"       << std::endl
                  << "Args:"                  << std::endl
                  << "    a hex color." << std::endl;
        return EXIT_FAILURE;
    }
    // parser
    // clang-format off
    auto hexDigit = HexDigit<ConstString>();
    auto triColor = (hexDigit * 3_n) 
                 >> ToInteger<uint32_t>(16) 
                 >> [](uint32_t val) -> Result<Color>
                 {
                     const auto sr = (val >> 8) & 0xf;
                     const auto sg = (val >> 4) & 0xf;
                     const auto sb = val & 0xf;
                     return Success(Color {
                         .r = static_cast<uint8_t>((sr << 4) | sr), 
                         .g = static_cast<uint8_t>((sg << 4) | sg),
                         .b = static_cast<uint8_t>((sb << 4) | sb),
                     });
                 };
    auto sixColor = (hexDigit * 6_n) 
                 >> ToInteger<uint32_t>(16) 
                 >> [](uint32_t val) -> Result<Color>
                 {
                    return Success(Color {
                        .r = static_cast<uint8_t>((val >> 16) & 0xff), 
                        .g = static_cast<uint8_t>((val >> 8) & 0xff),
                        .b = static_cast<uint8_t>(val & 0xff)
                    });
                 };
    auto cssColor = ('#'_t + (sixColor | triColor)) >> Right<unichar, Color>;
    // clang-format on
    // 输入
    ConstString str = argv[1];
    // 执行分析
    std::cout << std::format(">> {}", str.c_str()) << std::endl;
    
    auto [res, nextstr] = cssColor(str);
    if (res.label == Label::Success)
    {
        std::cout << std::format(
            ">> Success.\n   result: r={:#x},g={:#x},b={:#x}\n   then: '{}'", 
            res.succ_val.r, res.succ_val.g, res.succ_val.b, 
            nextstr.c_str()
         ) << std::endl;
    }
    else {
        std::cout << std::format(">> Error.\n   {}", res.failed_val.msg) << std::endl;
    }

    std::cout << std::format(">> {}", res.label == Label::Success) << std::endl;
    //
    return EXIT_SUCCESS;
}

