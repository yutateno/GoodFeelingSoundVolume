#include "SoundProcess.hpp"

namespace SoundProcess
{
	// SEに関する
	/// 保存する
	int se_sound[22];
	/// ロードしたかどうか
	bool se_loadFlag[22];
	/// 再生しているかどうか
	bool se_playFlag[22];

	// BGMに関する
	/// 保存する
	int bgm_sound[11];
	/// ロードしたかどうか
	bool bgm_loadFlag[11];
	/// ボリューム
	int bgm_nowVolume[2];
	/// BGMの名前
	ESOUNDNAME_BGM bgm_name[2];
	/// 要求されたBGMのボリューム
	int bgm_requestVolume[2];
	/// 変更中の目的ボリューム
	int bgm_nextVolume[2];
	/// フェードカウント
	int bgm_feedCount[2];
	/// BGMが流れているかどうか
	bool bgm_soundFlag;



	void Init()
	{
		for (int i = 0; i != 22; ++i)
		{
			se_sound[i] = -1;
		}
		ZeroMemory(se_loadFlag		, sizeof(se_loadFlag)		);
		ZeroMemory(se_playFlag		, sizeof(se_playFlag)		);
		for (int i = 0; i != 11; ++i)
		{
			bgm_sound[i] = -1;
		}
		ZeroMemory(bgm_loadFlag		, sizeof(bgm_loadFlag)		);
		ZeroMemory(bgm_nowVolume	, sizeof(bgm_nowVolume)		);
		for (int i = 0; i != 2; ++i)
		{
			bgm_name[i] = ESOUNDNAME_BGM::none;
		}
		ZeroMemory(bgm_requestVolume, sizeof(bgm_requestVolume)	);
		ZeroMemory(bgm_nextVolume	, sizeof(bgm_nextVolume)	);
		ZeroMemory(bgm_feedCount	, sizeof(bgm_feedCount)		);

		bgm_soundFlag = false;
	}

	void Load(int loadFile, ESOUNDNAME_SE name, ESOUNDTYPE type)
	{
		se_sound[static_cast<int>(name)] = loadFile;
		se_loadFlag[static_cast<int>(name)] = true;
	}

	void Load(int loadFile, ESOUNDNAME_BGM name)
	{
		bgm_sound[static_cast<int>(name)] = loadFile;
		bgm_loadFlag[static_cast<int>(name)] = true;
	}

	void Load(int loadFile, ESOUNDNAME_SE name, ESOUNDTYPE type, VECTOR partnerArea)
	{
		/// 3D関連
	}

	void Process()
	{
		/// 再生個数
		int count = 0;

		// 再生しているかどうか判断
		for (int i = 0; i != 22; ++i)
		{
			// 音が流れていなかったら
			if (!CheckSoundMem(se_sound[i]))
			{
				se_playFlag[i] = false;
				continue;
			}

			// 再生していたら
			if (se_playFlag[i])
			{
				count++;		// 再生個数を加算
			}
		}

		if (bgm_soundFlag) count++;


		/// 下げる値を減らしながら再生音量調整
		for (int i = 21; i >= 0; --i)
		{
			// 再生していなかったら
			if (!se_playFlag[i]) continue;

			if (bgm_soundFlag)
			{
				bgm_requestVolume[0] = 255 - (10 * count);
				continue;
			}

			// 音量を下げる
			ChangeVolumeSoundMem(255 - (10 * count), se_sound[i]);
			if (count > 0)
			{
				count--;
			}
			else
			{
				break;
			}
		}

		BGMProcess();
	}

	void DoSound(ESOUNDNAME_SE name, int volume)
	{
		if (!se_playFlag[static_cast<int>(name)])
		{
			PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK);
			ChangeVolumeSoundMem(0, se_sound[static_cast<int>(name)]);
		}

		se_playFlag[static_cast<int>(name)] = true;
	}

	void BGMEnd()
	{
		/*
		for (int i = 0; i != 2; ++i)
		{
			if (bgm_name[i] == ESOUNDNAME_BGM::none) continue;
			StopSoundMem(bgm_sound[static_cast<int>(bgm_name[i])]);
		}*/
	}

	void BGMTrans(ESOUNDNAME_BGM nextName, int volume)
	{
		if (bgm_name[1] == ESOUNDNAME_BGM::none)
		{
			SetBGMVolume(bgm_name[0], 0);
			SetBGMVolume(nextName, volume);
			return;
		}

		if (bgm_name[1] == nextName)
		{
			SetBGMVolume(bgm_name[0], 0);
			SetBGMVolume(nextName, volume);
			return;
		}

		if (bgm_name[0] == nextName)
		{
			SetBGMVolume(bgm_name[1], 0);
			SetBGMVolume(nextName, volume);
			return;
		}

		if (bgm_requestVolume[0] < bgm_requestVolume[1])
		{
			ChangeVolumeSoundMem(0, bgm_sound[static_cast<int>(bgm_name[0])]);
			StopSoundMem(bgm_sound[static_cast<int>(bgm_name[0])]);
			bgm_nowVolume[0] = 0;
			bgm_nextVolume[0] = 0;
			bgm_requestVolume[0] = 0;
			bgm_name[0] = ESOUNDNAME_BGM::none;

			SetBGMVolume(nextName, volume);
			SetBGMVolume(bgm_name[1], 0);
			return;
		}

		if (bgm_requestVolume[0] > bgm_requestVolume[1])
		{
			ChangeVolumeSoundMem(0, bgm_sound[static_cast<int>(bgm_name[1])]);
			StopSoundMem(bgm_sound[static_cast<int>(bgm_name[1])]);
			bgm_nowVolume[1] = 0;
			bgm_nextVolume[1] = 0;
			bgm_requestVolume[1] = 0;
			bgm_name[1] = ESOUNDNAME_BGM::none;

			SetBGMVolume(bgm_name[0], 0);
			SetBGMVolume(nextName, volume);
			return;
		}
	}

	void BGMProcess()
	{
		/*for (int i = 0; i != 2; ++i)
		{
			if (bgm_nowVolume[i] <= 2)
			{
				StopSoundMem(bgm_sound[static_cast<int>(bgm_name[i])]);
			}
		}*/

		BGMFeed();
	}

	void BGMFeed()
	{
		for (int i = 0; i != 2; ++i)
		{
			if (bgm_nowVolume[i] < bgm_nextVolume[i])
			{
				ChangeVolumeSoundMem(bgm_nowVolume[i] + static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgm_feedCount[i]) + 1) / 2 * (bgm_nextVolume[i] - bgm_nowVolume[i]))
					, bgm_sound[static_cast<int>(bgm_name[i])]);
			}
			else
			{
				ChangeVolumeSoundMem(bgm_nowVolume[i] - static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgm_feedCount[i]) + 1) / 2 * (bgm_nowVolume[i] - bgm_nextVolume[i]))
					, bgm_sound[static_cast<int>(bgm_name[i])]);
			}
			if (bgm_feedCount[i] <= 120)
			{
				bgm_feedCount[i]++;
			}
			else
			{
				bgm_nowVolume[i] = bgm_nextVolume[i];
			}
		}
	}

	void SetBGMVolume(ESOUNDNAME_BGM name, int volume)
	{
		for (int i = 0; i != 2; ++i)
		{
			if (bgm_name[i] == ESOUNDNAME_BGM::none
				|| bgm_name[i] == name)
			{
				bgm_name[i] = name;
				bgm_requestVolume[i] = volume;
				if (bgm_nextVolume[i] != bgm_requestVolume[i])
				{
					bgm_nowVolume[i] = static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgm_feedCount[i]) + 1) / 2 * bgm_nextVolume[i]);
					bgm_nextVolume[i] = bgm_requestVolume[i];
				}
				bgm_feedCount[i] = 0;
				if (bgm_nowVolume[i] <= 2
					|| !(CheckSoundMem(bgm_sound[static_cast<int>(bgm_name[i])])))
				{
					StopSoundMem(bgm_sound[static_cast<int>(bgm_name[i])]);
					PlaySoundMem(bgm_sound[static_cast<int>(bgm_name[i])], DX_PLAYTYPE_LOOP);
					ChangeVolumeSoundMem(0, bgm_sound[static_cast<int>(bgm_name[i])]);
					bgm_nowVolume[i] = 0;
				}
				return;
			}
		}
	}

	void Release()
	{
		for (int i = 0; i != 22; ++i)
		{
			if (!se_loadFlag[i]) continue;
			DeleteSoundMem(se_sound[i]);
			se_sound[i] = -1;
		}
		for (int i = 0; i != 11; ++i)
		{
			if (!bgm_loadFlag[i]) continue;
			StopSoundMem(bgm_sound[i]);
			DeleteSoundMem(bgm_sound[i]);
			bgm_sound[i] = -1;
		}
	}

	void Test()
	{
		printfDx("Name: %d\tnow: %d\trequest: %d\tnext: %d\n", bgm_name[0], bgm_nowVolume[0], bgm_requestVolume[0], bgm_nextVolume[0]);
	}


	///// SEに関する----------------------
	///// IDのやつがロードしたかどうか
	//bool loadFlag[22];
	///// 再生しているかどうか
	//bool playFlag[22];
	///// ロードしたサウンド
	//int soundLoad[22];
	///// ロードしたサウンドのタイプ
	//ESOUNDTYPE soundType[22];

	///// プレイヤーの座標
	//VECTOR charaArea;
	///// リスナーの座標
	//VECTOR lisnerArea;

	///// BGMに関する----------------------
	///// BGMボリュームのフェード数値
	//int bgmVolumeCount = 0;

	///// ボリューム上げる
	//bool volumeUpFlag = false;

	///// ボリューム下げる
	//bool volumeDownFlag = false;

	///// BGMを変える
	//bool bgmTransFlag = false;

	///// BGMの名前
	//ESOUNDNAME bgmName;

	///// BGM切り替え時の名前
	//ESOUNDNAME nextBGMName = bgmName;

	///// BGM途中切り替え時の名前
	//ESOUNDNAME tempBGMName = bgmName;

	///// BGMボリューム
	//int bgmVolume = 0;

	///// 直前のBGMボリューム
	//int preBGMVolume = bgmVolume;

	///// 次のBGMボリューム
	//int nextBGMVolume = bgmVolume;
	///// ---------------------------------

	//void VolumeInProcess();
	//void VolumeResetProcess();
	//void BGMVolumeTransProcess();


	///// 初期化
	//void Init()
	//{
	//	ZeroMemory(loadFlag, sizeof(loadFlag));
	//	ZeroMemory(playFlag, sizeof(playFlag));
	//	volumeUpFlag = false;
	//	volumeDownFlag = false;
	//	bgmTransFlag = false;
	//	for (int i = 0; i != 33; ++i)
	//	{
	//		soundLoad[i] = -1;
	//	}
	//	for (int i = 0; i != 33; ++i)
	//	{
	//		soundType[i] = ESOUNDTYPE::none;
	//	}
	//	charaArea = VGet(0, 0, 0);
	//	lisnerArea = VGet(0, 0, 0);
	//}

	//void Load(int loadFile, ESOUNDNAME_SE name, ESOUNDTYPE type)
	//{
	//	soundLoad[static_cast<int>(name)] = loadFile;
	//	loadFlag[static_cast<int>(name)] = true;
	//	soundType[static_cast<int>(name)] = type;
	//	if (type == ESOUNDTYPE::sound3DSourceChara)
	//	{
	//		Set3DPositionSoundMem(charaArea, soundLoad[static_cast<int>(name)]);
	//		Set3DRadiusSoundMem(VSize(VSub(charaArea, lisnerArea)), soundLoad[static_cast<int>(name)]);
	//	}
	//}

	//void Load(int loadFile, ESOUNDNAME_BGM name, ESOUNDTYPE type)
	//{
	//	soundLoad[static_cast<int>(name)] = loadFile;
	//	loadFlag[static_cast<int>(name)] = true;
	//	soundType[static_cast<int>(name)] = type;
	//}

	//void Load(int loadFile, ESOUNDNAME_SE name, ESOUNDTYPE type, VECTOR partnerArea)
	//{
	//	soundLoad[static_cast<int>(name)] = loadFile;
	//	loadFlag[static_cast<int>(name)] = true;
	//	soundType[static_cast<int>(name)] = type;
	//	if (type == ESOUNDTYPE::sound3DSourcePartner)
	//	{
	//		Set3DPositionSoundMem(partnerArea, soundLoad[static_cast<int>(name)]);
	//		Set3DRadiusSoundMem(VSize(VSub(partnerArea, lisnerArea)), soundLoad[static_cast<int>(name)]);
	//	}
	//}


	///// 読み込み
	//void Load(int loadFile, ESOUNDNAME name, ESOUNDTYPE type)
	//{
	//	soundLoad[static_cast<int>(name)] = loadFile;
	//	loadFlag[static_cast<int>(name)] = true;
	//	soundType[static_cast<int>(name)] = type;
	//	if (type == ESOUNDTYPE::sound3DSourceChara)
	//	{
	//		Set3DPositionSoundMem(charaArea, soundLoad[static_cast<int>(name)]);
	//		Set3DRadiusSoundMem(VSize(VSub(charaArea, lisnerArea)), soundLoad[static_cast<int>(name)]);
	//	}
	//	if (type == ESOUNDTYPE::soundStreem)
	//	{

	//	}
	//}


	///// 読み込み
	//void Load(int loadFile, ESOUNDNAME name, ESOUNDTYPE type, VECTOR partnerArea)
	//{
	//	soundLoad[static_cast<int>(name)] = loadFile;
	//	loadFlag[static_cast<int>(name)] = true;
	//	soundType[static_cast<int>(name)] = type;
	//	if (type == ESOUNDTYPE::sound3DSourcePartner)
	//	{
	//		Set3DPositionSoundMem(partnerArea, soundLoad[static_cast<int>(name)]);
	//		Set3DRadiusSoundMem(VSize(VSub(partnerArea, lisnerArea)), soundLoad[static_cast<int>(name)]);
	//	}
	//}


	///// プロセス
	//void Process()
	//{
	//	/// 再生個数
	//	int count = 0;

	//	// 再生しているかどうか判断
	//	for (int i = 0; i != 33; ++i)
	//	{
	//		// 音が流れていなかったら
	//		if (!CheckSoundMem(soundLoad[i]))
	//		{
	//			playFlag[i] = false;
	//			continue;
	//		}

	//		// 3D音源だったら
	//		if (soundType[i] == ESOUNDTYPE::sound3DSourceChara)
	//		{
	//			Set3DPositionSoundMem(charaArea, soundLoad[i]);
	//			continue;
	//		}

	//		// 再生していたら
	//		if (playFlag[i])
	//		{
	//			count++;		// 再生個数を加算
	//		}
	//	}

	//	// 一個以下なら特に何もしない
	//	//if (count <= 1)return;

	//	// 最小がボリューム0にならないように
	//	count--;
	//	//printfDx("%d\n", count);
	//	/// 下げる値を減らしながら再生音量調整
	//	for (int i = 32; i >= 0; --i)
	//	{
	//		// 再生していなかったら
	//		if (!playFlag[i]) continue;

	//		if (i >= 22)
	//		{
	//			nextBGMVolume = 255 - (6 * count);
	//			continue;
	//		}

	//		// 音量を下げる
	//		//printfDx("%d\n", 255 - (60 * count));
	//		ChangeVolumeSoundMem(255 - (60 * count), soundLoad[i]);
	//		if (count > 0) count--;
	//	}

	//	if (bgmVolume > nextBGMVolume)
	//	{

	//	}
	//	if (bgmVolume < nextBGMVolume)
	//	{

	//	}

	//	if (volumeDownFlag)
	//	{
	//		VolumeResetProcess();
	//	}
	//	if (volumeUpFlag)
	//	{
	//		VolumeInProcess();
	//	}
	//	if (bgmTransFlag)
	//	{
	//		BGMVolumeTransProcess();
	//	}

	//	//printfDx("%d\tDown: %d\tUp: %d\tTrans: %d\n", preBGMVolume, volumeDownFlag, volumeUpFlag, bgmTransFlag);
	//}

	//void StageOneSound(int touchNum)
	//{
	//	if (volumeDownFlag)
	//	{
	//		VolumeResetProcess();
	//	}
	//	if (volumeUpFlag)
	//	{
	//		VolumeInProcess();
	//	}

	//	if (touchNum > 4)return;
	//	ChangeVolumeSoundMem(touchNum * 60, soundLoad[static_cast<int>(ESOUNDNAME::titleMusicBox)]);
	//}

	//void StageOneBallCatch()
	//{
	//	int rand = GetRand(1);
	//	if (rand == 0)
	//	{
	//		PlaySoundMem(soundLoad[static_cast<int>(ESOUNDNAME::ballPawnHigh)], DX_PLAYTYPE_BACK);
	//		ChangeVolumeSoundMem(125, soundLoad[static_cast<int>(ESOUNDNAME::ballPawnHigh)]);
	//	}
	//	else
	//	{
	//		PlaySoundMem(soundLoad[static_cast<int>(ESOUNDNAME::ballPawn)], DX_PLAYTYPE_BACK);
	//		ChangeVolumeSoundMem(125, soundLoad[static_cast<int>(ESOUNDNAME::ballPawn)]);
	//	}
	//}


	///// 再生させる
	//void DoSound(ESOUNDNAME name)
	//{
	//	if (playFlag[static_cast<int>(name)]) return;
	//	PlaySoundMem(soundLoad[static_cast<int>(name)], DX_PLAYTYPE_BACK);
	//	ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(name)]);

	//	playFlag[static_cast<int>(name)] = true;
	//}


	///// 音を入れていく処理
	//void VolumeInProcess()
	//{
	//	ChangeVolumeSoundMem(static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * bgmVolume), soundLoad[static_cast<int>(bgmName)]);
	//	if (bgmVolumeCount <= 120)
	//	{
	//		bgmVolumeCount++;
	//	}
	//	
	//	if (bgmVolumeCount > 120 && volumeUpFlag)
	//	{
	//		preBGMVolume = GetVolumeSoundMem2(soundLoad[static_cast<int>(bgmName)]);
	//		volumeUpFlag = false;
	//	}
	//}


	///// 音を入れていく
	//void BGMVolumeIn(ESOUNDNAME name, int volume)
	//{
	//	PlaySoundMem(soundLoad[static_cast<int>(name)], DX_PLAYTYPE_LOOP);
	//	ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(name)]);
	//	playFlag[static_cast<int>(name)] = true;

	//	volumeUpFlag = true;
	//	bgmName = name;
	//	nextBGMName = bgmName;
	//	bgmVolume = volume;
	//	bgmVolumeCount = 0;
	//}


	///// BGMを切り替える処理
	//void BGMVolumeTransProcess()
	//{
	//	ChangeVolumeSoundMem(static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * bgmVolume), soundLoad[static_cast<int>(bgmName)]);

	//	ChangeVolumeSoundMem(static_cast<int>(nextBGMVolume - (sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * nextBGMVolume), soundLoad[static_cast<int>(nextBGMName)]);
	//	if (bgmVolumeCount >= 0)
	//	{
	//		bgmVolumeCount--;
	//	}
	//	
	//	if (bgmVolumeCount < 0 && bgmTransFlag)
	//	{
	//		bgmName = nextBGMName;
	//		//preBGMVolume = GetVolumeSoundMem2(soundLoad[static_cast<int>(bgmName)]);
	//		bgmVolume = GetVolumeSoundMem2(soundLoad[static_cast<int>(bgmName)]);
	//		bgmTransFlag = false;
	//	}
	//}
	//
	///// BGMを切り替える
	//void BGMVolumeTrans(ESOUNDNAME nextName, int volume)
	//{
	//	PlaySoundMem(soundLoad[static_cast<int>(nextName)], DX_PLAYTYPE_LOOP);
	//	ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(nextName)]);

	//	// BGM切り替えがすでに行われていて変更後のBGMが異なる場合
	//	if (bgmTransFlag && nextName != tempBGMName)
	//	{
	//		if (bgmName != nextName)
	//		{
	//			StopSoundMem(soundLoad[static_cast<int>(bgmName)]);
	//			ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(bgmName)]);
	//		}
	//		bgmName = nextBGMName;
	//	}

	//	bgmVolumeCount = 120;
	//	bgmVolume = GetVolumeSoundMem2(soundLoad[static_cast<int>(bgmName)]);
	//	preBGMVolume = bgmVolume;
	//	nextBGMName = nextName;
	//	tempBGMName = nextBGMName;
	//	nextBGMVolume = volume;
	//	bgmTransFlag = true;
	//}


	///// 音を消す処理
	//void VolumeResetProcess()
	//{
	//	ChangeVolumeSoundMem(static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * bgmVolume), soundLoad[static_cast<int>(bgmName)]);
	//	if (bgmVolumeCount >= 0)
	//	{
	//		bgmVolumeCount--;
	//	}
	//	
	//	if (bgmVolumeCount < 0 && volumeDownFlag)
	//	{
	//		volumeDownFlag = false;
	//	}
	//}


	///// 音を消す
	//void BGMVolumeEnd(ESOUNDNAME name)
	//{
	//	volumeUpFlag = false;
	//	bgmVolume = GetVolumeSoundMem2(soundLoad[static_cast<int>(name)]);
	//	volumeDownFlag = true;
	//	bgmVolumeCount = 120;
	//	bgmName = name;
	//}


	///// 解放
	//void Release()
	//{
	//	BGMVolumeEnd(bgmName);
	//	for (int i = 0; i != 33; ++i)
	//	{
	//		StopSoundMem(soundLoad[i]);
	//		DeleteSoundMem(soundLoad[i]);
	//		soundLoad[i] = -1;
	//	}
	//}


	///// キャラクターのエリア
	//void SetCharaArea(VECTOR area)
	//{
	//	charaArea = area;
	//}



///// リスナーのエリア
	//void SetLisnerArea(VECTOR area)
	//{
	//	lisnerArea = area;
	//}
	//void Test()
	//{
	//	//printfDx("loadF: %d\tfile: %d\tPlayF: %d\n", loadFlag[static_cast<int>(ESOUNDNAME::titleMusicBox)], soundLoad[static_cast<int>(ESOUNDNAME::titleMusicBox)], playFlag[static_cast<int>(ESOUNDNAME::titleMusicBox)]);
	//}
}
