/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Node::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_pProgram = new GLProgram;
	m_pProgram->initWithFilenames("shaders/shader_0tex.vsh", "shaders/shader_0tex.fsh");
	m_pProgram->bindAttribLocation("a_position", GLProgram::VERTEX_ATTRIB_POSITION);
	m_pProgram->bindAttribLocation("a_color", GLProgram::VERTEX_ATTRIB_COLOR);
	//m_pProgram->bindAttribLocation("a_texCoord", GLProgram::VERTEX_ATTRIB_TEX_COORD);

	m_pProgram->link();
	m_pProgram->updateUniforms();

	//uniform_sampler = glGetUniformLocation(m_pProgram->getProgram(), "sampler");

	//m_pTexture = Director::getInstance()->getTextureCache()->addImage("texture.jpg");

	uniform_wvp_matrix = glGetUniformLocation(m_pProgram->getProgram(), "u_wvp_matrix");

	Director::getInstance()->setClearColor(Color4F(0, 1, 0, 0));
	//Director::getInstance()->setClearColor(Color4F(1, 1, 1, 0));

    return true;
}

void HelloWorld::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
	// onDraw๐JX^R}hฦตฤ\๑
	_customCommand.init(_globalZOrder, transform, flags);
	_customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);

}

void HelloWorld::onDraw(const Mat4& transform, uint32_t /*flags*/)
{
	counter++;
	//// ฎSใซ`ๆ
	//GL::blendFunc(GL_ONE, GL_ZERO);
	//// มZฌ
	//GL::blendFunc(GL_ONE, GL_ONE);
	//// ธZฌ
	//glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	//GL::blendFunc(GL_ONE, GL_ONE);
	// ผงพฌ
	GL::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION| GL::VERTEX_ATTRIB_FLAG_COLOR | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);

	m_pProgram->use();

	Vec3 pos[6];
	Vec4 color[6];
	Vec2 uv[6];
	const float x = 50.0f;
	const float y = 50.0f;
	const float z = 50.0f;

	// ภW
	pos[0] = Vec3(-x, -y, z); // ถบ
	pos[1] = Vec3(-x, y, z); // ถใ
	pos[2] = Vec3(x, -y, z); // Eบ
	pos[3] = Vec3(x, y, z);   // Eใ

	// F
	color[0] = Vec4(1, 0, 0, 1);
	color[1] = Vec4(1, 0, 0, 1);
	color[2] = Vec4(1, 0, 0, 1);
	color[3] = Vec4(1, 0, 0, 1);

	// eธ_ษUV๐่ฤ
	uv[0] = Vec2(0, 1); // ถบ
	uv[1] = Vec2(0, 0); // ถใ
	uv[2] = Vec2(1, 1); // Eบ
	uv[3] = Vec2(1, 0); // Eใ

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, pos);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);
	//glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, uv);

	//glUniform1i(uniform_sampler, 0);
	//GL::bindTexture2D(m_pTexture->getName());

	static float yaw = 0.0f;
	//yaw += CC_RADIANS_TO_DEGREES(1.0f);
	yaw += CC_DEGREES_TO_RADIANS(5.0f);
	Mat4 matProjection;
	Mat4 matView;
	Mat4 matWVP;
	Mat4 matTrans, matScale, matRot, matWorld;
	Mat4 matRotX, matRotY, matRotZ;

	matProjection = _director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	matView = _director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	Mat4::createTranslation(Vec3(1280 / 2.0f, 720 / 2.0f, 0), &matTrans);
	Mat4::createRotationZ(0, &matRotZ);
	Mat4::createRotationX(0, &matRotX);
	Mat4::createRotationY(yaw, &matRotY);
	// IC[p๑]ฬฌ
	matRot = matRotY * matRotX * matRotZ;
	// 1`3{ลzย
	//float scale = sinf(yaw)+2.0f;
	float scale = 2.0f;
	Mat4::createScale(Vec3(scale, scale, scale), &matScale);
	matWorld = matTrans * matRot * matScale;

	matWVP = matProjection * matView * matWorld;

	glUniformMatrix4fv(uniform_wvp_matrix, 1, GL_FALSE, matWVP.m);

	// `ๆ
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// ใส
	pos[0] = Vec3(-x, -y, -z); // ถบ
	pos[1] = Vec3(-x, y, -z); // ถใ
	pos[2] = Vec3(x, -y, -z); // Eบ
	pos[3] = Vec3(x, y, -z); // Eใ
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// ถส
	pos[0] = Vec3(-x, -y, -z); // ถบ
	pos[1] = Vec3(-x, -y, +z); // ถใ
	pos[2] = Vec3(-x, +y, -z); // Eบ
	pos[3] = Vec3(-x, +y, +z); // Eใ
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Eส
	pos[0] = Vec3(+x, -y, -z); // ถบ
	pos[1] = Vec3(+x, -y, +z); // ถใ
	pos[2] = Vec3(+x, +y, -z); // Eบ
	pos[3] = Vec3(+x, +y, +z); // Eใ
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//glBlendEquation(GL_FUNC_ADD);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
