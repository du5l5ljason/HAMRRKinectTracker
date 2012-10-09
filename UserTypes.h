#pragma once
typedef struct POINT2D {
	float x;
	float y;
}POINT2D;

typedef struct POINT3D {
	float x;
	float y;
	float z;
}POINT3D;

typedef struct POINT3D_CONF {
	float x;
	float y;
	float z;
	float conf;
}POINT3D_CONF;

typedef struct Rect{
	int x;
	int y;
	int width;
	int height;
}Rect;

typedef struct Plane3D{
	float A;
	float B;
	float C;
	float D;
}Plane3D; // plane is represented as Ax + By + Cz + D = 0;

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

//define address and port
