#pragma once


typedef enum
{
	SCENE_NONE = 0,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_CLEAR_RESULT,
	SCENE_FAILURE_RESULT,

	SCENE_MAX
}SCENE_STATE;

class SceneState
{
protected:
	SCENE_STATE m_Satate = SCENE_STATE::SCENE_NONE;

	bool m_isChageScene = false;

	int m_DrawFrame = 0;
	int m_ChageSceneFrame = 0;
public:
	virtual~SceneState() {}
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	SCENE_STATE GetSceneState() { return m_Satate; }

};
