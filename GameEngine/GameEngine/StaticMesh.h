#ifndef STATICMESH_H
#define STATICMESH_H
#include "Mesh.h"


class StaticMesh : public Mesh
{
public:
    StaticMesh();


protected:
    std::vector<RawMesh> meshes;

public:
    template < class T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<Mesh>(this), meshes);
    }
};

#endif // !STATICMESH_H
