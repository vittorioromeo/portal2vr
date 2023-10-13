#include "hooks.h"
#include "game.h"
#include "texture.h"
#include "sdk.h"
#include "sdk_server.h"
#include "vr.h"
#include "offsets.h"
#include <iostream>

Hooks::Hooks(Game *game)
{
	if (MH_Initialize() != MH_OK)
	{
		Game::errorMsg("Failed to init MinHook");
	}

	m_Game = game;
	m_VR = m_Game->m_VR;

	m_PushHUDStep = -999;
	m_PushedHud = true;

	initSourceHooks();

	//hkGetRenderTarget.enableHook();
	hkCalcViewModelView.enableHook();

	hkProcessUsercmds.enableHook();
	hkReadUsercmd.enableHook();

	//hkWriteUsercmdDeltaToBuffer.enableHook();
	hkWriteUsercmd.enableHook();

	hkCreateMove.enableHook();
	hkEyePosition.enableHook();
	hkRenderView.enableHook();


	hkWeapon_ShootPosition.enableHook();
	hkTraceFirePortal.enableHook();
	hkCWeaponPortalgun_FirePortal.enableHook();

	hkDrawSelf.enableHook();
	hkPlayerPortalled.enableHook();

	//hkComputeError.enableHook();
	hkUpdateObject.enableHook();
	hkUpdateObjectVM.enableHook();
	//hkRotateObject.enableHook();
	hkEyeAngles.enableHook();

	hkGetDefaultFOV.enableHook();
	hkGetFOV.enableHook();
	hkGetViewModelFOV.enableHook();

	hkSetDrawOnlyForSplitScreenUser.enableHook();
	//kClientThink.enableHook();
	hkPrecache.enableHook();
	hkCHudCrosshair_ShouldDraw.enableHook();
}

Hooks::~Hooks()
{
	if (MH_Uninitialize() != MH_OK)
	{
		Game::errorMsg("Failed to uninitialize MinHook");
	}
}


int Hooks::initSourceHooks()
{
	LPVOID pGetRenderTargetVFunc = (LPVOID)(m_Game->m_Offsets->GetRenderTarget.address);
	hkGetRenderTarget.createHook(pGetRenderTargetVFunc, &dGetRenderTarget);

	LPVOID pRenderViewVFunc = (LPVOID)(m_Game->m_Offsets->RenderView.address);
	hkRenderView.createHook(pRenderViewVFunc, &dRenderView);

	LPVOID calcViewModelViewAddr = (LPVOID)(m_Game->m_Offsets->CalcViewModelView.address);
	hkCalcViewModelView.createHook(calcViewModelViewAddr, &dCalcViewModelView);

	LPVOID ProcessUsercmdsAddr = (LPVOID)(m_Game->m_Offsets->ProcessUsercmds.address);
	hkProcessUsercmds.createHook(ProcessUsercmdsAddr, &dProcessUsercmds);

	LPVOID ReadUserCmdAddr = (LPVOID)(m_Game->m_Offsets->ReadUserCmd.address);
	hkReadUsercmd.createHook(ReadUserCmdAddr, &dReadUsercmd);

	/*LPVOID WriteUsercmdDeltaToBufferAddr = (LPVOID)(m_Game->m_Offsets->WriteUsercmdDeltaToBuffer.address);
	hkWriteUsercmdDeltaToBuffer.createHook(WriteUsercmdDeltaToBufferAddr, &dWriteUsercmdDeltaToBuffer);*/

	LPVOID WriteUsercmdAddr = (LPVOID)(m_Game->m_Offsets->WriteUsercmd.address);
	hkWriteUsercmd.createHook(WriteUsercmdAddr, &dWriteUsercmd);

	LPVOID AdjustEngineViewportAddr = (LPVOID)(m_Game->m_Offsets->AdjustEngineViewport.address);
	hkAdjustEngineViewport.createHook(AdjustEngineViewportAddr, &dAdjustEngineViewport);

	LPVOID ViewportAddr = (LPVOID)(m_Game->m_Offsets->Viewport.address);
	hkViewport.createHook(ViewportAddr, &dViewport);

	LPVOID GetViewportAddr = (LPVOID)(m_Game->m_Offsets->GetViewport.address);
	hkGetViewport.createHook(GetViewportAddr, &dGetViewport);

	LPVOID EyePositionAddr = (LPVOID)(m_Game->m_Offsets->EyePosition.address);
	hkEyePosition.createHook(EyePositionAddr, &dEyePosition);

	/*LPVOID DrawModelExecuteAddr = (LPVOID)(m_Game->m_Offsets->DrawModelExecute.address);
	hkDrawModelExecute.createHook(DrawModelExecuteAddr, &dDrawModelExecute);*/

	LPVOID PushRenderTargetAddr = (LPVOID)(m_Game->m_Offsets->PushRenderTargetAndViewport.address);
	hkPushRenderTargetAndViewport.createHook(PushRenderTargetAddr, &dPushRenderTargetAndViewport);

	LPVOID PopRenderTargetAddr = (LPVOID)(m_Game->m_Offsets->PopRenderTargetAndViewport.address);
	hkPopRenderTargetAndViewport.createHook(PopRenderTargetAddr, &dPopRenderTargetAndViewport);

	LPVOID VGui_PaintAddr = (LPVOID)(m_Game->m_Offsets->VGui_Paint.address);
	hkVgui_Paint.createHook(VGui_PaintAddr, &dVGui_Paint);

	LPVOID IsSplitScreenAddr = (LPVOID)(m_Game->m_Offsets->IsSplitScreen.address);
	hkIsSplitScreen.createHook(IsSplitScreenAddr, &dIsSplitScreen);

	LPVOID PrePushRenderTargetAddr = (LPVOID)(m_Game->m_Offsets->PrePushRenderTarget.address);
	hkPrePushRenderTarget.createHook(PrePushRenderTargetAddr, &dPrePushRenderTarget);

	LPVOID GetFullScreenTextureAddr = (LPVOID)(m_Game->m_Offsets->GetFullScreenTexture.address);
	hkGetFullScreenTexture.createHook(GetFullScreenTextureAddr, &dGetFullScreenTexture);

	LPVOID Weapon_ShootPositionAddr = (LPVOID)(m_Game->m_Offsets->Weapon_ShootPosition.address);
	hkWeapon_ShootPosition.createHook(Weapon_ShootPositionAddr, &dWeapon_ShootPosition);

	LPVOID TraceFirePortalAddr = (LPVOID)(m_Game->m_Offsets->TraceFirePortalServer.address);
	hkTraceFirePortal.createHook(TraceFirePortalAddr, &dTraceFirePortal);

	hkCWeaponPortalgun_FirePortal.createHook((LPVOID)m_Game->m_Offsets->CWeaponPortalgun_FirePortal.address, &dCWeaponPortalgun_FirePortal);

	LPVOID DrawSelfAddr = (LPVOID)(m_Game->m_Offsets->DrawSelf.address);
	hkDrawSelf.createHook(DrawSelfAddr, &dDrawSelf);

	LPVOID ClipTransformAddr = (LPVOID)(m_Game->m_Offsets->ClipTransform.address);
	hkClipTransform.createHook(ClipTransformAddr, &dClipTransform);

	// Portalling
	LPVOID PlayerPortalledAddr = (LPVOID)(m_Game->m_Offsets->PlayerPortalled.address);
	hkPlayerPortalled.createHook(PlayerPortalledAddr, &dPlayerPortalled);

	UTIL_Portal_FirstAlongRay = (tUTIL_Portal_FirstAlongRay)m_Game->m_Offsets->UTIL_Portal_FirstAlongRay.address;
	UTIL_IntersectRayWithPortal = (tUTIL_IntersectRayWithPortal)m_Game->m_Offsets->UTIL_IntersectRayWithPortal.address;
	UTIL_Portal_AngleTransform = (tUTIL_Portal_AngleTransform)m_Game->m_Offsets->UTIL_Portal_AngleTransform.address;

	LPVOID CreateMoveAddr = (LPVOID)(m_Game->m_Offsets->CreateMove.address);
	hkCreateMove.createHook(CreateMoveAddr, &dCreateMove);

	// Grababbles
	hkComputeError.createHook((LPVOID)(m_Game->m_Offsets->ComputeError.address), &dComputeError);
	hkUpdateObject.createHook((LPVOID)(m_Game->m_Offsets->UpdateObject.address), &dUpdateObject);
	hkUpdateObjectVM.createHook((LPVOID)(m_Game->m_Offsets->UpdateObjectVM.address), &dUpdateObjectVM);
	hkRotateObject.createHook((LPVOID)(m_Game->m_Offsets->RotateObject.address), &dRotateObject);
	hkEyeAngles.createHook((LPVOID)(m_Game->m_Offsets->EyeAngles.address), &dEyeAngles);

	hkMatrixBuildPerspectiveX.createHook((LPVOID)(m_Game->m_Offsets->MatrixBuildPerspectiveX.address), &dMatrixBuildPerspectiveX);

	// Portal Gun VFX
	hkGetDefaultFOV.createHook((LPVOID)(m_Game->m_Offsets->GetDefaultFOV.address), &dGetDefaultFOV);
	hkGetFOV.createHook((LPVOID)(m_Game->m_Offsets->GetFOV.address), &dGetFOV);
	hkGetViewModelFOV.createHook((LPVOID)(m_Game->m_Offsets->GetViewModelFOV.address), &dGetViewModelFOV);

	// Laser Pointer
	GetPortalPlayer = (tGetPortalPlayer)m_Game->m_Offsets->GetPortalPlayer.address;
	CreatePingPointer = (tCreatePingPointer)m_Game->m_Offsets->CreatePingPointer.address;
	PrecacheParticleSystem = (tPrecacheParticleSystem)m_Game->m_Offsets->PrecacheParticleSystem.address;
	hkPrecache.createHook((LPVOID)(m_Game->m_Offsets->Precache.address), &dPrecache);
	hkSetDrawOnlyForSplitScreenUser.createHook((LPVOID)m_Game->m_Offsets->SetDrawOnlyForSplitScreenUser.address, &dSetDrawOnlyForSplitScreenUser);
	hkCHudCrosshair_ShouldDraw.createHook((LPVOID)m_Game->m_Offsets->CHudCrosshair_ShouldDraw.address, &dCHudCrosshair_ShouldDraw);

	//
	EntityIndex = (tEntindex)m_Game->m_Offsets->CBaseEntity_entindex.address;
	GetOwner = (tGetOwner)m_Game->m_Offsets->GetOwner.address;

	return 1;
}

bool __fastcall Hooks::dCHudCrosshair_ShouldDraw(void* ecx, void* edx) {
	bool shouldDraw = hkCHudCrosshair_ShouldDraw.fOriginal(ecx);

	m_VR->m_DrawCrosshair = shouldDraw;

	return ((m_VR->m_AimMode == 1) ? shouldDraw : false);
}

void __fastcall Hooks::dPrecache(void* ecx, void* edx) {
	hkPrecache.fOriginal(ecx);
	PrecacheParticleSystem("robot_point_beam");
}

void __fastcall Hooks::dClientThink(void* ecx, void* edx) {
	//std::cout << "dSetDrawOnlyForSplitScreenUser: " << nSlot << "\n";

	/*int playerIndex = m_Game->m_EngineClient->GetLocalPlayer();
	C_BasePlayer* localPlayer = (C_BasePlayer*)m_Game->GetClientEntity(playerIndex);

	if (localPlayer) {
		uintptr_t* m_PointLaser = *(uintptr_t**)((uintptr_t)localPlayer + 0x23C0);

		if (!m_PointLaser) {
			std::cout << "Creating Point Laser Beam Sight Thingy" << "\n";
			m_Game->m_Hooks->CreatePingPointer(localPlayer, m_VR->m_AimPos);
		}
	}*/

	hkClientThink.fOriginal(ecx);
}

void __fastcall Hooks::dSetDrawOnlyForSplitScreenUser(void* ecx, void* edx, int nSlot) {
	//std::cout << "dSetDrawOnlyForSplitScreenUser: " << nSlot << "\n";
	hkSetDrawOnlyForSplitScreenUser.fOriginal(ecx, -1);
}

ITexture *__fastcall Hooks::dGetFullScreenTexture()
{
	ITexture *result = hkGetFullScreenTexture.fOriginal();
	return result;
}

ITexture* __fastcall Hooks::dGetRenderTarget(void* ecx, void* edx)
{
	ITexture* result = hkGetRenderTarget.fOriginal(ecx);
	return result;
}

void __fastcall Hooks::dRenderView(void *ecx, void *edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw)
{
	if (!m_VR->m_CreatedVRTextures) {
		m_VR->CreateVRTextures();
	}

	if (m_Game->m_VguiSurface->IsCursorVisible())
		return hkRenderView.fOriginal(ecx, setup, hudViewSetup, nClearFlags, whatToDraw);

	//VPanel* g_pFullscreenRootPanel = *(VPanel**)(m_Game->m_Offsets->g_pFullscreenRootPanel.address);

	IMaterialSystem* matSystem = m_Game->m_MaterialSystem;

	hudViewSetup.width = m_VR->m_RenderWidth;
	hudViewSetup.height = m_VR->m_RenderHeight;
	hudViewSetup.fov = m_VR->m_Fov;
	//hudViewSetup.fovViewmodel = m_VR->m_Fov;
	hudViewSetup.m_flAspectRatio = m_VR->m_Aspect;

	hudViewSetup.m_nUnscaledWidth = m_VR->m_RenderWidth;
	hudViewSetup.m_nUnscaledHeight = m_VR->m_RenderHeight;

	Vector position = setup.origin;

	if (m_VR->m_ApplyPortalRotationOffset) {
		const Vector vec = position - m_VR->m_SetupOrigin;
		const float distance = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

		// Rudimentary portalling detection
		if (distance > m_VR->m_PortallingDetectionDistanceThreshold) {
			m_VR->m_RotationOffset.y += m_VR->m_PortalRotationOffset.y;

			// If enabled, the camera pitch/roll follows the direction of the portal -- might be disorienting
			// for some people:
			if (m_VR->m_ApplyPitchAndRollPortalRotationOffset) {
				m_VR->m_RotationOffset.x += m_VR->m_PortalRotationOffset.x;
				m_VR->m_RotationOffset.z += m_VR->m_PortalRotationOffset.z;
			}

			m_VR->UpdateHMDAngles();

			m_VR->m_ApplyPortalRotationOffset = false;
		}
	}

	m_VR->m_SetupOrigin = position;

	Vector hmdAngle = m_VR->GetViewAngle();
	QAngle inGameAngle(hmdAngle.x, hmdAngle.y, hmdAngle.z);
	m_Game->m_EngineClient->SetViewAngles(inGameAngle);

	setup.x = 0;
	setup.y = 0;
	setup.width = m_VR->m_RenderWidth;
	setup.height = m_VR->m_RenderHeight;
	setup.m_nUnscaledWidth = m_VR->m_RenderWidth;
	setup.m_nUnscaledHeight = m_VR->m_RenderHeight;
	setup.fov = m_VR->m_Fov;
	setup.fovViewmodel = m_VR->m_Fov;
	setup.m_flAspectRatio = m_VR->m_Aspect;
	setup.zNear = 6;
	setup.zNearViewmodel = 2;
	setup.angles = hmdAngle;

	CViewSetup leftEyeView = setup;
	CViewSetup rightEyeView = setup;

	int playerIndex = m_Game->m_EngineClient->GetLocalPlayer();
	C_BasePlayer* localPlayer = (C_BasePlayer*)m_Game->GetClientEntity(playerIndex);

	// Left eye CViewSetup
	QAngle tempAngle = QAngle(setup.angles.x, setup.angles.y, setup.angles.z);
	leftEyeView.origin = m_VR->TraceEye((uint32_t*)localPlayer, position, m_VR->GetViewOriginLeft(position), tempAngle);
	leftEyeView.angles.y = tempAngle.y;

	//std::cout << "dRenderView - Left Start\n";
	IMatRenderContext* rndrContext = matSystem->GetRenderContext();
	rndrContext->SetRenderTarget(m_VR->m_LeftEyeTexture);
	rndrContext->Release();
	hkRenderView.fOriginal(ecx, leftEyeView, hudViewSetup, nClearFlags, whatToDraw);

	// Right eye CViewSetup
	tempAngle = QAngle(setup.angles.x, setup.angles.y, setup.angles.z);
	rightEyeView.origin = m_VR->TraceEye((uint32_t*)localPlayer, position, m_VR->GetViewOriginRight(position), tempAngle);
	rightEyeView.angles.y = tempAngle.y;

	//std::cout << "dRenderView - Right Start\n";
	rndrContext = matSystem->GetRenderContext();
	rndrContext->SetRenderTarget(m_VR->m_RightEyeTexture);
	rndrContext->Release();
	hkRenderView.fOriginal(ecx, rightEyeView, hudViewSetup, nClearFlags, whatToDraw);

	m_PushedHud = false;

	rndrContext = matSystem->GetRenderContext();
	rndrContext->SetRenderTarget(NULL);
	rndrContext->Release();

	m_VR->m_RenderedNewFrame = true;
}

bool __fastcall Hooks::dCreateMove(void *ecx, void *edx, float flInputSampleTime, CUserCmd *cmd)
{
	if (!cmd->command_number)
		return hkCreateMove.fOriginal(ecx, flInputSampleTime, cmd);

	if (m_VR->m_IsVREnabled)
	{
		cmd->viewangles = m_VR->m_HmdAngAbs;

		vr::InputAnalogActionData_t analogActionData;
		if (m_VR->GetAnalogActionData(m_VR->m_ActionWalk, analogActionData)) {
			// Run toward other guy
			cmd->buttons &= ~(IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT);

			cmd->forwardmove += analogActionData.y * MAX_LINEAR_SPEED;
			cmd->sidemove += analogActionData.x * MAX_LINEAR_SPEED;

			// We'll only be moving fwd or sideways
			cmd->upmove = 0.0f;

			if (cmd->forwardmove > 0.0f)
			{
				cmd->buttons |= IN_FORWARD;
			}
			else if (cmd->forwardmove < 0.0f)
			{
				cmd->buttons |= IN_BACK;
			}

			if (cmd->sidemove > 0.0f)
			{
				cmd->buttons |= IN_MOVELEFT;
			}
			else if (cmd->sidemove < 0.0f)
			{
				cmd->buttons |= IN_MOVERIGHT;
			}

		}

		if (m_VR->m_RoomscaleActive)
		{
			// How much have we moved since last CreateMove?
			Vector setupOriginToHMD = (m_VR->m_HmdPosRelativeRaw - m_VR->m_HmdPosRelativeRawPrev) * m_VR->m_VRScale; //m_VR->m_HmdPosRelative - m_VR->m_HmdPosRelativePrev;
			m_VR->m_HmdPosRelativeRawPrev = m_VR->m_HmdPosRelativeRaw;

			setupOriginToHMD.z = 0;
			float distance = VectorLength(setupOriginToHMD);
			if (distance > 0)
			{
				float forwardSpeed = DotProduct2D(setupOriginToHMD, m_VR->m_HmdForward);
				float sideSpeed = DotProduct2D(setupOriginToHMD, m_VR->m_HmdRight);
				cmd->forwardmove += distance * forwardSpeed;
				cmd->sidemove += distance * sideSpeed;

				// Let's update the position and the previous too
				/*m_VR->m_HmdPosRelative -= setupOriginToHMD;
				m_VR->m_HmdPosRelativePrev = m_VR->m_HmdPosRelative;*/

				/*m_VR->m_Center += m_VR->m_HmdPosRelativeRaw - m_VR->m_HmdPosRelativeRawPrev;
				m_VR->m_HmdPosRelativeRawPrev = m_VR->m_HmdPosRelativeRaw;*/

				//m_VR->ResetPosition();
			}
		}
	}

	return false;
}

void __fastcall Hooks::dEndFrame(void *ecx, void *edx)
{
	return hkEndFrame.fOriginal(ecx);
}

void __fastcall Hooks::dCalcViewModelView(void *ecx, void *edx, const Vector &eyePosition, const QAngle &eyeAngles)
{
	Vector vecNewOrigin = eyePosition;
	QAngle vecNewAngles = eyeAngles;

	//std::cout << "dCalcViewModelView: (" << m_VR->m_IsVREnabled << ")\n";

	if (m_VR->m_IsVREnabled)
	{
		vecNewOrigin = m_VR->GetRecommendedViewmodelAbsPos(eyePosition);
		vecNewAngles = m_VR->GetRecommendedViewmodelAbsAngle();
	}


	return hkCalcViewModelView.fOriginal(ecx, vecNewOrigin, vecNewAngles);
}

float __fastcall Hooks::dProcessUsercmds(void *ecx, void *edx, edict_t *player, void *buf, int numcmds, int totalcmds, int dropped_packets, bool ignore, bool paused)
{
	// Function pointer for CBaseEntity::entindex
	typedef int(__thiscall *tEntindex)(void *thisptr);
	static tEntindex oEntindex = (tEntindex)(m_Game->m_Offsets->CBaseEntity_entindex.address);

	IServerUnknown * pUnknown = player->m_pUnk;
	Server_BaseEntity *pPlayer = (Server_BaseEntity*)pUnknown->GetBaseEntity();

	//std::cout << "dProcessUsercmds:" << pPlayer << "\n";

	/*Vector test = {0, 0, 0};
	(*(void(__thiscall**)(int, Vector*))(*(DWORD*)pPlayer + 1156))((int)pPlayer, &test);

	std::cout << "dProcessUsercmds:" << test.x << ", " << test.y << ", " << test.z << "\n";*/

	int index = oEntindex(pPlayer);
	m_Game->m_CurrentUsercmdID = index;

	float result = hkProcessUsercmds.fOriginal(ecx, player, buf, numcmds, totalcmds, dropped_packets, ignore, paused);

	// check if swinging melee wep
	/*if (m_Game->m_PlayersVRInfo[index].isUsingVR && m_Game->m_PlayersVRInfo[index].isMeleeing)
	{
		typedef Server_WeaponCSBase *(__thiscall *tGetActiveWep)(void *thisptr);
		static tGetActiveWep oGetActiveWep = (tGetActiveWep)(m_Game->m_Offsets->GetActiveWeapon.address);
		Server_WeaponCSBase *curWep = oGetActiveWep(pPlayer);

		if (curWep)
		{
			int wepID = curWep->GetWeaponID();
			if (wepID == 19) // melee weapon
			{
				if (m_Game->m_PlayersVRInfo[index].isNewSwing)
				{
					m_Game->m_PlayersVRInfo[index].isNewSwing = false;
					curWep->entitiesHitThisSwing = 0;
				}

				typedef void *(__thiscall *tGetMeleeWepInfo)(void *thisptr);
				static tGetMeleeWepInfo oGetMeleeWepInfo = (tGetMeleeWepInfo)(m_Game->m_Offsets->GetMeleeWeaponInfo.address);
				void *meleeWepInfo = oGetMeleeWepInfo(curWep);

				Vector initialForward, initialRight, initialUp;
				QAngle::AngleVectors(m_Game->m_PlayersVRInfo[index].prevControllerAngle, &initialForward, &initialRight, &initialUp);
				Vector initialMeleeDirection = VectorRotate(initialForward, initialRight, 50.0);
				VectorNormalize(initialMeleeDirection);

				Vector finalForward, finalRight, finalUp;
				QAngle::AngleVectors(m_Game->m_PlayersVRInfo[index].controllerAngle, &finalForward, &finalRight, &finalUp);
				Vector finalMeleeDirection = VectorRotate(finalForward, finalRight, 50.0);
				VectorNormalize(finalMeleeDirection);

				Vector pivot;
				CrossProduct(initialMeleeDirection, finalMeleeDirection, pivot);
				VectorNormalize(pivot);

				float swingAngle = acos(DotProduct(initialMeleeDirection, finalMeleeDirection)) * 180 / 3.14159265;

				m_Game->m_Hooks->hkGetPrimaryAttackActivity.fOriginal(curWep, meleeWepInfo); // Needed to call TestMeleeSwingCollision

				m_Game->m_PerformingMelee = true;

				Vector traceDirection = initialMeleeDirection;
				int numTraces = 10;
				float traceAngle = swingAngle / numTraces;
				for (int i = 0; i < numTraces; ++i)
				{
					traceDirection = VectorRotate(traceDirection, pivot, traceAngle);
					m_Game->m_Hooks->hkTestMeleeSwingCollisionServer.fOriginal(curWep, traceDirection);
				}

				m_Game->m_PerformingMelee = false;
			}
		}
	}
	else
	{
		m_Game->m_PlayersVRInfo[index].isNewSwing = true;
	}*/

	m_Game->m_PlayersVRInfo[index].prevControllerAngle = m_Game->m_PlayersVRInfo[index].controllerAngle;

	return result;
}

int Hooks::dReadUsercmd(void *buf, CUserCmd *move, CUserCmd *from)
{
	hkReadUsercmd.fOriginal(buf, move, from);

	int i = m_Game->m_CurrentUsercmdID;
	if (move->tick_count < 0) // Signal for VR CUserCmd
	{
		move->tick_count *= -1;

		m_Game->m_PlayersVRInfo[i].isUsingVR = true;
		m_Game->m_PlayersVRInfo[i].controllerAngle.x = (float)move->mousedx / 10;
		m_Game->m_PlayersVRInfo[i].controllerAngle.y = (float)move->mousedy / 10;
		m_Game->m_PlayersVRInfo[i].controllerPos.x = move->viewangles.z;
		m_Game->m_PlayersVRInfo[i].controllerPos.y = move->upmove;

		// Decode controllerAngle.z
		int rollEncoding = move->command_number / 10000000;
		move->command_number -= rollEncoding * 10000000;
		m_Game->m_PlayersVRInfo[i].controllerAngle.z = (rollEncoding * 2) - 180;

		// Decode viewangles.x
		int decodedZInt = (move->viewangles.x / 10000);
		float decodedAngle = abs((float)(move->viewangles.x - (decodedZInt * 10000)) / 10);
		decodedAngle -= 360;
		float decodedZ = (float)decodedZInt / 10;

		m_Game->m_PlayersVRInfo[i].controllerPos.z = decodedZ;

		move->viewangles.x = decodedAngle;
		move->viewangles.z = 0;
		move->upmove = 0;
	}
	else
	{
		m_Game->m_PlayersVRInfo[i].isUsingVR = false;
	}
	return 1;
}

void __fastcall Hooks::dWriteUsercmdDeltaToBuffer(void *ecx, void *edx, int a1, void *buf, int from, int to, bool isnewcommand)
{
	return hkWriteUsercmdDeltaToBuffer.fOriginal(ecx, a1, buf, from, to, isnewcommand);
}

int Hooks::dWriteUsercmd(void *buf, CUserCmd *to, CUserCmd *from)
{
	if (m_VR->m_IsVREnabled)
	{
		CInput *m_Input = **(CInput ***)(m_Game->m_Offsets->g_pppInput.address);
		CVerifiedUserCmd *pVerifiedCommands = *(CVerifiedUserCmd **)((uintptr_t)m_Input + 0xF0);
		CVerifiedUserCmd *pVerified = &pVerifiedCommands[(to->command_number) % 150];

		// Signal to the server that this CUserCmd has VR info
		to->tick_count *= -1;

		int originalCommandNum = to->command_number;

		QAngle controllerAngles = m_VR->GetRightControllerAbsAngle();
		to->mousedx = controllerAngles.x * 10; // Strip off 2nd decimal to save bits.
		to->mousedy = controllerAngles.y * 10;
		int rollEncoding = (((int)controllerAngles.z + 180) / 2 * 10000000);
		to->command_number += rollEncoding;

		Vector controllerPos = m_VR->GetRightControllerAbsPos();
		to->viewangles.z = controllerPos.x;
		to->upmove = controllerPos.y;

		// Space in CUserCmd is tight, so encode viewangle.x and controllerPos.z together.
		// Encoding will overflow if controllerPos.z goes beyond +-21474.8
		float xAngle = to->viewangles.x;
		int encodedAngle = (xAngle + 360) * 10;
		int encoding = (int)(controllerPos.z * 10) * 10000;
		encoding += encoding < 0 ? -encodedAngle : encodedAngle;
		to->viewangles.x = encoding;

		hkWriteUsercmd.fOriginal(buf, to, from);

		to->viewangles.x = xAngle;
		to->tick_count *= -1;
		to->viewangles.z = 0;
		to->upmove = 0;
		to->command_number = originalCommandNum;

		// Must recalculate checksum for the edited CUserCmd or gunshots will sound
		// terrible in multiplayer.
		/*pVerified->m_cmd = *to;
		pVerified->m_crc = to->GetChecksum();*/
		return 1;
	}
	return hkWriteUsercmd.fOriginal(buf, to, from);
}

void Hooks::dAdjustEngineViewport(int &x, int &y, int &width, int &height)
{
	width = m_VR->m_RenderWidth;
	height = m_VR->m_RenderHeight;

	hkAdjustEngineViewport.fOriginal(x, y, width, height);
}

void Hooks::dViewport(void *ecx, void *edx, int x, int y, int width, int height)
{
	//std::cout << "dViewport - X: " << x << ", Y: " << y << ", W: " << width << ", H: " << height << "\n";


	/*if (m_VR->m_IsVREnabled && m_Game->m_EngineClient->IsInGame() && !m_Game->m_VguiSurface->IsCursorVisible())
	{
		int windowWidth, windowHeight;
		m_Game->m_MaterialSystem->GetRenderContext()->GetWindowSize(windowWidth, windowHeight);

		if (width == windowWidth && height == windowHeight) {
			return hkViewport.fOriginal(ecx, x, y, m_VR->m_RenderWidth, m_VR->m_RenderHeight);
		}
	}*/


	//hkViewport.fOriginal(ecx, x, y, m_VR->m_RenderWidth, m_VR->m_RenderHeight);
	hkViewport.fOriginal(ecx, x, y, width, height);
}

void Hooks::dGetViewport(void *ecx, void *edx, int &x, int &y, int &width, int &height)
{
	hkGetViewport.fOriginal(ecx, x, y, width, height);

	width = m_VR->m_RenderWidth;
	height = m_VR->m_RenderHeight;

	//std::cout << "dGetViewport - X: " << x << ", Y: " << y << ", W: " << width << ", H: " << height << "\n";
}

int Hooks::dGetPrimaryAttackActivity(void *ecx, void *edx, void *meleeInfo)
{
	return hkGetPrimaryAttackActivity.fOriginal(ecx, meleeInfo);
}

Vector *Hooks::dEyePosition(void *ecx, void *edx, Vector *eyePos)
{
	Vector *result = hkEyePosition.fOriginal(ecx, eyePos);
	return result;
}

// We'll keep this for... future reference!
void Hooks::dDrawModelExecute(void *ecx, void *edx, void *state, const ModelRenderInfo_t &info, void *pCustomBoneToWorld)
{
	if (info.pModel)
	{
		std::string modelName = m_Game->m_ModelInfo->GetModelName(info.pModel);
		if (modelName.find("/arms/") != std::string::npos)
		{
			m_Game->m_ArmsMaterial = m_Game->m_MaterialSystem->FindMaterial(modelName.c_str(), "Model textures");
			m_Game->m_ArmsModel = info.pModel;
			m_Game->m_CachedArmsModel = true;
		}
	}

	if (info.pModel && info.pModel == m_Game->m_ArmsModel)
	{
		m_Game->m_ArmsMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		m_Game->m_ModelRender->ForcedMaterialOverride(m_Game->m_ArmsMaterial);
		hkDrawModelExecute.fOriginal(ecx, state, info, pCustomBoneToWorld);
		m_Game->m_ModelRender->ForcedMaterialOverride(NULL);
		return;
	}

	hkDrawModelExecute.fOriginal(ecx, state, info, pCustomBoneToWorld);
}

void Hooks::dPushRenderTargetAndViewport(void *ecx, void *edx, ITexture *pTexture, ITexture *pDepthTexture, int nViewX, int nViewY, int nViewW, int nViewH)
{
	if (m_VR->m_CreatedVRTextures && !m_PushedHud)
	{
		pTexture = m_VR->m_HUDTexture;

		//pTexture = m_VR->m_RightEyeTexture;

		IMatRenderContext *renderContext = m_Game->m_MaterialSystem->GetRenderContext();
		renderContext->ClearBuffers(false, true, true);
		renderContext->Release();

		hkPushRenderTargetAndViewport.fOriginal(ecx, pTexture, pDepthTexture, nViewX, nViewY, nViewW, nViewH);

		renderContext = m_Game->m_MaterialSystem->GetRenderContext();
		renderContext->OverrideAlphaWriteEnable(true, true);
		renderContext->ClearColor4ub(0, 0, 0, 0);
		renderContext->ClearBuffers(true, false);
		renderContext->Release();

		m_VR->m_RenderedHud = true;
		m_PushedHud = true;
	}
	else
	{
		hkPushRenderTargetAndViewport.fOriginal(ecx, pTexture, pDepthTexture, nViewX, nViewY, nViewW, nViewH);
	}
}

void Hooks::dPopRenderTargetAndViewport(void *ecx, void *edx)
{
	if (!m_VR->m_CreatedVRTextures)
		return hkPopRenderTargetAndViewport.fOriginal(ecx);

	//std::cout << "dPopRenderTargetAndViewport: " << m_PushHUDStep << "\n";

	m_PushHUDStep = 0;

	if (m_PushedHud)
	{
		IMatRenderContext* renderContext = m_Game->m_MaterialSystem->GetRenderContext();
		renderContext->OverrideAlphaWriteEnable(false, true);
		renderContext->ClearColor4ub(0, 0, 0, 255);
		renderContext->Release();
	}

	hkPopRenderTargetAndViewport.fOriginal(ecx);
}

void Hooks::dVGui_Paint(void *ecx, void *edx, int mode)
{
	if (!m_VR->m_CreatedVRTextures || m_VR->m_Game->m_VguiSurface->IsCursorVisible())
		return hkVgui_Paint.fOriginal(ecx, mode);

	//std::cout << "dVGui_Paint\n";

	if (m_PushedHud)
		mode = PAINT_UIPANELS | PAINT_INGAMEPANELS;

	hkVgui_Paint.fOriginal(ecx, mode);
}

int Hooks::dIsSplitScreen()
{
	//std::cout << "dIsSplitScreen: " << m_PushHUDStep << "\n";

	if (m_PushHUDStep == 0)
		++m_PushHUDStep;
	else
		m_PushHUDStep = -999;

	return hkIsSplitScreen.fOriginal();
}

DWORD *Hooks::dPrePushRenderTarget(void *ecx, void *edx, int a2)
{
	//std::cout << "dPrePushRenderTarget: " << m_PushHUDStep << "\n";

	if (m_PushHUDStep == 1)
		++m_PushHUDStep;
	else
		m_PushHUDStep = -999;

	return hkPrePushRenderTarget.fOriginal(ecx, a2);
}

Vector* Hooks::dWeapon_ShootPosition(void* ecx, void* edx, Vector* eyePos)
{
	Vector* result = hkWeapon_ShootPosition.fOriginal(ecx, eyePos);

	int localIndex = m_Game->m_EngineClient->GetLocalPlayer();
	int index = EntityIndex(ecx);

	auto vrPlayer = m_Game->m_PlayersVRInfo[index];

	if (m_VR->m_IsVREnabled && localIndex == index) {
		*result = m_VR->GetRightControllerAbsPos();
	}
	else if (vrPlayer.isUsingVR)
	{
		*result = vrPlayer.controllerPos;
	}

	return result;
}

void* Hooks::dCWeaponPortalgun_FirePortal(void* ecx, void* edx, bool bPortal2, Vector* pVector) {
	bool wasTrue = m_VR->m_OverrideEyeAngles;

	m_VR->m_OverrideEyeAngles = true;

	auto result = hkCWeaponPortalgun_FirePortal.fOriginal(ecx, bPortal2, pVector);

	if (!wasTrue)
		m_VR->m_OverrideEyeAngles = false;

	return result;
}

bool __fastcall Hooks::dTraceFirePortal(void* ecx, void* edx, const Vector& vTraceStart, const Vector& vDirection, bool bPortal2, int iPlacedBy, void* tr) //trace_tx& tr, Vector& vFinalPosition //  , Vector& vFinalPosition, QAngle& qFinalAngles, int iPlacedBy, bool bTest /*= false*/
{
	Vector vNewTraceStart = vTraceStart;
	Vector vNewDirection = vDirection;

	if (iPlacedBy == 2) {
		int localIndex = m_Game->m_EngineClient->GetLocalPlayer();

		auto owner = GetOwner(ecx);

		if (owner) {
			int index = EntityIndex(owner);

			auto vrPlayer = m_Game->m_PlayersVRInfo[index];

			if (m_VR->m_IsVREnabled && localIndex == index) {
				vNewTraceStart = m_VR->GetRightControllerAbsPos();
				vNewDirection = m_VR->m_RightControllerForward;
			}
			else if (vrPlayer.isUsingVR)
			{
				vNewTraceStart = vrPlayer.controllerPos;
				Vector fwd, rt, up;
				QAngle::AngleVectors(vrPlayer.controllerAngle, &fwd, &rt, &up);
				vNewDirection = fwd;
			}
		}
	}

	bool bTraceSucceeded = hkTraceFirePortal.fOriginal(ecx, vNewTraceStart, vNewDirection, bPortal2, iPlacedBy, tr);

	return bTraceSucceeded;
}

void __fastcall Hooks::dPlayerPortalled(void* ecx, void* edx, void* a2, __int64 a3)
{
	CBaseEntity* pBaseEntity = (CBaseEntity*)ecx;

	QAngle angAbsRotationBefore;
	m_Game->m_EngineClient->GetViewAngles(angAbsRotationBefore);

	hkPlayerPortalled.fOriginal(ecx, a2, a3);

	QAngle angAbsRotationAfter;
	m_Game->m_EngineClient->GetViewAngles(angAbsRotationAfter);

	if (angAbsRotationBefore != angAbsRotationAfter) {
		m_VR->m_PortalRotationOffset = angAbsRotationAfter - angAbsRotationBefore;
		m_VR->m_ApplyPortalRotationOffset = true;
	}

	return;
}

int Hooks::dGetModeHeight(void* ecx, void* edx) {
	//std::cout << "dGetModeHeight\n";
	return m_VR->m_RenderHeight;
}

bool Hooks::dClipTransform(const Vector& point, Vector* pScreen)
{
	return hkClipTransform.fOriginal(point, pScreen);
}

bool Hooks::ScreenTransform(const Vector& point, Vector* pScreen, int width, int height)
{
	bool retval = hkClipTransform.fOriginal(point, pScreen);

	pScreen->x = 0.5f * (pScreen->x + 1.0f) * width;
	pScreen->y = 0.5f * (-pScreen->y + 1.0f) * height;

	return retval;
}

int __fastcall Hooks::dDrawSelf(void* ecx, void* edx, int x, int y, int w, int h, const void* clr, float flApparentZ) {
	//std::cout << "dDrawSelf - X: " << x << ", Y: " << y << ", W: " << w << ", H: " << h << ", Z: " << flApparentZ << "\n";

	//int playerIndex = m_Game->m_EngineClient->GetLocalPlayer();

	//auto viewport = m_Game->m_ClientMode->GetViewport();

	int newX = x;
	int	newY = y;

	if (m_VR->m_IsVREnabled)
	{
		int windowWidth, windowHeight;
		m_Game->m_MaterialSystem->GetRenderContext()->GetWindowSize(windowWidth, windowHeight);

		Vector screen = { 0, 0, 0 };

		//Vector vec = m_VR->m_AimPos - m_VR->GetRightControllerAbsPos();

		//newZ = 1.0 / sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

		ScreenTransform(m_VR->m_AimPos, &screen, m_VR->m_RenderWidth, m_VR->m_RenderHeight);

		int offsetX = x - (windowWidth * 0.5f);
		int offsetY = y - (windowHeight * 0.5f);

		newX = screen.x + offsetX;
		newY = screen.y + offsetY;
	}

	return hkDrawSelf.fOriginal(ecx, newX, newY, w, h, clr, flApparentZ);
}

void __cdecl Hooks::dVGui_GetHudBounds(int slot, int& x, int& y, int& w, int& h) {
	if (m_VR->m_IsVREnabled && !m_Game->m_VguiSurface->IsCursorVisible())
	{
		x = y = 0;
		w = m_VR->m_RenderWidth;
		h = m_VR->m_RenderHeight;
	} else {
		hkVGui_GetHudBounds.fOriginal(slot, x, y, w, h);
	}

	//std::cout << "dVGui_GetHudBounds - X: " << x << ", Y: " << y << ", W: " << w << ", H: " << h << "\n";
}

void __cdecl Hooks::dVGui_GetPanelBounds(int slot, int& x, int& y, int& w, int& h) {
	if (m_VR->m_IsVREnabled && !m_Game->m_VguiSurface->IsCursorVisible())
	{
		x = y = 0;
		w = m_VR->m_RenderWidth;
		h = m_VR->m_RenderHeight;
	}
	else {
		hkVGui_GetPanelBounds.fOriginal(slot, x, y, w, h);
	}

	//std::cout << "dVGui_GetPanelBounds - X: " << x << ", Y: " << y << ", W: " << w << ", H: " << h << "\n";
}

void __cdecl Hooks::dVGUI_UpdateScreenSpaceBounds(int nNumSplits, int sx, int sy, int sw, int sh) {
	hkVGUI_UpdateScreenSpaceBounds.fOriginal(nNumSplits, sx, sy, m_VR->m_RenderWidth, m_VR->m_RenderHeight);
}

void __cdecl Hooks::dVGui_GetTrueScreenSize(int &w, int &h) {
	w = m_VR->m_RenderWidth;
	h = m_VR->m_RenderHeight;
}

void __fastcall Hooks::dGetScreenSize(void* ecx, void* edx, int& wide, int& tall) {
	//hkGetScreenSize.fOriginal(ecx, wide, tall);
	wide = m_VR->m_RenderWidth;
	tall = m_VR->m_RenderHeight;
}

void __cdecl Hooks::dGetHudSize(int& w, int& h) {
	w = m_VR->m_RenderWidth;
	h = m_VR->m_RenderHeight;
}

void __fastcall Hooks::dPush2DView(void* ecx, void* edx, IMatRenderContext* pRenderContext, const CViewSetup& view, int nFlags, ITexture* pRenderTarget, void* frustumPlanes) {
	m_PushedHud = false;

	return hkPush2DView.fOriginal(ecx, pRenderContext, view, nFlags, pRenderTarget, frustumPlanes);
}

void __fastcall Hooks::dRender(void* ecx, void* edx, vrect_t* rect) {
	//std::cout << "dRender - X: " << rect->x << ", Y: " << rect->y << ", W: " << rect->width << ", H: " << rect->height  << "\n";

	return hkRender.fOriginal(ecx, rect);
}

void __fastcall Hooks::dSetBounds(void* ecx, void* edx, int x, int y, int w, int h) {
	std::cout << "dSetBounds - X: " << x << ", Y: " << y << ", W: " << w << ", H: " << h << "\n";

	hkSetBounds.fOriginal(ecx, x, y, m_VR->m_RenderWidth, m_VR->m_RenderHeight);
}

void __fastcall Hooks::dSetSize(void* ecx, void* edx, int wide, int tall) {
	hkSetSize.fOriginal(ecx, wide, tall);

	//std::cout << "dSetSize - Wide: " << wide << ", Tall: " << tall  << "\n";
}

void __fastcall Hooks::dGetClipRect(void* ecx, void* edx, int& x0, int& y0, int& x1, int& y1) {
	hkGetClipRect.fOriginal(ecx, x0, y0, x1, y1);

	//std::cout << "dGetClipRect - X: " << x0 << ", Y: " << y0 << ", W: " << x1 << ", H: " << y1  << "\n";
}

double __fastcall Hooks::dComputeError(void* ecx, void* edx) {
	bool wasTrue = m_VR->m_OverrideEyeAngles;

	m_VR->m_OverrideEyeAngles = true;

	double computedError = hkComputeError.fOriginal(edx);

	if (!wasTrue)
		m_VR->m_OverrideEyeAngles = false;

	return computedError;
}

bool __fastcall Hooks::dUpdateObject(void* ecx, void* edx, void* pPlayer, float flError, bool bIsTeleport) {
	bool wasTrue = m_VR->m_OverrideEyeAngles;

	m_VR->m_OverrideEyeAngles = true;

	bool value = hkUpdateObject.fOriginal(ecx, pPlayer, flError, bIsTeleport);

	if (!wasTrue)
		m_VR->m_OverrideEyeAngles = false;

	return value;
}

bool __fastcall Hooks::dUpdateObjectVM(void* ecx, void* edx, void* pPlayer, float flError) {
	bool wasTrue = m_VR->m_OverrideEyeAngles;

	m_VR->m_OverrideEyeAngles = true;

	bool value = hkUpdateObjectVM.fOriginal(ecx, pPlayer, flError);

	if (!wasTrue)
		m_VR->m_OverrideEyeAngles = false;

	return value;
}

// This function is apparently not used by Portal 2, remove?
void __fastcall Hooks::dRotateObject(void* ecx, void* edx, void* pPlayer, float fRotAboutUp, float fRotAboutRight, bool bUseWorldUpInsteadOfPlayerUp) {
	bool wasTrue = m_VR->m_OverrideEyeAngles;

	m_VR->m_OverrideEyeAngles = true;

	hkRotateObject.fOriginal(ecx, pPlayer, fRotAboutUp, fRotAboutRight, bUseWorldUpInsteadOfPlayerUp);

	if (!wasTrue)
		m_VR->m_OverrideEyeAngles = false;
}

// This is CPlayerBase, do we also need to hook CPortalPlayer? can the same function be used by both?
// This works for release, but why was it crashing before??? TODO: buy a c++ book...
QAngle& __fastcall Hooks::dEyeAngles(void* ecx, void* edx) {
	if (m_VR->m_OverrideEyeAngles) {
		int localIndex = m_Game->m_EngineClient->GetLocalPlayer();
		int index = EntityIndex(ecx);

		auto vrPlayer = m_Game->m_PlayersVRInfo[index];

		if (m_VR->m_IsVREnabled && localIndex == index) {
			return m_VR->GetRightControllerAbsAngleConst();
		}
		else if (vrPlayer.isUsingVR)
		{
			return vrPlayer.controllerAngle;
		}
	}
	else {
		return hkEyeAngles.fOriginal(ecx);
	}
}

void Hooks::dMatrixBuildPerspectiveX(void*& dst, double flFovX, double flAspect, double flZNear, double flZFar) {
	std::cout << "dMatrixBuildPerspectiveX - flFovX: " << flFovX << ", flAspect: " << flAspect << ", flZNear: " << flZNear << ", flZFar: " << flZFar << "\n";

	hkMatrixBuildPerspectiveX.fOriginal(dst, flFovX, flAspect, flZNear, flZFar);
}

int __fastcall Hooks::dGetDefaultFOV(void* ecx, void* edx) {
	return m_VR->m_Fov;
}

double __fastcall Hooks::dGetFOV(void* ecx, void* edx) {
	return m_VR->m_Fov;
}

double __fastcall Hooks::dGetViewModelFOV(void* ecx, void* edx) {
	return m_VR->m_Fov;
}