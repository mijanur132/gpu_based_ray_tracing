#pragma once

#include "gui.h"
#include "framebuffer.h"
#include "ppc.h"
#include "TMesh.h"
#include "CGInterface.h"


class Scene {
public:

	CGInterface * cgi;
	ShaderOneInterface *soi;

	GUI *gui;
	FrameBuffer *fb, *fb3, *hwfb, *gpufb;
	PPC *ppc, *ppc3;
	TMesh *tmeshes;
	int tmeshesN;
	Scene();
	void DBG();
	void NewButton();
	void Render();
	void Render(FrameBuffer *rfb, PPC *rppc); // projection followed by rasterization
	void RenderHW();
	void RenderGPU();
	void RenderRayTracing(FrameBuffer *rfb, PPC *rppc); // ray tracing
	int RayTrace(V3 rO, V3 rdir, int rayOrder, V3& rc, float &currz);
	float vf; // ppc visualization focal length
	V3 L; // point light source
	float ka; // ambient lighting coefficient
	float p1; // test parameter for fragment shader


	float morphFraction; // morphing parameters
};

extern Scene *scene;