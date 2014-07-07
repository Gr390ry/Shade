#pragma once
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx11.h>
//#include <d3d9.h>
//#include <d3dx9.h>
#include <xnamath.h>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <fbxsdk.h>

//std
#include <map>
#include <vector>
#include <hash_map>
#include <ppl.h>

#include "../../Lib/directX11/d3dx11effect.h"

#define SAFE_RELEASE(p) { if (p) { p->Release(); p = nullptr; } }
#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] p; p = nullptr; } }
#define UPDATE_OBJECT(p, delta) { if (p) { p->Update(delta); } }
#define DEBUG_CONSOLE(p) { Console::Get()->print(p); }

#include "GenericVariable.h"
#include "ConstructHelper.h"
#include "../Console.h"