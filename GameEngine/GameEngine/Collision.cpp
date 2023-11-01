#include "Collision.h"
#include "GameEngine.h"
#include "Ray.h"
#include "CameraManager.h"
VECTOR3 Collision::ObjectAVsObjectBRaycastReturnPoint(OBJ3D& objectA, OBJ3D& objectB, RayType rayType, bool& hit) const
{
    VECTOR3 pos = {};
    for (size_t i = 0; i < objectA.boundingHit_.raylist.size(); i++)
    {
        Ray* ray = objectA.boundingHit_.raylist.at(i).get();
        if (ray->getType() != rayType) continue;
        DirectX::XMVECTOR Begin = DirectX::XMLoadFloat3(&ray->getBeginPos());
        DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&ray->getEndPos());
        DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&objectA.worldTransform);
        Begin = DirectX::XMVector3TransformCoord(Begin, Transform);
        End = DirectX::XMVector3TransformCoord(End, Transform);
        VECTOR3 begin;
        VECTOR3 end;
        DirectX::XMStoreFloat3(&begin, Begin);
        DirectX::XMStoreFloat3(&end, End);
        pos = this->RayCastReturnPoint(objectB, begin, end, hit);
        if (hit)
        {
            DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&pos);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Pos, End);
            float dis = DirectX::XMVectorGetX(DirectX::XMVector3Length(Vec));
            Vec = DirectX::XMVector3Normalize(Vec);
            Pos = DirectX::XMLoadFloat3(&objectA.getPosition());
            DirectX::XMStoreFloat3(&pos,DirectX::XMVectorAdd(Pos, DirectX::XMVectorScale(Vec, dis - 0.05f)));
            break;
        }
    }
    return pos;
}

VECTOR3 Collision::ObjectAVsObjectBRaycastReturnPoint(OBJ3D& objectA, OBJ3D& objectB, Ray& ray, bool& hit) const
{
    VECTOR3 pos = {};
    DirectX::XMVECTOR Begin = DirectX::XMLoadFloat3(&ray.getBeginPos());
    DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&ray.getEndPos());
    DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&objectA.worldTransform);
    Begin = DirectX::XMVector3TransformCoord(Begin, Transform);
    End = DirectX::XMVector3TransformCoord(End, Transform);
    VECTOR3 begin;
    VECTOR3 end;
    DirectX::XMStoreFloat3(&begin, Begin);
    DirectX::XMStoreFloat3(&end, End);
    pos = this->RayCastReturnPoint(objectB, begin, end, hit);
    if (hit)
    {
        DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&pos);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Pos, End);
        float dis = DirectX::XMVectorGetX(DirectX::XMVector3Length(Vec));
        Vec = DirectX::XMVector3Normalize(Vec);
        Pos = DirectX::XMLoadFloat3(&objectA.getPosition());
        DirectX::XMStoreFloat3(&pos, DirectX::XMVectorAdd(Pos, DirectX::XMVectorScale(Vec, dis)));
    }

    return pos;
}

VECTOR3 Collision::RayCastReturnPoint(OBJ3D& obj, const VECTOR3& Begin, const VECTOR3& End, bool& hit) const
{
    DirectX::XMVECTOR beginWorld = DirectX::XMLoadFloat3(&Begin);
    DirectX::XMVECTOR endWorld = DirectX::XMLoadFloat3(&End);

    DirectX::XMVECTOR RayVectorWorld = DirectX::XMVectorSubtract(endWorld, beginWorld);
    DirectX::XMVECTOR RayLengthWorld = DirectX::XMVector3Length(RayVectorWorld);

    float worldRayLength;
    

    DirectX::XMStoreFloat(&worldRayLength, RayLengthWorld);
    SkinnedMesh* pSkinnedMesh = GetFrom<SkinnedMesh>(obj.meshInfor.mesh_.get());
    if(pSkinnedMesh)
    for (auto& mesh : pSkinnedMesh->getMeshRawList())
    {
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&mesh.default_global_transform) * DirectX::XMLoadFloat4x4(&pSkinnedMesh->getDefaultTransform()) * DirectX::XMLoadFloat4x4(&obj.worldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
    
        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(beginWorld, InverseWorldTransform);
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(endWorld, InverseWorldTransform);
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);
    
        float neart;
        DirectX::XMStoreFloat(&neart, Length);
    
        const std::vector<MeshRawVertex>& vertices = mesh.vertices;
        const std::vector<UINT> indices = mesh.indices;
    
        int64_t materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        //DirectX::XMVECTOR HitNormal;
        for (auto& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.index_count; i += 3)
            {
                UINT index = subset.start_index_location + i;
                const MeshRawVertex& a = vertices.at(indices.at(index));
                const MeshRawVertex& b = vertices.at(indices.at(index + 1));
                const MeshRawVertex& c = vertices.at(indices.at(index + 2));
    
                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);
    
                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);
    
                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);
                N = DirectX::XMVector3Normalize(N);
    
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
                float dot;
                DirectX::XMStoreFloat(&dot, Dot);
                if (dot >= 0) continue;
    
                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
    
                ///
                float d1;
                DirectX::XMStoreFloat(&d1, DirectX::XMVector3Dot(SA, N));
                float d2;
                DirectX::XMVECTOR D2 = DirectX::XMVector3Dot(V, N);
                DirectX::XMStoreFloat(&d2, D2);
                float x = (d1 / d2);
                if (x<0.0f || x > neart) continue;
                DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));
                DirectX::XMFLOAT3 p;
                DirectX::XMStoreFloat3(&p, P);
    
                ///
                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(N, Cross1);
                DirectX::XMStoreFloat(&dot, Dot1);
    
                if (dot < 0) continue;
    
                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(N, Cross2);
                DirectX::XMStoreFloat(&dot, Dot2);
    
                if (dot < 0) continue;
    
                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(N, Cross3);
                DirectX::XMStoreFloat(&dot, Dot3);
    
                if (dot < 0) continue;
    
    
                neart = x;
                HitPosition = P;
                //HitNormal = N;
                materialIndex = subset.material_unique_id;
            }
        }
        if (materialIndex >= 0)
        {
            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
            
            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, beginWorld);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);
    
            if (worldRayLength > distance)
            {
                //DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);
    
                //result.distance = distance;
                //result.materialIndex = materialIndex;
                //DirectX::XMStoreFloat3(&result.position, WorldPosition);
                // DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
                VECTOR3 pos;
                DirectX::XMStoreFloat3(&pos, WorldPosition);
                return pos;
            }
        }
    
    }
    else 
    if (obj.sprite3D_)
    {
        
        size_t n = obj.sprite3D_->getIndices().size();
        for (size_t i = 0; i < n-3; i+=3)
        {
            DirectX::XMVECTOR A = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&obj.sprite3D_->getVertices().at(i).position),DirectX::XMLoadFloat4x4(&obj.worldTransform));
            DirectX::XMVECTOR B = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&obj.sprite3D_->getVertices().at(i+1).position),DirectX::XMLoadFloat4x4(&obj.worldTransform));
            DirectX::XMVECTOR C = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&obj.sprite3D_->getVertices().at(i+2).position),DirectX::XMLoadFloat4x4(&obj.worldTransform));
            
            
            
            DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(beginWorld, endWorld);
            DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
            DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);
            
            DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
            DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
            DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

            DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);
            N = DirectX::XMVector3Normalize(N);
            DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, beginWorld);
            float d1;
            DirectX::XMStoreFloat(&d1, DirectX::XMVector3Dot(SA, N));
            float d2;
            DirectX::XMVECTOR D2 = DirectX::XMVector3Dot(V, N);
            DirectX::XMStoreFloat(&d2, D2);
            float x = (d1 / d2);
            DirectX::XMVECTOR P = DirectX::XMVectorAdd(beginWorld, DirectX::XMVectorScale(V, x));
            DirectX::XMFLOAT3 p;
            DirectX::XMStoreFloat3(&p, P);
            float dot;
            ///
            DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
            DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
            DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(N, Cross1);
            DirectX::XMStoreFloat(&dot, Dot1);

            if (dot < 0) continue;

            DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
            DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
            DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(N, Cross2);
            DirectX::XMStoreFloat(&dot, Dot2);

            if (dot < 0) continue;

            DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
            DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
            DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(N, Cross3);
            DirectX::XMStoreFloat(&dot, Dot3);

            if (dot < 0) continue;

            VECTOR3 pos;
            DirectX::XMStoreFloat3(&pos, P);
            hit = true;
            return pos;
        }
        
    }
    return { 0,0,0 };
   
}


VECTOR3 Collision::SphereVsSphereReturnPoint(OBJ3D& objectA, OBJ3D& objectB, bool& hit) const
{
    float radiA = objectA.boundingHit_.sphere_->getRadius();
    float radiB = objectB.boundingHit_.sphere_->getRadius();


    DirectX::XMVECTOR PointA = DirectX::XMLoadFloat3(&objectA.getPosition());
    DirectX::XMVECTOR PointB = DirectX::XMLoadFloat3(&objectB.getPosition());
    DirectX::XMVECTOR BA = DirectX::XMVectorSubtract(PointA, PointB);
    float dis = DirectX::XMVectorGetX(DirectX::XMVector3Length(BA));
    
    if (radiA + radiB > dis)
    {
        hit = true;
        BA = DirectX::XMVector3Normalize(BA);
        VECTOR3 pos;
        DirectX::XMStoreFloat3(&pos,DirectX::XMVectorAdd(PointB, DirectX::XMVectorScale(BA, radiA + radiB + 0.01f)));
        return pos;
    }
    hit = false;
    return VECTOR3();
}

VECTOR3 Collision::CapsuleVsCapsuleReturnPoint(OBJ3D& objectA, OBJ3D& objectB, bool& hit) const
{
    BoundingCapsule& capsuleA = *objectA.boundingHit_.capsule_;
    BoundingCapsule& capsuleB = *objectB.boundingHit_.capsule_;

    VECTOR3 aStart = capsuleA.getOffset();
    VECTOR3 aEnd = capsuleA.getOffset();

    aStart.y -= capsuleA.getHeight();
    aEnd.y += capsuleA.getHeight();
    DirectX::XMVECTOR AStart = DirectX::XMLoadFloat3(&aStart);
    DirectX::XMVECTOR AEnd = DirectX::XMLoadFloat3(&aEnd);
    AStart = DirectX::XMVector3TransformCoord(AStart, DirectX::XMLoadFloat4x4(&objectA.worldTransform));
    AEnd = DirectX::XMVector3TransformCoord(AEnd, DirectX::XMLoadFloat4x4(&objectA.worldTransform));
    DirectX::XMVECTOR A = DirectX::XMVectorSubtract(AEnd, AStart);
    DirectX::XMVECTOR Va = DirectX::XMVector3Normalize(A);


    VECTOR3 bStart = capsuleB.getOffset();
    VECTOR3 bEnd = capsuleB.getOffset();

    bStart.y -= capsuleB.getHeight();
    bEnd.y += capsuleB.getHeight();
    DirectX::XMVECTOR BStart = DirectX::XMLoadFloat3(&bStart);
    DirectX::XMVECTOR BEnd = DirectX::XMLoadFloat3(&bEnd);
    BStart = DirectX::XMVector3TransformCoord(BStart, DirectX::XMLoadFloat4x4(&objectB.worldTransform));
    BEnd = DirectX::XMVector3TransformCoord(BEnd, DirectX::XMLoadFloat4x4(&objectB.worldTransform));
    DirectX::XMVECTOR B = DirectX::XMVectorSubtract(BEnd, BStart);
    DirectX::XMVECTOR Vb = DirectX::XMVector3Normalize(B);


    float angle;
    float distance = capsuleA.getWeight() + capsuleB.getWeight();
    DirectX::XMStoreFloat(&angle, DirectX::XMVector3AngleBetweenNormals(Va, Vb));
    if (angle == 0 || fabsf(angle - DirectX::XM_PI) <= 0.0001f )
    {
        float dot;
        DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(DirectX::XMVectorSubtract(BStart, AStart), Va));
        DirectX::XMVECTOR Point = DirectX::XMVectorAdd(AStart, DirectX::XMVectorScale(Va, dot));
        
        float disPoint = DirectX::XMVectorGetX(
            DirectX::XMVectorAdd(DirectX::XMVector3Length(DirectX::XMVectorSubtract(Point, AStart)), 
                DirectX::XMVector3Length(DirectX::XMVectorSubtract(Point, AEnd)))
        );
        if (disPoint > capsuleA.getHeight() * 2)
            if (DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(AEnd, Point)))
            > DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(AStart, Point))))
            {
                Point = AStart;
            }
            else
                Point = AEnd;

        float dis1 = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(Point, BStart)));
        float dis2 = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(Point, BEnd)));
        float dis = 0;
        (dis1 < dis2) ? dis = dis1 : dis = dis2;
        if (dis < distance)
        {
            DirectX::XMVECTOR PosA = DirectX::XMLoadFloat3(&objectA.getPosition());
            DirectX::XMVECTOR PointPosA = DirectX::XMVectorSubtract(PosA, Point);
            float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(PointPosA));
            Point = DirectX::XMVectorAdd(BStart,
                DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(Point, BStart)), distance+0.01f));
            PosA = DirectX::XMVectorAdd(Point, DirectX::XMVectorScale(DirectX::XMVector3Normalize(PointPosA), length));
            VECTOR3 pos;
            DirectX::XMStoreFloat3(&pos, PosA);
            hit = true;
            return pos;
        }
        else
        {
            hit = false;
            return {};
        }

    }

    float dot;
    DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(Va, Vb));
    float t;
    DirectX::XMStoreFloat(&t,
        DirectX::XMVector3Dot(
            DirectX::XMVectorSubtract(Va, DirectX::XMVectorScale(Vb, dot)),
            DirectX::XMVectorSubtract(BStart, AStart)
        ));

    t /= (1 - powf(dot, 2));
    DirectX::XMVECTOR APoint = DirectX::XMVectorAdd(AStart, DirectX::XMVectorScale(Va, t));


    float disPointA = DirectX::XMVectorGetX(
        DirectX::XMVectorAdd(DirectX::XMVector3Length(DirectX::XMVectorSubtract(AEnd, APoint)),
            DirectX::XMVector3Length(DirectX::XMVectorSubtract(AStart, APoint)))
    );
    float lengthA = capsuleA.getHeight() * 2;

    if (disPointA > lengthA)
    {

        if (disPointA > lengthA)
            if (DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(AEnd, APoint)))
            > DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(AStart, APoint))))
            {
                APoint = AStart;
            }
            else
                APoint = AEnd;

    }





    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(APoint, BStart);
    DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(Vec, Vb));
    DirectX::XMVECTOR BPoint = DirectX::XMVectorAdd(BStart, DirectX::XMVectorScale(Vb, dot));


    

    float disPointB = DirectX::XMVectorGetX(
        DirectX::XMVectorAdd(DirectX::XMVector3Length(DirectX::XMVectorSubtract(BEnd, BPoint)),
            DirectX::XMVector3Length(DirectX::XMVectorSubtract(BStart, BPoint)))
    );
    float lengthB = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(BEnd, BStart)));
   
    if (disPointB > lengthB)
    {
        if (disPointB > lengthB)
            if (DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(BEnd, BPoint)))
            > DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(BStart, BPoint))))
            {
                BPoint = BStart;
            }
            else
                BPoint = BEnd;
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(BPoint, AStart);
        DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(Vec, Va));
        APoint = DirectX::XMVectorAdd(AStart, DirectX::XMVectorScale(Va, dot));
        disPointA = DirectX::XMVectorGetX(
            DirectX::XMVectorAdd(DirectX::XMVector3Length(DirectX::XMVectorSubtract(AEnd, APoint)),
                DirectX::XMVector3Length(DirectX::XMVectorSubtract(AStart, APoint)))
        );
        
        if (disPointA > lengthA)
        {

            if (disPointA > lengthA)
                if (DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(AEnd, APoint)))
            > DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(AStart, APoint))))
                {
                    APoint = AStart;
                }
                else
                    APoint = AEnd;

        }


    }


    float dis = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(BPoint, APoint)));
   
    if (dis > distance)
    {
        hit = false;
        return {};
    }

    DirectX::XMVECTOR PosA = DirectX::XMLoadFloat3(&objectA.getPosition());
    DirectX::XMVECTOR APointPosA = DirectX::XMVectorSubtract(PosA, APoint);
    
    APoint = DirectX::XMVectorAdd(BPoint, 
        DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(APoint, BPoint)),distance +0.01f)
    );

    DirectX::XMVECTOR Point = DirectX::XMVectorAdd(APoint, 
        DirectX::XMVectorScale(DirectX::XMVector3Normalize(APointPosA), DirectX::XMVectorGetX(DirectX::XMVector3Length(APointPosA)))
    );

    hit = true;
    VECTOR3 pos;
    DirectX::XMStoreFloat3(&pos, Point);
    return pos;
}

VECTOR3 Collision::RayCastMousePointReturnPoint(const VECTOR2& mousePosition, OBJ3D& obj, bool& hit) const
{

    CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());
    std::shared_ptr<Camera> camera = cameraManager->getCamera(CameraName::GameScene);
    D3D11_VIEWPORT viewPort = GameEngine::get()->getViewPort(FrameBufferName::MAINBLITFRAMEBUFFER);
#ifdef _DEBUG
    camera = cameraManager->getCamera(CameraName::DebugScene);
    viewPort = GameEngine::get()->getViewPort(FrameBufferName::FRAMESCENEDEBUG);
#endif // _DEBUG

    DirectX::XMVECTOR Begin = { mousePosition.x,mousePosition.y,0 };
    DirectX::XMVECTOR End = { mousePosition.x,mousePosition.y,1 };
   
    
    DirectX::XMMATRIX world = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    DirectX::XMMATRIX view = camera->GetView3D();
    DirectX::XMVECTOR WorldBegin = DirectX::XMVector3Unproject(
        Begin,
        viewPort.TopLeftX, viewPort.TopLeftY,
        viewPort.Width, viewPort.Height,
        viewPort.MinDepth, viewPort.MaxDepth,
        camera->GetProjection3D(), view,
        world
    );
    DirectX::XMVECTOR WorldEnd = DirectX::XMVector3Unproject(
        End,
        viewPort.TopLeftX, viewPort.TopLeftY,
        viewPort.Width, viewPort.Height,
        viewPort.MinDepth, viewPort.MaxDepth,
        camera->GetProjection3D(), view,
        world
    );
    hit = false;


    VECTOR3 worldBegin;
    VECTOR3 worldEnd;

    DirectX::XMStoreFloat3(&worldBegin, WorldBegin);
    DirectX::XMStoreFloat3(&worldEnd, WorldEnd);


    return RayCastReturnPoint(obj, worldBegin, worldEnd, hit);
}

VECTOR3 Collision::ScreenToWorldTransition(const VECTOR2& mousePosition, const float& maxDis)
{
    CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());
    std::shared_ptr<Camera> camera = cameraManager->getCamera(CameraName::GameScene);
    D3D11_VIEWPORT viewPort = GameEngine::get()->getViewPort(FrameBufferName::MAINBLITFRAMEBUFFER);
#ifdef _DEBUG
    camera = cameraManager->getCamera(CameraName::DebugScene);
    viewPort = GameEngine::get()->getViewPort(FrameBufferName::FRAMESCENEDEBUG);
#endif // _DEBUG

    DirectX::XMVECTOR Begin = { mousePosition.x,mousePosition.y,0 };
    DirectX::XMVECTOR End = { mousePosition.x,mousePosition.y,1 };


    DirectX::XMMATRIX world = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    DirectX::XMMATRIX view = camera->GetView3D();
    DirectX::XMVECTOR WorldBegin = DirectX::XMVector3Unproject(
        Begin,
        viewPort.TopLeftX, viewPort.TopLeftY,
        viewPort.Width, viewPort.Height,
        viewPort.MinDepth, viewPort.MaxDepth,
        camera->GetProjection3D(), view,
        world
    );
    DirectX::XMVECTOR WorldEnd = DirectX::XMVector3Unproject(
        End,
        viewPort.TopLeftX, viewPort.TopLeftY,
        viewPort.Width, viewPort.Height,
        viewPort.MinDepth, viewPort.MaxDepth,
        camera->GetProjection3D(), view,
        world
    );

    VECTOR3 worldBegin;
    VECTOR3 worldEnd;

    DirectX::XMStoreFloat3(&worldBegin, WorldBegin);
    DirectX::XMStoreFloat3(&worldEnd, WorldEnd);

    VECTOR3 result = worldBegin + MyMath::get()->ScaleVector3(worldEnd - worldBegin, maxDis);
    return result;
}

bool Collision::RayCastMousePointReturnBool(const VECTOR2& mousePosition, OBJ3D& obj) const
{
    bool hit = false;
    RayCastMousePointReturnPoint(mousePosition, obj, hit);
    return hit;
}
