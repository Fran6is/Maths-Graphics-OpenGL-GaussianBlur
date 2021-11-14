#pragma once
#include <array>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//SD: standard deviation from a mean of 0, V: Vector from center of grid
float CalculateNormalDistribution(float SD, glm::vec2 V);


//SD: Standard deviation
template<uint32_t GRID_DIMENSION_XY>
void GenerateWeights(std::array< std::array<float, GRID_DIMENSION_XY>, GRID_DIMENSION_XY>& Out_Grid, float SD, bool bNormalizeAfter = true );

template<uint32_t GRID_DIMENSION_XY>
void NormalizeWeights(std::array< std::array<float, GRID_DIMENSION_XY>, GRID_DIMENSION_XY>& Out_Grid );

template<uint32_t GRID_DIMENSION_XY>
void NormalizeWeights(std::array< std::array<float, GRID_DIMENSION_XY>, GRID_DIMENSION_XY>& Out_Grid, float GridWeight );

template<uint32_t GRID_DIMENSION_XY>
float GetTotalWeight(const std::array< std::array<float, GRID_DIMENSION_XY>, GRID_DIMENSION_XY>& Grid );

template<uint32_t GRID_DIMENSION_XY>
void DisplayGrid(const std::array< std::array<float, GRID_DIMENSION_XY>, GRID_DIMENSION_XY>& Grid );

#include "TBlur.h"
