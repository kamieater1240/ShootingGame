#ifndef TAMA_H_
#define TAMA_H_

struct TAMA {
	float x, y;
	int tama_textureID;
	bool toggle;		//κtO
	float raise;		//singΙg€pxΜΑΚ
	double angle;		//px
};

void  tamaInit();
void  tamaAll(float nowX, float nowY);
float tamaGetPosition();
void  tamaMove(float moveX, float moveY);
void  tamaDraw();

//class TAMA {
//public:
//	TAMA();
//	void All(float nowX, float nowY);
//	float getPosition();
//
//private:
//	float x, y;
//	int tama_textureID;
//	bool toggle;		//κtO
//	int raise;			//singΙg€pxΜΑΚ
//	double angle;		//px
//	void Move(float moveX, float moveY);
//	void Draw();
//}

#endif // !TAMA_H_

