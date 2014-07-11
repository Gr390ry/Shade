#pragma once

//타입 선업
//typedef D3DXMATRIXA16	Matrix4x4;
//typedef D3DXVECTOR2		Vector2;
//typedef D3DXVECTOR3		Vector3;
//typedef D3DXVECTOR4		Vector4;
//typedef D3DXQUATERNION	Queternion;

namespace GENERIC {

	//전역 상수값 선언
	const std::string	gAppName		= "ShadeEngine";
	const int			windowWidth		= 1024;
	const int			windowHeight	= 768;
	const float			PI				= 3.14159265f;
	const float			FOV				= (PI / 4.f);
	const float			ASPECT_RATIO	= (windowWidth / (float)windowHeight);
	const float			nearPlane		= 1;
	const float			farPlane		= 10000;
	const int			shadowMapSize	= 2048;
	const XMFLOAT4		worldLightPosition(500, 500, -500, 1);
	const XMFLOAT4		worldLightColor(0.7f, 0.7f, 0.7f, 1);
	const XMFLOAT4		worldCameraPosition(0, 0, -200, 1);

	const float			RadToDeg = 57.29577951f;
	const float			DegToRad = 0.017453293f;

	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 uv;

		Vertex() : position(0, 0, 0), normal(0, 0, 0), uv(0, 0)
		{
		}
		Vertex(XMFLOAT3 _position, XMFLOAT3 _normal, XMFLOAT2 _uv) : position(_position), normal(_normal), uv(_uv)
		{
		}
	};
};