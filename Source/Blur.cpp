#include "Blur.h"


float CalculateNormalDistribution(float SD, glm::vec2 V)
{
    SD = std::max(SD, 0.00001f); //Why? To avoid dividing by zero when the standard deviation is 0
    float a = ( V.x * V.x + V.y *  V.y ) / (SD*SD*2.f); 
    
    a = std::exp(-a);


    return a / (SD * SD * 3.14f * 2.f);
}