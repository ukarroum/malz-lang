#pragma once

#include <vector>
#include <optional>
#include <unordered_map>

enum class OpType
{
    JMP, BRANCH, RETURN, LABEL
};

struct IntermediateInstr
{
    // Not sure about this one, maybe I'll switch to inheritance later
    OpType op;
    std::string label;
    std::string true_label;
    std::string false_label;
    std::vector<std::string> args;
    std::string dest;

    bool operator==(const IntermediateInstr& other) const = default;
};

using Block = std::vector<IntermediateInstr>;

std::vector<Block> formBlocks(const std::vector<IntermediateInstr>& body);
std::unordered_map<std::string, Block> block_map(const std::vector<Block>& blocks);
std::unordered_map<std::string, std::vector<std::string>> getCfg(const std::unordered_map<std::string, Block>& map);