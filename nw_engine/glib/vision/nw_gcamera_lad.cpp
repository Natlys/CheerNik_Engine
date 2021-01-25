#include <nw_pch.hpp>
#include "nw_gcamera_lad.h"

#include <sys/nw_time_sys.h>
#include <sys/nw_io_sys.h>

namespace NW
{
	GCameraLad::GCameraLad() :
		nZoomSpeed(1000.0f),
		nMoveSpeed(2.0f),
		nRtnSpeed(200.0f),
		whBounds{ 800.0f, 600.0f } { }
	GCameraLad::~GCameraLad() = default;

	// --==<core_methods>==--
	void GCameraLad::UpdateCamera(GCamera* pGCamera)
	{
		float MoveSpeed = this->nMoveSpeed * TimeSys::GetRealDelta();
		if (IOSys::GetMouseIMode() != IM_CURSOR_DISABLED) return;
		pGCamera->nAspectRatio = whBounds.x / whBounds.y;
		if (pGCamera->GetMode() == GCamera::GCM_2D)
		{
			if (IOSys::GetKeyHeld(NW_KEY_W_87))
				pGCamera->yCrd += MoveSpeed;
			if (IOSys::GetKeyHeld(NW_KEY_S_83))
				pGCamera->yCrd -= MoveSpeed;
			if (IOSys::GetKeyHeld(NW_KEY_D_68))
				pGCamera->xCrd += MoveSpeed;
			if (IOSys::GetKeyHeld(NW_KEY_A_65))
				pGCamera->xCrd -= MoveSpeed;
		}
		else if (pGCamera->GetMode() == GCamera::GCM_3D) {
			MoveSpeed = -MoveSpeed;
			if (IOSys::GetKeyHeld(NW_KEY_W_87)) {	// Move Forward
				pGCamera->xCrd += pGCamera->dirFront.x * MoveSpeed;
				//pGCamera->yCrd += pGCamera->dirFront.y * MoveSpeed;
				pGCamera->zCrd += pGCamera->dirFront.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_S_83)) {	// Move Back
				pGCamera->xCrd -= pGCamera->dirFront.x * MoveSpeed;
				//pGCamera->yCrd -= pGCamera->dirFront.y * MoveSpeed;
				pGCamera->zCrd -= pGCamera->dirFront.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_D_68)) {	// Move Right
				pGCamera->xCrd += pGCamera->dirRight.x * MoveSpeed;
				pGCamera->yCrd += pGCamera->dirRight.y * MoveSpeed;
				pGCamera->zCrd += pGCamera->dirRight.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_A_65)) {	// Move Left
				pGCamera->xCrd -= pGCamera->dirRight.x * MoveSpeed;
				pGCamera->yCrd -= pGCamera->dirRight.y * MoveSpeed;
				pGCamera->zCrd -= pGCamera->dirRight.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_SPACE_32)) {		// Move Up
				pGCamera->yCrd -= MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_SHIFT_LEFT)) {	// Move Down
				pGCamera->yCrd += MoveSpeed;
			}
		}
	}
	// --==</core_methods>==--

	// --==<--on_event_methods>==--
	void GCameraLad::OnEvent(MouseEvent& rmEvt, GCamera* pGCamera)
	{
		switch (rmEvt.EvtType) {
		case ET_MOUSE_MOVE:
			if (IOSys::GetMouseIMode() != IM_CURSOR_DISABLED) return;
			if (pGCamera->GetMode() == GCamera::GCM_2D) {
				if (IOSys::GetMouseHeld(MB_BUTTON_2)) {
					pGCamera->xyzCrd.x += -IOSys::s_Mouse.xMoveDelta * TimeSys::GetRealDelta() * nMoveSpeed;
					pGCamera->xyzCrd.y += IOSys::s_Mouse.yMoveDelta * TimeSys::GetRealDelta() * nMoveSpeed;
				}
				float nRoll_deg = pGCamera->nRoll + IOSys::s_Mouse.xMoveDelta * nRtnSpeed * TimeSys::GetRealDelta();
				if (nRoll_deg < -nMaxRoll)
					pGCamera->nRoll = nMaxRoll;
				else if (nRoll_deg > nMaxRoll)
					pGCamera->nRoll = -nMaxRoll;
				else
					pGCamera->nRoll = nRoll_deg;
			}
			else if (pGCamera->GetMode() == GCamera::GCM_3D) {
				float nYaw_deg = pGCamera->nYaw - IOSys::s_Mouse.xMoveDelta * nRtnSpeed * TimeSys::GetRealDelta();
				float nPitch_deg = pGCamera->nPitch - IOSys::s_Mouse.yMoveDelta * nRtnSpeed * TimeSys::GetRealDelta();

				if (nYaw_deg < -nMaxYaw)
					pGCamera->nYaw = nMaxYaw;
				else if (nYaw_deg > nMaxYaw)
					pGCamera->nYaw = -nMaxYaw;
				else
					pGCamera->nYaw = nYaw_deg;

				if (nPitch_deg > nMaxPitch)
					pGCamera->nPitch = nMaxPitch;
				else if (nPitch_deg < -nMaxPitch)
					pGCamera->nPitch = -nMaxPitch;
				else
					pGCamera->nPitch = nPitch_deg;
			}
			break;
		case ET_MOUSE_SCROLL:
			if (IOSys::GetMouseIMode() != IM_CURSOR_DISABLED) return;
			float nZoom = -rmEvt.nY * TimeSys::GetRealDelta() * nZoomSpeed;
			if (pGCamera->GetType() == GCamera::GCT_ORTHO) {
				float nScale = pGCamera->nViewScale + nZoom * pGCamera->nViewScale / 40.0f + 0.01f;
				if (nScale > 0.0f) pGCamera->nViewScale = nScale;
			}
			else if (pGCamera->GetType() == GCamera::GCT_PERSPECT) {
				float nViewField = pGCamera->nViewField + nZoom;
				if (nViewField >= 0.01f && nViewField <= 180.0f) pGCamera->nViewField = nViewField;
			}
			break;
		}
	}
	void GCameraLad::OnEvent(KeyboardEvent& rkEvt, GCamera* pGCamera)
	{
		switch (rkEvt.EvtType) {
		case ET_KEY_PRESS:
			switch (rkEvt.unKeyCode) {
			case NW_KEY_0_48:
				if (IOSys::GetKeyHeld(NW_KEY_C_67)) {
					m_GCamera = GCamera();
				}
				break;
			case NW_KEY_E_69:
				pGCamera->nRoll += 10.0f;
				break;
			case NW_KEY_Q_81:
				pGCamera->nRoll -= 10.0f;
				break;
			}
			break;
		}
	}
	void GCameraLad::OnEvent(WindowEvent& rwEvt, GCamera* pGCamera)
	{
		switch (rwEvt.EvtType) {
		case ET_WINDOW_RESIZE:
			whBounds = V2f{ rwEvt.nX, rwEvt.nY };
			break;
		}
	}
	// --==</--on_event_methods>==--
}