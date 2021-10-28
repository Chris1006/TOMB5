#include "../tomb5/pch.h"
#include "specificfx.h"
#include "3dmath.h"
#include "../game/delstuff.h"
#include "../game/control.h"
#include "../specific/function_table.h"
#include "../game/objects.h"
#include "polyinsert.h"
#include "drawroom.h"
#include "d3dmatrix.h"

#define LINE_POINTS	4	//number of points in each grid line
#define NUM_TRIS	14	//number of triangles needed to create the shadow (this depends on what shape you're doing)
#define GRID_POINTS	(LINE_POINTS * LINE_POINTS)	//number of points in the whole grid
#define POINT_HEIGHT_CORRECTION	196	//if the difference between the floor below Lara and the floor height below the point is greater than this value, point height is corrected to lara's floor level.

long ShadowTable[NUM_TRIS * 3] =	//num of triangles * 3 points
{
	//shadow is split in 3 parts. top, middle, bottom, each part made of triangles. first 4 tris are the top part,
	//the following 6 are the middle part, and the last 4 are the bottom part.

	//tris for each part go left to right. i.e the first line for the top part is the leftmost tri, 4th one is the rightmost, and so on
	//but this isn't a hard rule at all, it's just how Core did it
/*
	the default shadow grid is 4 x 4 points
	0	1	2	3

	4	5	6	7

	8	9	10	11

	12	13	14	15

	the values here are which grid points the tri points are at.
	for example, the first tri, 4, 1, 5. connect the dots. 4 -> 1 -> 5
	which makes the top left tri.
	and so on.
*/
4, 1, 5,
5, 1, 6,	//top part
6, 1, 2,
6, 2, 7,
//
8, 4, 9,
9, 4, 5,
9, 5, 10,	//middle part
10, 5, 6,
10, 6, 11,
11, 6, 7,
//
13, 8, 9,
13, 9, 14,	//bottom part
14, 9, 10,
14, 10, 11
};

void S_PrintShadow(short size, short* box, ITEM_INFO* item)
{
	TEXTURESTRUCT Tex;
	D3DTLVERTEX v[3];
	PHD_VECTOR pos;
	float* sXYZ;
	long* hXZ;
	long* hY;
	float sxyz[GRID_POINTS * 3];
	float fx, fy, fz;
	long hxz[GRID_POINTS * 2];
	long hy[GRID_POINTS];
	long x, y, z, x1, y1, z1, x2, y2, z2, x3, y3, z3, xSize, zSize, xDist, zDist, triA, triB, triC;
	short room_number;

	xSize = size * (box[1] - box[0]) / 192;	//x size of grid
	zSize = size * (box[5] - box[4]) / 192;	//z size of grid
	xDist = xSize / LINE_POINTS;			//distance between each point of the grid on X
	zDist = zSize / LINE_POINTS;			//distance between each point of the grid on Z
	x = -xDist - (xDist >> 1);				//idfk
	z = zDist + (zDist >> 1);
	sXYZ = sxyz;
	hXZ = hxz;

	for (int i = 0; i < LINE_POINTS; i++, z -= zDist)
	{
		for (int j = 0; j < LINE_POINTS; j++, sXYZ += 3, hXZ += 2, x += xDist)
		{
			sXYZ[0] = (float)x;		//fill shadow XYZ array with the points of the grid
			sXYZ[2] = (float)z;
			hXZ[0] = x;				//fill height XZ array with the points of the grid
			hXZ[1] = z;
		}

		x = -xDist - (xDist >> 1);
	}

	phd_PushUnitMatrix();

	if (item == lara_item)	//position the grid
	{
		pos.x = 0;
		pos.y = 0;
		pos.z = 0;
		GetLaraJointPos(&pos, LM_TORSO);
		room_number = lara_item->room_number;
		y = GetHeight(GetFloor(pos.x, pos.y, pos.z, &room_number), pos.x, pos.y, pos.z);

		if (y == NO_HEIGHT)
			y = item->floor;
	}
	else
	{
		pos.x = item->pos.x_pos;
		y = item->floor;
		pos.z = item->pos.z_pos;
	}

	y -= 16;
	phd_TranslateRel(pos.x, y, pos.z);
	phd_RotY(item->pos.y_rot);	//rot the grid to correct Y
	hXZ = hxz;

	for (int i = 0; i < GRID_POINTS; i++, hXZ += 2)
	{
		x = hXZ[0];
		z = hXZ[1];
		hXZ[0] = (x * phd_mxptr[M00] + z * phd_mxptr[M02] + phd_mxptr[M03]) >> 14;
		hXZ[1] = (x * phd_mxptr[M20] + z * phd_mxptr[M22] + phd_mxptr[M23]) >> 14;
	}

	phd_PopMatrix();
	hXZ = hxz;
	hY = hy;

	for (int i = 0; i < GRID_POINTS; i++, hXZ += 2, hY++)	//Get height on each grid point and store it in hy array
	{
		room_number = item->room_number;
		*hY = GetHeight(GetFloor(hXZ[0], item->floor, hXZ[1], &room_number), hXZ[0], item->floor, hXZ[1]);

		if (ABS(*hY - item->floor) > POINT_HEIGHT_CORRECTION)
			*hY = item->floor;
	}

	phd_PushMatrix();
	phd_TranslateAbs(pos.x, y, pos.z);
	phd_RotY(item->pos.y_rot);
	sXYZ = sxyz;
	hY = hy;

	for (int i = 0; i < GRID_POINTS; i++, sXYZ += 3, hY++)
	{
		fx = sXYZ[0];
		fy = (float)(*hY - item->floor);
		fz = sXYZ[2];
		sXYZ[0] = aMXPtr[M00] * fx + aMXPtr[M01] * fy + aMXPtr[M02] * fz + aMXPtr[M03];
		sXYZ[1] = aMXPtr[M10] * fx + aMXPtr[M11] * fy + aMXPtr[M12] * fz + aMXPtr[M13];
		sXYZ[2] = aMXPtr[M20] * fx + aMXPtr[M21] * fy + aMXPtr[M22] * fz + aMXPtr[M23];
	}

	phd_PopMatrix();
	sXYZ = sxyz;

	for (int i = 0; i < NUM_TRIS; i++)	//draw triangles
	{
		triA = 3 * ShadowTable[(i * 3) + 0];	//get tri points
		triB = 3 * ShadowTable[(i * 3) + 1];
		triC = 3 * ShadowTable[(i * 3) + 2];
		x1 = (long)sXYZ[triA + 0];
		y1 = (long)sXYZ[triA + 1];
		z1 = (long)sXYZ[triA + 2];
		x2 = (long)sXYZ[triB + 0];
		y2 = (long)sXYZ[triB + 1];
		z2 = (long)sXYZ[triB + 2];
		x3 = (long)sXYZ[triC + 0];
		y3 = (long)sXYZ[triC + 1];
		z3 = (long)sXYZ[triC + 2];
		setXYZ3(v, x1, y1, z1, x2, y2, z2, x3, y3, z3, clipflags);
		v[0].color = 0x4F000000;
		v[1].color = 0x4F000000;
		v[2].color = 0x4F000000;

		if (item->after_death)
		{
			v[0].color = 0x80000000 - (item->after_death << 24);
			v[1].color = v[0].color;
			v[2].color = v[0].color;
		}

		v[0].specular = 0xFF000000;
		v[1].specular = 0xFF000000;
		v[2].specular = 0xFF000000;
		Tex.flag = 0;
		Tex.tpage = 0;
		Tex.drawtype = 3;
		Tex.u1 = 0;
		Tex.v1 = 0;
		Tex.u2 = 0;
		Tex.v2 = 0;
		Tex.u3 = 0;
		Tex.v3 = 0;
		Tex.u4 = 0;
		Tex.v4 = 0;
		AddTriSorted(v, 0, 1, 2, &Tex, 1);
	}
}

void DrawLaserSightSprite()
{
	SPRITESTRUCT* sprite;
	D3DTLVERTEX v[4];
	TEXTURESTRUCT Tex;
	long* TempIDK;
	short* TempDist;
	short* TempXY;
	float zv, u1, u2, v1, v2;
	long results[3];

	phd_PushMatrix();
	phd_TranslateAbs(lara_item->pos.x_pos, lara_item->pos.y_pos, lara_item->pos.z_pos);
	TempDist = (short*)&UNK_EffectDistance;
	TempXY = (short*)&UNK_EffectXY;
	TempIDK = (long*)&UNK_00E913E0;
	TempDist[0] = (short)(LaserSightX - lara_item->pos.x_pos);
	TempDist[1] = (short)(LaserSightY - lara_item->pos.y_pos);
	TempDist[2] = (short)(LaserSightZ - lara_item->pos.z_pos);
	results[0] = phd_mxptr[M00] * TempDist[0] + phd_mxptr[M01] * TempDist[1] + phd_mxptr[M02] * TempDist[2] + phd_mxptr[M03];
	results[1] = phd_mxptr[M10] * TempDist[0] + phd_mxptr[M11] * TempDist[1] + phd_mxptr[M12] * TempDist[2] + phd_mxptr[M13];
	results[2] = phd_mxptr[M20] * TempDist[0] + phd_mxptr[M21] * TempDist[1] + phd_mxptr[M22] * TempDist[2] + phd_mxptr[M23];
	zv = f_persp / (float)results[2];
	TempXY[0] = short(float(results[0] * zv + f_centerx));
	TempXY[1] = short(float(results[1] * zv + f_centery));
	TempIDK[0] = results[2] >> 14;
	phd_PopMatrix();
	sprite = &spriteinfo[objects[DEFAULT_SPRITES].mesh_index + 14];
	setXY4(v, TempXY[0] - 2, TempXY[1] - 2, TempXY[0] + 2, TempXY[1] - 2, TempXY[0] - 2, TempXY[1] + 2, TempXY[0] + 2, TempXY[1] + 2, (int)f_mznear, clipflags);
	v[0].color = LaserSightCol ? 0x0000FF00 : 0x00FF0000;//if LaserSightCol is on, it turns green
	v[1].color = v[0].color;
	v[2].color = v[0].color;
	v[3].color = v[0].color;
	v[0].specular = 0xFF000000;
	v[1].specular = 0xFF000000;
	v[2].specular = 0xFF000000;
	v[3].specular = 0xFF000000;
	u1 = sprite->x2;
	u2 = sprite->x1;
	v1 = sprite->y2;
	v2 = sprite->y1;
	Tex.drawtype = 2;
	Tex.flag = 0;
	Tex.tpage = sprite->tpage;
	Tex.u1 = u1;
	Tex.v1 = v1;
	Tex.u2 = u2;
	Tex.v2 = v1;
	Tex.u3 = u2;
	Tex.v3 = v2;
	Tex.u4 = u1;
	Tex.v4 = v2;
	AddQuadSorted(v, 0, 1, 2, 3, &Tex, 0);
	LaserSightCol = 0;
	LaserSightActive = 0;
}

#ifdef USE_SKY_SPRITE
void DrawSkySegment(ulong color, char drawtype, char def, long zpos, long ypos)
{
	SPRITESTRUCT* sprite;
	FVECTOR vec[4];
	D3DTLVERTEX v[4];
	TEXTURESTRUCT Tex;
	short* clip;
	float perspz, u1, v1, u2, v2, x, y, z;
	short clipdistance;

	phd_PushMatrix();

	if (gfCurrentLevel)
		phd_TranslateRel(zpos, ypos, 0);
	else
		phd_TranslateRel(0, ypos, 0);

	switch (def)
	{
	case 0:
		vec[0].x = -4864;
		vec[0].y = 0;
		vec[0].z = 0;
		vec[1].x = 0;
		vec[1].y = 0;
		vec[1].z = 0;
		vec[2].x = 0;
		vec[2].y = 0;
		vec[2].z = -4864;
		vec[3].x = -4864;
		vec[3].y = 0;
		vec[3].z = -4864;
		break;

	case 1:
		vec[0].x = 0;
		vec[0].y = 0;
		vec[0].z = 0;
		vec[1].x = 4864;
		vec[1].y = 0;
		vec[1].z = 0;
		vec[2].x = 4864;
		vec[2].y = 0;
		vec[2].z = -4864;
		vec[3].x = 0;
		vec[3].y = 0;
		vec[3].z = -4864;
		break;

	case 2:
		vec[0].x = -4864;
		vec[0].y = 0;
		vec[0].z = 4864;
		vec[1].x = 0;
		vec[1].y = 0;
		vec[1].z = 4864;
		vec[2].x = 0;
		vec[2].y = 0;
		vec[2].z = 0;
		vec[3].x = -4864;
		vec[3].y = 0;
		vec[3].z = 0;
		break;

	case 3:
		vec[0].x = 0;
		vec[0].y = 0;
		vec[0].z = 4864;
		vec[1].x = 4864;
		vec[1].y = 0;
		vec[1].z = 4864;
		vec[2].x = 4864;
		vec[2].y = 0;
		vec[2].z = 0;
		vec[3].x = 0;
		vec[3].y = 0;
		vec[3].z = 0;
		break;
	}

	for (int i = 0; i < 4; i++)
	{
		x = vec[i].x;
		y = vec[i].y;
		z = vec[i].z;
		vec[i].x = aMXPtr[M00] * x + aMXPtr[M01] * y + aMXPtr[M02] * z + aMXPtr[M03];
		vec[i].y = aMXPtr[M10] * x + aMXPtr[M11] * y + aMXPtr[M12] * z + aMXPtr[M13];
		vec[i].z = aMXPtr[M20] * x + aMXPtr[M21] * y + aMXPtr[M22] * z + aMXPtr[M23];
		v[i].color = color | 0xFF000000;
		v[i].specular = 0xFF000000;
		CalcColorSplit(color, &v[i].color);
	}

	clip = clipflags;

	for (int i = 0; i < 4; i++, clip++)
	{
		v[i].tu = (float)vec[i].x;
		v[i].tv = (float)vec[i].y;
		v[i].sz = (float)vec[i].z;
		clipdistance = 0;

		if (v[i].sz < f_mznear)
			clipdistance = -128;
		else
		{
			perspz = f_mpersp / v[i].sz;

			if (v[i].sz > FogEnd)
			{
				v[i].sz = f_zfar;
				clipdistance = 256;
			}

			v[i].sx = perspz * v[i].tu + f_centerx;
			v[i].sy = perspz * v[i].tv + f_centery;
			v[i].rhw = perspz * f_moneopersp;

			if (v[i].sx < phd_winxmin)
				clipdistance++;
			else if (phd_winxmax < v[i].sx)
				clipdistance += 2;

			if (v[i].sy < phd_winymin)
				clipdistance += 4;
			else if (v[i].sy > phd_winymax)
				clipdistance += 8;
		}

		clip[0] = clipdistance;
	}

	sprite = &spriteinfo[objects[SKY_GRAPHICS].mesh_index + def];
	Tex.drawtype = (ushort)drawtype;
	Tex.flag = 0;
	Tex.tpage = sprite->tpage;
	u1 = sprite->x2;
	u2 = sprite->x1;
	v1 = sprite->y2;
	v2 = sprite->y1;
	Tex.u1 = u1;
	Tex.v1 = v1;
	Tex.u2 = u2;
	Tex.v2 = v1;
	Tex.u3 = u2;
	Tex.v3 = v2;
	Tex.u4 = u1;
	Tex.v4 = v2;
	AddQuadSorted(v, 3, 2, 1, 0, &Tex, 1);
	phd_TranslateRel(-9728, 0, 0);

	switch (def)
	{
	case 0:
		vec[0].x = -4864;
		vec[0].y = 0;
		vec[0].z = 0;
		vec[1].x = 0;
		vec[1].y = 0;
		vec[1].z = 0;
		vec[2].x = 0;
		vec[2].y = 0;
		vec[2].z = -4864;
		vec[3].x = -4864;
		vec[3].y = 0;
		vec[3].z = -4864;
		break;

	case 1:
		vec[0].x = 0;
		vec[0].y = 0;
		vec[0].z = 0;
		vec[1].x = 4864;
		vec[1].y = 0;
		vec[1].z = 0;
		vec[2].x = 4864;
		vec[2].y = 0;
		vec[2].z = -4864;
		vec[3].x = 0;
		vec[3].y = 0;
		vec[3].z = -4864;
		break;

	case 2:
		vec[0].x = -4864;
		vec[0].y = 0;
		vec[0].z = 4864;
		vec[1].x = 0;
		vec[1].y = 0;
		vec[1].z = 4864;
		vec[2].x = 0;
		vec[2].y = 0;
		vec[2].z = 0;
		vec[3].x = -4864;
		vec[3].y = 0;
		vec[3].z = 0;
		break;

	case 3:
		vec[0].x = 0;
		vec[0].y = 0;
		vec[0].z = 4864;
		vec[1].x = 4864;
		vec[1].y = 0;
		vec[1].z = 4864;
		vec[2].x = 4864;
		vec[2].y = 0;
		vec[2].z = 0;
		vec[3].x = 0;
		vec[3].y = 0;
		vec[3].z = 0;
		break;
	}

	for (int i = 0; i < 4; i++)
	{
		x = vec[i].x;
		y = vec[i].y;
		z = vec[i].z;
		vec[i].x = aMXPtr[M00] * x + aMXPtr[M01] * y + aMXPtr[M02] * z + aMXPtr[M03];
		vec[i].y = aMXPtr[M10] * x + aMXPtr[M11] * y + aMXPtr[M12] * z + aMXPtr[M13];
		vec[i].z = aMXPtr[M20] * x + aMXPtr[M21] * y + aMXPtr[M22] * z + aMXPtr[M23];
		v[i].color |= 0xFF000000;
		v[i].specular = 0xFF000000;
		CalcColorSplit(color, &v[i].color);
	}

	clip = clipflags;

	for (int i = 0; i < 4; i++, clip++)
	{
		v[i].tu = (float)vec[i].x;
		v[i].tv = (float)vec[i].y;
		v[i].sz = (float)vec[i].z;
		clipdistance = 0;

		if (v[i].sz < f_mznear)
			clipdistance = -128;
		else
		{
			perspz = f_mpersp / v[i].sz;

			if (v[i].sz > FogEnd)
			{
				v[i].sz = f_zfar;
				clipdistance = 256;
			}

			v[i].sx = perspz * v[i].tu + f_centerx;
			v[i].sy = perspz * v[i].tv + f_centery;
			v[i].rhw = perspz * f_moneopersp;

			if (v[i].sx < phd_winxmin)
				clipdistance++;
			else if (phd_winxmax < v[i].sx)
				clipdistance += 2;

			if (v[i].sy < phd_winymin)
				clipdistance += 4;
			else if (v[i].sy > phd_winymax)
				clipdistance += 8;
		}

		clip[0] = clipdistance;
	}

	if (gfCurrentLevel != 0)
		AddQuadSorted(v, 3, 2, 1, 0, &Tex, 1);

	phd_PopMatrix();
}

void DrawFlatSky(ulong color, long zpos, long ypos, long drawtype)
{
	DrawSkySegment(color, (char)drawtype, 0, zpos, ypos);
	DrawSkySegment(color, (char)drawtype, 1, zpos, ypos);
	DrawSkySegment(color, (char)drawtype, 2, zpos, ypos);
	DrawSkySegment(color, (char)drawtype, 3, zpos, ypos);
}
#else
void DrawFlatSky(ulong color, long zpos, long ypos, long drawtype)
{
	PHD_VECTOR vec[4];
	D3DTLVERTEX v[4];
	TEXTURESTRUCT Tex;
	short* clip;
	float perspz;
	long x, y, z;
	short clipdistance;

	phd_PushMatrix();

	if (gfCurrentLevel)
		phd_TranslateRel(zpos, ypos, 0);
	else
		phd_TranslateRel(0, ypos, 0);

	vec[0].x = -4864;
	vec[0].y = 0;
	vec[0].z = 4864;
	vec[1].x = 4864;
	vec[1].y = 0;
	vec[1].z = 4864;
	vec[2].x = 4864;
	vec[2].y = 0;
	vec[2].z = -4864;
	vec[3].x = -4864;
	vec[3].y = 0;
	vec[3].z = -4864;

	for (int i = 0; i < 4; i++)
	{
		x = vec[i].x;
		y = vec[i].y;
		z = vec[i].z;
		vec[i].x = (phd_mxptr[M00] * x + phd_mxptr[M01] * y + phd_mxptr[M02] * z + phd_mxptr[M03]) >> 14;
		vec[i].y = (phd_mxptr[M10] * x + phd_mxptr[M11] * y + phd_mxptr[M12] * z + phd_mxptr[M13]) >> 14;
		vec[i].z = (phd_mxptr[M20] * x + phd_mxptr[M21] * y + phd_mxptr[M22] * z + phd_mxptr[M23]) >> 14;
		v[i].color = color | 0xFF000000;
		v[i].specular = 0xFF000000;
		CalcColorSplit(color, &v[i].color);
	}

	clip = clipflags;
	v[0].tu = (float)vec[0].x;
	v[0].tv = (float)vec[0].y;
	v[0].sz = (float)vec[0].z;
	clipdistance = 0;

	if (v[0].sz < f_mznear)
		clipdistance = -128;
	else
	{
		perspz = f_mpersp / v[0].sz;

		if (v[0].sz > FogEnd)
		{
			v[0].sz = f_zfar;
			clipdistance = 256;
		}

		v[0].sx = perspz * v[0].tu + f_centerx;
		v[0].sy = perspz * v[0].tv + f_centery;
		v[0].rhw = perspz * f_moneopersp;

		if (v[0].sx < phd_winxmin)
			clipdistance++;
		else if (phd_winxmax < v[0].sx)
			clipdistance += 2;

		if (v[0].sy < phd_winymin)
			clipdistance += 4;
		else if (v[0].sy > phd_winymax)
			clipdistance += 8;
	}

	clip[0] = clipdistance;
	clip++;
	v[1].tu = (float)vec[1].x;
	v[1].tv = (float)vec[1].y;
	v[1].sz = (float)vec[1].z;
	clipdistance = 0;

	if (v[1].sz < f_mznear)
		clipdistance = -128;
	else
	{
		perspz = f_mpersp / v[1].sz;

		if (v[1].sz > FogEnd)
		{
			v[1].sz = f_zfar;
			clipdistance = 256;
		}

		v[1].sx = perspz * v[1].tu + f_centerx;
		v[1].sy = perspz * v[1].tv + f_centery;
		v[1].rhw = perspz * f_moneopersp;

		if (v[1].sx < phd_winxmin)
			clipdistance++;
		else if (phd_winxmax < v[1].sx)
			clipdistance += 2;

		if (v[1].sy < phd_winymin)
			clipdistance += 4;
		else if (v[1].sy > phd_winymax)
			clipdistance += 8;
	}

	clip[0] = clipdistance;
	clip++;
	v[2].tu = (float)vec[2].x;
	v[2].tv = (float)vec[2].y;
	v[2].sz = (float)vec[2].z;
	clipdistance = 0;

	if (v[2].sz < f_mznear)
		clipdistance = -128;
	else
	{
		perspz = f_mpersp / v[2].sz;

		if (v[2].sz > FogEnd)
		{
			v[2].sz = f_zfar;
			clipdistance = 256;
		}

		v[2].sx = perspz * v[2].tu + f_centerx;
		v[2].sy = perspz * v[2].tv + f_centery;
		v[2].rhw = perspz * f_moneopersp;

		if (v[2].sx < phd_winxmin)
			clipdistance++;
		else if (phd_winxmax < v[2].sx)
			clipdistance += 2;

		if (v[2].sy < phd_winymin)
			clipdistance += 4;
		else if (v[2].sy > phd_winymax)
			clipdistance += 8;
	}

	clip[0] = clipdistance;
	clip++;
	_0x004C6BA0(&v[3], (float)vec[3].x, (float)vec[3].y, (float)vec[3].z, clip);
	Tex.drawtype = (ushort)drawtype;
	Tex.flag = 0;
	Tex.tpage = nTextures - 1;
	Tex.u1 = 0.0;
	Tex.v1 = 0.0;
	Tex.u2 = 1.0;
	Tex.v2 = 0.0;
	Tex.u3 = 1.0;
	Tex.v3 = 1.0;
	Tex.u4 = 0.0;
	Tex.v4 = 1.0;
	AddQuadSorted(v, 3, 2, 1, 0, &Tex, 1);
	phd_TranslateRel(-9728, 0, 0);
	vec[0].x = -4864;
	vec[0].y = 0;
	vec[0].z = 4864;
	vec[1].x = 4864;
	vec[1].y = 0;
	vec[1].z = 4864;
	vec[2].x = 4864;
	vec[2].y = 0;
	vec[2].z = -4864;
	vec[3].x = -4864;
	vec[3].y = 0;
	vec[3].z = -4864;

	for (int i = 0; i < 4; i++)
	{
		x = vec[i].x;
		y = vec[i].y;
		z = vec[i].z;
		vec[i].x = (phd_mxptr[M00] * x + phd_mxptr[M01] * y + phd_mxptr[M02] * z + phd_mxptr[M03]) >> 14;
		vec[i].y = (phd_mxptr[M10] * x + phd_mxptr[M11] * y + phd_mxptr[M12] * z + phd_mxptr[M13]) >> 14;
		vec[i].z = (phd_mxptr[M20] * x + phd_mxptr[M21] * y + phd_mxptr[M22] * z + phd_mxptr[M23]) >> 14;
		v[i].color |= 0xFF000000;
		v[i].specular = 0xFF000000;
		CalcColorSplit(color, &v[i].color);
	}

	clip = clipflags;
	v[0].tu = (float)vec[0].x;
	v[0].tv = (float)vec[0].y;
	v[0].sz = (float)vec[0].z;
	clipdistance = 0;

	if (v[0].sz < f_mznear)
		clipdistance = -128;
	else
	{
		perspz = f_mpersp / v[0].sz;

		if (v[0].sz > FogEnd)
		{
			v[0].sz = f_zfar;
			clipdistance = 256;
		}

		v[0].sx = perspz * v[0].tu + f_centerx;
		v[0].sy = perspz * v[0].tv + f_centery;
		v[0].rhw = perspz * f_moneopersp;

		if (v[0].sx < phd_winxmin)
			clipdistance++;
		else if (phd_winxmax < v[0].sx)
			clipdistance += 2;

		if (v[0].sy < phd_winymin)
			clipdistance += 4;
		else if (v[0].sy > phd_winymax)
			clipdistance += 8;
	}

	clip[0] = clipdistance;
	clip++;
	v[1].tu = (float)vec[1].x;
	v[1].tv = (float)vec[1].y;
	v[1].sz = (float)vec[1].z;
	clipdistance = 0;

	if (v[1].sz < f_mznear)
		clipdistance = -128;
	else
	{
		perspz = f_mpersp / v[1].sz;

		if (v[1].sz > FogEnd)
		{
			v[1].sz = f_zfar;
			clipdistance = 256;
		}

		v[1].sx = perspz * v[1].tu + f_centerx;
		v[1].sy = perspz * v[1].tv + f_centery;
		v[1].rhw = perspz * f_moneopersp;

		if (v[1].sx < phd_winxmin)
			clipdistance++;
		else if (phd_winxmax < v[1].sx)
			clipdistance += 2;

		if (v[1].sy < phd_winymin)
			clipdistance += 4;
		else if (v[1].sy > phd_winymax)
			clipdistance += 8;
	}

	clip[0] = clipdistance;
	clip++;
	v[2].tu = (float)vec[2].x;
	v[2].tv = (float)vec[2].y;
	v[2].sz = (float)vec[2].z;
	clipdistance = 0;

	if (v[2].sz < f_mznear)
		clipdistance = -128;
	else
	{
		perspz = f_mpersp / v[2].sz;

		if (v[2].sz > FogEnd)
		{
			v[2].sz = f_zfar;
			clipdistance = 256;
		}

		v[2].sx = perspz * v[2].tu + f_centerx;
		v[2].sy = perspz * v[2].tv + f_centery;
		v[2].rhw = perspz * f_moneopersp;

		if (v[2].sx < phd_winxmin)
			clipdistance++;
		else if (phd_winxmax < v[2].sx)
			clipdistance += 2;

		if (v[2].sy < phd_winymin)
			clipdistance += 4;
		else if (v[2].sy > phd_winymax)
			clipdistance += 8;
	}

	clip[0] = clipdistance;
	clip++;
	_0x004C6BA0(&v[3], (float)vec[3].x, (float)vec[3].y, (float)vec[3].z, clip);

	if (gfCurrentLevel != 0)
		AddQuadSorted(v, 3, 2, 1, 0, &Tex, 1);

	phd_PopMatrix();
}
#endif

void S_DrawDarts(ITEM_INFO* item)
{
	D3DTLVERTEX v[2];
	long x1, y1, z1, x2, y2, z2, num, mxx, mxy, mxz, xx, yy, zz;
	float zv;

	phd_PushMatrix();
	phd_TranslateAbs(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos);
	zv = f_persp / (float)phd_mxptr[M23];
	x1 = (short)((float)(phd_mxptr[M03] * zv + f_centerx));
	y1 = (short)((float)(phd_mxptr[M13] * zv + f_centery));
	z1 = phd_mxptr[M23] >> 14;
	num = (-96 * phd_cos(item->pos.x_rot)) >> 14;
	mxx = (num * phd_sin(item->pos.y_rot)) >> 14;
	mxy = (96 * phd_sin(item->pos.x_rot)) >> 14;
	mxz = (num * phd_cos(item->pos.y_rot)) >> 14;
	xx = phd_mxptr[M00] * mxx + phd_mxptr[M01] * mxy + phd_mxptr[M02] * mxz + phd_mxptr[M03];
	yy = phd_mxptr[M10] * mxx + phd_mxptr[M11] * mxy + phd_mxptr[M12] * mxz + phd_mxptr[M13];
	zz = phd_mxptr[M20] * mxx + phd_mxptr[M21] * mxy + phd_mxptr[M22] * mxz + phd_mxptr[M23];
	zv = f_persp / (float)zz;
	x2 = (short)((float)(xx * zv + f_centerx));
	y2 = (short)((float)(yy * zv + f_centery));
	z2 = zz >> 14;

	if (ClipLine(x1, y1, z1, x2, y2, z2, phd_winxmin, phd_winymin, phd_winxmax, phd_winymax))
	{
		zv = f_mpersp / (float)z1 * f_moneopersp;
		v[0].color = 0xFF000000;
		v[1].color = 0xFF783C14;
		v[0].specular = 0xFF000000;
		v[1].specular = 0xFF000000;
		v[0].sx = (float)x1;
		v[1].sx = (float)x2;
		v[0].sy = (float)y1;
		v[1].sy = (float)y2;
		v[0].sz = f_a - zv * f_boo;
		v[1].sz = f_a - zv * f_boo;
		v[0].rhw = zv;
		v[1].rhw = zv;
		AddLineSorted(v, &v[1], 6);
	}

	phd_PopMatrix();
}

void DrawMoon()
{
	D3DTLVERTEX v[4];
	SPRITESTRUCT* sprite;
	TEXTURESTRUCT tex;
	SVECTOR vec;
	short* c;
	float x1, x2, y1, y2, z;
	ushort tpage;

	c = clipflags;
	sprite = &spriteinfo[objects[MISC_SPRITES].mesh_index + 3];
	tpage = sprite->tpage < nTextures ? sprite->tpage : 0;
	phd_PushMatrix();
	aSetViewMatrix();
	D3DMView._41 = 0;
	D3DMView._42 = 0;
	D3DMView._43 = 0;
	vec.vx = 0;
	vec.vy = -1024;
	vec.vz = 1920;
	aTransformPerspSV(&vec, v, c, 1, 0);

	if (*c >= 0)
	{
		x1 = v[0].sx - 48.0F;
		x2 = v[0].sx + 48.0F;
		y1 = v[0].sy - 48.0F;
		y2 = v[0].sy + 48.0F;
		z = f_mzfar - 1024.0F;
		aSetXY4(v, x1, y1, x2, y1, x1, y2, x2, y2, z, c);
		v[0].color = 0xC0E0FF;
		v[1].color = 0xC0E0FF;
		v[2].color = 0xC0E0FF;
		v[3].color = 0xC0E0FF;
		v[0].specular = 0xFF000000;
		v[1].specular = 0xFF000000;
		v[2].specular = 0xFF000000;
		v[3].specular = 0xFF000000;
		tex.u1 = sprite->x1;
		tex.v1 = sprite->y1;
		tex.u2 = sprite->x2;
		tex.v2 = sprite->y1;
		tex.u3 = sprite->x2;
		tex.v3 = sprite->y2;
		tex.u4 = sprite->x1;
		tex.v4 = sprite->y2;
		tex.drawtype = 0;
		tex.tpage = tpage;
		AddQuadZBuffer(v, 0, 1, 3, 2, &tex, 1);
	}

	phd_PopMatrix();
}

void DrawGasCloud(ITEM_INFO* item)
{
	GAS_CLOUD* cloud;
	long num;

	if (!TriggerActive(item))
		return;

	if (item->trigger_flags < 2)
	{
		cloud = (GAS_CLOUD*)item->data;

		if (!cloud->mTime)
			cloud->yo = -6144.0F;

		TriggerFogBulbFX(0, 128, 0, item->pos.x_pos, (long)(item->pos.y_pos + cloud->yo), item->pos.z_pos, 4096, 40);

		if (cloud->yo >= -3584.0)
		{
			if (cloud->sTime == 32)
			{
				do num = rand() & 7; while (num == cloud->num);
				cloud->num = num;
			}
			else if (cloud->sTime > 32)
			{
				num = cloud->sTime - 32;

				if (num > 128)
				{
					num = 256 - num;

					if (!num)
						cloud->sTime = 0;
				}

				num = 255 - (num << 1);

				if (num < 64)
					num = 64;

				TriggerFogBulbFX(0, 255, 0, item->pos.x_pos + cloud[cloud->num].t.vx, item->pos.y_pos + cloud[cloud->num].t.vy,
					item->pos.z_pos + cloud[cloud->num].t.vz, 1024, num);
			}

			cloud->sTime++;
		}
		else
			cloud->yo += 12.0F;

		cloud->mTime++;

		for (int i = 0; i < 8; i++, cloud++)//what's the point of this loop
		{
			phd_PushMatrix();
			phd_TranslateAbs(item->pos.x_pos + cloud->t.vx, item->pos.y_pos + cloud->t.vy, item->pos.z_pos + cloud->t.vz);
			phd_RotY(-CamRot.vy << 4);
			phd_RotX(-4096);
			phd_PopMatrix();
		}
	}
	else
		item->item_flags[0] = 1;
}

void DrawStarField()
{
	D3DTLVERTEX v[4];
	TEXTURESTRUCT tex;
	static long first_time = 0;
	float* pPos;
	long* pCol;
	float x, y, z, fx, fy, fz, bx, by;
	long col;

	if (!first_time)
	{
		pPos = StarFieldPositions;
		pCol = StarFieldColors;

		for (int i = 0; i < 256; i++)
		{
			pPos[0] = ((rand() & 0x1FF) + 512.0F) * fSin(i * 512);
			pPos++;
			pPos[0] = (float)(-rand() % 1900);
			pPos++;
			pPos[0] = ((rand() & 0x1FF) + 512.0F) * fCos(i * 512);
			pPos++;
			pPos[0] = (rand() & 1) + 1.0F;
			pPos++;
			col = rand() & 0x7F;
			pCol[0] = RGBONLY(col + 128, col + 128, col + 192);
			pCol++;
		}

		first_time = 1;
	}

	tex.drawtype = 0;
	tex.tpage = 0;
	tex.flag = 0;
	phd_PushMatrix();
	phd_TranslateAbs(camera.pos.x, camera.pos.y, camera.pos.z);
	SetD3DViewMatrix();
	phd_PopMatrix();
	pPos = StarFieldPositions;
	pCol = StarFieldColors;
	clipflags[0] = 0;
	clipflags[1] = 0;
	clipflags[2] = 0;
	clipflags[3] = 0;

	for (int i = 0; i < 184; i++)
	{
		fx = pPos[0];
		pPos++;
		fy = pPos[0];
		pPos++;
		fz = pPos[0];
		pPos++;
		col = pCol[0];
		pCol++;
		x = fx * D3DMView._11 + fy * D3DMView._21 + fz * D3DMView._31;
		y = fx * D3DMView._12 + fy * D3DMView._22 + fz * D3DMView._32;
		z = fx * D3DMView._13 + fy * D3DMView._23 + fz * D3DMView._33;
		fy = pPos[0];
		pPos++;

		if (z >= f_mznear)
		{
			fz = f_mpersp / z;
			bx = fz * x + f_centerx;
			by = fz * y + f_centery;

			if (bx >= 0 && bx <= (float)phd_winxmax && by >= 0 && by <= (float)phd_winymax)
			{
				v[0].sx = bx;
				v[0].sy = by;
				v[0].color = col;
				v[0].specular = 0xFF000000;
				v[0].rhw = f_mpersp / f_mzfar * f_moneopersp;
				v[0].tu = 0;
				v[0].tv = 0;
				v[1].sx = bx + fy;
				v[1].sy = by;
				v[1].color = col;
				v[1].specular = 0xFF000000;
				v[1].rhw = f_mpersp / f_mzfar * f_moneopersp;
				v[1].tu = 0;
				v[1].tv = 0;
				v[2].sx = bx;
				v[2].sy = by + fy;
				v[2].color = col;
				v[2].specular = 0xFF000000;
				v[2].rhw = f_mpersp / f_mzfar * f_moneopersp;
				v[2].tu = 0;
				v[2].tv = 0;
				v[3].sx = bx + fy;
				v[3].sy = by + fy;
				v[3].color = col;
				v[3].specular = 0xFF000000;
				v[3].rhw = f_mpersp / f_mzfar * f_moneopersp;
				v[3].tu = 0;
				v[3].tv = 0;
				AddQuadZBuffer(v, 0, 1, 3, 2, &tex, 1);
			}
		}
	}
}

void inject_specificfx(bool replace)
{
	INJECT(0x004C2F10, S_PrintShadow, replace);
	INJECT(0x004C7320, DrawLaserSightSprite, replace);
	INJECT(0x004C5EA0, DrawFlatSky, replace);
	INJECT(0x004CBB10, S_DrawDarts, replace);
	INJECT(0x004CF2F0, DrawMoon, replace);
	INJECT(0x004CFF80, DrawGasCloud, replace);
	INJECT(0x004C0060, DrawStarField, replace);
}
