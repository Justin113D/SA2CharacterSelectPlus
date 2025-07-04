#pragma once

/*
This is a faster version of the FunctionHook classes from the SADX Mod Loader utilities.

While FunctionHook's method is very minimalist and safe, it requires memory reading and writing for every call to original.
This uses the same principle as Trampoline from the same utilities or Minhook/Detour, but it's wrapped in easy to use classes.

There are two main versions: FastFunctionHook (and its derivatives) and FastFunctionHookPtr.
The former requires you to type the return and argument types manually and the latter requires the function pointer.

Planned:
- Support conditional jumps in trampoline.
- Add an optional relay function before the hook function.

Contributions:
- MainMemory (original code, usercal wrappers, etc.)
- Kell (fast version)
- Vyacheslav Patkov (disassembler)
*/

#include <exception>
#include <cstdint>

#include "hde32.h"
#include "MemAccess.h"
#include "UsercallFunctionHandler.h"

static void* GenerateTrampoline(uint8_t* origbytes)
{
	// Todo: better way of managin relative addresses + also manage conditional jumps
	int callloc = -1;
	int jumploc = -1;

	int totalsize = 0;
	while (totalsize < 5)
	{
		hde32s hs;
		hde32_disasm(&origbytes[totalsize], &hs);

		if (hs.flags & F_ERROR)
		{
			throw std::exception("Fatal error while hooking function.");
		}

		switch (hs.opcode)
		{
		case 0xE8: // call
			callloc = totalsize;
			break;
		case 0xE9: // jump
		case 0xEB: // jump
			jumploc = totalsize;
			break;
		}

		totalsize += hs.len;
	}

	if (totalsize > 20)
	{
		throw std::exception("Trampoline is too large.");
	}

	// Create function to call original without hook
	auto func = AllocateCode(totalsize + 5);
	memcpy(func, origbytes, totalsize); // Write original instructions to trampoline
	WriteJump(&func[totalsize], &origbytes[totalsize]); // Append jump from trampoline function to original

	// Fix any call/jump in the trampoline
	if (callloc != -1)
	{
		intptr_t addr = (intptr_t)&origbytes[callloc] + 5 + *reinterpret_cast<intptr_t*>(&origbytes[callloc + 1]);
		WriteCall(&func[callloc], (void*)addr);
	}
	else if (jumploc != -1)
	{
		intptr_t addr = (intptr_t)&origbytes[jumploc] + 5 + *reinterpret_cast<intptr_t*>(&origbytes[jumploc + 1]);
		WriteJump(&func[jumploc], (void*)addr);
	}

	return func;
}

static void* GenerateThiscallWrapper(void* address, int stackcnt)
{
	int memsz = 10; // size of mov ecx, call and retn

	for (int i = 0; i < stackcnt; ++i)
	{
		memsz += 4;
	}

	auto codeData = AllocateCode(memsz);
	int cdoff = 0;
	uint8_t stackoff = (stackcnt + 1) * 4; // stack+this

	for (int i = 0; i < stackcnt; ++i)
	{
		writebytes(codeData, cdoff, 0xFF, 0x74, 0x24, stackoff); // stack
	}

	writebytes(codeData, cdoff, 0x8B, 0x4C, 0x24, stackoff); // ECX
	
	WriteCall(&codeData[cdoff], address);
	cdoff += 5;

	codeData[cdoff++] = 0xC3;
	assert(cdoff == memsz);
	return codeData;
}

static void GenerateThiscallHook(void* func, void* address, int stackcnt)
{
	int memsz = stackcnt > 0 ? 13 : 8; // size of push ecx, pop ecx, call and retn purge (and pop stack)

	for (int i = 0; i < stackcnt; ++i)
	{
		memsz += 4;
	}

	auto codeData = AllocateCode(memsz);
	int cdoff = 0;
	uint8_t stackoff = stackcnt * 4;

	for (int i = 0; i < stackcnt; ++i)
	{
		writebytes(codeData, cdoff, 0xFF, 0x74, 0x24, stackoff); // stack
	}

	codeData[cdoff++] = 0x51; // PUSH ECX

	WriteCall(&codeData[cdoff], func);
	cdoff += 5;
	
	codeData[cdoff++] = 0x59; // POP ECX
	
	if (stackcnt > 0)
	{
		writebytes(codeData, cdoff, 0x83, 0xC4, (uint8_t)(stackcnt * 4)); // pop stack
		writebytes(codeData, cdoff, 0xC2, stackoff, 0x00); // retn + callee cleanup
	}
	else
	{
		codeData[cdoff++] = 0xC3; // normal retn
	}

	assert(cdoff == memsz);
	if (*(uint8_t*)address == 0xE8)
		WriteCall((void*)address, codeData);
	else
		WriteJump((void*)address, codeData);
}

/// <summary>
/// A function hooking class using the function return and argument types as a base.
/// </summary>
/// <typeparam name="TRet">The return type.</typeparam>
/// <typeparam name="...TArgs">The type of each argument from left to right.</typeparam>
template<typename TRet, typename... TArgs>
class FastFunctionHook
{
public:
	typedef TRet(*FuncType)(TArgs...);

	FastFunctionHook(FuncType address) : origaddr(address) {	}

	FastFunctionHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastFunctionHook(FuncType address, FuncType hook) : FastFunctionHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastFunctionHook(intptr_t address, FuncType hook) : FastFunctionHook(address)
	{
		Hook(hook);
	}

	FastFunctionHook() = delete;
	FastFunctionHook(FastFunctionHook&) = delete;
	FastFunctionHook(FastFunctionHook&&) = delete;

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (trampoline)
			throw new std::exception("Cannot apply hook to already hooked function!");
		trampoline = (FuncType)GenerateTrampoline((uint8_t*)origaddr);
		WriteJump(origaddr, hook); // Write jump from original function to the hook
	}

	// Call the original function, bypassing the hook.
	TRet Original(TArgs... args)
	{
		if (trampoline)
		{
			return trampoline(args...);
		}
		else
		{
			TRet ret = TRet();
			return ret;
		}
	}

private:
	const FuncType origaddr;
	FuncType trampoline = nullptr;
};

/// <summary>
/// A void function hooking class using the function argument types as a base.
/// </summary>
/// <typeparam name="...TArgs">The type of each argument from left to right.</typeparam>
template<typename... TArgs>
class FastFunctionHook<void, TArgs...>
{
public:
	typedef void(*FuncType)(TArgs...);

	FastFunctionHook(FuncType address) : origaddr(address) {	}

	FastFunctionHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastFunctionHook(FuncType address, FuncType hook) : FastFunctionHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastFunctionHook(intptr_t address, FuncType hook) : FastFunctionHook(address)
	{
		Hook(hook);
	}

	FastFunctionHook() = delete;
	FastFunctionHook(FastFunctionHook&) = delete;
	FastFunctionHook(FastFunctionHook&&) = delete;

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (trampoline)
			throw new std::exception("Cannot apply hook to already hooked function!");
		trampoline = (FuncType)GenerateTrampoline((uint8_t*)origaddr);
		WriteJump(origaddr, hook); // Write jump from original function to the hook
	}

	// Call the original function, bypassing the hook.
	void Original(TArgs... args)
	{
		if (trampoline)
		{
			trampoline(args...);
		}
	}

private:
	const FuncType origaddr;
	FuncType trampoline = nullptr;
};

/// <summary>
/// An stdcall function hooking class using the function return and argument types as a base.
/// </summary>
/// <typeparam name="TRet">The return type.</typeparam>
/// <typeparam name="...TArgs">The type of each argument from left to right.</typeparam>
template<typename TRet, typename... TArgs>
class FastStdcallHook
{
public:
	typedef TRet(__stdcall*FuncType)(TArgs...);

	FastStdcallHook(FuncType address) : origaddr(address) {	}

	FastStdcallHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastStdcallHook(FuncType address, FuncType hook) : FastStdcallHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastStdcallHook(intptr_t address, FuncType hook) : FastStdcallHook(address)
	{
		Hook(hook);
	}

	FastStdcallHook() = delete;
	FastStdcallHook(FastStdcallHook&) = delete;
	FastStdcallHook(FastStdcallHook&&) = delete;

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (trampoline)
			throw new std::exception("Cannot apply hook to already hooked function!");
		trampoline = (FuncType)GenerateTrampoline((uint8_t*)origaddr);
		WriteJump(origaddr, hook); // Write jump from original function to the hook
	}

	// Call the original function, bypassing the hook.
	TRet Original(TArgs... args)
	{
		if (trampoline)
		{
			return trampoline(args...);
		}
		else
		{
			TRet ret = TRet();
			return ret;
		}
	}

private:
	const FuncType origaddr;
	FuncType trampoline = nullptr;
};

/// <summary>
/// A void stdcall function hooking class using the function argument types as a base.
/// </summary>
/// <typeparam name="...TArgs">The type of each argument from left to right.</typeparam>
template<typename... TArgs>
class FastStdcallHook<void, TArgs...>
{
public:
	typedef void(__stdcall*FuncType)(TArgs...);

	FastStdcallHook(FuncType address) : origaddr(address) {	}

	FastStdcallHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastStdcallHook(FuncType address, FuncType hook) : FastStdcallHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastStdcallHook(intptr_t address, FuncType hook) : FastStdcallHook(address)
	{
		Hook(hook);
	}

	FastStdcallHook() = delete;
	FastStdcallHook(FastStdcallHook&) = delete;
	FastStdcallHook(FastStdcallHook&&) = delete;

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (trampoline)
			throw new std::exception("Cannot apply hook to already hooked function!");
		trampoline = (FuncType)GenerateTrampoline((uint8_t*)origaddr);
		WriteJump(origaddr, hook); // Write jump from original function to the hook
	}

	// Call the original function, bypassing the hook.
	void Original(TArgs... args)
	{
		if (trampoline)
		{
			trampoline(args...);
		}
	}

private:
	const FuncType origaddr;
	FuncType trampoline = nullptr;
};

/// <summary>
/// A fastcall function hooking class using the function return and argument types as a base.
/// </summary>
/// <typeparam name="TRet">The return type.</typeparam>
/// <typeparam name="...TArgs">The type of each argument from left to right.</typeparam>
template<typename TRet, typename... TArgs>
class FastFastcallHook
{
public:
	typedef TRet(__fastcall* FuncType)(TArgs...);

	FastFastcallHook(FuncType address) : origaddr(address) {	}

	FastFastcallHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastFastcallHook(FuncType address, FuncType hook) : FastFastcallHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastFastcallHook(intptr_t address, FuncType hook) : FastFastcallHook(address)
	{
		Hook(hook);
	}

	FastFastcallHook() = delete;
	FastFastcallHook(FastFastcallHook&) = delete;
	FastFastcallHook(FastFastcallHook&&) = delete;

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (trampoline)
			throw new std::exception("Cannot apply hook to already hooked function!");
		trampoline = (FuncType)GenerateTrampoline((uint8_t*)origaddr);
		WriteJump(origaddr, hook); // Write jump from original function to the hook
	}

	// Call the original function, bypassing the hook.
	TRet Original(TArgs... args)
	{
		if (trampoline)
		{
			return trampoline(args...);
		}
		else
		{
			TRet ret = TRet();
			return ret;
		}
	}

private:
	const FuncType origaddr;
	FuncType trampoline = nullptr;
};

/// <summary>
/// A void fastcall function hooking class using the function argument types as a base.
/// </summary>
/// <typeparam name="...TArgs">The type of each argument from left to right.</typeparam>
template<typename... TArgs>
class FastFastcallHook<void, TArgs...>
{
public:
	typedef void(__fastcall* FuncType)(TArgs...);

	FastFastcallHook(FuncType address) : origaddr(address) {	}

	FastFastcallHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastFastcallHook(FuncType address, FuncType hook) : FastFastcallHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastFastcallHook(intptr_t address, FuncType hook) : FastFastcallHook(address)
	{
		Hook(hook);
	}

	FastFastcallHook() = delete;
	FastFastcallHook(FastFastcallHook&) = delete;
	FastFastcallHook(FastFastcallHook&&) = delete;

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (trampoline)
			throw new std::exception("Cannot apply hook to already hooked function!");
		trampoline = (FuncType)GenerateTrampoline((uint8_t*)origaddr);
		WriteJump(origaddr, hook); // Write jump from original function to the hook
	}

	// Call the original function, bypassing the hook.
	void Original(TArgs... args)
	{
		if (trampoline)
		{
			trampoline(args...);
		}
	}

private:
	const FuncType origaddr;
	FuncType trampoline = nullptr;
};

/// <summary>
/// A thiscall function hooking class using the function return and argument types as a base.
/// The hooked function should be cdecl with the this pointer as the first argument.
/// </summary>
/// <typeparam name="TRet">The return type.</typeparam>
/// <typeparam name="...TArgs">The type of each argument from left to right.</typeparam>
template<typename TRet, typename... TArgs>
class FastThiscallHook
{
public:
	typedef TRet(*FuncType)(TArgs...);

	FastThiscallHook(FuncType address) : origaddr(address) {   }

	FastThiscallHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastThiscallHook(FuncType address, FuncType hook) : FastThiscallHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastThiscallHook(intptr_t address, FuncType hook) : FastThiscallHook(address)
	{
		Hook(hook);
	}

	FastThiscallHook() = delete;
	FastThiscallHook(FastThiscallHook&) = delete;
	FastThiscallHook(FastThiscallHook&&) = delete;

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (trampoline)
			throw new std::exception("Cannot apply hook to already hooked function!");
		trampoline = (FuncType)GenerateThiscallWrapper(GenerateTrampoline((uint8_t*)origaddr), sizeof...(TArgs) - 1);
		GenerateThiscallHook(hook, origaddr, sizeof...(TArgs) - 1);
	}

	// Call the original function, bypassing the hook.
	TRet Original(TArgs... args)
	{
		if (trampoline)
		{
			return trampoline(args...);
		}
		else
		{
			TRet ret = TRet();
			return ret;
		}
	}

private:
	const FuncType origaddr;
	FuncType trampoline = nullptr;
};

/// <summary>
/// A void function hooking class using the function argument types as a base.
/// The hooked function should be cdecl with the this pointer as the first argument.
/// </summary>
/// <typeparam name="...TArgs">The type of each argument from left to right.</typeparam>
template<typename... TArgs>
class FastThiscallHook<void, TArgs...>
{
public:
	typedef void(*FuncType)(TArgs...);

	FastThiscallHook(FuncType address) : origaddr(address) {   }

	FastThiscallHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastThiscallHook(FuncType address, FuncType hook) : FastThiscallHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastThiscallHook(intptr_t address, FuncType hook) : FastThiscallHook(address)
	{
		Hook(hook);
	}

	FastThiscallHook() = delete;
	FastThiscallHook(FastThiscallHook&) = delete;
	FastThiscallHook(FastThiscallHook&&) = delete;

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (trampoline)
			throw new std::exception("Cannot apply hook to already hooked function!");
		trampoline = (FuncType)GenerateThiscallWrapper(GenerateTrampoline((uint8_t*)origaddr), sizeof...(TArgs) - 1);
		GenerateThiscallHook(hook, origaddr, sizeof...(TArgs) - 1);
	}

	// Call the original function, bypassing the hook.
	void Original(TArgs... args)
	{
		if (trampoline)
		{
			return trampoline(args...);
		}
	}

private:
	const FuncType origaddr;
	FuncType trampoline = nullptr;
};

/// <summary>
/// A function hooking class using the function return and argument types as a base for non-standard calling conventions.
/// </summary>
/// <typeparam name="TRet">The return type.</typeparam>
/// <typeparam name="...TArgs">The type of each argument from left to right.</typeparam>
template<typename TRet, typename... TArgs>
class FastUsercallHook
{
public:
	typedef TRet(*FuncType)(TArgs...);

	FastUsercallHook(FuncType address) : origaddr(address) {   }

	FastUsercallHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}
	
	// Initialize the object and immediately apply a hook.
	template<typename... TRegs>
	FastUsercallHook(FuncType address, FuncType hook, int ret, TRegs... regs) : FastUsercallHook(address)
	{
		Hook(hook, ret, regs...);
	}
	
	// Initialize the object and immediately apply a hook.
	template<typename... TRegs>
	FastUsercallHook(intptr_t address, FuncType hook, int ret, TRegs... regs) : FastUsercallHook(address)
	{
		Hook(hook, ret, regs...);
	}

	FastUsercallHook() = delete;
	FastUsercallHook(FastUsercallHook&) = delete;
	FastUsercallHook(FastUsercallHook&&) = delete;

	// Apply a hook to the unhooked function.
	template<typename... TRegs>
	void Hook(FuncType hook, int ret, TRegs... regs)
	{
		if (trampoline)
			throw new std::exception("Cannot apply hook to already hooked function!");
		trampoline = (FuncType)GenerateUsercallWrapper<FuncType>(ret, (intptr_t)GenerateTrampoline((uint8_t*)origaddr), regs...);
		GenerateUsercallHook<FuncType>(hook, ret, (intptr_t)origaddr, regs...);
	}

	// Call the original function, bypassing the hook.
	TRet Original(TArgs... args)
	{
		if (trampoline)
		{
			return trampoline(args...);
		}
		else
		{
			TRet ret = TRet();
			return ret;
		}
	}

private:
	const FuncType origaddr;
	FuncType trampoline = nullptr;
};

/// <summary>
/// A void function hooking class using the function argument types as a base for non-standard calling conventions.
/// </summary>
/// <typeparam name="...TArgs">The type of each argument from left to right.</typeparam>
template<typename... TArgs>
class FastUsercallHook<void, TArgs...>
{
public:
	typedef void(*FuncType)(TArgs...);

	FastUsercallHook(FuncType address) : origaddr(address) {   }

	FastUsercallHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	template<typename... TRegs>
	FastUsercallHook(FuncType address, FuncType hook, int ret, TRegs... regs) : FastUsercallHook(address)
	{
		Hook(hook, ret, regs...);
	}

	// Initialize the object and immediately apply a hook.
	template<typename... TRegs>
	FastUsercallHook(intptr_t address, FuncType hook, int ret, TRegs... regs) : FastUsercallHook(address)
	{
		Hook(hook, ret, regs...);
	}

	FastUsercallHook() = delete;
	FastUsercallHook(FastUsercallHook&) = delete;
	FastUsercallHook(FastUsercallHook&&) = delete;

	// Apply a hook to the unhooked function.
	template<typename... TRegs>
	void Hook(FuncType hook, int ret, TRegs... regs)
	{
		if (trampoline)
			throw new std::exception("Cannot apply hook to already hooked function!");
		trampoline = (FuncType)GenerateUsercallWrapper<FuncType>(ret, (intptr_t)GenerateTrampoline((uint8_t*)origaddr), regs...);
		GenerateUsercallHook<FuncType>(hook, ret, (intptr_t)origaddr, regs...);
	}

	// Call the original function, bypassing the hook.
	void Original(TArgs... args)
	{
		if (trampoline)
		{
			return trampoline(args...);
		}
	}

private:
	const FuncType origaddr;
	FuncType trampoline = nullptr;
};

/// <summary>
/// A function hooking class using the function return and argument types as a base for non-standard calling conventions with callee cleanup.
/// </summary>
/// <typeparam name="TRet">The return type.</typeparam>
/// <typeparam name="...TArgs">The type of each argument from left to right.</typeparam>
template<typename TRet, typename... TArgs>
class FastUserpurgeHook
{
public:
	typedef TRet(*FuncType)(TArgs...);

	FastUserpurgeHook(FuncType address) : origaddr(address) {   }

	FastUserpurgeHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}
	
	// Initialize the object and immediately apply a hook.
	template<typename... TRegs>
	FastUserpurgeHook(FuncType address, FuncType hook, int ret, TRegs... regs) : FastUserpurgeHook(address)
	{
		Hook(hook, ret, regs...);
	}
	
	// Initialize the object and immediately apply a hook.
	template<typename... TRegs>
	FastUserpurgeHook(intptr_t address, FuncType hook, int ret, TRegs... regs) : FastUserpurgeHook(address)
	{
		Hook(hook, ret, regs...);
	}

	FastUserpurgeHook() = delete;
	FastUserpurgeHook(FastUserpurgeHook&) = delete;
	FastUserpurgeHook(FastUserpurgeHook&&) = delete;

	// Apply a hook to the unhooked function.
	template<typename... TRegs>
	void Hook(FuncType hook, int ret, TRegs... regs)
	{
		if (trampoline)
			throw new std::exception("Cannot apply hook to already hooked function!");
		trampoline = (FuncType)GenerateUserpurgeWrapper<FuncType>(ret, (intptr_t)GenerateTrampoline((uint8_t*)origaddr), regs...);
		GenerateUserpurgeHook<FuncType>(hook, ret, (intptr_t)origaddr, regs...);
	}

	// Call the original function, bypassing the hook.
	TRet Original(TArgs... args)
	{
		if (trampoline)
		{
			return trampoline(args...);
		}
		else
		{
			TRet ret = TRet();
			return ret;
		}
	}

private:
	const FuncType origaddr;
	FuncType trampoline = nullptr;
};

/// <summary>
/// A void function hooking class using the function argument types as a base for non-standard calling conventions with callee cleanup.
/// </summary>
/// <typeparam name="...TArgs">The type of each argument from left to right.</typeparam>
template<typename... TArgs>
class FastUserpurgeHook<void, TArgs...>
{
public:
	typedef void(*FuncType)(TArgs...);

	FastUserpurgeHook(FuncType address) : origaddr(address) {   }

	FastUserpurgeHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	template<typename... TRegs>
	FastUserpurgeHook(FuncType address, FuncType hook, int ret, TRegs... regs) : FastUserpurgeHook(address)
	{
		Hook(hook, ret, regs...);
	}

	// Initialize the object and immediately apply a hook.
	template<typename... TRegs>
	FastUserpurgeHook(intptr_t address, FuncType hook, int ret, TRegs... regs) : FastUserpurgeHook(address)
	{
		Hook(hook, ret, regs...);
	}

	FastUserpurgeHook() = delete;
	FastUserpurgeHook(FastUserpurgeHook&) = delete;
	FastUserpurgeHook(FastUserpurgeHook&&) = delete;

	// Apply a hook to the unhooked function.
	template<typename... TRegs>
	void Hook(FuncType hook, int ret, TRegs... regs)
	{
		if (trampoline)
			throw new std::exception("Cannot apply hook to already hooked function!");
		trampoline = (FuncType)GenerateUserpurgeWrapper<FuncType>(ret, (intptr_t)GenerateTrampoline((uint8_t*)origaddr), regs...);
		GenerateUserpurgeHook<FuncType>(hook, ret, (intptr_t)origaddr, regs...);
	}

	// Call the original function, bypassing the hook.
	void Original(TArgs... args)
	{
		if (trampoline)
		{
			return trampoline(args...);
		}
	}

private:
	const FuncType origaddr;
	FuncType trampoline = nullptr;
};

/// <summary>
/// A function hooking class using a function pointer as a base.
/// Note: Original is a function pointer and may be null if Hook was not called before.
/// </summary>
/// <typeparam name="FuncType">Function pointer defining the hooked function.</typeparam>
template<typename FuncType>
class FastFunctionHookPtr
{
public:
	FastFunctionHookPtr(FuncType address) : origaddr(address) {	}

	FastFunctionHookPtr(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastFunctionHookPtr(FuncType address, FuncType hook) : FastFunctionHookPtr(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastFunctionHookPtr(intptr_t address, FuncType hook) : FastFunctionHookPtr(address)
	{
		Hook(hook);
	}

	FastFunctionHookPtr() = delete;
	FastFunctionHookPtr(FastFunctionHookPtr&) = delete;
	FastFunctionHookPtr(FastFunctionHookPtr&&) = delete;

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (Original)
			throw new std::exception("Cannot apply hook to already hooked function!");
		Original = (FuncType)GenerateTrampoline((uint8_t*)origaddr);
		WriteJump(origaddr, hook); // Write jump from original function to the hook
	}

	// Call the original function, bypassing the hook.
	FuncType Original = nullptr;

private:
	const FuncType origaddr;
};

/// <summary>
/// A function hooking class using a function pointer as a base for non-standard calling conventions.
/// Note: Original is a function pointer and may be null if Hook was not called before.
/// Use enum in UsercallFunctionHandler.h to specify each register from left to right.
/// </summary>
/// <typeparam name="FuncType">Function pointer defining the hooked function.</typeparam>
/// <typeparam name="TRetReg">The return register.</typeparam>
/// <typeparam name="TRegs">Registers of each argument in left to right order.</typeparam>
template<typename FuncType, int TRetReg, int... TRegs>
class FastUsercallHookPtr
{
public:
	FastUsercallHookPtr(FuncType address) : origaddr(address) {   }

	FastUsercallHookPtr(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastUsercallHookPtr(FuncType address, FuncType hook) : FastUsercallHookPtr(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastUsercallHookPtr(intptr_t address, FuncType hook) : FastUsercallHookPtr(address)
	{
		Hook(hook);
	}

	FastUsercallHookPtr() = delete;
	FastUsercallHookPtr(FastUsercallHookPtr&) = delete;
	FastUsercallHookPtr(FastUsercallHookPtr&&) = delete;

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (Original)
			throw new std::exception("Cannot apply hook to already hooked function!");
		Original = (FuncType)GenerateUsercallWrapper<FuncType>(TRetReg, (intptr_t)GenerateTrampoline((uint8_t*)origaddr), TRegs...);
		GenerateUsercallHook<FuncType>(hook, TRetReg, (intptr_t)origaddr, TRegs...);
	}

	// Call the original function, bypassing the hook.
	FuncType Original = nullptr;

private:
	const FuncType origaddr;
};

/// <summary>
/// A function hooking class using a function pointer as a base for non-standard calling conventions with callee cleanup.
/// Note: Original is a function pointer and may be null if Hook was not called before.
/// Use enum in UsercallFunctionHandler.h to specify each register from left to right.
/// </summary>
/// <typeparam name="FuncType">Function pointer defining the hooked function.</typeparam>
/// <typeparam name="TRetReg">The return register.</typeparam>
/// <typeparam name="TRegs">Registers of each argument in left to right order.</typeparam>
template<typename FuncType, int TRetReg, int... TRegs>
class FastUserpurgeHookPtr
{
public:
	FastUserpurgeHookPtr(FuncType address) : origaddr(address) {   }

	FastUserpurgeHookPtr(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastUserpurgeHookPtr(FuncType address, FuncType hook) : FastUserpurgeHookPtr(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastUserpurgeHookPtr(intptr_t address, FuncType hook) : FastUserpurgeHookPtr(address)
	{
		Hook(hook);
	}

	FastUserpurgeHookPtr() = delete;
	FastUserpurgeHookPtr(FastUserpurgeHookPtr&) = delete;
	FastUserpurgeHookPtr(FastUserpurgeHookPtr&&) = delete;

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (Original)
			throw new std::exception("Cannot apply hook to already hooked function!");
		Original = (FuncType)GenerateUserpurgeWrapper<FuncType>(TRetReg, (intptr_t)GenerateTrampoline((uint8_t*)origaddr), TRegs...);
		GenerateUserpurgeHook<FuncType>(hook, TRetReg, (intptr_t)origaddr, TRegs...);
	}

	// Call the original function, bypassing the hook.
	FuncType Original = nullptr;

private:
	const FuncType origaddr;
};