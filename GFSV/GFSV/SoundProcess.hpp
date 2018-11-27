#pragma once
#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <cmath>

#include <vector>

namespace SoundProcess
{
	/// サウンドの名前ID
	enum class ESOUNDNAME
	{
		saveOn, saveMiss, pianoAttack3, pianoAttack2, pianoAttack1, pianoAttack
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
	void BGMVolumeIn(ESOUNDNAME name, int volume = 255);

	/// 音を変える
	void BGMVolumeTrans(ESOUNDNAME nextName, int volume = 255);

	/// 音を消す
	void BGMVolumeEnd(ESOUNDNAME name);

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


/*
風呂あがって思いついた

/// 今のBGMから次のBGMに遷移する
void Volume遷移(ESOUNDNAME nowName, int nowVolume, ESOUNDNAME nextName, int nextVolume);

/// 3D音源はボリュームの調整はしない

/// SEとBGMで優先度から音量を調整する
/// ぶっちゃけSEはすぐ消えるから音量をフェードさせるよう切り替えなくても良い気がする
/// でも作れたらSE用のフェード遷移作る

/// 今のBGMの音量を次の音量に変える
void VolumeFeed(ESOUNDNAME name, int nowVolume, int nextVolume);
*/