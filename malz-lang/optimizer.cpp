#include <unordered_set>
#include <string>
#include <unordered_map>
#include <algorithm>

#include "optimizer.hpp"
#include "ir.hpp"
#include "helpers.hpp"

Block deadCodeOpt(const Block& block)
{
    Block original_block;
    Block optimized_block = block;

    do
    {
        original_block = optimized_block;
        optimized_block.clear();

        std::unordered_set<std::string> used = {};

        for(auto& instr: original_block)
            for(auto& arg: instr.args)
                used.insert(arg);

        for(int i = 0; i < block.size(); i++)
            if(!(!block[i].dest.empty() && !used.contains(block[i].dest)))
            {
                bool useless_affect = true;

                // Check for definitions without usage or multiple definitions were the previous ones have no effect
                if(!block[i].dest.empty())
                {
                    for(int j = i + 1; j < block.size(); j++)
                    {
                        if(std::ranges::find(block[j].args, block[i].dest) != block[j].args.end())
                        {
                            useless_affect = false;
                            break;
                        }
                        if(block[j].dest == block[i].dest)
                            break;
                    }
                }
                // Instruction with side effect
                else
                    useless_affect = false;

                if(!useless_affect)
                    optimized_block.push_back(block[i]);
            }
    }while(optimized_block != original_block);

    return optimized_block;
}

Block LVN(const Block &block)
{
    Block result;

    // A tuple representing a value (OpType, args...)
    // Example: (AFFECT_CONST, 5)
    //           (ADD, #1, #2)
    // Not that args can be both immediate values (for AFFECT_CONST) and ids for other ops
    using Value = std::tuple<OpType, std::vector<size_t>>;

    // example:
    // pair<ADD (#1, #2, ...), "d"> (where d is the dest)
    //std::unordered_map<Value, std::tuple<size_t, std::string>> lvn_table;
    std::vector<std::pair<Value, std::string>> lvn_table;
    // var name : id in lvn_table
    std::unordered_map<std::string, size_t> varname2id;
    std::unordered_map<std::size_t, std::string> id2varname;
    std::unordered_map<Value, size_t> value2id;

    for(auto& instr: block)
    {
        if(!instr.dest.empty())
        {
            // For AFFECT_CONST ops, the args are real values and not some ids
            std::vector<size_t> args_ids;

            if(instr.op == OpType::AFFECT_CONST)
                for(auto& arg: instr.args)
                    args_ids.push_back(stoi(arg));
            else
                for(auto& arg: instr.args)
                    args_ids.push_back(varname2id.at(arg));

            // Since ADD and MUL are both commutative operations, we sort the ids to detect that "a b" is same as "b a"
            if(instr.op == OpType::ADD || instr.op == OpType::MUL)
                std::ranges::sort(args_ids);

            Value val = std::make_tuple(instr.op, args_ids);

            IntermediateInstr new_instr = IntermediateInstr();

            if(!value2id.contains(val) && instr.op != OpType::AFFECT_VAR)
            {
                // Constant folding
                if(std::get<0>(val) == OpType::ADD || std::get<0>(val) == OpType::MUL)
                {
                    if(std::get<0>(lvn_table[std::get<1>(val)[0]].first) == OpType::AFFECT_CONST && std::get<0>(lvn_table[std::get<1>(val)[1]].first) == OpType::AFFECT_CONST)
                    {
                        if(std::get<0>(val) == OpType::ADD)
                            val = std::make_tuple(OpType::AFFECT_CONST, std::vector<size_t>({std::get<1>(lvn_table[std::get<1>(val)[0]].first)[0] + std::get<1>(lvn_table[std::get<1>(val)[1]].first)[0]}));
                        else if(std::get<0>(val) == OpType::MUL)
                            val = std::make_tuple(OpType::AFFECT_CONST, std::vector<size_t>({std::get<1>(lvn_table[std::get<1>(val)[0]].first)[0] * std::get<1>(lvn_table[std::get<1>(val)[1]].first)[0]}));
                    }
                }

                lvn_table.emplace_back(val, instr.dest);

                value2id[val] = lvn_table.size() - 1;
                varname2id[instr.dest] = lvn_table.size() - 1;
                id2varname[lvn_table.size() - 1] = instr.dest;

                new_instr.op = std::get<0>(val);

                std::vector<std::string> new_args;
                for(auto& arg: std::get<1>(val))
                    new_args.push_back(new_instr.op == OpType::AFFECT_CONST?std::to_string(arg):id2varname.at(arg));

                new_instr.args = new_args;
                new_instr.dest = instr.dest;
            }
            else
            {
                new_instr.op = OpType::AFFECT_VAR;

                if(instr.op != OpType::AFFECT_VAR)
                {
                    varname2id[instr.dest] = value2id[val];
                    new_instr.args = {id2varname[value2id[val]]};
                }
                // For copy propagation
                else
                {
                    varname2id[instr.dest] = std::get<1>(val)[0];
                    new_instr.args = {id2varname[std::get<1>(val)[0]]};
                }

                // Constant propagation
                if(std::get<0>(val) == OpType::AFFECT_VAR && std::get<0>(lvn_table[std::get<1>(val)[0]].first) == OpType::AFFECT_CONST)
                {
                    new_instr.op = OpType::AFFECT_CONST;

                    std::vector<std::string> new_args;
                    for(auto arg: std::get<1>(lvn_table[std::get<1>(val)[0]].first))
                        new_args.push_back(std::to_string(arg));

                    new_instr.args = new_args;
                }
                new_instr.dest = instr.dest;
            }
            result.push_back(new_instr);
        }
        else
        {
            IntermediateInstr new_instr = instr;

            std::vector<std::string> new_args;
            for(auto& arg: instr.args)
                new_args.push_back(id2varname[varname2id[arg]]);

            new_instr.args = new_args;
            result.push_back(new_instr);
        }
    }

    return result;
}
