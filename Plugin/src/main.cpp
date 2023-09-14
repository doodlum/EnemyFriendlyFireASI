#include "DKUtil/Config.hpp"

using namespace DKUtil::Alias;

namespace EnemyFriendlyFire
{
	bool CheckValidTarget(std::uintptr_t, std::uintptr_t)
	{
		return true;
	};

	void Install()
	{
		const auto scan = static_cast<uint8_t*>(dku::Hook::Assembly::search_pattern<"48 89 01 48 8D 05 ?? ?? ?? ?? 48 89 41 10 48 8D 05 ?? ?? ?? ?? 48 89 41 38 48 8D 05 ?? ?? ?? ?? 48 89 41 40 48 8D 05 ?? ?? ?? ?? 48 89 41 48 48 8D 05 ?? ?? ?? ?? 48 89 41 50 48 8D 05 ?? ?? ?? ?? 48 89 41 58 48 8D 05 ?? ?? ?? ?? 48 89 41 60 48 8D 05 ?? ?? ?? ?? 48 89 41 68 48 8D 05 ?? ?? ?? ?? 48 89 41 70 48 8D 05 ?? ?? ?? ?? 48 89 41 78 48 8D B1 10 01 00 00">());
		if (!scan) {
			ERROR("Failed to find Actor VTable!")
		}
		auto Actor_Vtable = AsAddress(dku::Hook::GetDisp<void*>(scan + 3));
		INFO("Found Actor VTable at 0x{:X}", Actor_Vtable - dku::Hook::Module::get().base() + 0x140000000);
		dku::Hook::WriteImm(Actor_Vtable + 0xB10, AsAddress(&CheckValidTarget));
		INFO("Hooked CheckValidTarget");
	}
}

BOOL APIENTRY DllMain(HMODULE a_hModule, DWORD a_ul_reason_for_call, LPVOID a_lpReserved)
{
	if (a_ul_reason_for_call == DLL_PROCESS_ATTACH) {
#ifndef NDEBUG
		while (!IsDebuggerPresent()) {
			Sleep(100);
		}
#endif
		dku::Logger::Init(Plugin::NAME, std::to_string(Plugin::Version));
		EnemyFriendlyFire::Install();
	}
	return TRUE;
}