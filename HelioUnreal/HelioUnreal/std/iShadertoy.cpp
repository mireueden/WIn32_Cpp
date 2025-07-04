#include "iShadertoy.h"

#include "iStd.h"

iShadertoy::iShadertoy(STInfo* info)
{
	programID = new uint32[5];
	memset(programID, 0xFF, sizeof(uint32) * 5);

	//memset(texs, 0x00, sizeof(Texture*) * 8);
	texs = new Texture**[4];
	memset(texs, 0x00, sizeof(Texture**) * 4);

	texiChannel = new Texture ** [5];
	bufiChannel = new int* [5];
	memset(texiChannel, 0x00, sizeof(Texture**) * 5);
	memset(bufiChannel, 0x00, sizeof(int*) * 5);

	int len;
	char* strCode = loadFile(len, info->pathVert);
	uint32 vertID = iShader::compileVert(strCode);
	delete strCode;

	int lenBefore, lenAfter;
	char* strBefore = loadFile(lenBefore, "assets/shader/shadertoyBefore.frag");
	char* strAfter = loadFile(lenAfter, "assets/shader/shadertoyAfter.frag");

	for (int i = 0; i < 5; i++)
	{
		if (info->pathFrag[i] == NULL)
			continue;

		if (i < 4)
		{
			texs[i] = new Texture * [2];
			for (int j = 0; j < 2; j++)
				texs[i][j] = iFBO::createImage(devSize.width, devSize.height);
		}

		strCode = loadFile(len, info->pathFrag[i]);
		int lenTotal = lenBefore + len + lenAfter;
		char* str = new char[lenTotal + 1];
		strcpy(str, strBefore);
		strcpy(&str[lenBefore], strCode);
		strcpy(&str[lenBefore + len], strAfter);

		uint32 fragID = iShader::compileFrag(str);
		delete str;
		delete strCode;

		programID[i] = iShader::link(vertID, fragID);
		iShader::deleteShader(fragID);

		texiChannel[i] = new Texture * [4];
#if 1
		memcpy(texiChannel[i], info->tex[i], sizeof(Texture*) * 4);
#else
		for (int j = 0; j < 4; j++)
		{
			if (info->tex[i][j])
			{
				texiChannel[i][j] = info->tex[i][j];
				texiChannel[i][j]->retainCount++;
			}
			else// if (info->tex[i][j] == NULL)
				texiChannel[i][j] = NULL;
		}
#endif
		bufiChannel[i] = new int[4];
		memcpy(bufiChannel[i], info->buf[i], sizeof(int) * 4);
	}
	delete strBefore;
	delete strAfter;

	iShader::deleteShader(vertID);

	toggle = false;

	iTime = 0.0f;
	iFrame = 0;
	iChannelTime = new float[4];
	memset(iChannelTime, 0x00, sizeof(float) * 4);
	iMouse = new float[4];
	memset(iMouse, 0x00, sizeof(float) * 4);
}

iShadertoy::~iShadertoy()
{
	for (int i = 0; i < 5; i++)
	{
		if (programID[i] == 0xFFFFFFFF)
			continue;
		iShader::deleteProgram(programID[i]);
	}
	delete programID;

	for (int i = 0; i < 4; i++)
	{
		if (texs[i] == NULL)
			continue;
		for (int j = 0; j < 2; j++)
			freeImage(texs[i][j]);
		delete texs[i];
	}
	delete texs;

	for (int i = 0; i < 5; i++)
	{
		if (texiChannel[i])
		{
			for (int j = 0; j < 4; j++)
			{
				if (texiChannel[i][j])
					freeImage(texiChannel[i][j]);
			}
			delete texiChannel[i];
		}
		if (bufiChannel)
			delete bufiChannel;
	}
	delete texiChannel;
	delete bufiChannel;

	delete iChannelTime;
	delete iMouse;
}

void iShadertoy::setUniform(float dt, uint32 programID)
{
	uniform3f("iResolution", devSize.width, devSize.height, 0);
	uniform1f("iTime", iTime); iTime += dt;
	uniform1f("iTimeDelta", dt);
	uniform1f("iFrameRate", 0);
	uniform1i("iFrame", iFrame); iFrame++;
#if 1
	uniform1fv("iChannelTime", 4, iChannelTime);
#elif 0
	uniform4f("iChannelTime", iChannelTime[0], iChannelTime[1], iChannelTime[2], iChannelTime[3]);
#else
	for (int i = 0; i < 4; i++)
	{
		char s[16];
		sprintf(s, "iChannelTime[%d]", i);
		uniform1f("s", iChannelTime[i]);
	}
#endif
	uniform4f("iMouse", iMouse[0], iMouse[1], iMouse[2], iMouse[3]);
#if 0
	float v[12] = {
		texs[0][0]->width, texs[0][0]->height, 0,
		texs[1][0]->width, texs[1][0]->height, 0,
		texs[2][0]->width, texs[2][0]->height, 0,
		texs[3][0]->width, texs[3][0]->height, 0,
	};
	uniform3fv("iChannelResolution", 12, v);
	uniform vec4      iDate;
	uniform float     iSampleRate;
#endif
}

void iShadertoy::paint(float dt)
{
	glDisable(GL_BLEND);
	
	for (int i = 0; i < 5; i++)// Buffer A ~ D + Image
	{
		uint32 programID = this->programID[i];
		if (programID == 0xFFFFFFFF)
			continue;

		// programID 5개 존재, [0, 1, 2, 3] 후면버퍼 작업, [4] 전면(화면)버퍼
		if (i < 4)
		{
			fbo->bind(texs[i][toggle]);
		}

		glUseProgram(programID);

		glm::mat4 projMatrix = glm::ortho(0.0f, devSize.width, devSize.height, 0.0f, -1000.0f, 1000.0f);
		uniformM("projMatrix", projMatrix);
		glm::mat4 viewMatrix(1.0f);
		uniformM("viewMatrix", viewMatrix);

		setUniform(dt, programID);// for shadertoy uniform

		float position[] = {
			0, 0, 0, 1,                   devSize.width, 0,  0, 1,
			0, devSize.height, 0, 1,      devSize.width, devSize.height, 0, 1,
		};
		glBindBuffer(GL_ARRAY_BUFFER, vbo);// 1
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, position);

		uint32 pAttr = glGetAttribLocation(programID, "position");
		glEnableVertexAttribArray(pAttr);// 2
		glVertexAttribPointer(pAttr, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 0));

		Texture* ts[4] = {NULL, NULL, NULL, NULL};
		for (int j = 0; j < 4; j++)
		{
			Texture* tex = NULL;
			if (texiChannel[i][j])
				tex = texiChannel[i][j];
			else if (bufiChannel[i][j] != -1)
				tex = texs[bufiChannel[i][j]][!toggle];
			if (tex == NULL)
				continue;
			ts[j] = tex;

			char s[16];
			sprintf(s, "iChannel%d", j);
			glUniform1i(glGetUniformLocation(programID, s), j);
			glActiveTexture(GL_TEXTURE0 + j);
			glBindTexture(GL_TEXTURE_2D, tex->texID);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);// 3
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);// 3

		glDisableVertexAttribArray(pAttr);// 2
		glBindBuffer(GL_ARRAY_BUFFER, 0);// 1
		for (int j = 0; j < 4; j++)
		{
			if (ts[j])
			{
				glActiveTexture(GL_TEXTURE0 + j);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		if (i < 4)
		{
			fbo->unbind();
		}
	}

	toggle = !toggle;

	glEnable(GL_BLEND);
}

void iShadertoy::key(iKeyStat stat, iPoint point)
{
	switch (stat) {
	case iKeyStatBegan:
		//iMouse[0] = point.x;
		//iMouse[1] = point.y;
		memcpy(iMouse, &point, sizeof(iPoint));
		iMouse[2] = 1;
		break;
	case iKeyStatMoved:
		if (iMouse[2])
		{
			//iMouse[0] = point.x;
			//iMouse[1] = point.y;
			memcpy(iMouse, &point, sizeof(iPoint));
		}
		break;
	case iKeyStatEnded:
		iMouse[2] = 0;
		break;
	}
}

