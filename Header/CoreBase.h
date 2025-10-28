#pragma once
#include "../ThirdParty/nlohmann/json.hpp"

namespace Engine
{
	class CoreBase abstract
	{
	protected:
		explicit CoreBase() = default;
		virtual ~CoreBase() = default;

	public:
		virtual void Terminate()
		{
			Destroy();
			delete this; 
		}

		#undef GetClassName
		const char* GetClassName() const 
		{
			const std::type_info& self = typeid(*this);
			const char* className = self.name();
        
			// "class " 접두어를 제거
			const char* prefix = "class ";
			size_t prefixLength = std::strlen(prefix);
        
			if (!std::strncmp(className, prefix, prefixLength)) 
			{
				className += prefixLength;
			}
        
			return className;
		}

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) abstract;
		virtual void SerializeOut(nlohmann::ordered_json& object) abstract;

	protected:
		virtual void Destroy() abstract;
	};
}