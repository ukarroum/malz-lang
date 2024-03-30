#pragma once

#include <vector>
#include <optional>
#include <unordered_map>

enum class OpType
{
    JMP, BRANCH, RETURN
};

struct IntermediateInstr
{
    // Not sure about this one, maybe I'll switch to inheritence later
    std::optional<OpType> op;
    std::optional<std::string> label;
    std::optional<std::string> true_label;
    std::optional<std::string> false_label;

    [[nodiscard]] bool isLabel() const { return !op.has_value(); }
};

using Block = std::vector<IntermediateInstr>;

std::vector<Block> formBlocks(const std::vector<IntermediateInstr>& body);
std::unordered_map<std::string, Block> block_map(const std::vector<Block>& blocks);
std::unordered_map<std::string, std::vector<std::string>> getCfg(const std::unordered_map<std::string, Block>& map);