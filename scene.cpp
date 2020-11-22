#include "stdafx.h"

#include "scene.h"

#include "V3.h"
#include "M33.h"
#include "ppc.h"
#include "TMesh.h"
#include "AABB.h"



Scene *scene;

using namespace std;

#include <iostream>

Scene::Scene() {

	cgi = 0;
	soi = 0;

	gui = new GUI();
	gui->show();

	int u0 = 20;
	int v0 = 100;
	int h = 300;
	int w = 500;

	fb = new FrameBuffer(u0, v0, w, h, 0);
	fb->label("SW 1");
	fb->show();
	fb->redraw();

	fb3 = new FrameBuffer(u0+w+30, v0, w, h, 0);
	fb3->label("SW 3");
//	fb3->show();
	fb3->redraw();


	gui->uiw->position(u0, v0 + h + 50);

	float hfov = 55.0f;
	ppc = new PPC(hfov, fb->w, fb->h);
	ppc3 = new PPC(hfov, fb3->w, fb3->h);

	tmeshesN = 10;
	tmeshes = new TMesh[tmeshesN];

	V3 cc(0.0f, 0.0f, -100.0f);
	float sideLength = 60.0f;
	tmeshes[0].SetToCube(cc, sideLength, 0xFF0000FF, 0xFF000000);
	tmeshes[0].onFlag = 0;

	tmeshes[1].LoadBin("geometry/teapot1K.bin");
//	tmeshes[1].LoadBin("geometry/teapot57K.bin");
	tmeshes[1].SetCenter(V3(0.0f, 0.0f, -140.0f));
	tmeshes[1].onFlag = 0;
	tmeshes[1].reflectorFlag = 1;

	V3 qverts[4];
	qverts[0] = V3(-20.0f, 5.0f, -20.0f);
	qverts[1] = V3(100.0f, 5.0f, -200.0f);
	qverts[2] = V3(100.0f, -5.0f, -200.0f);
	qverts[3] = V3(-20.0f, -5.0f, -20.0f);
	V3 qcolors[4] = { V3(1.0f, 0.0f, 0.0f), V3(0.0f, 0.0f, 0.0f), V3(0.0f, 0.0f, 0.0f),
		V3(1.0f, 0.0f, 0.0f) };
	tmeshes[2].SetQuad(qverts, qcolors);
	tmeshes[2].Translate(V3(0.0f, 0.0f, -20.0f));
	tmeshes[2].Translate(V3(0.0f, 6.0f, 0.0f));
	tmeshes[2].onFlag = 0;

	tmeshes[3].SetQuad(qverts, qcolors);
	tmeshes[3].Translate(V3(0.0f, 0.0f, -20.0f));
	tmeshes[3].Translate(V3(0.0f, -6.0f, 0.0f));
	tmeshes[3].msiFlag = 0;

	tmeshes[4].SetQuad(qverts, qcolors);
	tmeshes[4].Translate(V3(0.0f, 0.0f, -22.0f));
	tmeshes[4].colors[0] = tmeshes[4].colors[3] = tmeshes[4].colors[1] = 
		tmeshes[4].colors[2] = V3(0.0f, 1.0f, 0.0f);
	tmeshes[4].verts[0] = (tmeshes[4].verts[0] + tmeshes[4].verts[1]) / 2.0f;
	tmeshes[4].verts[3] = (tmeshes[4].verts[2] + tmeshes[4].verts[3]) / 2.0f;

	
	AABB aabb;
	tmeshes[1].SetAABB(aabb);
	aabb.corners[0][1] += 3.0f;

	// CONTINUE HERE
	qverts[0] = V3(aabb.corners[0][0], aabb.corners[0][1], aabb.corners[0][2]);
	qverts[1] = V3(aabb.corners[0][0], aabb.corners[0][1], aabb.corners[1][2]);
	qverts[2] = V3(aabb.corners[1][0], aabb.corners[0][1], aabb.corners[1][2]);
	qverts[3] = V3(aabb.corners[1][0], aabb.corners[0][1], aabb.corners[0][2]);
//	qcolors[0] = qcolors[1] = qcolors[2] = qcolors[3] = V3(1.0f, 0.0f, 0.0f);
	tmeshes[6].SetQuad(qverts, qcolors);
	tmeshes[6].texture = new FrameBuffer(100, 100, 64, 64, 0);
	tmeshes[6].texture->SetChecker(0xFFFFFFFF, 0xFF000000, 8);
	tmeshes[6].Scale(2.0f);

//	ppc->SetPose(ppc->C + V3(0.0f, 50.0f, 0.0f), tmeshes[1].GetCenter(), V3(0.0f, 1.0f, 0.0f));
	ppc3->SetPose(V3(0.0f, 40.0f, 50.0f), tmeshes[2].GetCenter(), V3(0.0f, 1.0f, 0.0f));

	tmeshes[3].onFlag = 0;
	tmeshes[4].onFlag = 0;
	
	tmeshes[6].onFlag = 0;
	tmeshes[7].onFlag = 0;
	tmeshes[8].onFlag = 0;

	float qz = -50.0f;
	float qs = 10.0f;
	qverts[0] = V3(-qs, qs, qz);
	qverts[1] = V3(-qs, -qs, qz);
	qverts[2] = V3(qs, -qs, qz);
	qverts[3] = V3(qs, qs, qz);
	qcolors[0] = V3(0.0f, 0.0f, 1.0f);
	qcolors[1] = V3(0.0f, 0.0f, 1.0f);
	qcolors[2] = V3(0.0f, 0.0f, 1.0f);
	qcolors[3] = V3(0.0f, 0.0f, 1.0f);
	//int texw = 1024;
	//FrameBuffer *texture = new FrameBuffer(30, 30, texw, texw, 0);
//	texture->show();
	//texture->label("Texture");
	//texture->SetChecker(0xFF000000, 0xFFFFFFFF, 16);
	//tmeshes[5].texture = texture;
	tmeshes[5].SetQuad(qverts, qcolors);
	//	tmeshes[5].Translate(V3(0.0f, 0.0f, -400.0f));

	tmeshes[5].onFlag = 1;
	tmeshes[9].onFlag = 1;
	qs = 20.0f;
	qz = -100.0f;
	qverts[0] = V3(-qs*2.0f, qs, qz);
	qverts[1] = V3(-qs*2.0f, -qs, qz);
	qverts[2] = V3(qs*2.0f, -qs, qz);
	qverts[3] = V3(qs*2.0f, qs, qz);
	qcolors[0] = V3(0.0f, 0.0f, 0.0f);
	qcolors[1] = V3(0.0f, 1.0f, 0.0f);
	qcolors[2] = V3(1.0f, 1.0f, 0.0f);
	qcolors[3] = V3(1.0f, 0.0f, 0.0f);
	tmeshes[9].SetQuad(qverts, qcolors);

	vf = 20.0f;
	L = V3(ppc->C);
	ka = 0.2f;

	Render();

	hwfb = new FrameBuffer(u0 + w + 30, v0, w, h, 0);
	hwfb->label("HW fb");
	hwfb->isHW = 1;
	hwfb->show();
	hwfb->redraw();

	gpufb = new FrameBuffer(u0 + w + 30, v0 + h + 50, w, h, 0);
	gpufb->label("GPU Framebuffer");
	gpufb->isHW = 2;
	gpufb->show();
	gpufb->redraw();

}

void Scene::Render() {

	Render(fb, ppc);
}

void Scene::Render(FrameBuffer* rfb, PPC* rppc) {

	rfb->SetBGR(0xFFFFFFFF);
	rfb->ClearZB();
	/*
	for (int tmi = 0; tmi < tmeshesN; tmi++) {
		if (!tmeshes[tmi].onFlag)
			continue;
		//		tmeshes[tmi].DrawWireFrame(rfb, rppc, 0xFF000000);
		V3 C(1.0f, 0.0f, 0.0f);
		//		tmeshes[tmi].Light(C, L, ka);
		tmeshes[tmi].RenderFilled(rfb, rppc, C, L, ka);
		//		tmeshes[tmi].RayTrace(rfb, rppc);
	}
	cout << "calling redraw()" << endl;
	*/
	rfb->redraw();

}

void Scene::RenderHW() {

	// initializations (could be done once per session)
	glEnable(GL_DEPTH_TEST);

	// clear buffers
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// set the view desired by the application (the user)
	ppc->SetIntrinsicsHW();
	ppc->SetExtrinsicsHW();

	// draw the actual geometry
	for (int tmi = 0; tmi < tmeshesN; tmi++) {
		if (!tmeshes[tmi].onFlag)
			continue;
		tmeshes[tmi].RenderHW();
	}

}

void Scene::RenderGPU() {

	// if the first time, call per session initialization
	if (cgi == NULL) {
		cgi = new CGInterface();
		cgi->PerSessionInit();
		soi = new ShaderOneInterface();
		soi->PerSessionInit(cgi);
	}

	// clear the framebuffer
	glClearColor(0.0, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set intrinsics
	ppc->SetIntrinsicsHW();
	// set extrinsics
	ppc->SetExtrinsicsHW();

	// per frame initialization
	cgi->EnableProfiles();
	soi->PerFrameInit();
	soi->BindPrograms();

	// render geometry
	for (int tmi = 0; tmi < tmeshesN; tmi++) {
		if (!tmeshes[tmi].onFlag)
			continue;
		tmeshes[tmi].RenderHW();
	}

	soi->PerFrameDisable();
	cgi->DisableProfiles();
	   
}





void Scene::DBG() {


	{
		int fN = 1000;
		for (int i = 0; i < fN; i++) {
			scene->p1 = (float)i / (float) (fN-1);
			scene->morphFraction = scene->p1;
			gpufb->redraw();
			Fl::check();
		}
		return;

	}



	{
		int fN = 1000;
		PPC ppc0(*ppc);
		PPC ppc1(*ppc);
		ppc1.C = ppc1.C + V3(20.0f, 30.0f, -50.0f);
		ppc1.SetPose(ppc1.C, tmeshes[1].GetCenter(), V3(0.0f, 1.0f, 0.0f));
		for (int fi = 0; fi < fN; fi++) {
			ppc->SetInterpolated(&ppc0, &ppc1, fN, fi);
			Render(fb, ppc);
			fb->redraw();
			hwfb->redraw();
			gpufb->redraw();
			Fl::check();
		}
		*ppc = ppc0;
		return;

	}


	{

		RenderRayTracing(fb, ppc);
		fb->redraw();
		Fl::check();
		return;

	}

	{

		int fN = 10000;
		for (int fi = 0; fi < fN; fi++) {
			Render();
			Fl::check();
			tmeshes[5].Translate(V3(0.0f, 0.0f, -1.0f));
			if ((fi % 100) == 0)
				cerr << "INFO: " << (float)((int)((float)fi / (float)fN * 10000.0f)) / 100.0f << "%        \r";
		}
		return;

	}

	{
		fb3->SaveAsTiff("mydbg/ssivis.tif");
		return;

	}

	{

		V3 tcenter = tmeshes[1].GetCenter();
		V3 newC = V3(20.0f, 50.0f, -30.0f);
		ppc->SetPose(newC, tcenter, V3(0.0f, 1.0f, 0.0f));

		int fN = 1000;
		V3 L0 = L;
		V3 L1 = tmeshes[1].GetCenter();
		L = L0 + (L1 - L0)*0.75f;
		for (int fi = 0; fi < fN; fi++) {
//			L = L0 + (L1 - L0)*(float)fi / (float)(fN - 1);
			L = L.RotatePoint(L1, V3(0.0f, 1.0f, 0.0f), 360.0f*3.0f / (float)fN);
			Render();
			Fl::check();
		}
		L = L0;
		return;
	}

	{

		V3 tcenter = tmeshes[1].GetCenter();
		V3 newC = V3(20.0f, 50.0f, -30.0f);
		ppc->SetPose(newC, tcenter, V3(0.0f, 1.0f, 0.0f));
		ppc3->SetPose(V3(0.0f, 50.0f, 100.0f), tcenter, V3(0.0f, 1.0f, 0.0f));
		for (int i = 0; i < 200; i++) {
			Render();
			vf += 1.0f;
			Fl::check();
			return;
		}
		return;

		V3 aDir(0.0f, 1.0f, 0.0f);
		for (int i = 0; i < 100; i++) {
			Render();
			Fl::check();
//			tmeshes[1].Rotate(tcenter, aDir, 1.0f);
			ppc->PanLeftRight(1.0f);
		}
		return;

	}

	{
		fb->SaveAsTiff("mydbg/zb.tif");
		return;
	}

	{

		tmeshes[0].onFlag = 0;
		int fN = 300;
		float tstep = .1f;
		for (int fi = 0; fi < fN; fi++) {
			Render();
			Fl::check();
			ppc->TranslateRightLeft(-tstep);
		}
		return;
	}


	{
		int w = fb->w;
		int h = fb->h;
		float hfov = 90.0f;
		PPC ppc(hfov, w, h);
		V3 cc(0.0f, 0.0f, -100.0f);
		unsigned int color = 0xFF000000;
		float sideLength = 60.0f;
		TMesh tm;
		tm.SetToCube(cc, sideLength, 0xFF0000FF, 0xFF000000);
		int fN = 300;
		float tstep = .1f;
		for (int fi = 0; fi < fN; fi++) {
			fb->SetBGR(0xFFFFFFFF);
//			tm.DrawCubeQuadFaces(fb, &ppc, color);
			tm.DrawWireFrame(fb, &ppc, color);
			fb->redraw();
			Fl::check();
			ppc.TranslateRightLeft(-tstep);
//			ppc.TranslateFrontBack(tstep);
		}
		return;
	}



	{
		int w = fb->w;
		int h = fb->h;
		float hfov = 90.0f;
		PPC ppc(hfov, w, h);
		V3 P(0.0f, 0.0f, -100.0f);

		V3 uP, p;
		ppc.Project(P, p);
		uP = ppc.UnProject(p);
		cerr << uP;

		fb->SetBGR(0xFFF0000);
		V3 tr((float)w, 0.0f, 1.0f);
		V3 trP = ppc.UnProject(tr);
		V3 ptr;
		ppc.Project(trP, ptr);
		fb->DrawSquarePoint(ptr[0], ptr[1], 13, 0xFF00FF00);
		fb->redraw();
		return;

		return;

		V3 Q(0.0f, -10.0f, -50.0f);
		V3 q;

		fb->SetBGR(0xFFFFFFFF);
		for (int i = 0; i < 10; i++) {
			if (!ppc.Project(P, p))
				continue;
			fb->DrawSquarePoint(p[0], p[1], 5, 0xFF000000);

			if (!ppc.Project(Q, q))
				continue;
			fb->DrawSquarePoint(q[0], q[1], 5, 0xFF0000FF);

			fb->redraw();
			Fl::check();
			P = P + V3(10.0f, 0.0f, 0.0f);
			Q = Q + V3(10.0f, 0.0f, 0.0f);
		}


		if (ppc.Project(P, p)) {
			cerr << p << endl;
		}
		else {
			cerr << "INFO: point is behind the head" << endl;
		}

		return;

	}

	{

		M33 m;
		V3 r0(1.0f, 1.0f, 1.0f);
		V3 r1(-2.0f, 2.0f, 2.0f);
		V3 r2(3.0f, -3.0f, 3.0f);
		m[0] = r0;
		m[1] = r1;
		m[2] = r2;
		V3 v(1.0f, 2.0f, 3.0f);
		V3 ret = m*v;
		cerr << ret;
		M33 m1 = m.Inverted();
		cerr << m*m1.GetColumn(0) << m*m1.GetColumn(1) << m*m1.GetColumn(2);
		return;
	}


	{
		M33 m;
		V3 v0(1.0f, 3.0f, -1.0f);
		m[0] = v0;
		cerr << m[0] << endl;
		cerr << m[0][2] << endl;
		m[0][2] = 1000.0f;
		cerr << m[0][2] << endl;
		return;
	}

	{

		V3 v0(2.0f, 2.0f, 2.0f);
		V3 v1(4.0f, 3.0f, 5.0f);
		cerr << v0 + v1;
		cerr << "v0*v1 " << v0*v1 << endl;
		cerr << v0.Length() << endl;
		cerr << (v0.Normalized()).Length() << endl;
		cerr << v0;
		return;

	}

	{
		V3 v;
		v.xyz[0] = 1.0f;
		v.xyz[1] = -1.0f;
		v.xyz[2] = 0.0f;
		cerr << v[0] << endl;
		v[0] = 100.0f;
		cerr << v[0] << endl;
		return;

	}

	fb->LoadTiff("mydbg/im.tif");
	fb->redraw();
	return;
	cerr << "INFO: pressed DBG Button" << endl;

	{
		float uv0[2] = { 10.1f, 20.2f };
		float uv1[2] = { 510.1f, 420.2f };
		unsigned int col = 0xFF000000;
		int fN = 300;
		for (int fi = 0; fi < fN; fi++) {
			fb->SetBGR(0xFFFFFFFF);
//			fb->Draw2DSegment(uv0, uv1, cv, cv);
			uv0[1] += 1.0f;
			uv1[1] -= 1.0f;
			fb->redraw();
			Fl::check();
		}
		fb->SaveAsTiff("mydbg/im.tif");
	}

	return;

	{
		fb->SetBGR(0xFF0000FF);
		fb->SetChecker(0xFF000000, 0xFFFFFFFF, 40);
		fb->SetBGR(0xFFFFFFFF);
		float uv0[2] = { 20.3f, 300.45f };
		float uv1[2] = { 420.73f, 100.45f };
		unsigned int col = 0xFF000000;
//		fb->Draw2DSegment(uv0, uv1, col);
	}

}


void Scene::NewButton() {
	cerr << "INFO: pressed New Button" << endl;
}


void Scene::RenderRayTracing(FrameBuffer *rfb, PPC *rppc) {

	rfb->SetBGR(0xFFFFFFFF);
	rfb->ClearZB();
	for (int v = 0; v < rppc->h; v++) {
		for (int q = 0; q < rppc->w; q++)
			rfb->Set(q, v, 0xFF0000FF);
		rfb->redraw();
		Fl::check();
		for (int q = 0; q < rppc->w; q++)
			rfb->Set(q, v, 0xFFFFFFFF);
		for (int u = 0; u < rppc->w; u++) {
			V3 rdir = rppc->UnProject(V3(.5f + (float)u, .5f + (float)v, 1.0f)) - rppc->C;
			rdir = rdir.Normalized();
			V3 rO = rppc->C;
			V3 rc;
			float currz;
			if (!RayTrace(rO, rdir, 0, rc, currz))
				continue;
			if (rfb->Farther(u, v, currz))
				continue;
			rfb->Set(u, v, rc.GetColor());
		}
		rfb->redraw();
		Fl::check();
	}
	rfb->redraw();

}

int Scene::RayTrace(V3 rO, V3 rdir, int rayOrder, V3& rc, float &currz) {

	currz = 0.0f;
	V3 rrO, rrdir;
	for (int tmi = 0; tmi < tmeshesN; tmi++) {
		if (!tmeshes[tmi].onFlag)
			continue;
		float tmpz;
		V3 tmpc;
		V3 tmprrO, tmprrdir;
		if (tmeshes[tmi].RayTrace(rO, rdir, tmpc, tmpz, tmprrO, tmprrdir)) {
			if (currz < tmpz) {
				currz = tmpz;
				rc = tmpc;
				rrO = tmprrO;
				rrdir = tmprrdir;
			}
		}
	}

	if (currz == 0.0f)
		return 0;

	if (rayOrder == 0 && rrO[0] != FLT_MAX) {
		V3 rrc;
		float rrz;
		if (RayTrace(rrO, rrdir, 1, rrc, rrz)) {
			rc = rc*0.5f + rrc*0.5f;
		}
	}

	return 1;

}

