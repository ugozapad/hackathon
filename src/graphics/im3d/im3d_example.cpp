#include "pch.h"
#include "im3d_example.h"

#include "teapot.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace Im3d;

#ifdef IM3D_COMPILER_MSVC
	#pragma warning(disable: 4996) // vsnprintf

	#pragma warning(disable: 4311) // typecast
	#pragma warning(disable: 4302) //    "
	#pragma warning(disable: 4312) //    "
#endif

static const char* StripPath(const char* _path) 
{
	int i = 0, last = 0;
	while (_path[i] != '\0') {
		if (_path[i] == '\\' || _path[i] == '/') {
			last = i + 1;
		}
		++i;
	}
	return &_path[last];
}

/******************************************************************************/
#if defined(IM3D_PLATFORM_WIN)
	// force Nvidia/AMD drivers to use the discrete GPU
	extern "C" {
		__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
		__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	}

	static LARGE_INTEGER g_SysTimerFreq;

	const char* Im3d::GetPlatformErrorString(DWORD _err)
	{
		const int kErrMsgMax = 1024;
		static char buf[kErrMsgMax];
		buf[0] = '\0';
		IM3D_VERIFY(
			FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
				nullptr, 
				_err,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)buf, 
				kErrMsgMax, 
				nullptr
			) != 0
		);
		return buf;
	}
	
	static LRESULT CALLBACK WindowProc(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam)
	{


		return DefWindowProc(_hwnd, _umsg, _wparam, _lparam);
	}
	
	static bool InitWindow(int& _width_, int& _height_, const char* _title)
	{

		return true;
	}
	
	static void ShutdownWindow()
	{

	}
	
	#if defined(IM3D_OPENGL)

		static bool InitOpenGL(int _vmaj, int _vmin)
		{

		 // set the window pixel format
			PIXELFORMATDESCRIPTOR pfd = {};
			pfd.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion     = 1;
			pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_GENERIC_ACCELERATED;
			pfd.iPixelType   = PFD_TYPE_RGBA;
			pfd.cColorBits   = 24;
			pfd.cDepthBits   = 24;
			pfd.dwDamageMask = 8;
			int pformat = 0;

		 // check the platform supports the requested GL version
			GLint platformVMaj, platformVMin;
			glAssert(glGetIntegerv(GL_MAJOR_VERSION, &platformVMaj));
			glAssert(glGetIntegerv(GL_MINOR_VERSION, &platformVMin));
			_vmaj = _vmaj < 0 ? platformVMaj : _vmaj;
			_vmin = _vmin < 0 ? platformVMin : _vmin;
			if (platformVMaj < _vmaj || (platformVMaj >= _vmaj && platformVMin < _vmin))
			{
				fprintf(stderr, "OpenGL version %d.%d is not available (available version is %d.%d).", _vmaj, _vmin, platformVMaj, platformVMin);
				fprintf(stderr, "This error may occur if the platform has an integrated GPU.");

				return false;
			}
			



			fprintf(stdout, "OpenGL context:\n\tVersion: %s\n\tGLSL Version: %s\n\tVendor: %s\n\tRenderer: %s\n",
				GlGetString(GL_VERSION),
				GlGetString(GL_SHADING_LANGUAGE_VERSION),
				GlGetString(GL_VENDOR),
				GlGetString(GL_RENDERER)
				);

			if (_vmaj == 3 && _vmin == 1)
			{
			 // check that the uniform blocks size is at least 64kb
				GLint maxUniformBlockSize; 
				glAssert(glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize));
				if (maxUniformBlockSize < (64*1024))
				{
					IM3D_ASSERT(false);
					fprintf(stderr, "GL_MAX_UNIFORM_BLOCK_SIZE is less than 64kb (%dkb)", maxUniformBlockSize / 1024);
					return false;
				}
			}

			return true;
		}
		
		static void ShutdownOpenGL()
		{

		}
		
	#elif defined(IM3D_DX11)
		#include <d3dcompiler.h>	

		static bool InitDx11()
		{
			g_Example->m_dxgiSwapChain   = nullptr;
			g_Example->m_d3dDevice       = nullptr;
			g_Example->m_d3dDeviceCtx    = nullptr;
			g_Example->m_d3dRenderTarget = nullptr;
			g_Example->m_d3dDepthStencil = nullptr;

			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.OutputWindow = g_Example->m_hwnd;
			swapChainDesc.Windowed = TRUE;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.BufferCount = 2;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			swapChainDesc.SampleDesc.Count = 1;

			UINT createDeviceFlags = 0;
			//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
			D3D_FEATURE_LEVEL featureLevel;
			const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0, };
			dxAssert(D3D11CreateDeviceAndSwapChain(
				nullptr, 
				D3D_DRIVER_TYPE_HARDWARE, 
				nullptr, 
				createDeviceFlags, 
				featureLevelArray, 
				1, 
				D3D11_SDK_VERSION, 
				&swapChainDesc,
				&g_Example->m_dxgiSwapChain,
				&g_Example->m_d3dDevice, 
				&featureLevel, 
				&g_Example->m_d3dDeviceCtx
				));
			if (!g_Example->m_dxgiSwapChain || !g_Example->m_d3dDevice || !g_Example->m_d3dDeviceCtx)
			{
				fprintf(stderr, "Error initializing DirectX");
				return false;
			}

			ID3D11Texture2D* backBuffer;
			D3D11_TEXTURE2D_DESC backBufferDesc;
			dxAssert(g_Example->m_dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer));
			backBuffer->GetDesc(&backBufferDesc);
			dxAssert(g_Example->m_d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &g_Example->m_d3dRenderTarget));
			g_Example->m_d3dDepthStencil = CreateDepthStencil(backBufferDesc.Width, backBufferDesc.Height, DXGI_FORMAT_D24_UNORM_S8_UINT);
			g_Example->m_d3dDeviceCtx->OMSetRenderTargets(1, &g_Example->m_d3dRenderTarget, g_Example->m_d3dDepthStencil);
			backBuffer->Release();
			
			return true;
		}
		
		static void ShutdownDx11()
		{
			if (g_Example->m_d3dDepthStencil) g_Example->m_d3dDepthStencil->Release();
			if (g_Example->m_d3dRenderTarget) g_Example->m_d3dRenderTarget->Release();
			if (g_Example->m_dxgiSwapChain)   g_Example->m_dxgiSwapChain->Release();
			if (g_Example->m_d3dDeviceCtx)    g_Example->m_d3dDeviceCtx->Release();
			if (g_Example->m_d3dDevice)       g_Example->m_d3dDevice->Release();
		}
		
	#endif // graphics
	
#endif // platform

/******************************************************************************/
static void Append(const char* _str, Vector<char>& _out_)
{
	while (*_str)
	{
		_out_.push_back(*_str);
		++_str;
	}
}
static void AppendLine(const char* _str, Vector<char>& _out_)
{
	Append(_str, _out_);
	_out_.push_back('\n');
}
static bool LoadShader(const char* _path, const char* _defines, Vector<char>& _out_)
{
	fprintf(stdout, "Loading shader: '%s'", StripPath(_path));
	if (_defines)
	{
		fprintf(stdout, " ");
		while (*_defines != '\0')
		{
			fprintf(stdout, " %s,", _defines);
			Append("#define ", _out_);
			AppendLine(_defines, _out_);
			_defines = strchr(_defines, 0);
			IM3D_ASSERT(_defines);
			++_defines;
		}
	}
	fprintf(stdout, "\n");
	
	FILE* fin = fopen(_path, "rb");
	if (!fin)
	{
		fprintf(stderr, "Error opening '%s'\n", _path);
		return false;
	}
	IM3D_VERIFY(fseek(fin, 0, SEEK_END) == 0); // not portable but should work almost everywhere
	long fsize = ftell(fin);
	IM3D_VERIFY(fseek(fin, 0, SEEK_SET) == 0);
	
	int srcbeg = _out_.size();
	_out_.resize(srcbeg + fsize, '\0');
	if (fread(_out_.data() + srcbeg, 1, fsize, fin) != fsize)
	{
		fclose(fin);
		fprintf(stderr, "Error reading '%s'\n", _path);
		return false;
	}
	fclose(fin);
	_out_.push_back('\0');

	return true;
}

#if defined(IM3D_OPENGL)
	GLuint Im3d::LoadCompileShader(GLenum _stage, const char* _path, const char* _defines)
	{
		Vector<char> src;
		AppendLine("#version " IM3D_STRINGIFY(IM3D_OPENGL_VSHADER), src);
		if (!LoadShader(_path, _defines, src))
		{
			return 0;
		}
	
		GLuint ret = 0;
		glAssert(ret = glCreateShader(_stage));
		const GLchar* pd = src.data();
		GLint ps = src.size();
		glAssert(glShaderSource(ret, 1, &pd, &ps));
	
		glAssert(glCompileShader(ret));
		GLint compileStatus = GL_FALSE;
		glAssert(glGetShaderiv(ret, GL_COMPILE_STATUS, &compileStatus));
		if (compileStatus == GL_FALSE)
		{
			fprintf(stderr, "Error compiling '%s':\n\n", _path);
			GLint len;
			glAssert(glGetShaderiv(ret, GL_INFO_LOG_LENGTH, &len));
			char* log = new GLchar[len];
			glAssert(glGetShaderInfoLog(ret, len, 0, log));
			fprintf(stderr, log);
			delete[] log;
	
			//fprintf(stderr, "\n\n%s", src.data());
			fprintf(stderr, "\n");
			glAssert(glDeleteShader(ret));

			return 0;
		}

		return ret;
	}
	
	bool Im3d::LinkShaderProgram(GLuint _handle)
	{
		IM3D_ASSERT(_handle != 0);
	
		glAssert(glLinkProgram(_handle));
		GLint linkStatus = GL_FALSE;
		glAssert(glGetProgramiv(_handle, GL_LINK_STATUS, &linkStatus));
		if (linkStatus == GL_FALSE)
		{
			fprintf(stderr, "Error linking program:\n\n");
			GLint len;
			glAssert(glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &len));
			GLchar* log = new GLchar[len];
			glAssert(glGetProgramInfoLog(_handle, len, 0, log));
			fprintf(stderr, log);
			fprintf(stderr, "\n");
			delete[] log;
	
			return false;
		}

		return true;
	}

	void Im3d::DrawNdcQuad()
	{
		static GLuint vbQuad;
		static GLuint vaQuad;
		if (vbQuad == 0)
		{
			float quadv[8] =
				{
					-1.0f, -1.0f,
					 1.0f, -1.0f,
					-1.0f,  1.0f,
					 1.0f,  1.0f,
				};
			glAssert(glGenBuffers(1, &vbQuad));
			glAssert(glGenVertexArrays(1, &vaQuad));	
			glAssert(glBindVertexArray(vaQuad));
			glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbQuad));
			glAssert(glEnableVertexAttribArray(0));
			glAssert(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (GLvoid*)0));
			glAssert(glBufferData(GL_ARRAY_BUFFER, sizeof(quadv), (GLvoid*)quadv, GL_STATIC_DRAW));
			glAssert(glBindVertexArray(0));	
		}
		glAssert(glBindVertexArray(vaQuad));
		glAssert(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
		glAssert(glBindVertexArray(0));
	}

	void Im3d::DrawTeapot(const Mat4& _world, const Mat4& _viewProj)
	{
		static GLuint shTeapot;
		static GLuint vbTeapot;
		static GLuint ibTeapot;
		static GLuint vaTeapot;
		if (shTeapot == 0)
		{
			GLuint vs = LoadCompileShader(GL_VERTEX_SHADER,   "model.glsl", "VERTEX_SHADER\0");
			GLuint fs = LoadCompileShader(GL_FRAGMENT_SHADER, "model.glsl", "FRAGMENT_SHADER\0");
			if (vs && fs)
			{
				glAssert(shTeapot = glCreateProgram());
				glAssert(glAttachShader(shTeapot, vs));
				glAssert(glAttachShader(shTeapot, fs));
				bool ret = LinkShaderProgram(shTeapot);
				glAssert(glDeleteShader(vs));
				glAssert(glDeleteShader(fs));
				if (!ret)
				{
					return;
				}
			}
			else
			{
				return;
			}

			glAssert(glGenBuffers(1, &vbTeapot));
			glAssert(glGenBuffers(1, &ibTeapot));
			glAssert(glGenVertexArrays(1, &vaTeapot));	
			glAssert(glBindVertexArray(vaTeapot));
			glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbTeapot));
			glAssert(glEnableVertexAttribArray(0));
			glAssert(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3) * 2, (GLvoid*)0));
			glAssert(glEnableVertexAttribArray(1));
			glAssert(glEnableVertexAttribArray(0));
			glAssert(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)0));
			glAssert(glEnableVertexAttribArray(1));
			glAssert(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 3)));
			glAssert(glBufferData(GL_ARRAY_BUFFER, sizeof(s_teapotVertices), (GLvoid*)s_teapotVertices, GL_STATIC_DRAW));
			glAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibTeapot));
			glAssert(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_teapotIndices), (GLvoid*)s_teapotIndices, GL_STATIC_DRAW));
			glAssert(glBindVertexArray(0));
		}

		glAssert(glUseProgram(shTeapot));
		glAssert(glUniformMatrix4fv(glGetUniformLocation(shTeapot, "uWorldMatrix"), 1, false, _world.m));
		glAssert(glUniformMatrix4fv(glGetUniformLocation(shTeapot, "uViewProjMatrix"), 1, false, _viewProj.m));
		glAssert(glBindVertexArray(vaTeapot));
		glAssert(glEnable(GL_DEPTH_TEST));
		glAssert(glEnable(GL_CULL_FACE));
		glAssert(glDrawElements(GL_TRIANGLES, sizeof(s_teapotIndices) / sizeof(unsigned), GL_UNSIGNED_INT, (GLvoid*)0));
		glAssert(glDisable(GL_DEPTH_TEST));
		glAssert(glDisable(GL_CULL_FACE));
		glAssert(glBindVertexArray(0));
		glAssert(glUseProgram(0));
	}
	
	const char* Im3d::GetGlEnumString(GLenum _enum)
	{
		#define CASE_ENUM(e) case e: return #e
		switch (_enum)
		{
		// errors
			CASE_ENUM(GL_NONE);
			CASE_ENUM(GL_INVALID_ENUM);
			CASE_ENUM(GL_INVALID_VALUE);
			CASE_ENUM(GL_INVALID_OPERATION);
			CASE_ENUM(GL_INVALID_FRAMEBUFFER_OPERATION);
			CASE_ENUM(GL_OUT_OF_MEMORY);
	
			default: return "Unknown GLenum";
		};
		#undef CASE_ENUM
	}
	
	const char* Im3d::GlGetString(GLenum _name)
	{
		const char* ret;
		glAssert(ret = (const char*)glGetString(_name));
		return ret ? ret : "";
	}

#elif defined(IM3D_DX11)
	ID3DBlob* Im3d::LoadCompileShader(const char* _target, const char* _path, const char* _defines)
	{
		Vector<char> src;
		if (!LoadShader(_path, _defines, src))
		{
			return nullptr;
		}
		ID3DBlob* ret = nullptr;
		ID3DBlob* err = nullptr;
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

	 // D3DCompile is not portable - linking with d3dcompiler.lib introdices a dependency on d3dcompiler_XX.lib
	 // \todo get a ptr to D3DCompile at runtime via LoadLibrary/GetProcAddress
		D3DCompile(src.data(), src.size(), nullptr, nullptr, nullptr, "main", _target, flags, 0, &ret, &err);
		if (ret == nullptr)
		{
			fprintf(stderr, "Error compiling '%s':\n\n", _path);
			if (err)
			{
				fprintf(stderr, (char*)err->GetBufferPointer());
				err->Release();
			}
		}

		return ret;
	}

	ID3D11Buffer* Im3d::CreateBuffer(UINT _size, D3D11_USAGE _usage, UINT _bind, const void* _data)
	{
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = _size;
		desc.Usage = _usage;
		desc.BindFlags = _bind;
		desc.CPUAccessFlags = _usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;

		ID3D11Buffer* ret = nullptr;
		D3D11_SUBRESOURCE_DATA subRes = {};
		subRes.pSysMem = _data;
		dxAssert(g_Example->m_d3dDevice->CreateBuffer(&desc, _data ? &subRes : nullptr, &ret));
		return ret;
	}
	
	ID3D11Buffer* Im3d::CreateConstantBuffer(UINT _size, D3D11_USAGE _usage, const void* _data)
	{
		return CreateBuffer(_size, _usage, D3D11_BIND_CONSTANT_BUFFER, _data);
	}
	ID3D11Buffer* Im3d::CreateVertexBuffer(UINT _size, D3D11_USAGE _usage, const void* _data)
	{
		return CreateBuffer(_size, _usage, D3D11_BIND_VERTEX_BUFFER, _data);
	}
	ID3D11Buffer* Im3d::CreateIndexBuffer(UINT _size, D3D11_USAGE _usage, const void* _data)
	{
		return CreateBuffer(_size, _usage, D3D11_BIND_INDEX_BUFFER, _data);
	}

	void* Im3d::MapBuffer(ID3D11Buffer* _buffer, D3D11_MAP _mapType)
	{
		D3D11_MAPPED_SUBRESOURCE subRes;
		dxAssert(g_Example->m_d3dDeviceCtx->Map(_buffer, 0, _mapType, 0, &subRes));
		return subRes.pData;
	}

	void  Im3d::UnmapBuffer(ID3D11Buffer* _buffer)
	{
		g_Example->m_d3dDeviceCtx->Unmap(_buffer, 0);
	}

	ID3D11Texture2D* Im3d::CreateTexture2D(UINT _width, UINT _height, DXGI_FORMAT _format, ID3D11ShaderResourceView** resView_, const void* _data)
	{
		ID3D11Device* d3d = g_Example->m_d3dDevice;

		D3D11_TEXTURE2D_DESC txDesc = {};
		txDesc.Width = _width;
		txDesc.Height = _height;
		txDesc.MipLevels = 1;
		txDesc.ArraySize = 1;
		txDesc.Format = _format;
		txDesc.SampleDesc.Count = 1;
		txDesc.Usage = D3D11_USAGE_DEFAULT;
		txDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		ID3D11Texture2D* ret = nullptr;
		D3D11_SUBRESOURCE_DATA subRes = {};
		if (_data)
		{
			subRes.pSysMem = _data;
			subRes.SysMemPitch = _width;
			switch (_format) {
				case DXGI_FORMAT_R8_UNORM:       break;
				case DXGI_FORMAT_R8G8B8A8_UNORM:
				default:                         subRes.SysMemPitch *= 4;
			};
		}
		dxAssert(d3d->CreateTexture2D(&txDesc, _data ? &subRes : nullptr, &ret));

		if (resView_)
		{
			dxAssert(d3d->CreateShaderResourceView(ret, nullptr, resView_));
		}

		return ret;
	}

	ID3D11DepthStencilView* Im3d::CreateDepthStencil(UINT _width, UINT _height, DXGI_FORMAT _format)
	{
		ID3D11Device* d3d = g_Example->m_d3dDevice;

		D3D11_TEXTURE2D_DESC txDesc = {};
		txDesc.Width = _width;
		txDesc.Height = _height;
		txDesc.MipLevels = 1;
		txDesc.ArraySize = 1;
		txDesc.Format = _format;
		txDesc.SampleDesc.Count = 1;
		txDesc.Usage = D3D11_USAGE_DEFAULT;
		txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		ID3D11Texture2D* tx = nullptr;
		ID3D11DepthStencilView* ret;
		dxAssert(d3d->CreateTexture2D(&txDesc, nullptr, &tx));
		dxAssert(d3d->CreateDepthStencilView(tx, nullptr, &ret));
		return ret;
	}
	
	void Im3d::DrawNdcQuad()
	{
	}
	
	void Im3d::DrawTeapot(const Mat4& _world, const Mat4& _viewProj)
	{
		static ID3DBlob*              s_vsBlob;
		static ID3D11VertexShader*    s_vs;
		static ID3DBlob*              s_psBlob;
		static ID3D11PixelShader*     s_ps;
		static ID3D11InputLayout*     s_inputLayout;
		static ID3D11Buffer*          s_vb;
		static ID3D11Buffer*          s_ib;
		static ID3D11Buffer*          s_cb;
		static ID3D11RasterizerState* s_rasterizerState;

		ID3D11Device* d3d = g_Example->m_d3dDevice;
		ID3D11DeviceContext* ctx = g_Example->m_d3dDeviceCtx;

		if (s_vsBlob == 0)
		{
			s_vsBlob = LoadCompileShader("vs_" IM3D_DX11_VSHADER, "model.hlsl", "VERTEX_SHADER\0");
			dxAssert(d3d->CreateVertexShader((DWORD*)s_vsBlob->GetBufferPointer(), s_vsBlob->GetBufferSize(), nullptr, &s_vs));
			s_psBlob = LoadCompileShader("ps_" IM3D_DX11_VSHADER, "model.hlsl", "PIXEL_SHADER\0");
			dxAssert(d3d->CreatePixelShader((DWORD*)s_psBlob->GetBufferPointer(), s_psBlob->GetBufferSize(), nullptr, &s_ps));

			s_vb = CreateVertexBuffer(sizeof(s_teapotVertices), D3D11_USAGE_IMMUTABLE, s_teapotVertices);
			s_ib = CreateIndexBuffer(sizeof(s_teapotIndices), D3D11_USAGE_IMMUTABLE, s_teapotIndices); 
		
			D3D11_INPUT_ELEMENT_DESC inputDesc[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};
			dxAssert(d3d->CreateInputLayout(inputDesc, 2, s_vsBlob->GetBufferPointer(), s_vsBlob->GetBufferSize(), &s_inputLayout));
		
			s_cb = CreateConstantBuffer(sizeof(Mat4) * 2, D3D11_USAGE_DYNAMIC);

			D3D11_RASTERIZER_DESC rasterizerDesc = {};
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_BACK;
			rasterizerDesc.FrontCounterClockwise = true;
			dxAssert(d3d->CreateRasterizerState(&rasterizerDesc, &s_rasterizerState));
		}

		Mat4* cbData = (Mat4*)MapBuffer(s_cb, D3D11_MAP_WRITE_DISCARD);
		cbData[0] = _world;
		cbData[1] = _viewProj;
		UnmapBuffer(s_cb);

		unsigned int stride = 4 * 3 * 2;
		unsigned int offset = 0;
		ctx->IASetInputLayout(s_inputLayout);
		ctx->IASetVertexBuffers(0, 1, &s_vb, &stride, &offset);
		ctx->IASetIndexBuffer(s_ib, DXGI_FORMAT_R32_UINT, 0);
		ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ctx->VSSetShader(s_vs, nullptr, 0);
		ctx->VSSetConstantBuffers(0, 1, &s_cb);
		ctx->PSSetShader(s_ps, nullptr, 0);

		ctx->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		ctx->OMSetDepthStencilState(nullptr, 0);
		ctx->RSSetState(s_rasterizerState);

		ctx->DrawIndexed(sizeof(s_teapotIndices) / sizeof(unsigned), 0, 0);
	}
	
#endif // graphics

/******************************************************************************/
void Im3d::Assert(const char* _e, const char* _file, int _line, const char* _msg, ...)
{
	const int kAssertMsgMax = 1024;

	char buf[kAssertMsgMax];
	if (_msg != nullptr)
	{
		va_list args;
		va_start(args, _msg);
		vsnprintf(buf, kAssertMsgMax, _msg, args);
		va_end(args);
	}
	else
	{
		buf[0] = '\0';
	}
	fprintf(stderr, "Assert (%s, line %d)\n\t'%s' %s", StripPath(_file), _line, _e ? _e : "", buf);
}

void Im3d::RandSeed(int _seed)
{
	srand(_seed);
}
int Im3d::RandInt(int _min, int _max)
{
	return _min + (int)rand() % (_max - _min);
}
float Im3d::RandFloat(float _min, float _max)
{
	return _min + (float)rand() / (float)RAND_MAX * (_max - _min);
}
Mat3 Im3d::RandRotation()
{
	return Rotation(Normalize(RandVec3(-1.0f, 1.0f)), RandFloat(-Pi, Pi));
}
Vec3 Im3d::RandVec3(float _min, float _max)
{
	return Im3d::Vec3(
		RandFloat(_min, _max),
		RandFloat(_min, _max),
		RandFloat(_min, _max)
		);
}
Color Im3d::RandColor(float _min, float _max)
{
	Vec3 v = RandVec3(_min, _max);
	return Color(v.x, v.y, v.z);
}

