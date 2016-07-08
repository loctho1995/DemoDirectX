#include <d3dx9.h>
#include <d3d9.h>

class Shader
{
public:
    struct VertexData
    {
        D3DXVECTOR3 Position;
        D3DXCOLOR   Color;
    };

    Shader();
    ~Shader();

    void Init();
    void Render();

    //Geometry variables
    IDirect3DVertexBuffer9          *mVertexBuffer;
    IDirect3DVertexDeclaration9     *mVertexDeclaration;
    IDirect3DIndexBuffer9           *mIndexBuffer;

    //Shader variables
    ID3DXEffect *mShader;

    D3DXMATRIX mMatrixWorld, mMatrixView, mMatrixProject;
    LPDIRECT3DDEVICE9 mDevice;
};

