#type vertex
#version 330 core

layout(location = 0) in vec2 a_Position;

out vec2 v_texcoord;

void main()
{
	v_texcoord = (a_Position + vec2(1.0,1.0))/2;
	gl_Position = vec4(a_Position, 0.0, 1.0);
}

#type fragment
#version 330 core

in vec2 v_texcoord;
layout(location = 0) out vec4 v_color;

uniform isampler2D IDTexture;
uniform sampler2D DepthTexture;
uniform sampler2D SelectedDepth;

void main()
{
	int outlineWidth = 17;

	float depth = texture(DepthTexture, v_texcoord).r;

	int pixelId = texture(IDTexture, v_texcoord).r;

	bool nearby = false;
	bool border = false;

	float outlineDepth = 1.0;
	bool behind = false;

	float numSamplesInObject = 0;
	float numSamplesOutsideViewport = 0;

	for(int i = -outlineWidth/2; i < outlineWidth/2; i++)
	{
		for(int j = -outlineWidth/2; j < outlineWidth/2; j++)
		{
			if(texture(IDTexture, v_texcoord + vec2(i,j)/1000).r != 0)
			{
				nearby = true;
				float objDepth = texture(SelectedDepth, v_texcoord + vec2(i,j)/1000.0).r;
				if(objDepth < outlineDepth)
				{
					outlineDepth = objDepth;
				}
				numSamplesInObject = numSamplesInObject + exp(-float(pow(i,2) + pow(j,2))/(2*pow(outlineWidth*1.5,2)));
			}
			if(v_texcoord.x + float(i)/1000 <= 0 || v_texcoord.x + float(i)/1000 >= 1 || v_texcoord.y + float(j)/1000 <= 0 || v_texcoord.y + float(j)/1000 >= 1)
			{
				border = true;
				numSamplesOutsideViewport = numSamplesOutsideViewport + exp(-float(pow(i,2) + pow(j,2))/(2*pow(outlineWidth*1.5,2)));
			}
		}
	}

	if (outlineDepth > depth)
	{
		behind = true;
	}

	float outlineFalloff = 0.5;

	if ((pixelId == 0 && nearby && !behind))
	{
		float alphaFactor = 2 * float(numSamplesInObject) / (outlineWidth * outlineWidth);
		if (alphaFactor > outlineFalloff)
		{
			alphaFactor = 1;
		}
		else
		{
			alphaFactor /= outlineFalloff;
		}
		v_color = vec4(0.9, 0.6, 0.0, 1.0 * alphaFactor);
	}
	else if((pixelId != 0 && border))
	{
		float alphaFactor = 2 * float(numSamplesOutsideViewport) / (outlineWidth * outlineWidth);
		if (alphaFactor > outlineFalloff)
		{
			alphaFactor = 1;
		}
		else
		{
			alphaFactor /= outlineFalloff;
		}
		v_color = vec4(0.9, 0.6, 0.0, 1.0 * alphaFactor);
	}
	else if(pixelId == 0 && nearby)
	{
		float alphaFactor = 2 * float(numSamplesInObject) / (outlineWidth * outlineWidth);
		if (alphaFactor > outlineFalloff)
		{
			alphaFactor = 1;
		}
		else
		{
			alphaFactor /= outlineFalloff;
		}
		v_color = vec4(0.9, 0.6, 0.0, 0.3 * alphaFactor);
	}
	else
		discard;
}
