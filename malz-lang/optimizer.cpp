#include <unordered_set>
#include <string>
#include <ranges>

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

        for(int i = 0; i < block.size(); i++)
            if(!(!block[i].dest.empty() && !used.contains(block[i].dest)))
            {
                // Check for definitions without usage or multiple definitions were the previous ones have no effect
                bool useless_affect = true;
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
                if(!useless_affect)
                    optimized_block.push_back(block[i]);
            }
    }while(optimized_block != original_block);

    return optimized_block;
}
