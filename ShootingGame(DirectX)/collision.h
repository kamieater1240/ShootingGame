#ifndef COLLISION_H_
#define COLLISION_H_

typedef struct Circle_tag {
	D3DXVECTOR2 position;
	float radius;
}Circle;

bool circleCollsion(const Circle* pA, const Circle* pB);
void checkCollisionAll();

#endif // !COLLISION_H_

