#include "PickupItem.h"

HRESULT	PickupItem::initMesh(ID3D11Device* pd3dDevice)
{
	setTextureName(L"Resources\\yellow.dds");

	m_scale = XMFLOAT3(20, 20, 1);
	setTextureName(L"Resources\\pickup.dds");

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	setPosition(XMFLOAT3(100, 200, 0));

	return hr;
}