#pragma once
#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <cmath>


/*
音周りのプロセス
*/
namespace SoundProcess
{
	/// ゲームによって変わる部分-------------------------------------------------------

	// サウンドの名前ID_SEを優先度順に
	enum class ESOUNDNAME_SE
	{
		saveOn, saveMiss, pianoAttack3, pianoAttack2, pianoAttack1, pianoAttack
		, jump, pianoAttackMiss, landing, landing2, footFloor, foot
		, ballPickUp, ballPickUp2, strikeBomb, ballPawnHigh, ballPawn, lightOneFoot, lightFoot
		, crayonDie, titleBurnLow, titleBurnLowMagni, titleBurn
	};

	// サウンドの名前ID_BGM
	enum class ESOUNDNAME_BGM
	{
		titleMusicBox, normalBGM, battleBGM, lightBGM, noise3, noise2
		, noise, noise_2, noise_1, ending, boss, none
	};
	/// --------------------------------------------------------------------------------

	// サウンドタイプ
	enum class ESOUNDTYPE
	{
		soundMem, soundStreem, sound3DSourceChara, sound3DSourcePartner, none
	};


	// 初期化
	void Init();

	// ロード
	void Load(int loadFile, ESOUNDNAME_SE name);
	void Load(int loadFile, ESOUNDNAME_BGM name);

	// プロセス
	void Process();

	// SEの再生
	void DoSound(ESOUNDNAME_SE name, int volume = 255);

	// 3DSEの再生
	void DoSound(ESOUNDNAME_SE name, VECTOR area, int volume = 255);

	// BGMの終わり
	void BGMEnd();

	// BGM切り替え
	void BGMTrans(ESOUNDNAME_BGM nextName, int volume = 255);

	// BGM音量設定
	void SetBGMVolume(ESOUNDNAME_BGM name, int volume, int maxVolume = 255);

	// 解放
	void Release();

	// プレイヤーの座標を設定
	void SetCharaArea(VECTOR area);

	// リスナーの座標を設定
	void SetLisnerArea(VECTOR area);

	// リスナーのビュー座標を設定
	void SetLisnerViewArea(VECTOR area);

	// サウンドの3Dでの大きさを設定
	void Set3DRadius(float radius);

	// SEのユーザー音量調整を設定
	void SetSEVolumeEntire(float volumeEntire);

	// SEのユーザー音量調整を渡す
	float GetSEVolumeEntire();

	// BGMのユーザー音量調整を設定
	void SetBGMVolumeEntire(float volumeEntire);

	// BGMのユーザー音量調整を渡す
	float GetBGMVolumeEntire();

	// オプション画面にいるかどうか、またサウンド調整しているかどうかでもここで調整する
	void SetOptionMenuNow(bool nowTrue);
}
