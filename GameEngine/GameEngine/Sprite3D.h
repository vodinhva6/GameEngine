#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include <filesystem>
class Sprite3D 
{
public:
	Sprite3D() {}
    Sprite3D(ID3D11Device* device, std::filesystem::path local);
	void updateAnimation();
	void render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color);
	struct Constants
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4 materialColor;

	};
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texcoord;
	};
	std::vector<Vertex>& getVertices() { return vertices; }
	std::vector<uint32_t>& getIndices() { return indices; }
protected:
	
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	D3D11_TEXTURE2D_DESC texture2dDesc;
};

