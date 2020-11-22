#pragma once

#include "v3.h"
#include "M33.h"
#include "framebuffer.h"

class FrameBuffer;

class PPC {
public:
	V3 a, b, c, C;
	int w, h;
	PPC(float hfov, int _w, int _h);
	int Project(V3 P, V3 &p);
	V3 UnProject(V3 p);
	void TranslateRightLeft(float tstep);
	void TranslateFrontBack(float tstep);
	void PanLeftRight(float rstep);
	void TiltUpDown(float rstep);
	void SetPose(V3 newC, V3 lookAtPoint, V3 upGuidance);
	void Interpolate(PPC *ppc0, PPC *ppc1, int i, int n);
	void Visualize(FrameBuffer *vfb, PPC *vppc, float vf);
	void Visualize(FrameBuffer *vfb, PPC *vppc, FrameBuffer *fb);
	void Visualize(FrameBuffer *vfb, PPC *vppc, float vf, FrameBuffer *fb);
	void SetInterpolated(PPC *ppc0, PPC *ppc1, int stepsN, int stepi);
	float GetF();
	void ScaleFocalLength(float flscf);
	M33 CameraMatrix();
	void SetIntrinsicsHW();
	void SetExtrinsicsHW();
};