#include <ranges>
#include <format>

#include "ir.hpp"

std::vector<Block> formBlocks(const std::vector<IntermediateInstr> &body)
{
    std::vector<Block> blocks;
    std::vector<OpType> terminators = {OpType::JMP, OpType::BRANCH, OpType::RETURN};
    Block current_block;

    for(const auto& instr: body)
    {
        // An op
        if(instr.op != OpType::LABEL)
        {
            current_block.push_back(instr);

            if(std::ranges::find(terminators, instr.op) != terminators.end())
            {
                blocks.push_back(current_block);
                current_block.clear();
            }
        }
        // A label
        else
        {
            blocks.push_back(current_block);
            current_block.clear();
            current_block.push_back(instr);
        }
    }

    if(!current_block.empty())
        blocks.push_back(current_block);

    return blocks;
}

std::vector<std::pair<std::string, Block>> blocksNames(const std::vector<Block> &blocks)
{
    std::vector<std::pair<std::string, Block>> blocksWithNames;
    std::string name;
    Block new_block;

    for(auto& block: blocks)
    {
        if(block[0].op == OpType::LABEL)
        {
            name = block[0].label;
            new_block = Block(block.begin() + 1, block.end());
        }
        else
        {
            name = std::format("b{}", blocksWithNames.size());
            new_block = block;
        }

        blocksWithNames.emplace_back(name, new_block);
    }

    return blocksWithNames;
}

std::unordered_map<std::string, std::vector<std::string>> getCfg(const std::vector<std::pair<std::string, Block>> &blocksWithNames)
{
    std::unordered_map<std::string, std::vector<std::string>> cfg;

    for(int i = 0; i < blocksWithNames.size(); i++)
    {
        std::string name = blocksWithNames[i].first;
        const Block& block = blocksWithNames[i].second;

        auto last = block[block.size() - 1];

        if(last.op == OpType::JMP)
            cfg[name] = {last.label};
        else if(last.op == OpType::BRANCH)
            cfg[name] = {last.true_label, last.false_label};
        else if(last.op == OpType::RETURN || i == blocksWithNames.size() - 1)
            cfg[name] = std::vector<std::string>();
        else
            cfg[name] = {blocksWithNames[i + 1].first};
    }

    return cfg;
}


