#include <gtest/gtest.h>

#include "malz-lang/optimizer.hpp"

TEST(deadCodeOpt, VariableNeverUsedAsAnArgument) {
    std::vector<IntermediateInstr> program {
            {.op = OpType::AFFECT, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT, .args = {"2"}, .dest = "b"},
            {.op = OpType::AFFECT, .args = {"1"}, .dest = "c"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "d"},
            {.op = OpType::PRINT, .args = {"d"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT, .args = {"2"}, .dest = "b"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "d"},
            {.op = OpType::PRINT, .args = {"d"}}
    };

    auto optimized_program = deadCodeOpt(program);

    EXPECT_EQ(optimized_program, expected);
}

TEST(deadCodeOpt, VariableUsedInAVariableNeverUsed) {
    std::vector<IntermediateInstr> program {
            {.op = OpType::AFFECT, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT, .args = {"2"}, .dest = "b"},
            {.op = OpType::AFFECT, .args = {"1"}, .dest = "c"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "d"},
            {.op = OpType::ADD, .args = {"c", "d"}, .dest = "e"},
            {.op = OpType::PRINT, .args = {"d"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT, .args = {"2"}, .dest = "b"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "d"},
            {.op = OpType::PRINT, .args = {"d"}}
    };

    auto optimized_program = deadCodeOpt(program);

    EXPECT_EQ(optimized_program, expected);
}

TEST(deadCodeOpt, VariableDefinedTwice) {
    std::vector<IntermediateInstr> program {
            {.op = OpType::AFFECT, .args = {"100"}, .dest = "a"},
            {.op = OpType::AFFECT, .args = {"42"}, .dest = "a"},
            {.op = OpType::PRINT, .args = {"a"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT, .args = {"42"}, .dest = "a"},
            {.op = OpType::PRINT, .args = {"a"}}
    };

    auto optimized_program = deadCodeOpt(program);

    EXPECT_EQ(optimized_program, expected);
}