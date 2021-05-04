#include "../tomb5/pch.h"
#include "drawlara.h"
#include "../global/types.h"
#include "3dmath.h"
#include "specificfx.h"
#include "output.h"
#include "../game/draw.h"
#include "../game/hair.h"
#include "../game/objects.h"
#include "../game/gameflow.h"

char lara_underwater_skin_sweetness_table[15] =
{
	0, 2, 3, 0, 5, 6, 7, 9, 10, 11, 12, 13, 14, 8, 0
};

int	lara_mesh_sweetness_table[15] = 
{
	0, 1, 2, 3, 4, 5, 6, 7, 14, 8, 9, 10, 11, 12, 13
};

char NodesToStashToScratch[14][2] =
{
	{1, 3},
	{4, 5},
	{6, 7},
	{2, 8},
	{9, 10},
	{11, 12},
	{0, 13},
	{14, 17},
	{18, 19},
	{20, 21},
	{15, 22},
	{23, 24},
	{25, 26},
	{16, 27}
};

char NodesToStashFromScratch[15][4] =
{
	{0, 1, 2, -1},
	{3, 4, -1, 0},
	{5, 6, -1, 0},
	{7, -1, 0, 0},
	{8, 9, -1, 0},
	{10, 11, -1, 0},
	{12, -1, 0, 0},
	{13, 16, 14, 15},
	{27, 28, 34, -1},
	{17, 18, -1, 0},
	{19, 20, -1, 0},
	{21, -1, 0, 0},
	{22, 23, -1, 0},
	{24, 25, -1, 0},
	{26, -1, 0, 0}
};

unsigned char SkinUseMatrix[14][2] =
{
	{255, 255},
	{1, 2},
	{255, 255},
	{255, 255},
	{4, 5},
	{255, 255},
	{255, 255},
	{255, 255},
	{9, 10},
	{255, 255},
	{255, 255},
	{12, 13},
	{255, 255},
	{255, 255}
};

void DrawLara(ITEM_INFO* item, int mirror)
{
	if (lara.skelebob)
		DrawLara__6(item, mirror);
	else
	{
		switch (LaraDrawType)
		{
		case LARA_NORMAL:
		case LARA_YOUNG:
		case LARA_BUNHEAD:
			DrawLara__1(item, mirror);
			break;

		case LARA_CATSUIT:
			DrawLara__4(item, mirror);
			break;

		case LARA_DIVESUIT:
			DrawLara__5(item, mirror);
			break;
		}
	}
}

void DrawLara__1(ITEM_INFO* item, int mirror)
{
	OBJECT_INFO* obj;
	PCSVECTOR v0, v1;
	short** meshpp;
	int top, bottom, left, right, stash;
	long dx, dy, dz, dist;
	short cos, sin, xRot;
	static short trans_lara = 255;

	aGlobalSkinMesh = 1;
	top = phd_top;
	bottom = phd_bottom;
	left = phd_left;
	right = phd_right;
	phd_top = 0;
	phd_bottom = phd_winymax;
	phd_left = 0;
	phd_right = phd_winxmax;
	phd_PushMatrix();
	obj = &objects[item->object_number];//when called, it points at the skin..?
	S_PrintShadow(obj->shadow_size, GLaraShadowframe, item, 0);

	if (input & IN_LOOK)
	{
		dx = lara_item->pos.x_pos - CamPos.x;
		dy = lara_item->pos.y_pos - CamPos.y - 512;
		dz = lara_item->pos.z_pos - CamPos.z;
		dist = phd_sqrt(SQUARE(dx) + SQUARE(dy) + SQUARE(dz));
		trans_lara = dist >> 2;

		if (trans_lara < 0)
			trans_lara = 0;

		if (trans_lara > 255)
			trans_lara = 255;

		GlobalAlpha = trans_lara << 24;
	}
	else
	{
		if (trans_lara < 255)
		{
			trans_lara += 8;

			if (trans_lara > 255)
				trans_lara = 255;
		}

		GlobalAlpha = trans_lara << 24;
	}

	if (!mirror)
		CalculateObjectLightingLara();

	for (int i = 0; i < 15; i++)//skin
	{
		*(aMXPtr + 0) = *((float*)(&lara_matricesF[i * 12]) + 0);
		*(aMXPtr + 1) = *((float*)(&lara_matricesF[i * 12]) + 1);
		*(aMXPtr + 2) = *((float*)(&lara_matricesF[i * 12]) + 2);
		*(aMXPtr + 3) = *((float*)(&lara_matricesF[i * 12]) + 3);
		*(aMXPtr + 4) = *((float*)(&lara_matricesF[i * 12]) + 4);
		*(aMXPtr + 5) = *((float*)(&lara_matricesF[i * 12]) + 5);
		*(aMXPtr + 6) = *((float*)(&lara_matricesF[i * 12]) + 6);
		*(aMXPtr + 7) = *((float*)(&lara_matricesF[i * 12]) + 7);
		*(aMXPtr + 8) = *((float*)(&lara_matricesF[i * 12]) + 8);
		*(aMXPtr + 9) = *((float*)(&lara_matricesF[i * 12]) + 9);
		*(aMXPtr + 10) = *((float*)(&lara_matricesF[i * 12]) + 10);
		*(aMXPtr + 11) = *((float*)(&lara_matricesF[i * 12]) + 11);

		if (LaraNodeUnderwater[i])
			bLaraUnderWater = i;
		else
			bLaraUnderWater = -1;

		phd_PutPolygons(lara.mesh_ptrs[lara_mesh_sweetness_table[i]], -1);

		for (int j = 0; j < 4; j++)
		{
			stash = (unsigned char)NodesToStashFromScratch[i][j];

			if (stash == 255)
				break;

			StashSkinVertices(stash);
		}
	}

	phd_PopMatrix();
	bLaraUnderWater = LaraNodeUnderwater[8] != 0 ? 8 : -1;
	DrawHair();
	phd_PushMatrix();
	obj = &objects[LARA_SKIN_JOINTS];
	meshpp = &meshes[obj->mesh_index];
	meshpp += 2;

	for (int i = 0; i < 14; i++)//joints
	{
		SkinVerticesToScratch(NodesToStashToScratch[i][0]);
		SkinVerticesToScratch(NodesToStashToScratch[i][1]);

		if (LaraNodeUnderwater[lara_underwater_skin_sweetness_table[i]])
			bLaraUnderWater = lara_underwater_skin_sweetness_table[i];
		else
			bLaraUnderWater = -1;

		if (SkinUseMatrix[i][0] >= 255)
			phd_PutPolygons(*meshpp, -1);
		else
		{
			phd_PushMatrix();
			*(aMXPtr + 0) = *((float*)(&lara_matricesF[(SkinUseMatrix[i][1]) * 12]) + 0);
			*(aMXPtr + 1) = *((float*)(&lara_matricesF[(SkinUseMatrix[i][1]) * 12]) + 1);
			*(aMXPtr + 2) = *((float*)(&lara_matricesF[(SkinUseMatrix[i][1]) * 12]) + 2);
			*(aMXPtr + 3) = *((float*)(&lara_matricesF[(SkinUseMatrix[i][1]) * 12]) + 3);
			*(aMXPtr + 4) = *((float*)(&lara_matricesF[(SkinUseMatrix[i][1]) * 12]) + 4);
			*(aMXPtr + 5) = *((float*)(&lara_matricesF[(SkinUseMatrix[i][1]) * 12]) + 5);
			*(aMXPtr + 6) = *((float*)(&lara_matricesF[(SkinUseMatrix[i][1]) * 12]) + 6);
			*(aMXPtr + 7) = *((float*)(&lara_matricesF[(SkinUseMatrix[i][1]) * 12]) + 7);
			*(aMXPtr + 8) = *((float*)(&lara_matricesF[(SkinUseMatrix[i][1]) * 12]) + 8);
			*(aMXPtr + 9) = *((float*)(&lara_matricesF[(SkinUseMatrix[i][1]) * 12]) + 9);
			*(aMXPtr + 10) = *((float*)(&lara_matricesF[(SkinUseMatrix[i][1]) * 12]) + 10);
			*(aMXPtr + 11) = *((float*)(&lara_matricesF[(SkinUseMatrix[i][1]) * 12]) + 11);
			v0.vx = lara_matrices[SkinUseMatrix[i][0]].m01;
			v0.vy = lara_matrices[SkinUseMatrix[i][0]].m11;
			v0.vz = lara_matrices[SkinUseMatrix[i][0]].m21;
			v1.vx = lara_matrices[SkinUseMatrix[i][1]].m01;
			v1.vy = lara_matrices[SkinUseMatrix[i][1]].m11;
			v1.vz = lara_matrices[SkinUseMatrix[i][1]].m21;
			cos = ((v0.vx * v1.vx) + (v0.vy * v1.vy) + (v0.vz * v1.vz)) >> 14;
			sin = phd_sqrt(16777216 - SQUARE(cos));

			if (i == 1 || i == 4)
				xRot = -phd_atan(cos, sin);
			else
				xRot = phd_atan(cos, sin);

			phd_RotX(-xRot >> 1);
			phd_PutPolygons(*meshpp, -1);
			phd_PopMatrix();
		}

		meshpp += 2;
	}

	aGlobalSkinMesh = 0;
	bLaraSkinBits = 0;
	bLaraUnderWater = (LaraNodeUnderwater[0] != 0) - 1;

	if (!(gfLevelFlags & GF_LVOP_YOUNG_LARA))
	{
		obj = &objects[lara.holster];
		meshpp = &meshes[obj->mesh_index];
		meshpp += 8;
		*(aMXPtr + 0) = *((float*)(&lara_matricesF[1 * 12]) + 0);//holsters
		*(aMXPtr + 1) = *((float*)(&lara_matricesF[1 * 12]) + 1);
		*(aMXPtr + 2) = *((float*)(&lara_matricesF[1 * 12]) + 2);
		*(aMXPtr + 3) = *((float*)(&lara_matricesF[1 * 12]) + 3);
		*(aMXPtr + 4) = *((float*)(&lara_matricesF[1 * 12]) + 4);
		*(aMXPtr + 5) = *((float*)(&lara_matricesF[1 * 12]) + 5);
		*(aMXPtr + 6) = *((float*)(&lara_matricesF[1 * 12]) + 6);
		*(aMXPtr + 7) = *((float*)(&lara_matricesF[1 * 12]) + 7);
		*(aMXPtr + 8) = *((float*)(&lara_matricesF[1 * 12]) + 8);
		*(aMXPtr + 9) = *((float*)(&lara_matricesF[1 * 12]) + 9);
		*(aMXPtr + 10) = *((float*)(&lara_matricesF[1 * 12]) + 10);
		*(aMXPtr + 11) = *((float*)(&lara_matricesF[1 * 12]) + 11);
		phd_PutPolygons(*meshpp, -1);
		meshpp += 8;
		*(aMXPtr + 0) = *((float*)(&lara_matricesF[4 * 12]) + 0);//other side of holsters. (cba check which block is which)
		*(aMXPtr + 1) = *((float*)(&lara_matricesF[4 * 12]) + 1);
		*(aMXPtr + 2) = *((float*)(&lara_matricesF[4 * 12]) + 2);
		*(aMXPtr + 3) = *((float*)(&lara_matricesF[4 * 12]) + 3);
		*(aMXPtr + 4) = *((float*)(&lara_matricesF[4 * 12]) + 4);
		*(aMXPtr + 5) = *((float*)(&lara_matricesF[4 * 12]) + 5);
		*(aMXPtr + 6) = *((float*)(&lara_matricesF[4 * 12]) + 6);
		*(aMXPtr + 7) = *((float*)(&lara_matricesF[4 * 12]) + 7);
		*(aMXPtr + 8) = *((float*)(&lara_matricesF[4 * 12]) + 8);
		*(aMXPtr + 9) = *((float*)(&lara_matricesF[4 * 12]) + 9);
		*(aMXPtr + 10) = *((float*)(&lara_matricesF[4 * 12]) + 10);
		*(aMXPtr + 11) = *((float*)(&lara_matricesF[4 * 12]) + 11);
		phd_PutPolygons(*meshpp, -1);
		
		if (lara.back_gun)//back gun
		{
			short* rot;
			phd_PushMatrix();

#ifdef fix_backguns_drawing//fixes the original bug where Lara's back weapon wouldn't draw.
			*(aMXPtr + 0) = *((float*)(&lara_matricesF[84]) + 0);
			*(aMXPtr + 1) = *((float*)(&lara_matricesF[84]) + 1);
			*(aMXPtr + 2) = *((float*)(&lara_matricesF[84]) + 2);
			*(aMXPtr + 3) = *((float*)(&lara_matricesF[84]) + 3);
			*(aMXPtr + 4) = *((float*)(&lara_matricesF[84]) + 4);
			*(aMXPtr + 5) = *((float*)(&lara_matricesF[84]) + 5);
			*(aMXPtr + 6) = *((float*)(&lara_matricesF[84]) + 6);
			*(aMXPtr + 7) = *((float*)(&lara_matricesF[84]) + 7);
			*(aMXPtr + 8) = *((float*)(&lara_matricesF[84]) + 8);
			*(aMXPtr + 9) = *((float*)(&lara_matricesF[84]) + 9);
			*(aMXPtr + 10) = *((float*)(&lara_matricesF[84]) + 10);
			*(aMXPtr + 11) = *((float*)(&lara_matricesF[84]) + 11);
#else //fix_backguns_drawing
			* (aMXPtr + 0) = *((float*)(&lara_matrices[84]) + 0);
			*(aMXPtr + 1) = *((float*)(&lara_matrices[84]) + 1);
			*(aMXPtr + 2) = *((float*)(&lara_matrices[84]) + 2);
			*(aMXPtr + 3) = *((float*)(&lara_matrices[84]) + 3);
			*(aMXPtr + 4) = *((float*)(&lara_matrices[84]) + 4);
			*(aMXPtr + 5) = *((float*)(&lara_matrices[84]) + 5);
			*(aMXPtr + 6) = *((float*)(&lara_matrices[84]) + 6);
			*(aMXPtr + 7) = *((float*)(&lara_matrices[84]) + 7);
			*(aMXPtr + 8) = *((float*)(&lara_matrices[84]) + 8);
			*(aMXPtr + 9) = *((float*)(&lara_matrices[84]) + 9);
			*(aMXPtr + 10) = *((float*)(&lara_matrices[84]) + 10);
			*(aMXPtr + 11) = *((float*)(&lara_matrices[84]) + 11);
#endif //fix_backguns_drawing

			phd_TranslateRel(*((bones + objects[lara.back_gun].bone_index) + 53),
				*((bones + objects[lara.back_gun].bone_index) + 54),
				*((bones + objects[lara.back_gun].bone_index) + 55));

			rot = objects[lara.back_gun].frame_base + 9;
			gar_RotYXZsuperpack(&rot, 14);

#ifdef fix_backguns_drawing
			aSetViewMatrix();
#endif //fix_backguns_drawing

			phd_PutPolygons(meshes[objects[lara.back_gun].mesh_index + 28], -1);
			phd_PopMatrix();
		}
	}

	phd_PopMatrix();
	bLaraUnderWater = 0;
	phd_top = top;
	phd_bottom = bottom;
	phd_left = left;
	phd_right = right;
	GlobalAlpha = 0xFF000000;
}

void inject_drawlara()
{
	INJECT(0x00498030, DrawLara);
	INJECT(0x00498100, DrawLara__1);
}
