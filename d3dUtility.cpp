
#include "d3dUtility.h"

//Direct3D를 초기화한다.
bool d3d::InitD3D(
	HINSTANCE hInstance,
	int width, int height,
	bool windowed,
	D3DDEVTYPE deviceType,
	IDirect3DDevice9** device)
{
	//------------------------------------------
	// Create the main application window.
	//------------------------------------------

	WNDCLASS wc;  //WNDCLASS는 윈도우 프로그래밍에 사용되는 구조체, 윈도우 클래스를 정의할 때 사용,
    //WNDCLASS구조체에는 여러 속성을 설정하는 필드들이 포함되어 있다.
    /*
    style:클래스의 스타일을 정의-----예를 들어, CS_HREDRAW와 CS_VREDRAW는 윈도우의 크기가 변경될때, 수평 및 수직 방향으로 다시 그리도록 지정한다.
    lpfnWndProc: 윈도우 프로시저 함수에 대한 포인터, 윈도우가 받는 메시지를 처리한다.
    cbClsExtra, cbWndExtra: 각각 클래스와 윈도우에 할당할 추가 메모리의 양을 지정한다.
    hInstance: 애플리케이션 인스턴스의 핸들
    hIcon: 윈도우의 아이콘
    hCursur: 윈도우 내에서 사용될 커서
    hbrBackground: 윈도우의 배경색을 지정
    lpszMenuName: 윈도우의 메뉴를 지정
    lpszClassName: 등록할 클래스의 이름을 지정
    */

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)d3d::WndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "Direct3D9App";

	if( !RegisterClass(&wc) ) //윈도우클래스등록, 윈도우의 스타일, 아이콘,커서,배경색등을 정의
	{
		::MessageBox(0, "RegisterClass() - FAILED", 0, 0); //실패시 오류 메시지
		return false;
	}
		
	HWND hwnd = 0;
    hwnd = ::CreateWindow(
		"Direct3D9App", 	//클래스이름
        "Virtual Billiard", //윈도우 제목
		WS_EX_TOPMOST,		//윈도우를 다른 모든 윈도우 위에 배치를 의미
		0, 0, width, height,	///윈도우의 위치와 크기 설정
		0 /*parent hwnd*/, 0 /* menu */, hInstance, 0 /*extra*/); 

	if( !hwnd )
	{
		::MessageBox(0, "CreateWindow() - FAILED", 0, 0); //윈도우 생성 실패시 오류 메시지
		return false;
	}

	::ShowWindow(hwnd, SW_SHOW); //윈도우를 화면에 표시한다. SW_SHOW 옵션은 위녿우를 활성상태로 만들고 보이게 한다.
	::UpdateWindow(hwnd); 		//윈도우를 갱신하고 WM_PAINT메시지를 처리하여 윈도우를 즉시 그린다.

	//-----------------------------------------------------------------------------
	// Init D3D: 
	//-----------------------------------------------------------------------------

	HRESULT hr = 0;

	// Step 1: IDirect3D9의 인터페이스 인스턴스 생성, Direct3D장치를 생성하고 관리하는데 사용

	IDirect3D9* d3d9 = 0;
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

    if( !d3d9 )
	{
		::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0); //생성 실패시 false처리
		return false;
	}

	// Step 2: Check for hardware vp. 하드웨어 지원 확인

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps); //GetDeviceCaps는 장치의 기능을 확인한다. 여기서는 기본 어댑터의 기능을 확인하여 하드웨어 변환 및 조명 지원 여부를 판단한다.

	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;  //하드웨어 변환 및 조명이 지원되면 하드웨어 정점처리 사용
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;	//안되면 소프트웨어 정점 처리 사용


	// Step 3: Fill out the D3DPRESENT_PARAMETERS 구조체, 표시 매개변수 설정, 
	//백퍼퍼의 크기, 포맷, 멀티 샘플링, 옵션, 스왑 효과, 윈도우 모드, 깊이 스텐실 버퍼 설정
 
    RECT rc;
    GetClientRect(hwnd, &rc);
    UINT w = rc.right - rc.left;
    UINT h = rc.bottom - rc.top;
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = w;
	d3dpp.BackBufferHeight           = h;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = windowed;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: 장치 생성, 3D 그래픽을 렌더링하는데 사용된다. 

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		hwnd,               // window associated with device
		vp,                 // vertex processing
	    &d3dpp,             // present parameters
	    device);            // return created device

	if( FAILED(hr) )
	{
		//장치 생성에 실패하면 16비트 깊이 버퍼를 사용하여 다시 시도
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		
		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hwnd,
			vp,
			&d3dpp,
			device);

		if( FAILED(hr) )
		{
			d3d9->Release(); // 다시 실패하면 IDirect3D9 객체를 해제하고 오류 메시지 출력.
			::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}

	d3d9->Release(); // 객체를 해제한다.
	
	return true;
}


//메시지루프를 처리하는 부분, 메시지 루프는 운영체제로부터 메시지를 받아서 처리하고, 프로그래미이 유휴 상태일 때는 특정 작업을 함. 
int d3d::EnterMsgLoop( bool (*ptr_display)(float timeDelta) )
{
	MSG msg; //메시지를 저장할 구조체를 서언
	::ZeroMemory(&msg, sizeof(MSG));	//구조체를 0으로 초기화

	static double lastTime = (double)timeGetTime(); 	//마지막으로 메시지 루프가 실행된 시간을 추적

	while(msg.message != WM_QUIT) 	//WM_QUIT가 수신 될 때 까지 실행
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))		//대기중인 메시지 있는지 확인하고 있으면 큐에서 제거
		{
			::TranslateMessage(&msg);		//메시지를 번역하고 
			::DispatchMessage(&msg);		//해당 윈도우 프로시저로 전달
		}
		else
        {	
			double currTime  = (double)timeGetTime();
			double timeDelta = (currTime - lastTime)*0.0007; //현재시간 측정, 마지막 시간과 차이 계산
			ptr_display((float)timeDelta);	//렌더링이나 다른 계산 수해한다.

			lastTime = currTime;
        }
    }
    return msg.wParam;
}

//방향성 광원을 초기화 하는 함수
D3DLIGHT9 d3d::InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;	//광원을 정의하는 구조체
	::ZeroMemory(&light, sizeof(light));	//초기화

	light.Type      = D3DLIGHT_DIRECTIONAL;	//방향성 광원임을 지정
	light.Ambient   = *color * 0.6f;	//주변광의 색상
	light.Diffuse   = *color;		//확산광의 색상
	light.Specular  = *color * 0.6f;	//반사광의 색상
	light.Direction = *direction;	//광원의 방향

	return light;	
}

//점 광원을 초기화하는 함수, 점 광원: 모든 방향으로 빛을 발산하는 광원
D3DLIGHT9 d3d::InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
{
	D3DLIGHT9 light;	//광원을 정의하는 구조체
	::ZeroMemory(&light, sizeof(light));	//초기화

	light.Type      = D3DLIGHT_POINT;	
	light.Ambient   = *color * 0.6f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Position  = *position;		//위치 지정
	light.Range        = 1000.0f;		//광원의 범위 설정
	light.Falloff      = 1.0f;			//광원의 강도가 거리에 따라 어떻게 감소하는지 설정
	light.Attenuation0 = 1.0f;			//강원의 감쇠(Attenuation)을 나타냄.
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;

	return light;
}

//스포트라이트: 특정방향으로 빛을 집중시키는 광원
D3DLIGHT9 d3d::InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_SPOT;
	light.Ambient   = *color * 0.0f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Position  = *position;
	light.Direction = *direction;
	light.Range        = 1000.0f;
	light.Falloff      = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta        = 0.4f;
	light.Phi          = 0.9f;

	return light;
}

//----------------------------------------------------------------------------------------------------------------
	// Materials, D3DMATERIAL9는 위와 비슷, 재질를 나타냄. 
    // Ambient(a): 주변 광원에 의해 반사되는 색상을 정의, 
    // Diffuse(d): 물체에 빛이 직접 닿아 반사될 때의 색상을 정의,
    // Specular(s): 물체에 빛이 닿아 반사될 때의 반짝임을 정의
    // Emissive(e): 물체 자체에서 나오는 빛의 색상을 정의
    // Power(p): Specular의 반사 강도를 정의
//----------------------------------------------------------------------------------------------------------------
D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}

d3d::BoundingBox::BoundingBox()
{
	// infinite small 
	_min.x = INFINITY;
	_min.y = INFINITY;
	_min.z = INFINITY;

	_max.x = -INFINITY;
	_max.y = -INFINITY;
	_max.z = -INFINITY;
}

bool d3d::BoundingBox::isPointInside(D3DXVECTOR3& p)
{
	if( p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z )
	{
		return true;
	}
	else
	{
		return false;
	}
}

d3d::BoundingSphere::BoundingSphere()
{
	_radius = 0.0f;
}