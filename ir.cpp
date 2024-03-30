#include <ranges>
#include <format>

#include "ir.hpp"

std::vector<Block> formBlocks(const std::vector<IntermediateInstr> &body)
{
    std::vector<std::vector<IntermediateInstr>> blocks;
    std::vector<OpType> terminators = {OpType::JMP, OpType::BRANCH, OpType::RETURN};
    std::vector<IntermediateInstr> current_block;

    for(auto instr: body)
    {
        // An op
        if(!instr.isLabel())
        {
            current_block.push_back(instr);

            if(std::ranges::find(terminators, instr.op.value()) != terminators.end())
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

std::unordered_map<std::string, Block> block_map(const std::vector<Block> &blocks)
{
    std::unordered_map<std::string, Block> map;
    std::string name;
    Block new_block;

    for(auto& block: blocks)
    {
        if(block[0].isLabel())
        {
            name = block[0].label.value();
            new_block = Block(block.begin() + 1, block.end());
        }
        else
        {
            name = std::format("b{}", map.size());
            new_block = block;
        }

        map[name] = new_block;
    }

    return map;
}

std::unordered_map<std::string, std::vector<std::string>> getCfg(const std::unordered_map<std::string, Block> &map)
{
    std::unordered_map<std::string, std::vector<std::string>> cfg;

    for(auto block: map)
    {
        auto last = block.second[block.second.size() - 1];

        if(last.op == OpType::JMP)
        {
            cfg[block.first] = {last.label.value()};
        }
        else if(last.op == OpType::BRANCH)
        {
            cfg[block.first] = {last.true_label.value(), last.false_label.value()};
        }
        else if(last.op == OpType::RETURN)
        {
            cfg[block.first] = std::vector<std::string>();
        }
        // Need to manage use case of else
    }
}
