#include "ReelEndurance.h"
#include <iostream>
#include <thread>

using namespace std;

ImGuiWindowFlags window_flags = 0;
IGuiManager* imgui;

/**
 * Constructor and Deconstructor
 */
ReelEndurance::ReelEndurance(void)
    : m_AshitaCore(nullptr)
    , m_LogManager(nullptr)
    , m_Direct3DDevice(nullptr)
    , m_PluginId(0)
{}
ReelEndurance::~ReelEndurance(void)
{}

/**
 * Returns the plugins name.
 * 
 * @return {const char*} The plugins name.
 */
const char* ReelEndurance::GetName(void) const
{
    return "ReelEndurance";
}

/**
 * Returns the plugins author.
 * 
 * @return {const char*} The plugins author.
 */
const char* ReelEndurance::GetAuthor(void) const
{
    return "Fentus";
}

/**
 * Returns the plugins description.
 * 
 * @return {const char*} The plugins description.
 */
const char* ReelEndurance::GetDescription(void) const
{
    return "Slowest Fishing Bot Ever!";
}

/**
 * returns the plugins homepage link.
 * 
 * @return {const char*} The plugins homepage link.
 */
const char* ReelEndurance::GetLink(void) const
{
    return "https://fentusgames.com/";
}

/**
 * Returns the plugins version.
 * 
 * @return {double} The plugins version.
 */
double ReelEndurance::GetVersion(void) const
{
    return 1;
}

/**
 * Returns the plugins Ashita interface version it was compiled against.
 * 
 * @return {double} The plugins interface version.
 */
double ReelEndurance::GetInterfaceVersion(void) const
{
    return ASHITA_INTERFACE_VERSION;
}

/**
 * Returns the plugins execution priority.
 * 
 * @return {int32_t} The plugins execution priority.
 * @notes
 * 
 *      The priority value is used to determine the execution order of the plugins loaded in Ashita. When an event is fired, Ashita
 *      will loop through each of the loaded plugins and allow them to attempt to handle the event. The order that the plugins are
 *      passed the event is determined by this priority. By default, 0 will load a plugin 'next' in the list of plugins who also are
 *      set to 0. Meaning if other plugins are already loaded at 0, and your plugin is also 0 your plugin will be last.
 *      
 *      Plugins can force themselves sooner or later into the execution order by using this value (both negative and position).
 *      Negative values will place your plugin sooner in the execution order, while positive values will place it later.
 *      
 *      Plugins should use 0 by default if order does not matter to their purpose.
 */
int32_t ReelEndurance::GetPriority(void) const
{
    return 0;
}

/**
 * Returns the plugins flags.
 * 
 * @return {uint32_t} The plugins flags.
 * @notes
 * 
 *      Ashita offers a various number of events that can be invoked inside of a plugin, allowing plugins to have a lot of control
 *      over the flow of the game. However, this can create a lot of overhead, more specifically on lower end machines. Plugins can
 *      use this property to specify the exact events it plans to make use of, allowing Ashita to only invoke ones it needs.
 *      
 *      Plugins should explicitly state which flags they plan to make use of and avoid using 'All' flags.
 *      This can help with the performance of the game for lower end machines.
 *      
 *      See the 'Ashita::PluginFlags' enumeration for more information on what each flag does.
 */
uint32_t ReelEndurance::GetFlags(void) const
{
    return (uint32_t)Ashita::PluginFlags::All;
}

/**
* My random bad code....
*/



/**
 * Event invoked when the plugin is being loaded and initialized.
 *
 * @param {IAshitaCore*} core - The main Ashita core instance used to interact with the Ashita hook.
 * @param {ILogManager*} logger - The log manager instance used to write to the debug log files.
 * @param {uint32_t} id - The plugins unique id. (Matches the plugins module base address.)
 * @return {bool} True on success, false otherwise.
 *
 * @notes
 * 
 *      Plugins must return true from this function in order to be considered valid and continue to load.
 *      If your plugin fails to meet any requirements you feel are manditory for it to run, you should return false here and prevent it
 *      from loading further.
 */
bool ReelEndurance::Initialize(IAshitaCore* core, ILogManager* logger, uint32_t id)
{
    this->m_AshitaCore = core;
    this->m_LogManager = logger;
    this->m_PluginId   = id;

    this->Start();

    static bool no_titlebar = false;
    static bool no_scrollbar = true;
    static bool no_menu = true;
    static bool no_move = false;
    static bool no_resize = true;
    static bool no_collapse = true;
    static bool no_close = true;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;
    static bool unsaved_document = false;

    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;

    imgui = m_AshitaCore->GetGuiManager();

    return true;
}

/**
 * Event invoked when the plugin is being unloaded.
 *
 * @notes
 *
 *      Plugins should use this event to cleanup all resources they created or used during their lifespan.
 *      (ie. Fonts, primitives, textures, Direct3D related resources, memory allocations, etc.)
 */
void ReelEndurance::Release(void)
{
    this->Stop();
}

/**
 * Event invoked when another plugin has raised a custom event for other plugins to handle.
 *
 * @param {const char*} eventName - The name of the custom event being raised.
 * @param {const void*} eventData - The custom event data to pass through the event.
 * @param {uint32_t} eventSize - The size of the custom event data buffer.
 * 
 * @notes
 * 
 *      Only invoked if Ashita::PluginFlags::UsePluginEvents flag is set.
 *
 *      Plugins can make use of the custom event system as a way to talk to other plugins in a safe manner.
 *      Events can be raised via the PluginManager::RaiseEvent method which will cause this handler to be
 *      invoked in all loaded plugins with the given event information.
 */
void ReelEndurance::HandleEvent(const char* eventName, const void* eventData, const uint32_t eventSize)
{
    UNREFERENCED_PARAMETER(eventName);
    UNREFERENCED_PARAMETER(eventData);
    UNREFERENCED_PARAMETER(eventSize);
}

/**
 * Event invoked when an input command is being processed by the game client.
 *
 * @param {int32_t} mode - The mode of the command. (See: Ashita::CommandMode)
 * @param {const char*} command - The raw command string.
 * @param {bool} injected - Flag if the command was injected from an Ashita related resource. (Addon, plugin or internally.)
 * @return {bool} True if handled and should be blocked, false otherwise.
 *
 * @notes
 * 
 *      Only invoked if Ashita::PluginFlags::UseCommands flag is set.
 *      
 *      Any commands sent through the games input handler will be passed to plugins to be processed first allowing plugins to intercept
 *      and handle any kind of command they wish. This includes commands typed into the chat bar, commands invoked from macros, menu items
 *      and so on. Commands that have been injected by Ashita or another plugin will be marked via the injected parameter.
 *      
 *      If a plugin returns true, the command is blocked from further processing by Ashita or the game client and is considered handled.
 *      
 *      Plugins should return true for any commands they have handled or reacted to when appropriate. To prevent deadlocks by trying to
 *      inject another command here, plugins should instead use the IChatManager::QueueCommand function for any manual command inserts
 *      back into the game.
 */
bool ReelEndurance::HandleCommand(int32_t mode, const char* command, bool injected)
{
    UNREFERENCED_PARAMETER(mode);
    UNREFERENCED_PARAMETER(command);
    UNREFERENCED_PARAMETER(injected);

    return false;
}

/**
 * Event invoked when incoming text is being processed by the game client, to be added to the chat window.
 *
 * @param {int32_t} mode - The message mode. (Determines its base color.)
 * @param {bool} indent - Flag that determines if the message is indented.
 * @param {const char*} message - The raw message string.
 * @param {int32_t*} modifiedMode - The modified mode, if any, that has been altered by another addon/plugin.
 * @param {bool*} modifiedIndent - The modified indent flag, if any, that has been altered by another addon/plugin.
 * @param {char*} modifiedMessage - The modified message string, if any, that has been altered by another addon/plugin.
 * @param {bool} injected - Flag if the message was injected from an Ashita related resource. (Addon, plugin, or internally.)
 * @param {bool} blocked - Flag if the message has been blocked by another addon/plugin.
 * @return {bool} True if handled and should be blocked, false otherwise.
 *
 * @notes
 * 
 *      Only invoked if Ashita::PluginFlags::UseText flag is set.
 *      
 *      If a plugin returns true, the block flag is set to true (cannot be unset), however the message will still be passed to all loaded
 *      plugins whom are marked to accept the event. Plugins should check if the blocked flag has been set first before reacting to messages
 *      in case a previous plugin has deemed it to not be displayed in the game chat log. Unless your plugin requires reacting to certain/all
 *      messages, then it should simply return immediately if blocked is already true.
 *      
 *      mode and message contain the original, unmodified data of the event which can be used to see what was originally used to invoke the 
 *      event, as well as for plugins that require the original unedited message to properly operate. Plugins should however check for changes
 *      by seeing if modifiedMessage has been altered. If modifiedMessage has any value (strlen > 0) then plugins should honor previous plugin
 *      changes made to the message. You should only completely overwrite modifiedMessage if your plugin specifically needs to take over a given
 *      message. (For example if a certain plugin injects color tags into a message, your plugin should not remove them unless absolutely necessary.)
 *      
 *      modifiedMessage is an internal buffer of 4096 bytes, therefore it should have plenty of space for any message you wish to add.
 *      
 *      You should not call Write, Writef, or AddChatMessage functions here! Otherwise you will cause a stack overflow.

bool ReelEndurance::HandleIncomingText(int32_t mode, bool indent, const char* message, int32_t* modifiedMode, bool* modifiedIndent, char* modifiedMessage, bool injected, bool blocked)
{
    UNREFERENCED_PARAMETER(mode);
    UNREFERENCED_PARAMETER(indent);
    UNREFERENCED_PARAMETER(message);
    UNREFERENCED_PARAMETER(modifiedMode);
    UNREFERENCED_PARAMETER(modifiedIndent);
    UNREFERENCED_PARAMETER(modifiedMessage);
    UNREFERENCED_PARAMETER(injected);
    UNREFERENCED_PARAMETER(blocked);

    return false;
}
 */

/**
 * Event invoked when the game client is sending text to the server. 
 *
 * @param {int32_t} mode - The message mode. (See: Ashita::CommandMode)
 * @param {const char*} message - The raw message string.
 * @param {int32_t*} modifiedMode - The modified mode, if any, that has been altered by another addon/plugin.
 * @param {char*} modifiedMessage - The modified message string, if any, that has been altered by another addon/plugin.
 * @param {bool} injected - Flag if the message was injected from an Ashita related resource. (Addon, plugin, or internally.)
 * @param {bool} blocked - Flag if the message has been blocked by another addon/plugin.
 * @return {bool} True if handled and should be blocked, false otherwise.
 *
 * @notes
 * 
 *      Only invoked if Ashita::PluginFlags::UseText flag is set.
 *      
 *      This event is invoked after an input command has gone unhandled and is about to be sent to the game client, potentially to the server. This
 *      can be used as an end-all-catch-all of processing input commands. Actual command handling should be done in the HandleCommand event though.
 *      
 *      If a plugin returns true, the command is blocked from further processing by Ashita or the game client and is considered handled.
 *      
 *      Plugins should return true for any commands they have handled or reacted to when appropriate. To prevent deadlocks by trying to
 *      inject another command here, plugins should instead use the IChatManager::QueueCommand function for any manual command inserts
 *      back into the game.
 *      
 *      mode and message contain the original, unmodified data of the event which can be used to see what was originally used to invoke the 
 *      event, as well as for plugins that require the original unedited message to properly operate. Plugins should however check for changes
 *      by seeing if modifiedMessage has been altered. If modifiedMessage has any value (strlen > 0) then plugins should honor previous plugin
 *      changes made to the message. You should only completely overwrite modifiedMessage if your plugin specifically needs to take over a given
 *      message. 
 *      
 *      modifiedMessage is an internal buffer of 4096 bytes, therefore it should have plenty of space for any message you wish to add. 
 */
bool ReelEndurance::HandleOutgoingText(int32_t mode, const char* message, int32_t* modifiedMode, char* modifiedMessage, bool injected, bool blocked)
{
    UNREFERENCED_PARAMETER(mode);
    UNREFERENCED_PARAMETER(message);
    UNREFERENCED_PARAMETER(modifiedMode);
    UNREFERENCED_PARAMETER(modifiedMessage);
    UNREFERENCED_PARAMETER(injected);
    UNREFERENCED_PARAMETER(blocked);

    return false;
}

/**
 * Event invoked when the game client is processing an incoming packet.
 *
 * @param {uint16_t} id - The id of the packet.
 * @param {uint32_t} size - The size of the packet.
 * @param {const uint8_t*} data - The raw data of the packet.
 * @param {uint8_t*} modified - The modified packet data, if any, that has been altered by another addon/plugin.
 * @param {uint32_t} sizeChunk - The size of the full packet chunk that contained the packet.
 * @param {const uint8_t*} dataChunk - The raw data of the full packet chunk that contained the packet.
 * @param {bool} injected - Flag if the packet was injected from an Ashita related resource. (Addon, plugin, or internally.)
 * @param {bool} blocked - Flag if the packet has been blocked by another addon/plugin.
 * @return {bool} True if handled and should be blocked, false otherwise.
 *
 * @notes
 * 
 *      Only invoked if Ashita::PluginFlags::UsePackets flag is set.
 *      
 *      If a plugin returns true, the block flag is set to true (cannot be unset), however the event will still be passed to all loaded
 *      plugins whom are marked to accept it. Plugins should check if the blocked flag has been set first before reacting to the event
 *      in case a previous plugin has deemed it to be blocked. Unless your plugin requires reacting to certain/all packets, then it 
 *      should simply return immediately if blocked is already true.
 *      
 *      Packets in FFXI are sent in chunks, meaning there are multiple packets inside of each chunk. This information may be needed when
 *      dealing with certain packet ids, thus Ashita offers the ability to see the full chunk each packet was part of.
 *      
 *      id, size, and data are all specific to the individual packet that caused the event to be invoked and contain the unmodified
 *      information about the individual packet. These should not be edited.
 *      
 *      modified should be used to determine if changes have been made to the packet by Ashita or another addon/plugin. By default, 
 *      modified is a direct copy of the original packet data.
 *      
 *      sizeChunk and dataChunk hold the data of the entire chunk the packet was part of that is being processed in the event. These can
 *      be useful for plugins that may need to look at other packets in the chunk that relate to the current packet of the event. These
 *      should not be edited.
 */
bool ReelEndurance::HandleIncomingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked)
{
    UNREFERENCED_PARAMETER(id);
    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(data);
    UNREFERENCED_PARAMETER(modified);
    UNREFERENCED_PARAMETER(sizeChunk);
    UNREFERENCED_PARAMETER(dataChunk);
    UNREFERENCED_PARAMETER(injected);
    UNREFERENCED_PARAMETER(blocked);

    return false;
}

/**
 * Event invoked when the game client is processing an outgoing packet.
 *
 * @param {uint16_t} id - The id of the packet.
 * @param {uint32_t} size - The size of the packet.
 * @param {const uint8_t*} data - The raw data of the packet.
 * @param {uint8_t*} modified - The modified packet data, if any, that has been altered by another addon/plugin.
 * @param {uint32_t} sizeChunk - The size of the full packet chunk that contained the packet.
 * @param {const uint8_t*} dataChunk - The raw data of the full packet chunk that contained the packet.
 * @param {bool} injected - Flag if the packet was injected from an Ashita related resource. (Addon, plugin, or internally.)
 * @param {bool} blocked - Flag if the packet has been blocked by another addon/plugin.
 * @return {bool} True if handled and should be blocked, false otherwise.
 *
 * @notes
 * 
 *      Only invoked if Ashita::PluginFlags::UsePackets flag is set.
 *      
 *      If a plugin returns true, the block flag is set to true (cannot be unset), however the event will still be passed to all loaded
 *      plugins whom are marked to accept it. Plugins should check if the blocked flag has been set first before reacting to the event
 *      in case a previous plugin has deemed it to be blocked. Unless your plugin requires reacting to certain/all packets, then it 
 *      should simply return immediately if blocked is already true.
 *      
 *      Packets in FFXI are sent in chunks, meaning there are multiple packets inside of each chunk. This information may be needed when
 *      dealing with certain packet ids, thus Ashita offers the ability to see the full chunk each packet was part of.
 *      
 *      id, size, and data are all specific to the individual packet that caused the event to be invoked and contain the unmodified
 *      information about the individual packet. These should not be edited.
 *      
 *      modified should be used to determine if changes have been made to the packet by Ashita or another addon/plugin. By default, 
 *      modified is a direct copy of the original packet data.
 *      
 *      sizeChunk and dataChunk hold the data of the entire chunk the packet was part of that is being processed in the event. These can
 *      be useful for plugins that may need to look at other packets in the chunk that relate to the current packet of the event. These
 *      should not be edited.
 */
bool ReelEndurance::HandleOutgoingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked)
{
    UNREFERENCED_PARAMETER(id);
    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(data);
    UNREFERENCED_PARAMETER(modified);
    UNREFERENCED_PARAMETER(sizeChunk);
    UNREFERENCED_PARAMETER(dataChunk);
    UNREFERENCED_PARAMETER(injected);
    UNREFERENCED_PARAMETER(blocked);

    return false;
}

/**
 * Event invoked when the plugin is being initialized for Direct3D usage.
 *
 * @param {IDirect3DDevice8*} device - The Direct3D8 device object.
 * @return {bool} True on success, false otherwise.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 * 
 *      Plugins must return true from this function in order to be considered valid and continue to load if they do use Direct3D features.
 *      
 *      If your plugin fails to meet any Direct3D requirements you feel are manditory for it to run, you should return false here and
 *      prevent it from loading further.
 */
bool ReelEndurance::Direct3DInitialize(IDirect3DDevice8* device)
{
    this->m_Direct3DDevice = device;

    return true;
}

/**
 * Event invoked when the Direct3D device is beginning a scene.
 *
 * @param {bool} isRenderingBackBuffer - Flag set if the scene is being rendered to the back buffer.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 *
 *      This event is invoked before the actual IDirect3DDevice8::BeginScene call is invoked.
 *
 *      Multiple scenes can be rendered each frame, thus the isRenderingBackBuffer flag is available to determine when the scene is being
 *      rendered to the back buffer render target. (Previous Ashita versions only invoked this event when this flag would be true.)
 */
void ReelEndurance::Direct3DBeginScene(bool isRenderingBackBuffer)
{
    UNREFERENCED_PARAMETER(isRenderingBackBuffer);
}

/**
 * Event invoked when the Direct3D device is ending a scene.
 *
 * @param {bool} isRenderingBackBuffer - Flag set if the scene is being rendered to the back buffer.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 *
 *      This event is invoked before the actual IDirect3DDevice8::EndScene call is invoked.
 *
 *      Multiple scenes can be rendered each frame, thus the isRenderingBackBuffer flag is available to determine when the scene is being
 *      rendered to the back buffer render target. (Previous Ashita versions only invoked this event when this flag would be true.)
 */
void ReelEndurance::Direct3DEndScene(bool isRenderingBackBuffer)
{
    UNREFERENCED_PARAMETER(isRenderingBackBuffer);
}

/**
 * Event invoked when the Direct3D device is presenting a scene.
 *
 * @param {const RECT*} pSourceRect - The source rect being presented.
 * @param {const RECT*} pDestRect - The destination rect being presented into.
 * @param {HWND} hDestWindowOverride - The override window handle to present into.
 * @param {const RGNDATA*} pDirtyRegion - The present dirty regions.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 *
 *      This event is invoked before the actual IDirect3DDevice8::Present call is invoked.
 *
 *      For best results of custom Direct3D rendering, it is best to do your own custom drawing here to draw over-top of all game related
 *      scenes and objects. Usage of ImGui should be done here as well.
 */
void ReelEndurance::Direct3DPresent(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
    UNREFERENCED_PARAMETER(pSourceRect);
    UNREFERENCED_PARAMETER(pDestRect);
    UNREFERENCED_PARAMETER(hDestWindowOverride);
    UNREFERENCED_PARAMETER(pDirtyRegion);

    // GUI
    imgui->SetNextWindowSize(ImVec2(-FLT_MIN, -FLT_MIN), ImGuiCond_Always); // SIZE OF GUI

    char title[32];
    sprintf_s(title, "%s %.2f", this->GetName(), this->GetVersion());

    if (!imgui->Begin(title, NULL, window_flags)) {
        imgui->End();
        return;
    }

    // GUI PADDING
    imgui->PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

    // ADD GUI ELEMENTS...
    if (imgui->Button("Start", ImVec2(70, 30))) {
        SetRunning(true);
        QueueCommand(2, "/fish");
    }

    imgui->SameLine();

    if (imgui->Button("Stop", ImVec2(70, 30))) {
        SetRunning(false);
    }
    
    if (debug) {
        imgui->SameLine();

        if (imgui->Button("Unload", ImVec2(70, 30))) {
            SetRunning(false);

            QueueCommand(-1, "/unload %s", GetName());
        }
    }

    imgui->Separator();

    char buff[32];

    // BOT
    sprintf_s(buff, "Running: %s", IsRunning() ? "True" : "False");
    imgui->TextColored(IsRunning() ? ImVec4(0.1f, 0.8f, 0.1f, 1.0f) : ImVec4(0.8f, 0.1f, 0.1f, 1.0f), buff);

    imgui->Separator();

    // FUNCTIONS
    sprintf_s(buff, "Valid Fish Caught: %s", GetValid() ? "True" : "False");
    imgui->TextColored(GetValid() ? ImVec4(0.1f, 0.8f, 0.1f, 1.0f) : ImVec4(0.8f, 0.1f, 0.1f, 1.0f), buff);

    sprintf_s(buff, "Pulling In Catch: %s", GetFound() ? "True" : "False");
    imgui->TextColored(GetFound() ? ImVec4(0.1f, 0.8f, 0.1f, 1.0f) : ImVec4(0.8f, 0.1f, 0.1f, 1.0f), buff);

    sprintf_s(buff, "Releasing Monster: %s", GetMonst() ? "True" : "False");
    imgui->TextColored(GetMonst() ? ImVec4(0.1f, 0.8f, 0.1f, 1.0f) : ImVec4(0.8f, 0.1f, 0.1f, 1.0f), buff);

    sprintf_s(buff, "Reseting: %s", GetReset() ? "True" : "False");
    imgui->TextColored(GetReset() ? ImVec4(0.1f, 0.8f, 0.1f, 1.0f) : ImVec4(0.8f, 0.1f, 0.1f, 1.0f), buff);

    imgui->Separator();

    // PROGRESS
    sprintf_s(buff, "Fish Caught: %d", GetCatches());
    imgui->Text(buff);

    imgui->Separator();

    // INVENTORY
    IInventory* inventory = this->m_AshitaCore->GetMemoryManager()->GetInventory();

    sprintf_s(buff, "Inventory: %d/%d", inventory->GetContainerCount(0), inventory->GetContainerCountMax(0));
    imgui->TextColored(inventory->GetContainerCount(0) < inventory->GetContainerCountMax(0) ? ImVec4(0.1f, 0.8f, 0.1f, 1.0f) : ImVec4(0.8f, 0.1f, 0.1f, 1.0f), buff);

    imgui->ProgressBar(float(inventory->GetContainerCount(0)) / float(inventory->GetContainerCountMax(0)), ImVec2(-FLT_MIN, 0.0f));

    // GUI POP STYLE VAR
    imgui->PopStyleVar();

    // GUI END
    imgui->End();
}

/**
 * Event invoked when the Direct3D device is setting a render state.
 *
 * @param {D3DRENDERSTATETYPE} State - The render state type.
 * @param {DWORD*} Value - Pointer to the render state value.
 * @return {bool} True if blocked, false otherwise.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 *
 *      If a plugin returns true, the render state is prevented from being set and is blocked from further processing by Ashita 
 *      or the game client and is considered handled.
 *
 *      Plugins can edit the value being set by writing to the Value pointer. 
 */
bool ReelEndurance::Direct3DSetRenderState(D3DRENDERSTATETYPE State, DWORD* Value)
{
    UNREFERENCED_PARAMETER(State);
    UNREFERENCED_PARAMETER(Value);

    return false;
}

/**
 * Event invoked when the Direct3D device is drawing a primitive. (DrawPrimitive)
 *
 * @param {D3DPRIMITIVETYPE} PrimitiveType - The type of primitive being rendered.
 * @param {UINT} StartVertex - Index of the first vertex to load.
 * @param {UINT} PrimitiveCount - Number of primitives to render.
 * @return {bool} True if blocked, false otherwise.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 */
bool ReelEndurance::Direct3DDrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    UNREFERENCED_PARAMETER(PrimitiveType);
    UNREFERENCED_PARAMETER(StartVertex);
    UNREFERENCED_PARAMETER(PrimitiveCount);

    return false;
}

/**
 * Event invoked when the Direct3D device is drawing a primitive. (DrawIndexedPrimitive)
 *
 * @param {D3DPRIMITIVETYPE} PrimitiveType - The type of primitive being rendered.
 * @param {UINT} minIndex - Minimum vertex index for the vertices used during this call.
 * @param {UINT} NumVertices - Number of vertices used during this call.
 * @param {UINT} startIndex - Location in the index array to start reading indices.
 * @param {UINT} primCount - Number of primitives to render.
 * @return {bool} True if blocked, false otherwise.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 */
bool ReelEndurance::Direct3DDrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT minIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
    UNREFERENCED_PARAMETER(PrimitiveType);
    UNREFERENCED_PARAMETER(minIndex);
    UNREFERENCED_PARAMETER(NumVertices);
    UNREFERENCED_PARAMETER(startIndex);
    UNREFERENCED_PARAMETER(primCount);

    return false;
}

/**
 * Event invoked when the Direct3D device is drawing a primitive. (DrawPrimitiveUP)
 *
 * @param {D3DPRIMITIVETYPE} PrimitiveType - The type of primitive being rendered.
 * @param {UINT} PrimitiveCount - Number of primitives to render.
 * @param {const void*} pVertexStreamZeroData - User memory pointer to vertex data to use for vertex stream zero.
 * @param {UINT} VertexStreamZeroStride - Stride between data for each vertex, in bytes.
 * @return {bool} True if blocked, false otherwise.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 */
bool ReelEndurance::Direct3DDrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    UNREFERENCED_PARAMETER(PrimitiveType);
    UNREFERENCED_PARAMETER(PrimitiveCount);
    UNREFERENCED_PARAMETER(pVertexStreamZeroData);
    UNREFERENCED_PARAMETER(VertexStreamZeroStride);

    return false;
}

/**
 * Event invoked when the Direct3D device is drawing a primitive. (DrawIndexedPrimitiveUP)
 *
 * @param {D3DPRIMITIVETYPE} PrimitiveType - The type of primitive being rendered.
 * @param {UINT} MinVertexIndex - Minimum vertex index, relative to zero, for vertices used during this call. 
 * @param {UINT} NumVertexIndices - Number of vertices used during this call.
 * @param {UINT} PrimitiveCount - Number of primitives to render.
 * @param {const void*} pIndexData - User memory pointer to the index data.
 * @param {D3DFORMAT} IndexDataFormat - The format type of the index data.
 * @param {const void*} pVertexStreamZeroData - User memory pointer to vertex data to use for vertex stream zero.
 * @param {UINT} VertexStreamZeroStride - Stride between data for each vertex, in bytes.
 * @return {bool} True if blocked, false otherwise.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 */
bool ReelEndurance::Direct3DDrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertexIndices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    UNREFERENCED_PARAMETER(PrimitiveType);
    UNREFERENCED_PARAMETER(MinVertexIndex);
    UNREFERENCED_PARAMETER(NumVertexIndices);
    UNREFERENCED_PARAMETER(PrimitiveCount);
    UNREFERENCED_PARAMETER(pIndexData);
    UNREFERENCED_PARAMETER(IndexDataFormat);
    UNREFERENCED_PARAMETER(pVertexStreamZeroData);
    UNREFERENCED_PARAMETER(VertexStreamZeroStride);

    return false;
}

/**
 * Returns an instance of this plugins main class.
 * 
 * @param {const char*} args - The arguments passed to the plugin when it was loaded.
 * @return {IPlugin*} The plugins class object instance.
 * @notes
 * 
 *      This export must be present for a plugin to load properly!
 *      
 *      The args parameter can be used to prepare the plugin in any special manner for specific properties based on the arguments passed.
 *      For example, if you wish to have a password required to load the plugin, this can be used for that kind of thing. (Not recommended
 *      as this is a weak form of authentication, just an example.)
 */
__declspec(dllexport) IPlugin* __stdcall expCreatePlugin(const char* args)
{
    UNREFERENCED_PARAMETER(args);

    return new ReelEndurance();
}

/**
 * Returns the plugins Ashita interface version it was compiled against.
 * 
 * @return {double} The plugins interface version.
 * @notes
 * 
 *      This export must be present for a plugin to load properly!
 */
__declspec(dllexport) double __stdcall expGetInterfaceVersion(void)
{
    return ASHITA_INTERFACE_VERSION;
}