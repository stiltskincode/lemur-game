#include "CameraFPP.h"

CameraFPP::CameraFPP()
{
	sumPitch=0;
}
CameraFPP::~CameraFPP()
{
	sumPitch=0;
}

void CameraFPP::pitch(float angle)
{
	if(!angle || (angle+sumPitch)>85 || (angle+sumPitch)<-85) return;
	sumPitch+=angle;
	front.rotate(angle,right); front.normalize();
}
