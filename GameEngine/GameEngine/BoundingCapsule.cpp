#include "BoundingCapsule.h"
#include <vector>
BoundingCapsule::BoundingCapsule(ID3D11Device* device, const float& height, const float& weight, const VECTOR3& offset)
    :height_(height), weight_(weight), offset_(offset)
{
    
    std::vector<Vertex> Vertexes;
    float angle = 0;
    while (angle != 360)
    {
        Vertex Vertex = {};
        Vertex.position.x = weight*cosf(DirectX::XMConvertToRadians(angle));
        Vertex.position.z = weight*sinf(DirectX::XMConvertToRadians(angle));
        Vertex.position.y = height;
        Vertexes.push_back(Vertex);
        
        angle += 15;
    }
    angle = 0;
    while (angle != 360)
    {
        Vertex Vertex = {};
        Vertex.position.x = weight * cosf(DirectX::XMConvertToRadians(angle));
        Vertex.position.z = weight * sinf(DirectX::XMConvertToRadians(angle));
        Vertex.position.y = -height;
        Vertexes.push_back(Vertex);
    
        angle += 15;
    }
    
    
    std::vector<uint32_t> indicess;
    uint32_t i = 0;
    for (i = 0; i < Vertexes.size() /2 -1 ; i++)
    {
        indicess.push_back(i); 
        indicess.push_back(i+1);
    }
    indicess.push_back(static_cast<uint32_t>(Vertexes.size()/2 -1));
    indicess.push_back(0);
    i++;
    for ( i; i < Vertexes.size()-1; i++)
    {
        indicess.push_back(i);
        indicess.push_back(i + 1);
    }
    indicess.push_back(i);
    indicess.push_back(static_cast<uint32_t>(Vertexes.size() / 2));
    for (i = 0; i < Vertexes.size(); i++)
    if (i + 24 < Vertexes.size())
    {
        indicess.push_back(i);
        indicess.push_back(i + 24);
    }

    angle = 0;
    i = static_cast<uint32_t>(Vertexes.size());
    while (angle <= 180)
    {
        Vertex Vertex;
        Vertex.position.y = height + weight * sinf(DirectX::XMConvertToRadians(angle));
        Vertex.position.x = weight * cosf(DirectX::XMConvertToRadians(angle));
        Vertex.position.z = 0;
        Vertexes.push_back(Vertex);
        angle += 15;
    }
    angle = 15;
    uint32_t n = static_cast<uint32_t>(Vertexes.size());
    
    
    while (angle != 360)
    {
        for (uint32_t t = i; t < n; t++)
        {
            Vertex Vertex = {};
            DirectX::XMVECTOR Point = { Vertexes[t].position.x,Vertexes[t].position.y,Vertexes[t].position.z };
            Point = DirectX::XMVector3TransformCoord(Point, DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angle)));
            DirectX::XMStoreFloat3(&Vertex.position, Point);
            Vertexes.push_back(Vertex);
        }
        angle += 15;
    }

    uint32_t n2 = static_cast<uint32_t>(indicess.size());
    for (uint32_t t = n; t < Vertexes.size() -1; t++)
    {
        if ((t - n +1) % 13 == 0 && t != 0)
            t++;
        indicess.push_back(t);
        indicess.push_back(t + 1);
    }

    for (uint32_t t = i; t < Vertexes.size() - 13; t++)
    {
        if (t + 13 < Vertexes.size())
        {
            indicess.push_back(t);
            indicess.push_back(t + 13);
        }
    }

    n = static_cast<uint32_t>(Vertexes.size());
    for (uint32_t t = i; t < n; t++)
    {
        Vertex Vertex = {};
        DirectX::XMVECTOR Point = { Vertexes.at(t).position.x,Vertexes.at(t).position.y,Vertexes.at(t).position.z };
        Point = DirectX::XMVector3TransformCoord(Point, DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(180)));
        DirectX::XMStoreFloat3(&Vertex.position, Point);
        Vertexes.push_back(Vertex);
    }
    i = static_cast<uint32_t>(indicess.size() - 1);
    n = static_cast<uint32_t>(Vertexes.size() - n);
    for (uint32_t t = n2; t < i; t++)
    {
        indicess.push_back(indicess.at(t) + n );
    }


    for (uint32_t i = 0; i < Vertexes.size(); i++)
    {
        Vertexes.at(i).position.x += offset.x;
        Vertexes.at(i).position.y += offset.y;
        Vertexes.at(i).position.z += offset.z;
    }
    createCOMBuffers(device, &Vertexes.at(0), Vertexes.size(), &indicess.at(0), indicess.size());
}
