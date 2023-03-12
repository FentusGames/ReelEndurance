#ifndef __ASHITA_ReelEndurance_H_INCLUDED__
#define __ASHITA_ReelEndurance_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/**
 * Main Ashita SDK Header
 *
 * This is the main header to include while creating Ashita plugins. This header includes
 * the various interfaces, structures, and enumerations needed for working with Ashita. It
 * also includes the other headers included with the SDK in a single file.
 *
 * Update the path to this file to where you have the latest version of Ashita installed to.
 */

#include "S:/Steam/steamapps/common/FFXINA/SquareEnix/AshitaV4/plugins/sdk/Ashita.h"

/**
 * ReelEndurance Class Implementation
 * 
 * This is the main class object the plugin will create an instance of and return to Ashita when
 * the 'CreatePlugin' export is called. This class MUST inherit from the 'IPlugin' interface to
 * work properly with Ashita.
 */
class ReelEndurance final : public IPlugin, Ashita::Threading::Thread
{
    IAshitaCore* m_AshitaCore;          // The main AshitaCore object pointer.
    ILogManager* m_LogManager;          // The main LogManager object pointer.
    IDirect3DDevice8* m_Direct3DDevice; // The main Direct3D device object pointer.
    uint32_t m_PluginId;                // The plugins id. (It's base address.)

    // CONFIG STATIC
    bool debug = true;                  // Sets debug mode. (Default: false)

public:
    ReelEndurance(void);
    ~ReelEndurance(void);

    // Plugin Information Properties
    const char* GetName(void) const override;
    const char* GetAuthor(void) const override;
    const char* GetDescription(void) const override;
    const char* GetLink(void) const override;
    double GetVersion(void) const override;
    double GetInterfaceVersion(void) const override;
    int32_t GetPriority(void) const override;
    uint32_t GetFlags(void) const override;

    // General Methods
    bool Initialize(IAshitaCore* core, ILogManager* logger, uint32_t id) override;
    void Release(void) override;

    // Event Callbacks: PluginManager
    void HandleEvent(const char* eventName, const void* eventData, const uint32_t eventSize) override;

    // Event Callbacks: ChatManager
    bool HandleCommand(int32_t mode, const char* command, bool injected) override;
    bool HandleIncomingText(int32_t mode, bool indent, const char* message, int32_t* modifiedMode, bool* modifiedIndent, char* modifiedMessage, bool injected, bool blocked) override;
    bool HandleOutgoingText(int32_t mode, const char* message, int32_t* modifiedMode, char* modifiedMessage, bool injected, bool blocked) override;

    // Event Callbacks: PacketManager
    bool HandleIncomingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked) override;
    bool HandleOutgoingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked) override;

    // Event Callbacks: Direct3D
    bool Direct3DInitialize(IDirect3DDevice8* device) override;
    void Direct3DBeginScene(bool isRenderingBackBuffer) override;
    void Direct3DEndScene(bool isRenderingBackBuffer) override;
    void Direct3DPresent(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) override;
    bool Direct3DSetRenderState(D3DRENDERSTATETYPE State, DWORD* Value) override;
    bool Direct3DDrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) override;
    bool Direct3DDrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT minIndex, UINT NumVertices, UINT startIndex, UINT primCount) override;
    bool Direct3DDrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) override;
    bool Direct3DDrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertexIndices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) override;

    // Thread.cpp
    uint32_t ThreadEntry(void);

    // ReelEndurance.cpp
    void Log(const char* format, ...);
    void QueueCommand(int32_t, const char* format, ...);

    bool IsRunning();
    void SetRunning(bool _running);

    int RandomA(int factor);

    bool GetValid();
    bool GetMonst();
    bool GetReset();
    bool GetFound();

    bool InventoryFull();

    int GetCatches();
};

#endif // __ASHITA_ReelEndurance_H_INCLUDED__