#include "imgui_node_Manager.h"
#include <iostream>

namespace ed = ax::NodeEditor;
static ed::EditorContext* m_Context = nullptr;

void ImGuiNodeManager::Init()
{
	ed::Config config;
	config.SettingsFile = "Simple.json";
	m_Context = ed::CreateEditor(&config);
}

void ImGuiNodeManager::Uninit()
{
	ed::DestroyEditor(m_Context);
}

void ImGuiNodeManager::DrawNode()
{
	//auto& io = ImGui::GetIO();
	
	ImGui::Separator();

	ed::SetCurrentEditor(m_Context);
	ed::Begin("My Editor");
	for (const auto& node : m_Nodes)
	{
		//Start Drawing Nodes;
		ed::NodeId nodeId = node.id;
		ed::BeginNode(nodeId);
		ImGui::Text("%s", node.name.c_str());

		//入力ピン
		ed::BeginPin(node.id * 10 + 1, ed::PinKind::Input);
		ImGui::Text("-> In");
		ed::EndPin();

		//出力ピン
		//ImGui::SameLine();
		ed::BeginPin(node.id * 10 + 2, ed::PinKind::Output);
		ImGui::Text("Out ->");
		ed::EndPin();

		ed::EndNode();
	}

	//既存のリンク(接続線)を描画
	for (const auto& link : m_Link) {
		ed::Link(link.id, link.startPinId, link.endPinId);
	}

	//イベント処理
	//新しいリンク(接続)が作成されたか
	if (ed::BeginCreate()) 
	{
		ed::PinId startPin, endPin;
		if (ed::QueryNewLink(&startPin, &endPin))
		{
			if(startPin && endPin && startPin != endPin)
			{
				AddLink(startPin.Get(), endPin.Get());
				ed::AcceptNewItem();
			}
		}
	}
	ed::EndCreate();

	//リンクの削除処理
	if (ed::BeginDelete())
	{
		ed::LinkId linkId;
		while (ed::QueryDeletedLink(&linkId))
		{
			auto it = std::find_if(m_Link.begin(), m_Link.end(),
				[linkId](const Link& l) {return l.id == linkId.Get(); });

			if (it != m_Link.end())
			{
				m_Link.erase(it);
				ed::AcceptDeletedItem();
			}
		}
	}
	ed::EndDelete();

	if (ImGui::IsMouseClicked(1)) {
		AddNode();
	}

	ed::End();
	ed::SetCurrentEditor(nullptr);
}

void ImGuiNodeManager::DrawBehaviorTree()
{

	if (ImGui::Button("Open Menu")) {
		ImGui::OpenPopup("AddNodePopup");
	}

	if (ImGui::BeginPopup("AddNodePopup"))
	{
		if (ImGui::Button("Add Selector")) {
			AddBTNode(NodeType::Selector, "Selector");
		}
		if (ImGui::Button("Add Sequence")) {
			AddBTNode(NodeType::Sequence, "Sequence");
		}
		if (ImGui::Button("Add Action")) {
			AddBTNode(NodeType::Action, "Action");
		}
		if (ImGui::Button("Add Condition")) {
			AddBTNode(NodeType::Condition, "Condition");
		}
		ImGui::EndPopup();
	}


	if (ImGui::Button("Run Behavior Tree")) {
		EvaluateNode(1);
	}


	ed::SetCurrentEditor(m_Context);
	ed::Begin("Behavior Tree Editor");

	for (auto& node : m_BTNodes)
	{
		ed::BeginNode(node.id);
		ImGui::Text("%s", node.name.c_str());

		if (node.type == NodeType::Selector || node.type == NodeType::Sequence)
		{
			for (size_t i = 0; i < node.children.size(); i++)
			{
				ed::BeginPin(node.children[i] * 10 + 1, ed::PinKind::Input);
				ImGui::Text("Child %d", (int)i);
				ed::EndPin();
			}
		}

		ed::BeginPin(node.id * 10 + 2, ed::PinKind::Output);
		ImGui::Text("OutPut");
		ed::EndPin();

		ed::EndNode();
	}

	//リンクの作成
	if (ed::BeginCreate())
	{
		ed::PinId startPin, endPin;
		if (ed::QueryNewLink(&startPin, &endPin))
		{
			int parentId = startPin.Get() / 10;
			int childId = endPin.Get() / 10;

			if (parentId != childId)
			{
				auto& parentNode = m_BTNodes[parentId - 1];
				parentNode.children.push_back(childId);
				m_BTLinks.push_back({ static_cast<int>(m_BTLinks.size() + 1), parentId, childId });
				ed::AcceptNewItem();
			}
		}
	}
	ed::EndCreate();

	//リンクの削除処理
	if (ed::BeginDelete())
	{
		ed::LinkId linkId;
		while (ed::QueryDeletedLink(&linkId))
		{
			auto it = std::remove_if(m_BTLinks.begin(), m_BTLinks.end(),
				[linkId](const BTLink& l) {return l.id == linkId.Get(); });

			if (it != m_BTLinks.end())
			{
				m_BTLinks.erase(it);
				ed::AcceptDeletedItem();
			}
		}
	}
	ed::EndDelete();

	ed::End();
	ed::SetCurrentEditor(nullptr);

}

void ImGuiNodeManager::AddNode()
{
	Node node;
	node.id = m_Nodes.size() + 1;
	node.name = "New Node";
	node.pos = ImVec2(100,100);
	m_Nodes.push_back(node);
}

void ImGuiNodeManager::AddBTNode(NodeType type, std::string name)
{
	BTNode node;
	node.id = m_BTNodes.size() + 1;
	node.type = type;
	node.name = name;
	m_BTNodes.push_back(node);
}

void ImGuiNodeManager::AddLink(int startPin, int endPin)
{
	Link link;
	link.id = m_Link.size() + 1;
	link.startPinId = startPin;
	link.endPinId = endPin;
	m_Link.push_back(link);
}

bool ImGuiNodeManager::EvaluateNode(int nodeId)
{
	BTNode* node = nullptr;
	for (auto& n : m_BTNodes)
	{
		if(n.id == nodeId)
		{
			node = &n;
			break;
		}
	}

	if (node == nullptr) return false;

	switch (node->type)
	{
	case NodeType::Selector:
		for (int childId : node->children)
		{
			if (EvaluateNode(childId))
			{
				node->state = true;
				return true;
			}
		}
		node->state = false;
		return false;

	case NodeType::Sequence:
		for (int childId : node->children)
		{
			if (EvaluateNode(childId))
			{
				node->state = false;
				return false;
			}
		}
		node->state = true;
		return true;

	case NodeType::Action:
		node->state = (rand() % 2 == 0);
		return node->state;

	case NodeType::Condition:
		node->state = (rand() % 2 == 0);
		return node->state;
	}

	return false;
}

void ImGuiNodeManager::DrawNodeStatus(BTNode& node)
{
	if (node.state) {
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Success");
	}
	else {
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failure");
	}
}
