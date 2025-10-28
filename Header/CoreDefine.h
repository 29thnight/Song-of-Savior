#pragma once
#pragma warning(disable: 4251) //rlottie���� �߻��ϴ� ��� ����
#pragma warning(disable: 26819) // json���� �߻��ϴ� ��� ����
//Windows and Console
#include <iostream>
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN
#include <io.h>

// DirectInput
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

// Xinput
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

// DirectX
#include <d3d11.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dxgi.h>
#include <dwrite.h>
#include <dwrite_3.h>
// comptr
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dxgi.lib")

#include <shellscalingapi.h>  // Shcore.lib�� ���ǵǾ� ����
#pragma comment(lib, "Shcore.lib")

#include <wincodec.h>
#include <DirectXMath.h>

#include "Vfw.h"
#pragma comment(lib, "Vfw32.lib")

#include <dshow.h>
#include <atlbase.h>  // for CComPtr smart pointers

#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "Quartz.lib")

// STL
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <algorithm>
#include <memory>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <comdef.h>
#include <typeindex>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <stdexcept>
#include <random>
#include <future>  // std::async ����� ���� �߰�
//#include <memory_resource>

#include <CoreEnum.h>
#include <CoreType.h>
#include <CoreMacro.h>
#include <CoreFunction.h>
#include <CoreStruct.h>
#include <Delegate.h>