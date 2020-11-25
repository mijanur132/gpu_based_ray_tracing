#include "stdafx.h"

//#define GEOM_SHADER

#include "CGInterface.h"
#include "v3.h"
#include "scene.h"

#include <iostream>

using namespace std;

CGInterface::CGInterface() {};

void CGInterface::PerSessionInit() {

  glEnable(GL_DEPTH_TEST);

  CGprofile latestVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
#ifdef GEOM_SHADER
  CGprofile latestGeometryProfile = cgGLGetLatestProfile(CG_GL_GEOMETRY);
#endif
  CGprofile latestPixelProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

#ifdef GEOM_SHADER
  if (latestGeometryProfile == CG_PROFILE_UNKNOWN) {
	  cerr << "ERROR: geometry profile is not available" << endl;
    exit(0);
  }

  cgGLSetOptimalOptions(latestGeometryProfile);
  CGerror Error = cgGetError();
  if (Error) {
	  cerr << "CG ERROR: " << cgGetErrorString(Error) << endl;
  }

  cout << "Info: Latest GP Profile Supported: " << cgGetProfileString(latestGeometryProfile) << endl;

  geometryCGprofile = latestGeometryProfile;
#endif

  cout << "Info: Latest VP Profile Supported: " << cgGetProfileString(latestVertexProfile) << endl;
  cout << "Info: Latest FP Profile Supported: " << cgGetProfileString(latestPixelProfile) << endl;

  vertexCGprofile = latestVertexProfile;
  pixelCGprofile = latestPixelProfile;
  cgContext = cgCreateContext();


}

bool ShaderOneInterface::PerSessionInit(CGInterface *cgi) {

#ifdef GEOM_SHADER
  geometryProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE, 
    "CG/shaderOne.cg", cgi->geometryCGprofile, "GeometryMain", NULL);
  if (geometryProgram == NULL)  {
    CGerror Error = cgGetError();
    cerr << "Shader One Geometry Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
    cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
    return false;
  }
#endif

  vertexProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE, 
    "CG/shaderOne.cg", cgi->vertexCGprofile, "VertexMain", NULL);
  if (vertexProgram == NULL) {
    CGerror Error = cgGetError();
    cerr << "Shader One Vertex Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
    cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
    return false;
  }

  fragmentProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE, 
    "CG/shaderOne.cg", cgi->pixelCGprofile, "FragmentMain", NULL);
  if (fragmentProgram == NULL)  {
    CGerror Error = cgGetError();
    cerr << "Shader One Fragment Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
    cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
    return false;
  }

	// load programs
#ifdef GEOM_SHADER
	cgGLLoadProgram(geometryProgram);
#endif
	cgGLLoadProgram(vertexProgram);
	cgGLLoadProgram(fragmentProgram);

	// build some parameters by name such that we can set them later...
  vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj" );

  fragmentppcC = cgGetNamedParameter(fragmentProgram, "ppcC");
  fragmentbillv0 = cgGetNamedParameter(fragmentProgram, "billv0");
  fragmentbillv1 = cgGetNamedParameter(fragmentProgram, "billv1");
  fragmentbillv2 = cgGetNamedParameter(fragmentProgram, "billv2");
  fragmentbillv3 = cgGetNamedParameter(fragmentProgram, "billv3");
  fragmentenvReflec = cgGetNamedParameter(fragmentProgram, "envReflec");


  return true;

}

void ShaderOneInterface::PerFrameInit(float envReflecval) {

	//set parameters
	cgGLSetStateMatrixParameter(vertexModelViewProj, 
		CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

  cgGLSetStateMatrixParameter(
    geometryModelViewProj, 
	  CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);


  cgSetParameter3fv(fragmentppcC, (float*)&(scene->ppc->C));
  V3 bv0 = scene->tmeshes[5].GetCenter() + V3(-10, 10, 0); 
  V3 bv1 = scene->tmeshes[5].GetCenter() + V3(-10, -10, 0);
  V3 bv2 = scene->tmeshes[5].GetCenter() + V3(10, 10, 0);
  V3 bv3 = scene->tmeshes[5].GetCenter() + V3(10, -10, 0);
 // cout<<"center,billvo,billv1"<< scene->tmeshes[5].GetCenter() <<bv0<< bv1<<bv2<<bv3<<endl;
  cgSetParameter3fv(fragmentbillv0, (float*) & bv0);
  cgSetParameter3fv(fragmentbillv1, (float*) & bv1);
  cgSetParameter3fv(fragmentbillv2, (float*) &bv2);
  cgSetParameter3fv(fragmentbillv3, (float*) &bv3);
  cout << "envRflect to shader:" << envReflecval << endl;
  cgSetParameter3fv(fragmentenvReflec, (float*)&envReflecval);

}

void ShaderOneInterface::PerFrameDisable() {
}


void ShaderOneInterface::BindPrograms() {

#ifdef GEOM_SHADER
  cgGLBindProgram(geometryProgram);
#endif
  cgGLBindProgram(vertexProgram);
  cgGLBindProgram(fragmentProgram);

}

void CGInterface::DisableProfiles() {

  cgGLDisableProfile(vertexCGprofile);
#ifdef GEOM_SHADER
  cgGLDisableProfile(geometryCGprofile);
#endif
  cgGLDisableProfile(pixelCGprofile);

}

void CGInterface::EnableProfiles() {

  cgGLEnableProfile(vertexCGprofile);
#ifdef GEOM_SHADER
  cgGLEnableProfile(geometryCGprofile);
#endif
  cgGLEnableProfile(pixelCGprofile);

}

