#version 430 // 2D arrays only works in GLSL 4.30 and above

in vec2 TexCoord;
flat in int InstanceID;


//Uniforms
uniform vec2 UWindowDimension = vec2(0);

uniform sampler2D UImage;

uniform bool UShouldBlur = false;

uniform int UGridSize = 3;

#define MAX_GRID_SIZE_XY  9

uniform float UKernel[MAX_GRID_SIZE_XY][MAX_GRID_SIZE_XY];


vec4 Blur(sampler2D Texture,  vec2 CurrentTexCoord, float BlurKernel [MAX_GRID_SIZE_XY][MAX_GRID_SIZE_XY], int GRID_DIMENSION_XY);

void main(void)
{
	vec2 uv = gl_FragCoord.xy - ( UWindowDimension * 0.5f );

	
	vec4 Color = vec4(0.0); Color.a = 1.f;
	if(UShouldBlur)
	{
		Color = Blur(
			UImage, 
			TexCoord, 
			UKernel,
			UGridSize
		);

		
	}
	else
	{
		Color = texture(UImage, TexCoord);
	}
	

	gl_FragColor = InstanceID == 0 ? texture(UImage, TexCoord) : Color ;
}

vec4 Blur(sampler2D Texture,  vec2 CurrentTexCoord, float BlurKernel [MAX_GRID_SIZE_XY][MAX_GRID_SIZE_XY], int GRID_DIMENSION_XY)
{
	vec2 MidPoint = vec2(GRID_DIMENSION_XY/2);

	vec2 TexelOffset = 1.0 / textureSize(Texture, 0);

	vec4 FinalColor = vec4(0.0);

	for(int y = 0; y < GRID_DIMENSION_XY; y++) 
    {
        for(int x = 0; x < GRID_DIMENSION_XY; x++) 
        {

            vec2 Offset = TexelOffset * ( vec2(x, y) - MidPoint ); 
			
        	FinalColor += texture(Texture, CurrentTexCoord + Offset) * BlurKernel[x][y];
            
        }
    }

	return FinalColor;
}





