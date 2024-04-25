#pragma once

#include <vector>
#include <optional>
#include <unordered_map>

enum class OpType
{
    JMP, BRANCH, RETURN, LABEL, AFFECT, AFFECT_CONST, AFFECT_VAR, PRINT, ADD, MUL
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
std::vector<std::pair<std::string, Block>> blocksNames(const std::vector<Block>& blocks);
std::unordered_map<std::string, std::vector<std::string>> getCfg(const std::vector<std::pair<std::string, Block>> &blocksWithNames);