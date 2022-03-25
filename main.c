#include <raylib.h>

#include "../MetaLauncher/RaylibHandling.h"
#include "../MetaLauncher/EngineCode/Saving.h"
#include "../MetaLauncher/EngineCode/PhysicsObject.h"
#include "../MetaLauncher/EngineCode/KettleString.h"
#include "../MetaLauncher/EngineCode/DynallocCore.h"

#define RENDER_SCALE 50.f
#define RENDER_SCALE_FIX_VECTOR (Vector3){0,RENDER_SCALE/2,0}
#define DIRECTION_SHIFT_GAP 5.f

#ifndef FILTEREFFECTS
#define FILTEREFFECTS

#define SCREENDIM_X 1400
#define SCREENDIM_Y 1000

#define RENTEXTURE_X (SCREENDIM_X/3)
#define RENTEXTURE_Y (SCREENDIM_Y/3)

RenderTexture* getMainRenderTexture()
{
	static RenderTexture rentex;
	return &rentex;
}
void initRenderTexture()
{
	(*getMainRenderTexture()) = LoadRenderTexture(RENTEXTURE_X, RENTEXTURE_Y);
}
void drawRenderTextureToWindow(RenderTexture* rentext, Vector2 position, Vector2 size)
{
	DrawTexturePro (rentext->texture,(Rectangle) { 0.0f, 0.0f,
	(float)rentext->texture.width, (float)-rentext->texture.height },
	(Rectangle) {0.0, 0.0, (float)size.x, (float)size.y},
	(Vector2) { position.x - ((float)size.x / 2), position.y - ((float)size.y / 2)}, 0.0f, WHITE);
}
void drawMainRenderTextureToWindow()
{
	drawRenderTextureToWindow(
								getMainRenderTexture(),
								(Vector2){GetScreenWidth() / 2, GetScreenHeight() / 2},
								(Vector2){GetScreenWidth() ,GetScreenHeight()});
}
void startDrawingToRenderTexture()
{
	BeginTextureMode(*getMainRenderTexture());
}
void endDrawingToRenderTexture()
{
	EndTextureMode();
}

#endif

typedef struct Kobobo
{
	PhysicsObject physobj;
	bool is_mig;
}
Kobobo;

Kobobo newKobobo()
{
	Kobobo ret = { 0 };
	return ret;
}

#ifndef GAME
#define GAME

typedef struct GameData
{
	Camera camera;
	int is_cam_active;
	Kobobo* player;
	bool camera_follow_cursor;
	int cam_look_multiply;

	bool is_player_moving;
	Vector2 mouse_offset;

}
GameData;

GameData** getGameData()
{
	static GameData* gamedata;
	return &gamedata;
}

#ifndef PLAYER
#define PLAYER


#ifndef DYNALLOC_OBJECT_DEFINITIONS

#ifndef BULLET_OBJECT
#define BULLET_OBJECT newKettleString("BULLET")

typedef struct Bullet
{
	PhysicsObject physobj;
	int lifetime;
}
Bullet;

Bullet* ktoBullet(KObject* kobj)
{
	return (Bullet*)(kobj->data.data);
}

void bulletConstructor(KObject* kobj)
{
	ktoBullet(kobj)->lifetime = 100;
}
void bulletDeconstructor(KObject* kobj)
{
}
void bulletProcess(KObject* kobj)
{
	updatePhysicsObjectPosition(&ktoBullet(kobj)->physobj);

	kobj->flag_delete = (!(--ktoBullet(kobj)->lifetime));
}
void bulletRenderer(KObject* kobj)
{
	static Texture txtr_fl;
	static bool txtr_ld = false;
	if(!txtr_ld)
	{
		txtr_fl = LoadTexture("rsrcs/particles/basic_shot/basic_shot.png");
	}
	DrawBillboard((*getGameData())->camera, txtr_fl, addVectors(ktoBullet(kobj)->physobj.position, RENDER_SCALE_FIX_VECTOR), RENDER_SCALE / 3, RAYWHITE);
}
void registerBullet()
{
	registerNewKObject(newKOBJSet(
		BULLET_OBJECT,
		bulletConstructor,
		bulletDeconstructor,
		bulletProcess,
		bulletRenderer,
		sizeof(Bullet)
	));
}
#endif
#endif

void moyaiPhysicsModel(GameData* data)
{
}
void mig21FisPhysicsModelling(GameData* data)
{
}

void updatePlayerMovement(GameData* data)
{
	data->mouse_offset = (Vector2){-(GetMouseX() - (SCREENDIM_X / 2)), -(GetMouseY() - (SCREENDIM_Y / 2))};
	data->player->physobj.direction.x = data->mouse_offset.x / 25;
	data->player->physobj.direction.z = data->mouse_offset.y / 25;
	
	updatePhysicsObjectPosition(&data->player->physobj);

}

void renderKoboboMoyai(Kobobo* jj)
{
	//Up Down Left Right
	static Texture txtr_fl[4];
	static bool txtr_ld = false;
	if(!txtr_ld)
	{
		txtr_fl[0] = LoadTexture("rsrcs/kobobo/front.png");
		txtr_fl[1] = LoadTexture("rsrcs/kobobo/back.png");
		txtr_fl[2] = LoadTexture("rsrcs/kobobo/left.png");
		txtr_fl[3] = LoadTexture("rsrcs/kobobo/right.png");
	}

	Vector3 billboard_rendering_pos = addVectors(jj->physobj.position, RENDER_SCALE_FIX_VECTOR);
	//Down
	if(jj->physobj.position.z + DIRECTION_SHIFT_GAP < addVectors(jj->physobj.position, jj->physobj.direction).z)
	{
		DrawBillboard((*getGameData())->camera, txtr_fl[1], billboard_rendering_pos, RENDER_SCALE, RAYWHITE);
		return;
	}

	//left
	if(jj->physobj.position.x - DIRECTION_SHIFT_GAP > addVectors(jj->physobj.position, jj->physobj.direction).x)
	{
		DrawBillboard((*getGameData())->camera, txtr_fl[2], billboard_rendering_pos, RENDER_SCALE, RAYWHITE);
		return;
	}

	//right
	if(jj->physobj.position.x + DIRECTION_SHIFT_GAP < addVectors(jj->physobj.position, jj->physobj.direction).x)
	{
		DrawBillboard((*getGameData())->camera, txtr_fl[3], billboard_rendering_pos, RENDER_SCALE, RAYWHITE);
		return;
	}

	DrawBillboard((*getGameData())->camera, txtr_fl[0], billboard_rendering_pos, RENDER_SCALE, RAYWHITE);
}
void renderKoboboMig(Kobobo* jj)
{
}
void renderKoboboDebug(Kobobo* jj)
{
	//line end point.
	Vector3 lep = addVectors(jj->physobj.position, jj->physobj.direction);
	DrawCylinder(jj->physobj.position, 7.5f, 10.f, 20.f, 20, GRAY);
	DrawSphere(lep, 5.f, (Color){125, 125, 125, 125});
}
void renderKobobo(Kobobo* jj)
{
	if(!jj->is_mig)
	{
		renderKoboboMoyai(jj);
	}
	else
	{
		renderKoboboMig(jj);
	}
}

void KoboboShootBullet(Kobobo* jj)
{
	#define DEFAULT_FIRE_DELAY_TIMER 5
	if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		static int frames_till_next_shot = DEFAULT_FIRE_DELAY_TIMER;
		frames_till_next_shot--;
		if(!frames_till_next_shot)
		{
			frames_till_next_shot = DEFAULT_FIRE_DELAY_TIMER;
			resizeDynallocMemoryUp(BULLET_OBJECT);
			if(checkKettleStringEquality(getKobj(getDynallocObjCount()-1)->name, BULLET_OBJECT))
			{
				ktoBullet(getKobj(getDynallocObjCount()-1))->physobj = jj->physobj;
				ktoBullet(getKobj(getDynallocObjCount()-1))->physobj.direction.x *= 1.5;
				ktoBullet(getKobj(getDynallocObjCount()-1))->physobj.direction.z *= 1.5;
			}
		}
	}
}

#endif

void drawWorld()
{
	DrawPlane((Vector3){0, 0, 0}, (Vector2){1000, 1000}, BLUE);
}

void controlKeys(GameData* gamedata)
{
	if(IsKeyPressed(KEY_S))
	{
		saveData(gamedata, sizeof(GameData), "gmdt.sf");
	}
	if(IsKeyPressed(KEY_L))
	{
		loadData(gamedata, sizeof(GameData), "gmdt.sf");
	}
	if(IsKeyPressed(KEY_V))
	{
		int cam_mode[2] = {CAMERA_FIRST_PERSON, CAMERA_FREE};
		gamedata->is_cam_active = !gamedata->is_cam_active;
		SetCameraMode(gamedata->camera, cam_mode[gamedata->is_cam_active]);
	}
	if(IsKeyDown(KEY_Z))
	{
		gamedata->camera.fovy = 90.f;
		gamedata->cam_look_multiply = 3;
	}
	else
	{
		gamedata->camera.fovy = 120.f;
		gamedata->cam_look_multiply = 1;
	}

	if(IsKeyPressed(KEY_C))
	{
		gamedata->camera_follow_cursor = !gamedata->camera_follow_cursor;
	}
	while(IsKeyDown(KEY_H))
	{
		KettleString list[] =
		{
			newKettleString("Keybinds:"),
			newKettleString("S : save."),
			newKettleString("C : toggle camera look."),
			newKettleString("Z : zoom in."),
			newKettleString("V : change perspective mode."),
			newKettleString("H : help screen.")
		};
		BeginDrawing();
		ClearBackground(LIGHTGRAY);
		#define HELP_SCREEN_FONT_SIZE 50
		for(int i = 0; i != sizeof(list) / sizeof(*list); i++)
		{
			DrawText(list[i].contents, 5, i * (HELP_SCREEN_FONT_SIZE + 1), HELP_SCREEN_FONT_SIZE, BLACK);
		}
		EndDrawing();
	}

	gamedata->is_player_moving = (!IsKeyDown(KEY_LEFT_CONTROL));
}

void updateGameCamera(GameData* gamedata)
{
	if(!gamedata->camera_follow_cursor)
	{
		gamedata->camera.position.x = gamedata->player->physobj.position.x;
		gamedata->camera.position.y = 150.f;
		gamedata->camera.position.z = gamedata->player->physobj.position.z - 80;
		
		gamedata->camera.target = gamedata->player->physobj.position;
	}
	else
	{
		gamedata->camera.position.x = gamedata->player->physobj.position.x;
		gamedata->camera.position.y = 150.f;
		gamedata->camera.position.z = gamedata->player->physobj.position.z - 80;
		
		gamedata->camera.target = addVectors(	gamedata->player->physobj.position,
												multiplyVectors((Vector3){
													gamedata->cam_look_multiply,
													0,
													gamedata->cam_look_multiply
												},
												gamedata->player->physobj.direction));
	}
}

void swapVector(Vector3* swap)
{
	float fswap = swap->x;
	swap->x = -swap->z;
	swap->z = fswap;
}
Kobobo** getPlayerKoboboPtr()
{
	static Kobobo* ret;
	return &ret;
}
void setPlayerKoboboAddr(Kobobo* jj)
{
	(*getPlayerKoboboPtr()) = jj;
}
Kobobo* getPlayerKobobo()
{
}
void updateKoboboMovement(Kobobo* jj)
{
}

#define RADAR_TEXTURE_DIMS 150

RenderTexture2D* getRadarTexture()
{
	static RenderTexture radar_texture;
	return &radar_texture;
}

void initRadarTexture()
{
	bool loaded_yet = false;
	if(!loaded_yet)
	{
		(*getRadarTexture()) = LoadRenderTexture(RADAR_TEXTURE_DIMS, RADAR_TEXTURE_DIMS);
		loaded_yet = true;
	}
}

Vector2 getRadarPosition(Vector3 player_pos, Vector3 radar_obj_pos)
{
	return (Vector2){player_pos.x - radar_obj_pos.x, player_pos.z - radar_obj_pos.z};
}

void updateRadarTexture(Vector3 player_pos)
{
	BeginTextureMode(*getRadarTexture());
	ClearBackground((Color){0, 0, 0, 0});
	DrawCircle(RADAR_TEXTURE_DIMS / 2, RADAR_TEXTURE_DIMS / 2, (RADAR_TEXTURE_DIMS / 3) + 5, GREEN);

	for(int i = 0; i != getDynallocObjCount(); i++)
	{
		if(checkKettleStringEquality(getKobj(i)->name, DUMMY_OBJECT_NAME))
		{
			Vector2 current_radar_pos = getRadarPosition(player_pos, ktoBullet(getKobj(i))->physobj.position);
			DrawCircle(current_radar_pos.x, current_radar_pos.y, 3, RED);
		}
	}
	EndTextureMode();
}

void drawRadarToWindow()
{
	drawRenderTextureToWindow(getRadarTexture(), (Vector2){0, -(GetScreenHeight() - 300)}, (Vector2){150, 150});
}

void mainLoop(GameData* data)
{
	while(!WindowShouldClose())
	{
		
		updateGameCamera(data);

		startDrawingToRenderTexture();
		{
			ClearBackground(PINK);
			BeginMode3D(data->camera);
			{
				drawWorld();
				renderKobobo(data->player);

				renderKObjs();
			}
			EndMode3D();
		}
		endDrawingToRenderTexture();

		updateRadarTexture(data->player->physobj.position);

		BeginDrawing();
		{
			drawMainRenderTextureToWindow();
			drawRadarToWindow();
			DrawCircle(SCREENDIM_X / 2, SCREENDIM_Y / 2, 5, (Color){100, 100, 100, 100});
			DrawCircle(GetMouseX(), GetMouseY(), 5, (Color){100, 255, 100, 255});
			DrawFPS(0, 0);
		}
		EndDrawing();

		KoboboShootBullet(data->player);
		controlKeys(data);
		updatePlayerMovement(data);
			
		processDynalloc();
	}
}

int main()
{
	initDynallocCore();

	registerBullet();

	GameData data;
	(*getGameData()) = &data;
	startup("Jump Jet", SCREENDIM_X, SCREENDIM_Y, &data.camera);
	data.camera.position.y = 25.f;
	SetCameraMode(data.camera, CAMERA_ORBITAL);
	data.camera.fovy = 120;
	HideCursor();
	initRenderTexture();

	Kobobo jj = newKobobo();
	data.player = &jj;
	data.camera_follow_cursor = true;
	data.cam_look_multiply = 1;

	initRadarTexture();

	return 0;
}
#endif