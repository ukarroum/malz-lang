#pragma once

#include "ir.hpp"

Block deadCodeOpt(const Block& block);
Block LVN(const Block& block);