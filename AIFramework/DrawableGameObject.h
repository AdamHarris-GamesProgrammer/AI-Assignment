#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include <iostream>
#include "structures.h"
#include "Vector2D.h"
#include <algorithm>
#include <cstdint>

using namespace DirectX;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

class DrawableGameObject
{
public:
	DrawableGameObject();
	virtual ~DrawableGameObject();

	virtual HRESULT						initMesh(ID3D11Device* pd3dDevice);
	virtual void						update(const float t);
	void								draw(ID3D11DeviceContext* pContext);
	ID3D11ShaderResourceView**			getTextureResourceView() { return &m_pTextureResourceView; 	}
	XMFLOAT4X4*							getTransform() { return &m_world; }
	ID3D11SamplerState**				getTextureSamplerState() { return &m_pSamplerLinear; }

	void								setPosition(XMFLOAT3 position);
	XMFLOAT3*							getPosition() { return &m_position; }
	XMFLOAT3*							getDirection() { return &m_direction; }

	Vector2D GetVectorPosition() const {
		return Vector2D(m_position.x, m_position.y);
	}

	Vector2D GetTilePosition() const {
		Vector2D pos = GetVectorPosition();

		pos.x += SCREEN_WIDTH / 2;
		pos.y += SCREEN_HEIGHT / 2;

		pos.x /= 51.2;
		pos.y /= 38.4;

		return pos;
	}

	XMFLOAT3 GetScale() const {
		return m_scale;
	}

protected:
	void								setDirection(XMFLOAT3 direction); 
	void								setTextureName(wstring texName) { m_textureName = texName; }

	// helper functions
	XMFLOAT3							addFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							subtractFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							normaliseFloat3(XMFLOAT3& f1);
	float								magnitudeFloat3(XMFLOAT3& f1);
	XMFLOAT3							multiplyFloat3(XMFLOAT3& f1, const float scalar);
	XMFLOAT3							divideFloat3(XMFLOAT3& f1, const float scalar);

	float Lerp(float v0, float v1, float t);

protected: // protected variables
	XMFLOAT3							m_scale;
	float								m_radianRotation;

private: // private variables
	
	XMFLOAT4X4							m_world;
	ID3D11Buffer*						m_pVertexBuffer;
	ID3D11Buffer*						m_pIndexBuffer;
	ID3D11ShaderResourceView*			m_pTextureResourceView;
	ID3D11SamplerState *				m_pSamplerLinear;
	XMFLOAT3							m_position;

	XMFLOAT3							m_direction;
	wstring								m_textureName;
	
	
};

