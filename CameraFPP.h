#ifndef CAMERAFPP_H_
#define CAMERAFPP_H_

#include "Camera.h"

class CameraFPP : public Camera
{
	public:
		CameraFPP();
		virtual ~CameraFPP();
		
		void pitch(float);
	
	private:
		float sumPitch;
};

#endif /*CAMERAFPP_H_*/
