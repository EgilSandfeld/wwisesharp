//////////////////////////////////////////////////////////////////////
//
// Author: Adrien "Lucyberad" Bock.
//
// -------------------------------------------------------------------
//
// Important note about usage of System::String^ and StringToHGlobalUni heavily used in this file.
// At first, for your information, the ^ char means that the System::String is given as a managed object. It works like pointer.
// Because System::String is managed, it's hard to get a native char array. That's why I use a special interop method: StringToHGlobalUni which
// give a managed IntPtr. This one is managed but his role is to contain a native C pointer.
// Then I just need to cast it into LPCWSTR (const wchar*).
// Just after using it, we could (in fact we __must__ to avoid memory leaks) free the IntPtr to free up memory space.
//
//////////////////////////////////////////////////////////////////////

#include "WwiseSharpEngine.hpp"
using namespace WwiseSharp;

WwiseSharpEngine::WwiseSharpEngine()
{
	akengine = new WwiseEngine();
}
WwiseSharpEngine::~WwiseSharpEngine()
{
	delete akengine;
}

bool WwiseSharpEngine::Init()
{
	return akengine->Init();
}

void WwiseSharpEngine::SetBasePath(System::String^ basePath)
{
	//Because Wwise low level IO don't handle path without end '/' char, we need to add it in case it does'nt exists.
	if (basePath->default[basePath->Length - 1] != static_cast<wchar_t>('/'))
		basePath += "/";
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(basePath);
	akengine->SetBasePath(static_cast<LPCWSTR>(static_cast<void *>(p)));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}

void WwiseSharpEngine::SetLangSpecificDirName(System::String^ langSpecificDirName)
{
	//Because Wwise low level IO don't handle path without end '/' char, we need to add it in case it does'nt exists.
	if (langSpecificDirName->default[langSpecificDirName->Length - 1] != static_cast<wchar_t>('/'))
		langSpecificDirName += "/";
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(langSpecificDirName);
	akengine->SetLangSpecificDirName(static_cast<LPCWSTR>(static_cast<void *>(p)));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}

void WwiseSharpEngine::LoadBank(System::String^ bankName)
{
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(bankName);
	akengine->LoadBank(static_cast<LPCWSTR>(static_cast<void *>(p)));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}

WwiseSharpGameObject^ WwiseSharpEngine::RegisterGameObject(unsigned int objectId, System::String^ gameObjectLabel)
{
	WwiseSharpGameObject^ wgo = gcnew WwiseSharpGameObject(objectId, gameObjectLabel, akengine);
	akengine->RegisterGameObject(wgo->GetAkObjectId(), wgo->GetLabelChar());
	return wgo;
}

void WwiseSharpEngine::SetState(unsigned long stateGroupId, unsigned long stateId)
{
	akengine->SetState(static_cast<AkStateGroupID>(stateGroupId), static_cast<AkStateID>(stateId));
}
void WwiseSharpEngine::SetState(System::String^ stateGroup, System::String^ state)
{
	System::IntPtr p1 = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(stateGroup);
	System::IntPtr p2 = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(state);
	akengine->SetState(static_cast<LPCWSTR>(static_cast<void *>(p1)), static_cast<LPCWSTR>(static_cast<void *>(p2)));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p1);
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p2);
}

void WwiseSharpEngine::SetListenerPosition(WwiseSharpListenerPosition^ listenerPosition)
{
	AkVector akPosition;
	akPosition.X = listenerPosition->Xposition;
	akPosition.Y = listenerPosition->Yposition;
	akPosition.Z = listenerPosition->Zposition;

	AkVector akOrientationFront;
	akOrientationFront.X = listenerPosition->XorientationFront;
	akOrientationFront.Y = listenerPosition->YorientationFront;
	akOrientationFront.Z = listenerPosition->ZorientationFront;

	AkVector akOrientationTop;
	akOrientationTop.X = listenerPosition->XorientationTop;
	akOrientationTop.Y = listenerPosition->YorientationTop;
	akOrientationTop.Z = listenerPosition->ZorientationTop;

	AkListenerPosition akListenerPosition;
	akListenerPosition.Position = akPosition;
	akListenerPosition.OrientationFront = akOrientationFront;
	akListenerPosition.OrientationTop = akOrientationTop;

	akengine->SetListenerPosition(akListenerPosition);
}
void WwiseSharpEngine::SetListenerPosition(int listenerIndex, WwiseSharpListenerPosition^ listenerPosition)
{
	AkVector akPosition;
	akPosition.X = listenerPosition->Xposition;
	akPosition.Y = listenerPosition->Yposition;
	akPosition.Z = listenerPosition->Zposition;

	AkVector akOrientationFront;
	akOrientationFront.X = listenerPosition->XorientationFront;
	akOrientationFront.Y = listenerPosition->YorientationFront;
	akOrientationFront.Z = listenerPosition->ZorientationFront;

	AkVector akOrientationTop;
	akOrientationTop.X = listenerPosition->XorientationTop;
	akOrientationTop.Y = listenerPosition->YorientationTop;
	akOrientationTop.Z = listenerPosition->ZorientationTop;

	AkListenerPosition akListenerPosition;
	akListenerPosition.Position = akPosition;
	akListenerPosition.OrientationFront = akOrientationFront;
	akListenerPosition.OrientationTop = akOrientationTop;

	akengine->SetListenerPosition(listenerIndex, akListenerPosition);
}


void WwiseSharpEngine::Update()
{
	akengine->ProcessAudio();
}

void WwiseSharpEngine::UnregisterGameObject(WwiseSharpGameObject^ wgo)
{
	akengine->UnregisterGameObject(wgo->GetAkObjectId());
}

void WwiseSharpEngine::UnregisterAllGameObject()
{
	akengine->UnregisterAllGameObject();
}

void WwiseSharpEngine::Term()
{
	akengine->Term();
}