#pragma once
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx11.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <xnamath.h>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <iostream>

//std
#include <map>
#include <vector>
#include <hash_map>
#include <ppl.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define SAFE_RELEASE(p) { if (p) { p->Release(); p = nullptr; } }
#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] p; p = nullptr; } }
#define UPDATE_OBJECT(p, delta) { if (p) { p->Update(delta); } }

#include "GenericVariable.h"
#include "ConstructHelper.h"