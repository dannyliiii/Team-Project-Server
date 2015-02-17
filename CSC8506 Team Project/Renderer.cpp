#include "Renderer.h"
#include <sstream>
#include <iomanip>
#include "PhysicsSystem.h"
#include "MyGame.h"
#include "Skybox.h"
#include "Material.h"
#include "Texture.h"

Renderer* Renderer::instance = NULL;
int Renderer::physics_updates = 0;
int Renderer::renderer_updates = 0;
int Renderer::elapsed_time = 0;
bool Renderer::wireframe = false;
Renderer::Renderer(Window &parent) : OGLRenderer(parent), gState(GInterface::Instance())	{
	//02/02/15
	RenderFunc = &Renderer::DefaultPass;
	NodeFunc = &Renderer::DrawNodeYieldShaderControl;
	defaultShader = new Shader(SHADERDIR"fallbackVertex.glsl", SHADERDIR"fallbackFragment.glsl");
	normalShader = new Shader(SHADERDIR"normalVertex.glsl", SHADERDIR"normalFragment.glsl");
	simpleShader = new Shader(SHADERDIR"LightVertex.glsl", SHADERDIR"LightFragment.glsl");


	material = new Material(simpleShader);
	SceneNode::SetDefaultMaterial(material);

	if (!defaultShader->LinkProgram()) return;
	if (!normalShader->LinkProgram()) return;
	if (!simpleShader->LinkProgram()) return;
	
	SceneNode::SetDefaultShader(defaultShader);

	camera			= NULL;
	root			= new SceneNode();
	drawFirstList.push_back(new Skybox());

	light = new Light(Vector3(RAW_HEIGHT * HEIGHTMAP_X / 2.0f, 500.0f, (RAW_HEIGHT*HEIGHTMAP_Z / 2.0f)), Vector4(1, 1, 1, 1), 10000.0f);

	/*Initialise heightmap (TEMPORARY)*/
	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	SceneNode* heightMapSN = new SceneNode(heightMap);
	heightMapSN->SetShader(simpleShader);
	heightMapSN->SetBoundingRadius(5000.0f);
	root->AddChild(heightMapSN);

	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	/*Set up FBOs for radial blur ppe*/
	test[0] = Texture::GenerateTexture2D(width, height, GI::Texture::IFormat::DEPTH24_STENCIL8, GI::Texture::Format::DEPTH_STENCIL, GI::Texture::Data::UNSIGNED_INT_24_8);
	test[1] = Texture::GenerateTexture2D(width, height, GI::Texture::IFormat::RGBA8, GI::Texture::Format::RGBA, GI::Texture::Data::UNSIGNED_BYTE);
	test[2] = Texture::GenerateTexture2D(width, height, GI::Texture::IFormat::RGBA8, GI::Texture::Format::RGBA, GI::Texture::Data::UNSIGNED_BYTE);

	ftest[0] = Framebuffer(true);
	ftest[1] = Framebuffer(true);

	ftest[0].Bind();
	ftest[0].SetTexture2D(GI::FBO::DEPTH_ATTACHMENT, test[0]);
	ftest[0].SetTexture2D(GI::FBO::STENCIL_ATTACHMENT, test[0]);
	ftest[0].SetTexture2D(GI::FBO::COLOUR_ATTACHMENT0, test[1]);
	ftest[0].Unbind();

	gState.SetDepthTest(true);
	gState.SetBlend(true);

	screenQuad = Mesh::GenerateQuad();
	processShader = new Shader(SHADERDIR"processVertex.glsl", SHADERDIR"processFragment.glsl");
	radialShader = new Shader(SHADERDIR"radialVertex.glsl", SHADERDIR"radialFragment.glsl");
	if (!processShader->LinkProgram()) return;
	if (!radialShader->LinkProgram()) return;

	instance		= this;
	init			= true;
}

Renderer::~Renderer(void)	{
//	delete root;
//	delete simpleShader;
	currentShader = NULL;
}

void Renderer::UpdateScene(float msec)	{
	renderer_updates++;
	if(camera) {
		camera->UpdateCamera(msec); 
	}
	root->Update(msec);
	if (Window::GetWindow().GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
		Renderer::SetRenderMode(RenderMode::RENDER_DEFAULT);
}
	if (Window::GetWindow().GetKeyboard()->KeyTriggered(KEYBOARD_3)) {
		Renderer::SetRenderMode(RenderMode::RENDER_NORMALS);
	}
	if (Window::GetWindow().GetKeyboard()->KeyTriggered(KEYBOARD_4)) {
		Renderer::SetRenderMode(RenderMode::RENDER_GAUSSIAN_BLUR);
	}
	if (Window::GetWindow().GetKeyboard()->KeyTriggered(KEYBOARD_5)) {
		Renderer::SetRenderMode(RenderMode::RENDER_RADIAL_BLUR);
	}
}

void Renderer::RenderScene()	{


	(this->*RenderFunc)();

	glUseProgram(0);
	SwapBuffers();
}

void	Renderer::UpdateShaderMatrices() {
	if (currentShader && light) {
		gState.SendUniform3fv("lightPos", light->GetPosition(), currentShader);
		gState.SendUniform4fv("lightColour", light->GetColour(), currentShader);
		gState.SendUniform1f("lightRadius", light->GetRadius(), currentShader);
		gState.SendUniform3fv("cameraPos", camera->GetPosition(), currentShader);
	}
	OGLRenderer::UpdateShaderMatrices();
}

void Renderer::DrawNodeForceShaderControl(SceneNode* n) {
	/*
		Added this lines of code so I could use particles to create some of the
		obstacles.
		- Carlos
	*/
	if (n->GetMesh()|| (!n->isParent)) {
		if(!n->isParent)
			SetCurrentShader(n->GetShader());
		gState.SendUniformMatrix4("modelMatrix", (n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale())), currentShader);
		gState.SendUniform4fv("nodeColour", n->GetColour(), currentShader);
		n->GetMaterial()->BindTextures();
		n->Draw(*this);
	}
}
void Renderer::DrawNodeYieldShaderControl(SceneNode* n) {
	/*
		Added this lines of code so I could use particles to create some of the
		obstacles.
		- Carlos
	*/
	if (n->GetMesh() && n->GetShader() || (!n->isParent)) {
		if(!n->isParent)
			SetCurrentShader(n->GetShader());
		else
			SetCurrentShader(n->GetMaterial()->GetShader());
		UpdateShaderMatrices();
		gState.SendUniformMatrix4("modelMatrix", (n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale())), currentShader);
		gState.SendUniform4fv("nodeColour", n->GetColour(), currentShader);
		n->GetMaterial()->BindTextures();
		n->Draw(*this);
	}
}


void	Renderer::BuildNodeLists(SceneNode* from)	{
	/*if (!from->isParent)
		cout << "Im a particle scene node you stupid idiot!" << endl;*/
	//Vector3 direction = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
	//from->SetCameraDistance(Vector3::Dot(direction,direction));

	//if(frameFrustum.InsideFrustum(*from)) {
		if(from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else{
			nodeList.push_back(from);
		}
	//}

	for(vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}
}

void	Renderer::DrawNodes()	 {
	for (vector<SceneNode*>::const_iterator i = drawFirstList.begin(); i != drawFirstList.end(); ++i) {
		(this->*NodeFunc)(*i);
	}

	for(vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i ) {
		(this->*NodeFunc)(*i);
	}

	for(vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i ) {
		(this->*NodeFunc)(*i);
	}
}

void	Renderer::SortNodeLists()	{
	std::sort(transparentNodeList.begin(),	transparentNodeList.end(),	SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(),				nodeList.end(),				SceneNode::CompareByCameraDistance);
}

void	Renderer::ClearNodeLists()	{
	transparentNodeList.clear();
	nodeList.clear();
}

void	Renderer::SetCamera(Camera*c) {
	camera = c;
}

void	Renderer::AddNode(SceneNode* n) {
	root->AddChild(n);
}

void	Renderer::RemoveNode(SceneNode* n) {
	root->RemoveChild(n);
}

/*
Draw a line of text on screen. If we were to have a 'static' line of text, we'd
probably want to keep the TextMesh around to save processing it every frame,
but for a simple demonstration, this is fine...
*/
void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective)	{
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);
	gState.ActivateTextureUnit(0);
	gState.BindTexture(GI::Texture::Type::TEXTURE_2D, mesh->GetTexture());
	//This just does simple matrix setup to render in either perspective or
	//orthographic mode, there's nothing here that's particularly tricky.
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	}
	else{
		//In ortho mode, we subtract the y from the height, so that a height of 0
		//is at the top left of the screen, which is more intuitive
		//(for me anyway...)
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	//Either way, we update the matrices, and draw the mesh
	UpdateShaderMatrices();
	mesh->Draw();

	delete mesh; //Once it's drawn, we don't need it anymore!
}

void Renderer::DefaultPass() {
	gState.ClearBuffers(true, true, true);
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

	if (camera) {
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
		modelMatrix.ToIdentity();
		frameFrustum.FromMatrix(projMatrix * viewMatrix);
		BuildNodeLists(root);
		SortNodeLists();
		DrawNodes();
		ClearNodeLists();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	//DrawHUD();
}

void Renderer::NormalPass() {
	gState.ClearBuffers(true, true, true);
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

	if (camera) {
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
		modelMatrix.ToIdentity();
		frameFrustum.FromMatrix(projMatrix * viewMatrix);
		SetCurrentShader(normalShader);
		UpdateShaderMatrices();

		BuildNodeLists(root);
		SortNodeLists();

		NodeFunc = &Renderer::DrawNodeForceShaderControl;
		DrawNodes();
		NodeFunc = &Renderer::DrawNodeYieldShaderControl;

		ClearNodeLists();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		DrawHUD();
	}
}

void Renderer::RadialPass() {
	ftest[0].Bind();
	DefaultPass();

	//end 1st pass

	ftest[1].Bind();
	ftest[1].SetTexture2D(GI::FBO::COLOUR_ATTACHMENT0, test[2]);

	SetCurrentShader(processShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();

	//ping -> pong
	ftest[1].SetTexture2D(GI::FBO::COLOUR_ATTACHMENT0, test[2]);
	test[1].Bind(0);
	screenQuad->Draw();

	//and swap
	ftest[1].SetTexture2D(GI::FBO::COLOUR_ATTACHMENT0, test[1]);

	test[1].Bind();
	screenQuad->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	gState.SetDepthTest(true);

	DrawScreenQuad(test[1].GetTextureId(), radialShader);
}

void Renderer::GaussianPass() {
	ftest[0].Bind();

	DefaultPass();
	//end 1st pass

	ftest[1].Bind();
	ftest[1].SetTexture2D(GI::FBO::COLOUR_ATTACHMENT0, test[2]);

	SetCurrentShader(processShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();

	gState.SendUniform2f("pixelSize", Vector2(1.0f / width, 1.0f / height), currentShader);

	for (int i = 0; i < 10; ++i) {
		//ping -> pong
		ftest[1].SetTexture2D(GI::FBO::COLOUR_ATTACHMENT0, test[2]);

		gState.SendUniform1i("isVertical", 0, currentShader);

		test[1].Bind(0);
		screenQuad->Draw();
		
		//and swap
		gState.SendUniform1i("isVertical", 1, currentShader);
		ftest[1].SetTexture2D(GI::FBO::COLOUR_ATTACHMENT0, test[1]);

		test[1].Bind();
		screenQuad->Draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	gState.SetDepthTest(true);

	//end ping pong stage, present the scene
	DrawScreenQuad(test[1].GetTextureId());
	DrawHUD();
}

void Renderer::DrawScreenQuad(GLuint& texture, Shader* shader) {
	gState.ClearBuffers(true, true);
	SetCurrentShader(shader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	screenQuad->Draw();
}

/*Temporary*/
void Renderer::DrawHUD() {
	SetCurrentShader(defaultShader);
	UpdateShaderMatrices();
	gState.SetDepthTest(false);
	gState.SetBlendFunction(GI::BlendFunc::SRC_ALPHA, GI::BlendFunc::ONE);

	std::stringstream ss;
	//ss << "Physics updates: " << (PhysicsSystem::numUpdates / Window::GetWindow().GetTimer()->GetMS()) * 1000.0f;
	ss << "Physics rate cur: " << PhysicsSystem::numUpdatesPerSec << " avg: " << (PhysicsSystem::numUpdatesTotal / Window::GetWindow().GetTimer()->GetMS()) * 1000.0f;
	DrawText(ss.str(), Vector3(0, 0, 0), 16.0f);
	ss.clear(); ss.str(std::string());

	ss << "Renderer updates: " << (renderer_updates / Window::GetWindow().GetTimer()->GetMS())* 1000.0f;
	DrawText(ss.str(), Vector3(0, 16.0f, 0), 16.0f);
	ss.clear(); ss.str(std::string());

	ss << "Elapsed time: " << (int)(Window::GetWindow().GetTimer()->GetMS() * 0.001f);
	DrawText(ss.str(), Vector3(0, 32.0f, 0), 16.0f);
	ss.clear(); ss.str(std::string());

	ss << "Physics nodes: " << PhysicsSystem::numNodes;
	DrawText(ss.str(), Vector3(0, 48.0f, 0), 16.0f);
	ss.clear(); ss.str(std::string());

	ss << "Collisions detected: " << PhysicsSystem::numCollisions;
	DrawText(ss.str(), Vector3(0, 80.0f, 0), 16.0f);
	ss.clear(); ss.str(std::string());

	Vector3 pos = camera->GetPosition();
	ss << "Camera position: <" << std::setprecision(1) << (int)pos.x << "," << (int)pos.y << "," << (int)pos.z << ">";
	DrawText(ss.str(), Vector3(0, 96.0f, 0), 16.0f);
	ss.clear(); ss.str(std::string());

	gState.SetBlendFunction(GI::BlendFunc::SRC_ALPHA, GI::BlendFunc::ONE_MINUS_SRC_ALPHA);
	gState.SetDepthTest(true);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	viewMatrix = camera->BuildViewMatrix();
}

void Renderer::SetRenderMode(RenderMode rm) {
	switch (rm) {
	case RenderMode::RENDER_DEFAULT:
		instance->RenderFunc = &Renderer::DefaultPass;
		break;
	case RenderMode::RENDER_NORMALS:
		instance->RenderFunc = &Renderer::NormalPass;
		break;
	case RenderMode::RENDER_GAUSSIAN_BLUR:
		instance->RenderFunc = &Renderer::GaussianPass;
		break;
	case RenderMode::RENDER_RADIAL_BLUR:
		instance->RenderFunc = &Renderer::RadialPass;
		break;
	}
}