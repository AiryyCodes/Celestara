#version 410 core

in vec2 v_UV;

out vec4 v_FragColor;

uniform sampler2D u_Texture;
uniform vec2 u_SheetSize;
uniform int u_FrameIndex;
uniform int u_RowIndex;

void main()
{
    // Calculate frame dimensions
    vec2 frameSize = 1.0 / u_SheetSize;

    // Determine the column from the frame index
    int col = u_FrameIndex % int(u_SheetSize.x);

    // Flip the row index so 0 corresponds to the top row
    int row = int(u_SheetSize.y) - 1 - u_RowIndex;

    // Calculate the UV offset for the specified frame
    vec2 frameOffset = vec2(col, row) * frameSize;

    // Adjust the UV to sample only the specified frame
    vec2 adjustedUV = frameOffset + v_UV * frameSize;

	vec4 color = texture(u_Texture, adjustedUV);
	if (color.a <= 0.0f)
	{
		discard;
	}

    // Sample the texture at the adjusted UV
    v_FragColor = color;
}
