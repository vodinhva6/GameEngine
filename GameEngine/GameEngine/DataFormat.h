#ifndef INCLUDED_FORMAT
#define INCLUDED_FORMAT
#include <DirectXMath.h>
#include <functional>
#include <memory>
typedef void* pVoid;
typedef std::shared_ptr<void> pSmartVoid;
template <class T>
T* GetFrom(pSmartVoid cast)
{
    return static_cast<T*>(cast.get());
}
template <class T>
T* GetFrom(pVoid cast)
{
    return static_cast<T*>(cast);
}
template <class T>
pVoid SetTo(T* invCast)
{
    return static_cast<pVoid>(invCast);
}
template <class T>
std::shared_ptr<T> GetFromPoint(pSmartVoid cast)
{
    return std::static_pointer_cast<T>(cast);
}
template <class T>
pSmartVoid SetToPoint(std::shared_ptr<T> invCast)
{
    return std::static_pointer_cast<void>(invCast);
}
template <class Res>
class RenderId
{
public:
    RenderId(Res* res);
    
    ~RenderId();
private:
    void* res_;
};

class VECTOR4;
class VECTOR3;
class VECTOR2 : public DirectX::XMFLOAT2
{
public:
    VECTOR2() : DirectX::XMFLOAT2(0, 0) {}
    VECTOR2(float x, float y) : DirectX::XMFLOAT2(x, y) {}
    VECTOR2(const VECTOR2& v) { x = v.x; y = v.y; }
    VECTOR2(const VECTOR3& v);
    VECTOR2(const VECTOR4& v);
    ~VECTOR2() {}

    VECTOR2& operator=(const VECTOR2&);
    VECTOR2& operator=(const VECTOR3&);
    VECTOR2& operator+=(const VECTOR2&);
    VECTOR2& operator-=(const VECTOR2&);
    VECTOR2& operator*=(float);
    VECTOR2& operator/=(float);
    bool operator<(const VECTOR2&);

    VECTOR2 operator+() const;
    VECTOR2 operator-() const;

    VECTOR2 operator+(const VECTOR2&) const;
    VECTOR2 operator-(const VECTOR2&) const;
    VECTOR2 operator*(float) const;
    friend VECTOR2 operator*(float, const VECTOR2&);
    VECTOR2 operator/(float) const;

    bool operator == (const VECTOR2&) const;
    bool operator != (const VECTOR2&) const;
};




class VECTOR3 : public DirectX::XMFLOAT3
{
public:
    VECTOR3() : DirectX::XMFLOAT3(0, 0, 0) {}
    VECTOR3(float x, float y, float z) : DirectX::XMFLOAT3(x, y, z) {}
    VECTOR3(const VECTOR3& v) { x = v.x; y = v.y; z = v.z; }
    VECTOR3(const VECTOR4& v);
    ~VECTOR3() {}

    VECTOR3& operator=(const VECTOR3& v);
    VECTOR3& operator=(const VECTOR4& v);
    VECTOR3& operator+=(const VECTOR3&);
    VECTOR3& operator-=(const VECTOR3&);

    VECTOR3& operator*=(float);
    VECTOR3& operator/=(float);

    VECTOR3 operator+() const;
    VECTOR3 operator-() const;

    VECTOR3 operator+(const VECTOR3&) const;
    VECTOR3 operator-(const VECTOR3&) const;
    VECTOR3 operator*(float) const;
    friend VECTOR3 operator*(float, const VECTOR3&);
    VECTOR3 operator/(float) const;

    bool operator == (const VECTOR3&) const;
    bool operator != (const VECTOR3&) const;

    VECTOR3(const VECTOR2& v);
    // VECTOR3‚ÉVECTOR2‚ð‘ã“ü
    VECTOR3& operator=(const VECTOR2&);
};




class VECTOR4 : public DirectX::XMFLOAT4
{
public:
    VECTOR4() : DirectX::XMFLOAT4(0, 0, 0, 0) {}
    VECTOR4(const DirectX::XMFLOAT4& data) : DirectX::XMFLOAT4(data) {}
    VECTOR4& operator+=(const VECTOR4&);
    VECTOR4& operator-=(const VECTOR4&);
    VECTOR4& operator+=(const VECTOR3&);
    VECTOR4& operator=(const VECTOR3&);

    VECTOR4 operator-(const VECTOR4&) const;
    VECTOR4 operator*(const VECTOR4&) const;

    VECTOR4& operator*=(float);
    VECTOR4& operator/=(float);

    VECTOR4(float x, float y, float z, float w) : DirectX::XMFLOAT4(x, y, z, w) {}
    ~VECTOR4() {}
};






enum class DepthStencilStates
{
    DepthTestOff_DepthWriteOff,
    DepthTestOff_DepthWriteOn,
    DepthTestOn_DepthWriteOff,
    DepthTestOn_DepthWriteOn,
};
const int MAX_DEPTH_STATES = sizeof(DepthStencilStates) + 1;

enum class RasterizerStates
{
    CullBack_DrawTriagles,
    CullFront_DrawTriagles,
    CullBack_DrawLines,
    CullFront_DrawLines,
    CullNON_DrawTriagles,
};
const int MAX_RASTERIZER_STATES = sizeof(RasterizerStates) + 1;
enum BlendStates
{
    Default,
    Alpha,
    AlphaBlendAlphaCoverage,
    Subtract,
    RevSubtract,
    Additively,
};
const int MAX_BLEND_STATES = 6;
enum class SamplerStates
{
    Point,
    Linear,
    Anisotropic,
    LinearClamp,
    Shadow,
};
const int MAX_SAMPLER_STATES = 5;
enum ObjectType
{
    Object2D,
    Object3D
};

enum DrawType
{
    BACKGROUND,
    TEXTUREOBJECT,
    Text,
    Cube,
    Box,
    StaticMeshObject,
    SkinnedMeshObject,
    EffectObject,
    SKYBOX,
    BOUDING,
    SPRITE3DObject,
    MeshLightWithEmissionShader
};

struct DrawStates
{
    DepthStencilStates depthState;
    RasterizerStates rasterizerState;
    SamplerStates samplerState;
    BlendStates blendState;
    DrawType    drawType;
    template < class T>
    void serialize(T& archive)
    {
        archive(depthState, rasterizerState, samplerState, blendState, drawType);
    }
};

struct ShaderData
{
    const char* vsfile;
    const char* psfile;
};



enum SkinnedMeshObjectAxis
{
   
    TEXTURE_FLIP_Z,
    LHS_Y_UP,
    RHS_Z_UP,
    LHS_Z_UP,
    DEFAULT,
};


#define SizeOfConstantBuffer 7
enum ConstantBufferName
{
    SCENEBUFFER,
    LIGHTBUFFER,
    FILTERBUFFER,
    SHADOWBUFFER,
    //UVSCROLLBUFFER,
    FOGBUFFER,
    //BLURBUFFER
};

template <class myType>
void SafeDelete(myType*& T) 
{
    if (T != nullptr)
        delete T;
    T = nullptr;
}


#endif // !INCLUDED_FORMAT

