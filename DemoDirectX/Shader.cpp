#include "Shader.h"
#include "GameGlobal.h"

Shader::Shader()
{
    mVertexBuffer = nullptr;
    mVertexDeclaration = nullptr;
    mIndexBuffer = nullptr;
    mShader = nullptr;

    Init();
}

Shader::~Shader()
{

}

void Shader::Init()
{
    unsigned int indexDataSize = 6 * sizeof(unsigned int);

    D3DXMatrixIdentity(&mMatrixWorld);
    D3DXMatrixIdentity(&mMatrixView);
    D3DXMatrixIdentity(&mMatrixProject);

    D3DXMatrixTranslation(&mMatrixView, 0, 0, 2);
    D3DXMatrixPerspectiveFovLH(&mMatrixProject, D3DXToRadian(75), 800.0f / 600.0f, 0.01f, 100.0f);

    if (GameGlobal::GetCurrentDevice()->CreateIndexBuffer(indexDataSize, 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &mIndexBuffer, 0))
    {
        GAMELOG("Create Index Buffer Successed!");
    }

    //Create Vertex Data
    VertexData vertexData[]
    {
        {D3DXVECTOR3(-0.5f, -0.5f, 0), D3DCOLOR_XRGB(255, 0, 0)},
        { D3DXVECTOR3(0.0f, 0.5f, 0), D3DCOLOR_XRGB(0, 255, 0) },
        { D3DXVECTOR3(0.5f, 0.5f, 0), D3DCOLOR_XRGB(0, 0, 255) }
    };

    unsigned int vertexDataSize = 3 * sizeof(unsigned int);

    //Create Vertex Buffer
    HRESULT rs = GameGlobal::GetCurrentDevice()->CreateVertexBuffer(vertexDataSize, 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &mVertexBuffer, 0);

    switch (rs)
    {
    case D3DERR_INVALIDCALL:
        GAMELOG("D3DERR_INVALIDCALL");
        break;

    case D3DERR_OUTOFVIDEOMEMORY:
        GAMELOG("D3DERR_OUTOFVIDEOMEMORY");
        break;

    case E_OUTOFMEMORY:
        GAMELOG("E_OUTOFMEMORY");
        break;

    default:
        break;
    }

    void *tempVertexBuffer;
    mVertexBuffer->Lock(0, vertexDataSize, &tempVertexBuffer, 0);
    {
        memcpy(tempVertexBuffer, vertexData, vertexDataSize);
    }
    mVertexBuffer->Unlock();

    unsigned int indexData[]
    {
        0, 1, 2,
        0, 2, 1
    };

    //Create Index Buffer
    GameGlobal::GetCurrentDevice()->CreateIndexBuffer(indexDataSize, 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &mIndexBuffer, 0);
    
    void *tempIndexBuffer;
    mIndexBuffer->Lock(0, indexDataSize, &tempIndexBuffer, 0);
    {
        memcpy(tempIndexBuffer, mIndexBuffer, indexDataSize);
    }
    mIndexBuffer->Unlock();

    //Create Vertex Declaration
    D3DVERTEXELEMENT9 declaration[]
    {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
        D3DDECL_END(),
    };

    GameGlobal::GetCurrentDevice()->CreateVertexDeclaration(declaration, &mVertexDeclaration);

    //Create Shader
    D3DXCreateEffectFromFileA(GameGlobal::GetCurrentDevice(), "shader.fx", 0, 0, 0, 0, &mShader, 0);
}

void Shader::Render()
{
    unsigned int numPass = 0;
    mShader->Begin(&numPass, 0);
    {
        for (unsigned int i = 0; i < numPass; ++i)
        {
            mShader->BeginPass(i);
            mShader->SetMatrix("WorldViewProj", &(mMatrixProject * mMatrixWorld * mMatrixView));
            GameGlobal::GetCurrentDevice()->SetVertexDeclaration(mVertexDeclaration); 
            GameGlobal::GetCurrentDevice()->SetStreamSource(0, mVertexBuffer, 0, sizeof(VertexData));
            GameGlobal::GetCurrentDevice()->SetIndices(mIndexBuffer);
            mShader->CommitChanges();
            GameGlobal::GetCurrentDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 2);
            mShader->EndPass();
        }
    }
    mShader->End();
}
