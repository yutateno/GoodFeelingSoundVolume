#include "SoundProcess.hpp"

namespace SoundProcess
{
	/// IDのやつがロードしたかどうか
	bool loadFlag[33];
	/// 再生しているかどうか
	bool playFlag[33];
	/// ロードしたサウンド
	int soundLoad[33];
	/// ロードしたサウンドのタイプ
	ESOUNDTYPE soundType[33];

	/// プレイヤーの座標
	VECTOR charaArea;
	/// リスナーの座標
	VECTOR lisnerArea;

	std::vector<ESOUNDNAME> seVector;

	/// BGMに関する----------------------
	/// BGMボリュームのフェード数値
	int bgmVolumeCount = 0;

	/// ボリューム上げる
	bool volumeUpFlag = false;

	/// ボリューム下げる
	bool volumeDownFlag = false;

	/// BGMを変える
	bool bgmTransFlag = false;

	/// BGMの名前
	ESOUNDNAME bgmName;

	/// BGM切り替え時の名前
	ESOUNDNAME nextBGMName;

	/// BGMボリューム
	int bgmVolume;

	/// 直前のBGMボリューム
	int preBGMVolume = 255;

	/// 次のBGMボリューム
	int nextBGMVolume = 255;
	/// ---------------------------------

	void VolumeInProcess();
	void VolumeResetProcess();
	void BGMVolumeTransProcess();


	/// 初期化
	void Init()
	{
		ZeroMemory(loadFlag, sizeof(loadFlag));
		ZeroMemory(playFlag, sizeof(playFlag));
		for (int i = 0; i != 33; ++i)
		{
			soundLoad[i] = -1;
		}
		for (int i = 0; i != 33; ++i)
		{
			soundType[i] = ESOUNDTYPE::none;
		}
		charaArea = VGet(0, 0, 0);
		lisnerArea = VGet(0, 0, 0);
	}


	/// 読み込み
	void Load(int loadFile, ESOUNDNAME name, ESOUNDTYPE type)
	{
		soundLoad[static_cast<int>(name)] = loadFile;
		loadFlag[static_cast<int>(name)] = true;
		soundType[static_cast<int>(name)] = type;
		if (type == ESOUNDTYPE::sound3DSourceChara)
		{
			Set3DPositionSoundMem(charaArea, soundLoad[static_cast<int>(name)]);
			Set3DRadiusSoundMem(VSize(VSub(charaArea, lisnerArea)), soundLoad[static_cast<int>(name)]);
		}
		if (type == ESOUNDTYPE::soundStreem)
		{

		}
	}


	/// 読み込み
	void Load(int loadFile, ESOUNDNAME name, ESOUNDTYPE type, VECTOR partnerArea)
	{
		soundLoad[static_cast<int>(name)] = loadFile;
		loadFlag[static_cast<int>(name)] = true;
		soundType[static_cast<int>(name)] = type;
		if (type == ESOUNDTYPE::sound3DSourcePartner)
		{
			Set3DPositionSoundMem(partnerArea, soundLoad[static_cast<int>(name)]);
			Set3DRadiusSoundMem(VSize(VSub(partnerArea, lisnerArea)), soundLoad[static_cast<int>(name)]);
		}
	}


	/// プロセス
	void Process()
	{
		/// 再生個数
		int count = 0;

		// 再生しているかどうか判断
		for (int i = 0; i != 33; ++i)
		{
			// 音が流れていなかったら
			if (!CheckSoundMem(soundLoad[i]))
			{
				playFlag[i] = false;
				continue;
			}

			// 3D音源だったら
			if (soundType[i] == ESOUNDTYPE::sound3DSourceChara)
			{
				Set3DPositionSoundMem(charaArea, soundLoad[i]);
				continue;
			}

			// 再生していたら
			if (playFlag[i])
			{
				count++;		// 再生個数を加算
			}
		}

		// 一個以下なら特に何もしない
		//if (count <= 1)return;

		// 最小がボリューム0にならないように
		count--;
		//printfDx("%d\n", count);
		/// 下げる値を減らしながら再生音量調整
		for (int i = 32; i >= 0; --i)
		{
			// 再生していなかったら
			if (!playFlag[i]) continue;

			if (i >= 22)
			{
				nextBGMVolume = 255 - (6 * count);
				continue;
			}

			// 音量を下げる
			//printfDx("%d\n", 255 - (60 * count));
			ChangeVolumeSoundMem(255 - (60 * count), soundLoad[i]);
			if (count > 0) count--;
		}

		if (bgmVolume > nextBGMVolume)
		{

		}
		if (bgmVolume < nextBGMVolume)
		{

		}

		if (volumeDownFlag)
		{
			VolumeResetProcess();
		}
		if (volumeUpFlag)
		{
			VolumeInProcess();
		}
		if (bgmTransFlag)
		{
			BGMVolumeTransProcess();
		}
	}

	void StageOneSound(int touchNum)
	{
		if (volumeDownFlag)
		{
			VolumeResetProcess();
		}
		if (volumeUpFlag)
		{
			VolumeInProcess();
		}

		if (touchNum > 4)return;
		ChangeVolumeSoundMem(touchNum * 60, soundLoad[static_cast<int>(ESOUNDNAME::titleMusicBox)]);
	}

	void StageOneBallCatch()
	{
		int rand = GetRand(1);
		if (rand == 0)
		{
			PlaySoundMem(soundLoad[static_cast<int>(ESOUNDNAME::ballPawnHigh)], DX_PLAYTYPE_BACK);
			ChangeVolumeSoundMem(125, soundLoad[static_cast<int>(ESOUNDNAME::ballPawnHigh)]);
		}
		else
		{
			PlaySoundMem(soundLoad[static_cast<int>(ESOUNDNAME::ballPawn)], DX_PLAYTYPE_BACK);
			ChangeVolumeSoundMem(125, soundLoad[static_cast<int>(ESOUNDNAME::ballPawn)]);
		}
	}


	/// 再生させる
	void DoSound(ESOUNDNAME name)
	{
		if (playFlag[static_cast<int>(name)]) return;
		PlaySoundMem(soundLoad[static_cast<int>(name)], DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(name)]);

		playFlag[static_cast<int>(name)] = true;
	}


	/// 音を入れていく処理
	void VolumeInProcess()
	{
		preBGMVolume = static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * bgmVolume);
		ChangeVolumeSoundMem(static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * bgmVolume), soundLoad[static_cast<int>(bgmName)]);
		if (bgmVolumeCount <= 120)
		{
			bgmVolumeCount++;
		}
		else
		{
			preBGMVolume = bgmVolume;
			volumeUpFlag = false;
		}
	}


	/// 音を入れていく
	void BGMVolumeIn(ESOUNDNAME name, int volume)
	{
		PlaySoundMem(soundLoad[static_cast<int>(name)], DX_PLAYTYPE_LOOP);
		ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(name)]);
		playFlag[static_cast<int>(name)] = true;

		volumeUpFlag = true;
		bgmName = name;
		bgmVolume = volume;
		bgmVolumeCount = 0;
	}


	/// BGMを切り替える処理
	void BGMVolumeTransProcess()
	{
		ChangeVolumeSoundMem(static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * bgmVolume), soundLoad[static_cast<int>(bgmName)]);

		preBGMVolume = static_cast<int>(nextBGMVolume - (sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * nextBGMVolume);
		ChangeVolumeSoundMem(static_cast<int>(nextBGMVolume - (sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * nextBGMVolume), soundLoad[static_cast<int>(nextBGMName)]);
		if (bgmVolumeCount >= 0)
		{
			bgmVolumeCount--;
		}
		else
		{
			preBGMVolume = nextBGMVolume;
			bgmName = nextBGMName;
			bgmVolume = nextBGMVolume;
			bgmTransFlag = false;
		}
	}
	
	/// BGMを切り替える
	void BGMVolumeTrans(ESOUNDNAME nextName, int volume)
	{
		PlaySoundMem(soundLoad[static_cast<int>(nextName)], DX_PLAYTYPE_LOOP);
		ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(nextName)]);

		bgmVolumeCount = 120;
		//bgmVolume = preBGMVolume;
		nextBGMName = nextName;
		nextBGMVolume = volume;
		bgmTransFlag = true;
	}


	/// 音を消す処理
	void VolumeResetProcess()
	{
		preBGMVolume = static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * bgmVolume);
		ChangeVolumeSoundMem(static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * bgmVolume), soundLoad[static_cast<int>(bgmName)]);
		if (bgmVolumeCount >= 0)
		{
			bgmVolumeCount--;
		}
		else
		{
			volumeDownFlag = false;
		}
	}


	/// 音を消す
	void BGMVolumeEnd(ESOUNDNAME name)
	{
		volumeUpFlag = false;
		volumeDownFlag = true;
		bgmVolumeCount = 120;
		bgmName = name;
	}


	/// 解放
	void Release()
	{
		BGMVolumeEnd(bgmName);
		for (int i = 0; i != 33; ++i)
		{
			StopSoundMem(soundLoad[i]);
			DeleteSoundMem(soundLoad[i]);
			soundLoad[i] = -1;
		}
	}


	/// キャラクターのエリア
	void SetCharaArea(VECTOR area)
	{
		charaArea = area;
	}


	/// リスナーのエリア
	void SetLisnerArea(VECTOR area)
	{
		lisnerArea = area;
	}
	void Test()
	{
		//printfDx("loadF: %d\tfile: %d\tPlayF: %d\n", loadFlag[static_cast<int>(ESOUNDNAME::titleMusicBox)], soundLoad[static_cast<int>(ESOUNDNAME::titleMusicBox)], playFlag[static_cast<int>(ESOUNDNAME::titleMusicBox)]);
	}
}
