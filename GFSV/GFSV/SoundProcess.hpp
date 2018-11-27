#pragma once
#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <cmath>

namespace SoundProcess
{
	/// サウンドの名前ID
	enum class ESOUNDNAME
	{
		saveOn, saveMiss, pianoAttack, pianoAttack1, pianoAttack2, pianoAttack3
		, jump, pianoAttackMiss, landing, landing2, footFloor, foot
		, ballPickUp, strikeBomb, ballPawnHigh, ballPawn, lightOneFoot, lightFoot
		, crayonDie, titleBurnLow, titleBurnLowMagni, titleBurn, titleMusicBox, normalBGM
		, battleBGM, lightBGM, noise3, noise2, noise, noise_2
		, noise_1, ending, boss
	};

	/// サウンドタイプ
	enum class ESOUNDTYPE
	{
		soundMem, soundStreem, sound3DSourceChara, sound3DSourcePartner, none
	};


	/// 初期化
	void Init();

	/// ロード
	void Load(int loadFile, ESOUNDNAME name, ESOUNDTYPE type);

	/// 3Dサウンドのロード
	void Load(int loadFile, ESOUNDNAME name, ESOUNDTYPE type, VECTOR partnerArea);

	/// 再生する
	void DoSound(ESOUNDNAME name);

	/// 音を入れていく
	void VolumeIn(ESOUNDNAME name, int volume = 255);

	/// 音を消す
	void VolumeReset(ESOUNDNAME name, int volume = 255);

	/// 解放する
	void Release();

	/// プロセス
	void Process();

	/// ステージ1特殊サウンド
	void StageOneSound(int touchNum);
	void StageOneBallCatch();

	/// プレイヤーの座標を設定
	void SetCharaArea(VECTOR area);

	/// リスナーの座標を設定
	void SetLisnerArea(VECTOR area);



	void Test();
}