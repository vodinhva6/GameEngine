#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
#include <string>
#include "DataFormat.h"
inline void rotate(float& x, float& y, float cx, float cy, float angle);
class Sprite
{
public:
	Sprite(ID3D11Device* device, const wchar_t* filename);
	Sprite(ID3D11Device* device, const wchar_t* filename, ShaderData shaderData);
	//virtual void render(ID3D11DeviceContext* immediate_context,
	//	float dx, float dy, float dw, float dh,
	//	float cx, float cy,
	//	float r, float g, float b, float a,
	//	float angle
	//);
	//virtual void render(ID3D11DeviceContext* immediate_context,
	//	float dx, float dy, float dw, float dh,
	//	float cx, float cy,
	//	float r, float g, float b, float a,
	//	float angle,
	//	float sx, float sy, float sw, float sh
	//);
	virtual void render(ID3D11DeviceContext* immediate_context,
		float dx, float dy, float dw, float dh,
		float r, float g, float b, float a,
		float angle,
		float sx, float sy, float sw, float sh
		);
	virtual void render(ID3D11DeviceContext* immediate_context,
		float dx, float dy, float dw, float dh,
		float r, float g, float b, float a,
		float angle
	);
	virtual void render(ID3D11DeviceContext* immediate_context,
		float dx, float dy, float dw, float dh,
		float cx, float cy,
		float r, float g, float b, float a,
		float angle,
		float sx, float sy, float sw, float sh
	);
	virtual void render(ID3D11DeviceContext* immediate_context,
		float dx, float dy, float dw, float dh,
		float cx, float cy,
		float r, float g, float b, float a,
		float angle
	);
	virtual void render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh);

	void textout(ID3D11DeviceContext* immediate_context, std::string s,
		float x, float y, float w, float h, float r, float g, float b, float a);
	
	
	~Sprite();
	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
	};

protected:
	void draw(ID3D11DeviceContext* immediate_context);
	void createShaderData(ID3D11Device* device, ShaderData shaderData);
	void createVertexData(ID3D11Device* device, const wchar_t* filename);
	//Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	//Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
	//Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;
	

		
};
