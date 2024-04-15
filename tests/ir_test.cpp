#include <gtest/gtest.h>

#include "malz-lang/ir.hpp"

TEST(formBlocks, BasicTest) {
    std::vector<IntermediateInstr> program {
        {.op = OpType::AFFECT, .args = {"4"}, .dest = "v"},
        {.op = OpType::JMP, .label = "somewhere"},
        {.op = OpType::AFFECT, .args = {"2"}, .dest = "v"},
        {.op = OpType::LABEL, .label = "somewhere"},
        {.op = OpType::PRINT, .args = {"v"}}
    };

    std::vector<Block> expected {
        {
            {.op = OpType::AFFECT, .args = {"4"}, .dest = "v"},
            {.op = OpType::JMP, .label = "somewhere"}
        },
        {
            {.op = OpType::AFFECT, .args = {"2"}, .dest = "v"}
        },
        {
            {.op = OpType::LABEL, .label = "somewhere"},
            {.op = OpType::PRINT, .args = {"v"}}
        }
    };

    auto blocks = formBlocks(program);

    EXPECT_EQ(blocks, expected);
}

TEST(blocksNames, BasicTest) {
    std::vector<Block> blocks {
            {
                    {.op = OpType::AFFECT, .args = {"4"}, .dest = "v"},
                    {.op = OpType::JMP, .label = "somewhere"}
            },
            {
                    {.op = OpType::AFFECT, .args = {"2"}, .dest = "v"}
            },
            {
                    {.op = OpType::LABEL, .label = "somewhere"},
                    {.op = OpType::PRINT, .args = {"v"}}
            }
    };

    std::vector<std::pair<std::string, Block>> expected {
            {
                "b0",
                {
                    {.op = OpType::AFFECT, .args = {"4"}, .dest = "v"},
                    {.op = OpType::JMP, .label = "somewhere"}
                }
            },
            {
                "b1",
                {
                    {.op = OpType::AFFECT, .args = {"2"}, .dest = "v"}
                }
            },
            {
                "somewhere",
                {
                    {.op = OpType::PRINT, .args = {"v"}}
                }
            }
    };

    auto blocksWithNames = blocksNames(blocks);

    EXPECT_EQ(blocksWithNames, expected);
}

TEST(getCfg, BasicTest) {
    std::vector<std::pair<std::string, Block>> blocks {
            {
                    "b0",
                    {
                            {.op = OpType::AFFECT, .args = {"4"}, .dest = "v"},
                            {.op = OpType::JMP, .label = "somewhere"}
                    }
            },
            {
                    "b1",
                    {
                            {.op = OpType::AFFECT, .args = {"2"}, .dest = "v"}
                    }
            },
            {
                    "somewhere",
                    {
                            {.op = OpType::PRINT, .args = {"v"}}
                    }
            }
    };

    std::unordered_map<std::string, std::vector<std::string>> expected {
            {"b0", {"somewhere"}},
            {"b1", {"somewhere"}},
            {"somewhere", {}}
    };

    auto cfg = getCfg(blocks);

    EXPECT_EQ(cfg, expected);
}