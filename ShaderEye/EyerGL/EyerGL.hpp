#ifndef	EYER_LIB_GL_AV_H
#define	EYER_LIB_GL_AV_H

#include "EyerCore/EyerCore.hpp"
#include <vector>

#ifdef QT_EYER_PLAYER
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#else
#endif

namespace Eyer
{
    class EyerGLWindow;
    class EyerGLWindowPrivate;
    class EyerGLCMD;

    class EyerGLComponent;
    class EyerGLTextDraw;
    class EyerGLSingleTextureDraw;
    class EyerGLFrameDraw;

    class EyerGLShader;
    class EyerGLProgram;
    class EyerGLVAO;
    class EyerGLTexture;

    class _EyerGLContext
    {
    };

#ifdef QT_EYER_PLAYER
#define EyerGLContext QOpenGLFunctions_3_3_Core
#else
#define EyerGLContext _EyerGLContext
#endif

    class EyerGLCMD
    {
    public:
        virtual ~EyerGLCMD()
        {

        }
    };

    enum EyerGLShaderType
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER
    };

    class EyerGLShaderError
    {
    public:
        EyerGLShaderType shaderType;
        int errorLen;
        EyerString error;
    };

    class EyerGLProgramError
    {
    public:
        int errorLen;
        EyerString error;
    };

    class EyerGLShader : public EyerGLCMD
    {
    private:
        EyerGLShaderType type = EyerGLShaderType::VERTEX_SHADER;
        EyerString src;
        unsigned int shaderId = 0;

        EyerGLContext * ctx = nullptr;
    public:
        EyerGLShader(EyerGLShaderType type, EyerString src, EyerGLContext * _ctx = nullptr);
        ~EyerGLShader();

        int Compile(EyerGLShaderError & shaderError);

        unsigned int GL_GetShaderId();
    };

    class EyerGLProgram : public EyerGLCMD
    {
    private:
        EyerString vertexShaderSrc;
        EyerString fragmentShaderSrc;

        unsigned int programId = 0;

        EyerGLContext * ctx = nullptr;
    public:
        EyerGLProgram(EyerString vertexShaderSrc, EyerString fragmentShaderSrc, EyerGLContext * _ctx = nullptr);
        ~EyerGLProgram();
        int LinkProgram(EyerGLShaderError & vertexShaderError, EyerGLShaderError & fragmentShaderError, EyerGLProgramError & programError);
        int UseProgram();

        int PutUniform1i(EyerString key, int value);
        int PutMatrix4fv(EyerString key, EyerMat4x4 & mat);
        int PutUniform1f(EyerString key, float value);
    };

    class EyerGLVAO : public EyerGLCMD
    {
    private:
        unsigned int VAOId = 0;
        unsigned int EBOId = 0;
        std::vector<unsigned int> vboList;

        int DrawTime = 0;

        EyerGLContext * ctx = nullptr;
    public:
        EyerGLVAO(EyerGLContext * ctx = nullptr);
        ~EyerGLVAO();

        int SetEBO(unsigned int * EBOdata, int bufferSize);
        int AddVBO(float * VBOdata, int bufferSize, int layout, int size = 3, unsigned int stride = 0);

        int DrawVAO();
    };

    class EyerGLDrawTexture
    {
    public:
        EyerString uniformName;
        EyerGLTexture * texture = nullptr;
    };

    class EyerGLDraw : public EyerGLCMD
    {
    private:
        EyerString vertexShaderSrc;
        EyerString fragmentShaderSrc;

        EyerGLProgram * program = nullptr;
        EyerGLVAO * vao = nullptr;

        EyerGLContext * ctx = nullptr;
    public:
        EyerGLDraw(EyerString vertexShaderSrc, EyerString fragmentShaderSrc, EyerGLContext * ctx = nullptr);
        ~EyerGLDraw();

        int Init(EyerGLShaderError & vertexShaderError, EyerGLShaderError & fragmentShaderError, EyerGLProgramError & programError);

        int SetVAO(EyerGLVAO * vao);
        int PutTexture(EyerString uniform, EyerGLTexture * texture, int textureIndex = 0);
        int PutMatrix4fv(EyerString uniform, EyerMat4x4 & mat);
        int PutUniform1f(EyerString uniform, float val);
        int PutUniform1i(EyerString uniform, int val);

        int Draw();
    };

    class EyerGLTexture : public EyerGLCMD
    {
    private:
        unsigned int textureId = 0;
        EyerGLContext * ctx = nullptr;
    public:
        EyerGLTexture(EyerGLContext * ctx = nullptr);
        ~EyerGLTexture();

        unsigned int GL_GetTextureId();

        int SetDataRedChannel(unsigned char * data,int width,int height);
        int SetDataRGChannel(unsigned char * data,int width,int height);
        int SetDataRGBChannel(unsigned char * data,int width,int height);
        int SetDataRGBAChannel(unsigned char * data,int width,int height);
    };

    class EyerGLFrameBuffer : public EyerGLCMD
    {
    private:
        EyerLinkedList<EyerGLDraw *> drawList;
        EyerLinkedList<EyerGLComponent *> componentList;

        unsigned int fbo = 0;

        int width = 0;
        int height = 0;

        EyerGLTexture * texture = nullptr;
        EyerGLContext * ctx = nullptr;
    public:
        EyerGLFrameBuffer(int w, int h, EyerGLTexture * texture = nullptr, EyerGLContext * ctx = nullptr);
        ~EyerGLFrameBuffer();

        int AddDraw(EyerGLDraw * draw);
        int AddComponent(EyerGLComponent * component);
        int ClearAllComponent();

        int Clear();
        int Clear(float r, float g, float b, float a);

        int Draw();

        int ReadPixel(int x, int y, int width, int height, unsigned char * data);
    };


    class EyerGLComponent
    {
    public:
        int width = 0;
        int height = 0;
    public:
        virtual ~EyerGLComponent();

        virtual int Draw() = 0;

        int Viewport(int w, int h);
    };
}

#endif
