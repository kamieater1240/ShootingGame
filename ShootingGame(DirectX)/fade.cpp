#include "fade.h"
#include "sprite.h"

FADE_STATE		g_FadeState;
SCENE_STATE		g_FadeNextScene;
float			g_FadeAlpha;

void fadeInit() {
	g_FadeState = FADE_STATE_NONE;
	g_FadeNextScene = SCENE_NONE;
	g_FadeAlpha = 0.0f;
}

void fadeUpdate() {
	if (g_FadeState == FADE_STATE_OUT) {
		if (g_FadeAlpha >= 1.0f) {
			g_FadeAlpha = 1.0f;
			g_FadeState = FADE_STATE_IN;
			SetScene(g_FadeNextScene);
		}

		g_FadeAlpha += 1.0 / 20.0f;
	}
	else if (g_FadeState == FADE_STATE_IN) {
		if (g_FadeAlpha <= 0.0f) {
			g_FadeAlpha = 0.0;
			g_FadeState = FADE_STATE_NONE;
		}

		g_FadeAlpha -= 1.0f / 20.0f;
	}
}

void fadeDraw() {

}

void Fade(SCENE_STATE NextScene) {
	g_FadeNextScene = NextScene;
	g_FadeState = FADE_STATE_OUT;
}

FADE_STATE GetFadeState() {
	return g_FadeState;
}