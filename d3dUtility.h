#ifndef __d3dUtilityH__  //ifnotdefined로 뒤에있는거 정의 안되면 안돌림->중복을 방지->한번만 읽으라
#define __d3dUtilityH__

#include <d3dx9.h>   //마이크로소프트의 DirectX SDK의 일부인 헤더파일, 게임 및 고성능 멀티미디어 애플리케이션을 개발하는데 사용되는 일련의 API(application programming interface)
#include <string> 
#include <limits>

//#define INFINITY FLT_MAX

#define EPSILON 0.001f
#define INFINITY FLT_MAX


namespace d3d  //c++에서 네임스페이스를 정의, 이름 충돌을 방지하고 코드를 더 잘 조직화한다. d3d라는 네임스페이스 안에 코드를 정의하는데, 이 안에 정의된 모든 클래스, 함수, 변수 등은 d3d:: 라는 접두어를 사용하여 외부에서 사용할 수 있다. 
//namespace사용 이유: 1. 이름 충돌방지: 서로 다른 라이브러리나 코드 모듈에서 같은 이름의 함수나 클래스를 사용할 때, 네임스페이스를 통해 각각을 구분할 수 있다. 
//2. 코드 조직화: 관련된 기능을 하나의 네임스페이스로 묶어 코드의 구조를 명확하게 할 수 있다.
{
	//--------------------------------------------------------------------------------------------------------------------------------
	// Direct3D를 초기화 하기 위한 함수 InitD3D, Direct3D는 마이크로소프트에서 개발한 API로  주로 게임에 사용. 
	//--------------------------------------------------------------------------------------------------------------------------------
	bool InitD3D(
		HINSTANCE hInstance,       // 애플리케이션의 인스턴스 핸들. 윈도우 기반의 프로그램에서는 실행 중인 인스턴스를 식별한다.
		int width, int height,     // 백버퍼의 너비와 높이를 지정한다.
		bool windowed,             // 창 여부를 지정한다.
		D3DDEVTYPE deviceType,     // 장치 유형을 지정. HAL은 하드웨어 가속을 사용하는 장치, REF는 소프트웨어 렌더링을 사용하는 장치
		IDirect3DDevice9** device);// Direct3D 장치를 나타내는 포인터의 주소, Direct3D장치를 생성하고 이 포인터를 통해 생성된 장치에 대한 참조를 반환한다.
	
    int EnterMsgLoop( //함수 포인터 ptr_display를 받는다. 이 함수 포인터는 timeDelta를 매개변수로 받는다. 일반적으로 timeDelta는 이전 프레임과 현재 프레임 간의 시간차를 나타낸다. 
        //아마도 윈도우 메시지 루프를 실행하여. 각 루프에서 ptr_display를 호출해서 화면을 갱신하는 역할 일 것 이다.
		bool (*ptr_display)(float timeDelta));


    //LRESULT는 함수의 반환 타입으로 메시지 처리 결과를 나타낸다. CALLBACK은 함수가 콜백함수(시스템이나 다른 코드에 의해 호출되는 함수)임을 나타냄
	LRESULT CALLBACK WndProc(  //윈도우 프로시저 함수의 프로토타입이다. 운영체제로부터 발생하는 다양한 메시지 처리
		HWND hwnd,  //메시지를 받는 윈도우의 핸들을 나타냄. 윈도우 핸들은 윈도우를 식별하는 고유한 값
		UINT msg,  // 처리해야할 메시지의 유형을 나타낸다. ex) WM_PAINT는 윈도우가 다시 그려져야할 때
		WPARAM wParam,  //추가정보를 제공. 키보드 메시지의 경우 눌린 키의 코드를 나타낼 수 있음
		LPARAM lParam); //메시지와 함께 전달되는 추가 정보를 제공한다. wParam과 마찬가지로 그 의미는 메시지 유형에 따라 달라짐. 마우스 클릭 메시지의 경우 클릭된 위치의 좌표를 나타낼 수 있다.


// 핸들이란?-->운영체제에서 특정한 리소스나 객체를 식별하기 위해 사용되는 추상적인 참조이다. 
// 컴퓨터 프로그래밍에서 핸들은 일반적으로 리소스에 대한 포인터나 인덱스, 식별자 등을 의미하며, 프로그램이 해당 리소스를 효율적으로 관리,접근할 수 있다.
	
    //템플릿을 사용해서 다양한 타입의 객체에 동일한 작업을 수행하는 범용 함수이다.
	template<class T> void Release(T t)  //템플릿 매개변수 T: 어떤 타입의 객체든 받는다. 타입T 는 Release메서드를 가지고 있어야함. 
	{
		if( t ) //nullptr이 아닌지 확인
		{
			t->Release();  //Release메서드 호출, 객체의 참조 카운트를 감소 시키고, 필요한 경우 객체를 메모리에서 해제
			t = 0;  //포인터를 nullptr로 설정하여 포인터가 더 이상 유효하지 않은 객체를 가리키지 않도록 함
		}
	}
    //Direct3D에서 생성한 객체들은 사용 후에 이런 방식으로 해제되어야 함
		
    //동적으로 할당된 메모리를 관리하는데 사용하는 템플릿 함수
	template<class T> void Delete(T t) 
	{
		if( t )
		{
			delete t;
			t = 0;
		}
	}

	//
	// 색상들 미리 정해두기.
	//
	const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
	const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
	const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
	const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
	const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
	const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
	const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
	const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );
	const D3DXCOLOR	   DARKRED( D3DCOLOR_XRGB(215,	0,	0));

	//
	// Lights
	//

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);
 
	//
	// Materials
	//

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	const D3DMATERIAL9 WHITE_MTRL  = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL    = InitMtrl(RED, RED, RED, BLACK, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL  = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL   = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

	//
	// Bounding Objects / Math Objects
	//

	struct BoundingBox
	{
		BoundingBox();

		bool isPointInside(D3DXVECTOR3& p);

		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
	};

	struct BoundingSphere
	{
		BoundingSphere();

		D3DXVECTOR3 _center;
		float       _radius;
	};

	struct Ray
	{
		D3DXVECTOR3 _origin;
		D3DXVECTOR3 _direction;
	};

	//
	// Constants
	//
}

#endif // __d3dUtilityH__