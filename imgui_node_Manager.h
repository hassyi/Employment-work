#pragma once

#include "imguiManager.h"
#include "imgui_node_editor.h"
#include <string>
#include <vector>

class ImGuiNodeManager
{
private:
	struct Node {
		int id;
		std::string name;
		ImVec2 pos;
	};

	struct Link {
		int id;
		int startPinId;
		int endPinId;
	};

	enum class NodeType {
		Selector,
		Sequence,
		Action,
		Condition
	};

	struct BTNode {
		int id = 0;
		NodeType type;
		std::string name;
		std::vector<int> children;	//子ノードのID
		int parent = -1;			//親ノードのID
		bool state = false;			//実行結果

	};

	struct BTLink {
		int id;
		int parentId;
		int childId;
	};


	std::vector<Node> m_Nodes;
	std::vector<Link> m_Link;
	std::vector<BTNode> m_BTNodes;
	std::vector<BTLink> m_BTLinks;

public:
	ImGuiNodeManager() = default;
	~ImGuiNodeManager(){}

	void Init();
	void Uninit();
	void DrawNode();
	void DrawBehaviorTree();

	void AddNode();
	void AddBTNode(NodeType type, std::string name);
	void AddLink(int startPin, int endPin);

	bool EvaluateNode(int nodeId);

	void DrawNodeStatus(BTNode& node);

};