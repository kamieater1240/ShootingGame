#include "scene.h"
#include "title.h"
#include "game.h"

SCENE_STATE Scene_Now;

void sceneInit() {
	Scene_Now = SCENE_NONE;
}

void SetScene(SCENE_STATE g_Scene) {
	switch (Scene_Now)
	{
	case(SCENE_NONE):
		break;
	case(SCENE_TITLE):
		titleUninit();
		break;
	case(SCENE_GAME):
		gameUninit();
		break;
	case(SCENE_RESULT):
		break;
	case(SCENE_END):
		break;
	}

	Scene_Now = g_Scene;

	switch (Scene_Now)
	{
	case(SCENE_NONE):
		break;
	case(SCENE_TITLE):
		titleInit();
		break;
	case(SCENE_GAME):
		gameInit();
		break;
	case(SCENE_RESULT):
		break;
	case(SCENE_END):
		break;
	}
}

SCENE_STATE getSceneState() {
	return Scene_Now;
}