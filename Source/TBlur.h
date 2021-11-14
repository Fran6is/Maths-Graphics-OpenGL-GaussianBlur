#pragma once

template<uint32_t GRID_DIMENSION_XY>
void GenerateWeights(std::array< std::array<float, GRID_DIMENSION_XY>, GRID_DIMENSION_XY>& Out_Grid, float SD, bool bNormalizeAfter )
{
    glm::vec2 MidPoint = glm::vec2(GRID_DIMENSION_XY/2);

    float TotalWeight = 0;

    for(int y = 0; y < GRID_DIMENSION_XY; y++) 
    {
        for(int x = 0; x < GRID_DIMENSION_XY; x++) 
        {
            Out_Grid[y][x] = CalculateNormalDistribution(
                SD,
                glm::vec2( x, y ) - MidPoint
            );

            TotalWeight += Out_Grid[y][x];
        }
    }

    NormalizeWeights(Out_Grid, TotalWeight);
}


template<uint32_t GRID_DIMENSION_XY>
void NormalizeWeights(std::array< std::array<float, GRID_DIMENSION_XY>, GRID_DIMENSION_XY>& Out_Grid )
{
    float TotalWeight = GetTotalWeight<GRID_DIMENSION_XY>(Out_Grid);

    //std::cout << "Total weight BEFORE = " << TotalWeight << "\n";

    //This avoids a "divide by zero" nan value if weight is 0 OR  1(Already normalized)
    if(TotalWeight == 0.f || TotalWeight == 1.f)  //TODO: Comparing 2 floats
    {
        std::cout << "Early return. Total grid weight is either 0 (No weights generated) or already 1 (Already normalized) \n";
        return;
    }
    
    float NewWeight = 0.f;
    for(int y = 0; y < GRID_DIMENSION_XY; y++) 
    {
        for(int x = 0; x < GRID_DIMENSION_XY; x++) 
        {
            Out_Grid[y][x] /= TotalWeight;
            NewWeight  += Out_Grid[y][x];
        }
    }
    
    //std::cout << "Total weight After = " << NewWeight << "\n";
}

template<uint32_t GRID_DIMENSION_XY>
void NormalizeWeights(std::array< std::array<float, GRID_DIMENSION_XY>, GRID_DIMENSION_XY>& Out_Grid, float GridWeight )
{
    if(GridWeight == 0.f || GridWeight == 1.f) 
    {
        std::cout << "Early return. Total grid weight is either 0 (No weights generated) or 1 (Already normalized) \n";
        return;
    }

    for(int y = 0; y < GRID_DIMENSION_XY; y++) 
    {
        for(int x = 0; x < GRID_DIMENSION_XY; x++) 
        {
            Out_Grid[y][x] /= GridWeight;
        }
    }
}

template<uint32_t GRID_DIMENSION_XY>
float GetTotalWeight(const std::array< std::array<float, GRID_DIMENSION_XY>, GRID_DIMENSION_XY>& Grid )
{
    float TotalWeight = 0;
    for(int y = 0; y < GRID_DIMENSION_XY; y++) 
    {
        for(int x = 0; x < GRID_DIMENSION_XY; x++) 
        {
            TotalWeight += Grid[y][x];
        }
    }
    
    return TotalWeight;
}

template<uint32_t GRID_DIMENSION_XY>
void DisplayGrid(const std::array< std::array<float, GRID_DIMENSION_XY>, GRID_DIMENSION_XY>& Grid )
{
    std::cout << "\n";
    const char* SPACING = "  ";
    std::cout << "Grid.MidPoint  = "  << "[ " << GRID_DIMENSION_XY/2 << ", " << GRID_DIMENSION_XY/2  << " ]" 
              << "\t Grid.Origin = "  << " Top-Left " << "\n";
    
    std::cout << "(0,0)" << "\n";
    std::cout << "__|___"  << " X axis (COLUMNS)\n";
    std::cout << "  |    "  << "\n";
    std::cout << "  |    "  << "\n";
    std::cout << "  Y axis (ROWS)   "  << "\n";
    
    std::cout << "\t\t\t" << Grid.size() << " x " << Grid.at(0).size() << "\n";

    float TotalWeights = 0;
    for(int y = 0; y < GRID_DIMENSION_XY; y++) 
    {
        for(int x = 0; x < GRID_DIMENSION_XY; x++) 
        {
            printf("%.6f %s", Grid[y][x], SPACING);
            TotalWeights += Grid[y][x];
        }
        std::cout << "\n";
    }
    std::cout << "\nTotal weight = " << TotalWeights << "\tNote: Grid coordinate log might be commented out to save console space to print out all the grid weights\n";

}
