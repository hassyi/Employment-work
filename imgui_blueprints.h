#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include "builders.h"
#include "widgets.h"
#include "imgui_node_editor.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>

namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

using namespace ax;

using ax::Widgets::IconType;

static ed::EditorContext* m_Editor = nullptr;

//extern "C" __declspec(dllimport) short __stdcall GetAsyncKeyState(int vkey);
//extern "C" bool Debug_KeyPress(int vkey)
//{
//    static std::map<int, bool> state;
//    auto lastState = state[vkey];
//    state[vkey] = (GetAsyncKeyState(vkey) & 0x8000) != 0;
//    if (state[vkey] && !lastState)
//        return true;
//    else
//        return false;
//}

enum class PinType
{
    Flow,
    Bool,
    Int,
    Float,
    String,
    Object,
    Function,
    Delegate,
};

enum class PinKind
{
    Output,
    Input
};

enum class NodeType
{
    Blueprint,
    Simple,
    Tree,
    Comment,
    Houdini
};

struct Node;

struct Pin
{
    ed::PinId   ID;
    ::Node* Node;
    std::string Name;
    PinType     Type;
    PinKind     Kind;

    Pin(int id, const char* name, PinType type) :
        ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input)
    {
    }
};

struct Node
{
    ed::NodeId ID;
    std::string Name;
    std::vector<Pin> Inputs;
    std::vector<Pin> Outputs;
    ImColor Color;
    NodeType Type;
    ImVec2 Size;

    std::string State;
    std::string SavedState;

    Node(int id, const char* name, ImColor color = ImColor(255, 255, 255)) :
        ID(id), Name(name), Color(color), Type(NodeType::Blueprint), Size(0, 0)
    {
    }
};

struct Link
{
    ed::LinkId ID;

    ed::PinId StartPinID;
    ed::PinId EndPinID;

    ImColor Color;

    Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId) :
        ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
    {
    }
};

struct NodeIdLess
{
    bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const
    {
        return lhs.AsPointer() < rhs.AsPointer();
    }
};

static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f)
{
    using namespace ImGui;
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiID id = window->GetID("##Splitter");
    ImRect bb;
    bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
    bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
    return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}

class ImGuiBluePrint
{
private:
    int m_NextId = 1;
    const int m_PinIconSize = 24;
    std::vector<Node> m_Nodes;
    std::vector<Link> m_Links;
    ImTextureID          m_HeaderBackground = nullptr;
    ImTextureID          m_SaveIcon = nullptr;
    ImTextureID          m_RestoreIcon = nullptr;
    const float m_TouchTime = 1.0f;
    std::map < ed::NodeId, float, NodeIdLess > m_NodeTouchTime;
    bool m_ShowOrdinals = false;

public:
    ImGuiBluePrint() = default;
    ~ImGuiBluePrint(){}

    void Init();
    void Uninit();
    void Draw();

    void BuildNode(Node* node);
    void BuildNodes();
    void TouchNode(ed::NodeId id);
    void UpdateTouch();
    void DrawPinIcon(const Pin& pin, bool connected, int alpha);
    void ShowStyleEditor(bool* show = nullptr);
    void ShowLeftPane(float paneWidth);
    void DestroyTexture(ImTextureID texture) { return ImGui_DestroyTexture(texture); }
    Node* SpawnInputActionNode();
    Node* SpawnBranchNode();
    Node* SpawnDoNNode();
    Node* SpawnOutputActionNode();
    Node* SpawnPrintStringNode();
    Node* SpawnMessageNode();
    Node* SpawnSetTimerNode();
    Node* SpawnLessNode();
    Node* SpawnWeirdNode();
    Node* SpawnTraceByChannelNode();
    Node* SpawnTreeSequenceNode();
    Node* SpawnTreeTaskNode();
    Node* SpawnTreeTask2Node();
    Node* SpawnComment();
    Node* SpawnHoudiniTransformNode();
    Node* SpawnHoudiniGroupNode();
    Node* FindNode(ed::NodeId id);
    Link* FindLink(ed::LinkId id);
    Pin* FindPin(ed::PinId id);

    int GetNextId() { return m_NextId++; }
    ed::LinkId GetNextLinkId() { return ed::LinkId(GetNextId()); }
    float GetTouchProgress(ed::NodeId id);
    ImColor GetIconColor(PinType type);
    int GetTextureWidth(ImTextureID texture) { return ImGui_GetTextureWidth(texture); }
    int GetTextureHeight(ImTextureID texture) { return ImGui_GetTextureHeight(texture); }

    
    bool IsPinLinked(ed::PinId id);
    bool CanCreateLink(Pin* a, Pin* b);
    ImTextureID LoadTexture(const char* path);
    ImTextureID CreateTexture(const void* data, int width, int height) { return ImGui_CreateTexture(data, width, height); }


};