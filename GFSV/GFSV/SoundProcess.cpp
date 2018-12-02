#include "SoundProcess.hpp"

namespace SoundProcess
{
	/// ゲームによって変わる部分------------------
	const int seNum = 22;
	const int bgmNum = 11;
	const int bgmArray = 2;
	const int bgmFeedValue = 90;
	///-------------------------------------------

	/// SEに関する------------------------

	// 保存する
	int se_sound[seNum];

	// ロードしたかどうか
	bool se_loadFlag[seNum];

	// 再生しているかどうか
	bool se_playFlag[seNum];

	// SEのユーザー音量調整
	float se_volumeAdjustment;


	/// BGMに関する-----------------------

	// 保存する
	int bgm_sound[bgmNum];

	// ロードしたかどうか
	bool bgm_loadFlag[bgmNum];

	// ボリューム
	int bgm_nowVolume[bgmArray];

	// BGMの名前
	ESOUNDNAME_BGM bgm_name[bgmArray];

	// 要求されたBGMのボリューム
	int bgm_requestVolume[bgmArray];

	// 変更中の目的ボリューム
	int bgm_nextVolume[bgmArray];

	// フェードカウント
	int bgm_feedCount[bgmArray];

	// BGMが流れているかどうか
	bool bgm_soundFlag;

	// BGMの最大指定音量
	int bgm_maxVolume[bgmArray];

	// BGMのユーザー音量調整
	float bgm_volumeAdjustment;


	/// 3Dに関する-------------------------

	// プレイヤー位置
	VECTOR charaArea;

	// 発生位置
	VECTOR partnerArea;

	// リスナー位置
	VECTOR listenerArea;


	/// 内部関数---------------------------

	// BGMプロセス
	void BGMProcess();

	// BGMフェード
	void BGMFeed();



	void Init()
	{
		for (int i = 0; i != seNum; ++i)
		{
			se_sound[i] = -1;
		}
		ZeroMemory(se_loadFlag, sizeof(se_loadFlag));
		ZeroMemory(se_playFlag, sizeof(se_playFlag));
		for (int i = 0; i != bgmNum; ++i)
		{
			bgm_sound[i] = -1;
		}
		ZeroMemory(bgm_loadFlag, sizeof(bgm_loadFlag));
		ZeroMemory(bgm_nowVolume, sizeof(bgm_nowVolume));
		for (int i = 0; i != bgmArray; ++i)
		{
			bgm_name[i] = ESOUNDNAME_BGM::none;
		}
		ZeroMemory(bgm_requestVolume, sizeof(bgm_requestVolume));
		ZeroMemory(bgm_nextVolume, sizeof(bgm_nextVolume));
		ZeroMemory(bgm_feedCount, sizeof(bgm_feedCount));
		ZeroMemory(bgm_maxVolume, sizeof(bgm_maxVolume));

		bgm_soundFlag = false;

		se_volumeAdjustment = 1.0f;
		bgm_volumeAdjustment = 1.0f;
	}


	void Load(int loadFile, ESOUNDNAME_SE name, ESOUNDTYPE type, VECTOR partnerArea)
	{
		se_sound[static_cast<int>(name)] = loadFile;
		se_loadFlag[static_cast<int>(name)] = true;
		if (type == ESOUNDTYPE::sound3DSourceChara)
		{
			Set3DPositionSoundMem(charaArea, se_sound[static_cast<int>(name)]);
			Set3DRadiusSoundMem(VSize(VSub(charaArea, listenerArea)), se_sound[static_cast<int>(name)]);
		}
		else if (type == ESOUNDTYPE::sound3DSourcePartner)
		{
			Set3DPositionSoundMem(partnerArea, se_sound[static_cast<int>(name)]);
			Set3DRadiusSoundMem(VSize(VSub(partnerArea, listenerArea)), se_sound[static_cast<int>(name)]);
		}
	}


	void Load(int loadFile, ESOUNDNAME_BGM name)
	{
		bgm_sound[static_cast<int>(name)] = loadFile;
		bgm_loadFlag[static_cast<int>(name)] = true;
	}


	void Process()
	{
		/// 再生個数
		int count = 0;

		// 再生しているかどうか判断
		for (int i = 0; i != seNum; ++i)
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

		if (bgm_soundFlag)
		{
			if (bgm_requestVolume[0] < bgm_requestVolume[1]
				&& bgm_requestVolume[1] != bgm_maxVolume[1] - (10 * count))
			{
				SetBGMVolume(bgm_name[1], bgm_maxVolume[1] - (10 * count), bgm_maxVolume[1]);
			}
			if (bgm_requestVolume[0] > bgm_requestVolume[1]
				&& bgm_requestVolume[0] != bgm_maxVolume[0] - (10 * count))
			{
				SetBGMVolume(bgm_name[0], bgm_maxVolume[0] - (10 * count), bgm_maxVolume[0]);
			}
			count--;
		}


		/// 下げる値を減らしながら再生音量調整
		for (int i = seNum - 1; i >= 0; --i)
		{
			// 再生していなかったら
			if (!se_playFlag[i]) continue;

			// 音量を下げる
			ChangeVolumeSoundMem(static_cast<int>(((255 - (10 * count))) * se_volumeAdjustment), se_sound[i]);
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
		for (int i = 0; i != bgmArray; ++i)
		{
			if (bgm_name[i] == ESOUNDNAME_BGM::none) continue;
			SetBGMVolume(bgm_name[i], 0, 0);
		}
	}


	void BGMTrans(ESOUNDNAME_BGM nextName, int volume)
	{
		if (!bgm_soundFlag) bgm_soundFlag = true;

		if (bgm_name[1] == ESOUNDNAME_BGM::none)
		{
			SetBGMVolume(bgm_name[0], 0, 0);
			SetBGMVolume(nextName, volume, volume);
			return;
		}

		if (bgm_name[1] == nextName)
		{
			SetBGMVolume(bgm_name[0], 0, 0);
			SetBGMVolume(nextName, volume, volume);
			return;
		}

		if (bgm_name[0] == nextName)
		{
			SetBGMVolume(bgm_name[1], 0, 0);
			SetBGMVolume(nextName, volume, volume);
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

			SetBGMVolume(nextName, volume, volume);
			SetBGMVolume(bgm_name[1], 0, 0);
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

			SetBGMVolume(bgm_name[0], 0, 0);
			SetBGMVolume(nextName, volume, volume);
			return;
		}
	}


	void BGMProcess()
	{
		BGMFeed();
		printfDx("%d\n", GetVolumeSoundMem2(bgm_sound[static_cast<int>(bgm_name[0])]));
	}


	void BGMFeed()
	{
		if (!bgm_soundFlag) bgm_soundFlag = true;

		for (int i = 0; i != bgmArray; ++i)
		{
			if (bgm_nowVolume[i] < bgm_nextVolume[i])
			{
				ChangeVolumeSoundMem(static_cast<int>((bgm_nowVolume[i] + static_cast<int>((sin(-M_PI / 2 + M_PI / bgmFeedValue * bgm_feedCount[i]) + 1) / 2 * (bgm_nextVolume[i] - bgm_nowVolume[i]))) * bgm_volumeAdjustment)
					, bgm_sound[static_cast<int>(bgm_name[i])]);
			}
			else
			{
				ChangeVolumeSoundMem(static_cast<int>((bgm_nowVolume[i] - static_cast<int>((sin(-M_PI / 2 + M_PI / bgmFeedValue * bgm_feedCount[i]) + 1) / 2 * (bgm_nowVolume[i] - bgm_nextVolume[i]))) * bgm_volumeAdjustment)
					, bgm_sound[static_cast<int>(bgm_name[i])]);
			}
			if (bgm_feedCount[i] <= bgmFeedValue)
			{
				bgm_feedCount[i]++;
			}
			else
			{
				bgm_nowVolume[i] = bgm_nextVolume[i];
			}
		}
	}


	void SetBGMVolume(ESOUNDNAME_BGM name, int volume, int maxVolume)
	{
		if (!bgm_soundFlag) bgm_soundFlag = true;

		for (int i = 0; i != bgmArray; ++i)
		{
			if (bgm_name[i] == ESOUNDNAME_BGM::none
				|| bgm_name[i] == name)
			{
				bgm_name[i] = name;
				bgm_requestVolume[i] = volume;
				bgm_maxVolume[i] = maxVolume;
				if (bgm_nextVolume[i] != bgm_requestVolume[i])
				{
					bgm_nowVolume[i] = GetVolumeSoundMem2(bgm_sound[static_cast<int>(bgm_name[i])]);
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
		for (int i = 0; i != seNum; ++i)
		{
			if (!se_loadFlag[i]) continue;
			DeleteSoundMem(se_sound[i]);
			se_sound[i] = -1;
		}
		for (int i = 0; i != bgmNum; ++i)
		{
			if (!bgm_loadFlag[i]) continue;
			StopSoundMem(bgm_sound[i]);
			DeleteSoundMem(bgm_sound[i]);
			bgm_sound[i] = -1;
		}
	}


	void SetCharaArea(VECTOR area)
	{
		charaArea = area;
	}


	void SetLisnerArea(VECTOR area)
	{
		listenerArea = area;
	}


	void SetSEVolumeEntire(float volumeEntire)
	{
		se_volumeAdjustment = volumeEntire;
	}


	void SetBGMVolumeEntire(float volumeEntire)
	{
		bgm_volumeAdjustment = volumeEntire;
	}
}
