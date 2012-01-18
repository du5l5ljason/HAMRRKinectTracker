#pragma once
struct POINT2D {
	float x;
	float y;
};

struct POINT3D {
	float x;
	float y;
	float z;
};

struct POINT3D_CONF {
	float x;
	float y;
	float z;
	float conf;
};

struct Rect{
	int x;
	int y;
	int width;
	int height;
};

typedef struct QNode
{
	int data;
	struct QNode* next;
}QNode;

typedef struct Queue
{
	struct QNode* first;
	struct QNode* last;
}Queue;

