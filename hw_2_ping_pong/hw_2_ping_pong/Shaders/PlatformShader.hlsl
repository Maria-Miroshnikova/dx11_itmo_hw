struct VS_IN
{
    float4 pos : POSITION0;
    float4 col : COLOR0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};


// x, y, z, alpha
cbuffer Offset_and_size
{
    float4 offset;
    float4 size_coeff; // (x, y, 1, 1)
};


PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
	
    // сработает умножение?
    output.pos = input.pos * size_coeff + offset;
    
    // поворот вокруг оси
    //output.pos = mul((input.pos - center_pos), rot_matrix) + center_pos;
    
    // перекраска в белый
    //output.col = (0.4f, 0.4f, 0.4f, 1.0f);
    output.col = input.col;
	
    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
    float4 col = input.col;
#ifdef TEST
	if (input.pos.x > 400) col = TCOLOR;
#endif
    return col;
}