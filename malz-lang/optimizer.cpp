#include <unordered_set>
#include <string>

#include "optimizer.hpp"

Block deadCodeOpt(const Block& block)
{
    Block original_block;
    Block optimized_block = block;

    do
    {
        original_block = optimized_block;
        optimized_block.clear();

        std::unordered_set<std::string> used = {};

        for(auto& instr: block)
            for(auto& arg: instr.args)
                used.insert(arg);

        for(auto& instr: block)
            if(!(!instr.dest.empty() && !used.contains(instr.dest)))
                optimized_block.push_back(instr);
    }while(optimized_block != original_block);

    return optimized_block;
}
