#pragma once
#ifndef SPRITE_H
#define SPRITE_H

void Sprite_Init();
void Sprite_Uninit();

void Sprite_Draw(int textureID, float dx, float dy);
void Sprite_Draw(int textureID, float dx, float dy, float centerX, float centerY, float angle, float scaleX, float scaleY, float scaleZ);

//Cut Methods
void Sprite_Draw(int textureID, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h);
void Sprite_Draw(int textureID, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float centerX, float centerY, float angle);
void Sprite_Draw(int textureID, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float scaleX, float scaleY);
void Sprite_Draw(int textureID, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float centerX, float centerY, float angle, float scaleX, float scaleY);

#endif // !SPRITE_H