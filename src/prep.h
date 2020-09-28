#pragma once
#ifndef _PREP_H
#define _PREP_H

#include "pch.h"
#include "Graph.h"

namespace Surya{
	std::vector<Surya::Edge> readInput(std::string &file_path, int &vertex_count);
	std::vector<std::string> stringToToken(std::string &line);
}
#endif