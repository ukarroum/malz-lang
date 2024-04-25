#include <gtest/gtest.h>

#include "malz-lang/optimizer.hpp"

TEST(deadCodeOpt, VariableNeverUsedAsAnArgument) {
    std::vector<IntermediateInstr> program {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT_CONST, .args = {"2"}, .dest = "b"},
            {.op = OpType::AFFECT_CONST, .args = {"1"}, .dest = "c"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "d"},
            {.op = OpType::PRINT, .args = {"d"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT_CONST, .args = {"2"}, .dest = "b"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "d"},
            {.op = OpType::PRINT, .args = {"d"}}
    };

    auto optimized_program = deadCodeOpt(program);

    EXPECT_EQ(optimized_program, expected);
}

TEST(deadCodeOpt, VariableUsedInAVariableNeverUsed) {
    std::vector<IntermediateInstr> program {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT_CONST, .args = {"2"}, .dest = "b"},
            {.op = OpType::AFFECT_CONST, .args = {"1"}, .dest = "c"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "d"},
            {.op = OpType::ADD, .args = {"c", "d"}, .dest = "e"},
            {.op = OpType::PRINT, .args = {"d"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT_CONST, .args = {"2"}, .dest = "b"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "d"},
            {.op = OpType::PRINT, .args = {"d"}}
    };

    auto optimized_program = deadCodeOpt(program);

    EXPECT_EQ(optimized_program, expected);
}

TEST(deadCodeOpt, VariableDefinedTwice) {
    std::vector<IntermediateInstr> program {
            {.op = OpType::AFFECT_CONST, .args = {"100"}, .dest = "a"},
            {.op = OpType::AFFECT_CONST, .args = {"42"}, .dest = "a"},
            {.op = OpType::PRINT, .args = {"a"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT_CONST, .args = {"42"}, .dest = "a"},
            {.op = OpType::PRINT, .args = {"a"}}
    };

    auto optimized_program = deadCodeOpt(program);

    EXPECT_EQ(optimized_program, expected);
}

TEST(LVN, BasicTest) {
    std::vector<IntermediateInstr> block {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT_CONST, .args = {"2"}, .dest = "b"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "sum1"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "sum2"},
            {.op = OpType::MUL, .args = {"sum1", "sum2"}, .dest = "prod"},
            {.op = OpType::PRINT, .args = {"prod"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT_CONST, .args = {"2"}, .dest = "b"},
            {.op = OpType::AFFECT_CONST, .args = {"6"}, .dest = "sum1"},
            {.op = OpType::AFFECT_CONST, .args = {"6"}, .dest = "sum2"},
            {.op = OpType::AFFECT_CONST, .args = {"36"}, .dest = "prod"},
            {.op = OpType::PRINT, .args = {"prod"}}
    };

    auto optimized_program = LVN(block);

    EXPECT_EQ(optimized_program, expected);
}

TEST(LVN, CSE) {
    std::vector<IntermediateInstr> block {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT_CONST, .args = {"2"}, .dest = "b"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "sum1"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "sum2"},
            {.op = OpType::MUL, .args = {"sum1", "sum2"}, .dest = "prod"},
            {.op = OpType::PRINT, .args = {"prod"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT_CONST, .args = {"36"}, .dest = "prod"},
            {.op = OpType::PRINT, .args = {"prod"}}
    };

    auto optimized_program = deadCodeOpt(LVN(block));

    EXPECT_EQ(optimized_program, expected);
}

TEST(LVN, CopyPropagation) {
    std::vector<IntermediateInstr> block {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "x"},
            {.op = OpType::AFFECT_VAR, .args = {"x"}, .dest = "copy1"},
            {.op = OpType::AFFECT_VAR, .args = {"copy1"}, .dest = "copy2"},
            {.op = OpType::AFFECT_VAR, .args = {"copy2"}, .dest = "copy3"},
            {.op = OpType::PRINT, .args = {"copy3"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "x"},
            {.op = OpType::PRINT, .args = {"x"}}
    };

    auto optimized_program = deadCodeOpt(LVN(block));

    EXPECT_EQ(optimized_program, expected);
}

TEST(LVN, CopyPropagationNoDCE) {
    std::vector<IntermediateInstr> block {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT_CONST, .args = {"6"}, .dest = "b"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "x"},
            {.op = OpType::AFFECT_VAR, .args = {"x"}, .dest = "copy1"},
            {.op = OpType::AFFECT_VAR, .args = {"copy1"}, .dest = "copy2"},
            {.op = OpType::AFFECT_VAR, .args = {"copy2"}, .dest = "copy3"},
            {.op = OpType::PRINT, .args = {"copy3"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT_CONST, .args = {"6"}, .dest = "b"},
            {.op = OpType::AFFECT_CONST, .args = {"10"}, .dest = "x"},
            {.op = OpType::AFFECT_CONST, .args = {"10"}, .dest = "copy1"},
            {.op = OpType::AFFECT_CONST, .args = {"10"}, .dest = "copy2"},
            {.op = OpType::AFFECT_CONST, .args = {"10"}, .dest = "copy3"},
            {.op = OpType::PRINT, .args = {"x"}}
    };

    auto optimized_program = LVN(block);

    EXPECT_EQ(optimized_program, expected);
}

TEST(LVN, ConstantPropagationNoDCE) {
    std::vector<IntermediateInstr> block {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "x"},
            {.op = OpType::AFFECT_VAR, .args = {"x"}, .dest = "copy1"},
            {.op = OpType::AFFECT_VAR, .args = {"copy1"}, .dest = "copy2"},
            {.op = OpType::AFFECT_VAR, .args = {"copy2"}, .dest = "copy3"},
            {.op = OpType::PRINT, .args = {"copy3"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "x"},
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "copy1"},
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "copy2"},
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "copy3"},
            {.op = OpType::PRINT, .args = {"x"}}
    };

    auto optimized_program = LVN(block);

    EXPECT_EQ(optimized_program, expected);
}

TEST(LVN, CSECommutativity) {
    std::vector<IntermediateInstr> block {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT_CONST, .args = {"2"}, .dest = "b"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "sum1"},
            {.op = OpType::ADD, .args = {"b", "a"}, .dest = "sum2"},
            {.op = OpType::MUL, .args = {"sum1", "sum2"}, .dest = "prod"},
            {.op = OpType::PRINT, .args = {"prod"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT_CONST, .args = {"36"}, .dest = "prod"},
            {.op = OpType::PRINT, .args = {"prod"}}
    };

    auto optimized_program = deadCodeOpt(LVN(block));

    EXPECT_EQ(optimized_program, expected);
}

TEST(LVN, ConstantFolding) {
    std::vector<IntermediateInstr> block {
            {.op = OpType::AFFECT_CONST, .args = {"4"}, .dest = "a"},
            {.op = OpType::AFFECT_CONST, .args = {"2"}, .dest = "b"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "sum1"},
            {.op = OpType::ADD, .args = {"b", "a"}, .dest = "sum2"},
            {.op = OpType::MUL, .args = {"sum1", "sum2"}, .dest = "prod1"},
            {.op = OpType::ADD, .args = {"a", "b"}, .dest = "sum3"},
            {.op = OpType::MUL, .args = {"sum3", "sum3"}, .dest = "prod2"},
            {.op = OpType::PRINT, .args = {"prod2"}}
    };

    std::vector<IntermediateInstr> expected {
            {.op = OpType::AFFECT_CONST, .args = {"36"}, .dest = "prod2"},
            {.op = OpType::PRINT, .args = {"prod2"}}
    };

    auto optimized_program = deadCodeOpt(LVN(block));

    EXPECT_EQ(optimized_program, expected);
}