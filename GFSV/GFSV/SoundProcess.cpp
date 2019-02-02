#include "SoundProcess.hpp"


namespace SoundProcess
{
	/// ゲームによって変わる部分------------------
	const int seNum = 23;			// SEの数
	const int bgmNum = 11;			// BGMの数
	const int bgmArray = 2;			// BGMの最大再生個数
	const int bgmFeedValue = 60;	// BGMのフェード値
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

	// SEの指定最大音量
	int se_volumeSetMaxVolume[seNum];


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

	// リスナー位置
	VECTOR listenerArea;

	// リスナーのビュー位置
	VECTOR listenerViewArea;

	// サウンドの大きさ
	float volume3DRadius;


	/// 内部関数---------------------------

	// BGMプロセス
	void BGMProcess();

	// BGMフェード
	void BGMFeed();


	/// オプションに関する-------------

	// オプションにいる場合の音量調整用
	float optionNowVolume;



	/// --------------------------------------------------------------------------------------------------
	void Init()
	{
		for (int i = 0; i != seNum; ++i)
		{
			se_sound[i] = -1;
		}
		ZeroMemory(se_loadFlag, sizeof(se_loadFlag));
		ZeroMemory(se_playFlag, sizeof(se_playFlag));
		ZeroMemory(se_volumeSetMaxVolume, sizeof(se_volumeSetMaxVolume));
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
		optionNowVolume = 1.0f;
	} /// void Init()



	/// --------------------------------------------------------------------------------------------------
	void Load(int loadFile, ESOUNDNAME_SE name)
	{
		se_sound[static_cast<int>(name)] = loadFile;
		se_loadFlag[static_cast<int>(name)] = true;
	}



	/// --------------------------------------------------------------------------------------------------
	void Load(int loadFile, ESOUNDNAME_BGM name)
	{
		bgm_sound[static_cast<int>(name)] = loadFile;
		bgm_loadFlag[static_cast<int>(name)] = true;
	}



	/// --------------------------------------------------------------------------------------------------
	void Process()
	{
		// 3D音源の位置を設定
		Set3DSoundListenerPosAndFrontPos_UpVecY(VAdd(listenerArea, charaArea), VAdd(listenerViewArea, charaArea));


		int count = 0;			// 現在の再生個数


		// SEが再生しているかどうか判断
		for (int i = 0; i != seNum; ++i)
		{
			// 音が流れていなかったら
			if (!CheckSoundMem(se_sound[i]))
			{
				se_playFlag[i] = false;
				continue;
			}


			// 再生していたら
			if (se_playFlag[i]) count++;		// 再生個数を加算
		}


		// BGMが再生しているかどうか
		if (bgm_soundFlag)
		{
			// リクエストの音量が0番目より1番目の方が大きかったら
			if (bgm_requestVolume[0] < bgm_requestVolume[1]
				&& bgm_requestVolume[1] != bgm_maxVolume[1] - (10 * count))
			{
				// 大きい方のBGMの音量を調整する、その際にSEの個数に応じて音量を下げる
				SetBGMVolume(bgm_name[1], bgm_maxVolume[1] - (10 * count), bgm_maxVolume[1]);
			}


			// リクエストの音量が1番目より0番目の方が大きかったら
			if (bgm_requestVolume[0] > bgm_requestVolume[1]
				&& bgm_requestVolume[0] != bgm_maxVolume[0] - (10 * count))
			{
				// 大きい方のBGMの音量を調整する、その際にSEの個数に応じて音量を下げる
				SetBGMVolume(bgm_name[0], bgm_maxVolume[0] - (10 * count), bgm_maxVolume[0]);
			}


			BGMProcess();		// BGMのプロセスを呼ぶ
		}


		// 下げる値を減らしながら再生音量調整
		for (int i = seNum - 1; i >= 0; --i)
		{
			// 再生していなかったら
			if (!se_playFlag[i]) continue;


			// 優先度の低い順ものから音量を小さくする
			ChangeVolumeSoundMem(static_cast<int>((((255 - (10 * count))) * se_volumeAdjustment * optionNowVolume) / 255 * se_volumeSetMaxVolume[i]), se_sound[i]);


			// 再生しているSEの音量調整必要個数がまだある
			if (count > 0)
			{
				count--;
			}
			// 再生しているSEの音量調整が終わった
			else break;
		}
	} /// void Process()



	/// --------------------------------------------------------------------------------------------------
	void DoSound(ESOUNDNAME_SE name, int volume)
	{
		// 流すSEの最大音量を決める
		se_volumeSetMaxVolume[static_cast<int>(name)] = volume;

		// SEが流れていなかったら
		if (!se_playFlag[static_cast<int>(name)])
		{
			// 音を流して音量を0にする
			PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK);
			ChangeVolumeSoundMem(0, se_sound[static_cast<int>(name)]);
		}
		// SEがすでに流れていたら
		else
		{
			// 音量を消して再生しなおす
			ChangeVolumeSoundMem(0, se_sound[static_cast<int>(name)]);
			PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK);
		}


		// SEを流しているとする
		se_playFlag[static_cast<int>(name)] = true;
	} /// void DoSound(ESOUNDNAME_SE name, int volume)



	/// --------------------------------------------------------------------------------------------------
	void DoSound(ESOUNDNAME_SE name, VECTOR area, int volume)
	{
		// SEの座標と大きさを設定する
		Set3DPositionSoundMem(VAdd(area, VScale(listenerArea, 0.1f)), se_sound[static_cast<int>(name)]);
		Set3DRadiusSoundMem(volume3DRadius * 2, se_sound[static_cast<int>(name)]);


		// SEの最大音量を決める
		se_volumeSetMaxVolume[static_cast<int>(name)] = volume;


		// SEが流れていなかったら
		if (!se_playFlag[static_cast<int>(name)])
		{
			// 音を流して音量を0にする
			PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK);
			ChangeVolumeSoundMem(0, se_sound[static_cast<int>(name)]);
		}
		// SEがすでに流れていたら
		else
		{
			// 音量を0にして再生する
			ChangeVolumeSoundMem(0, se_sound[static_cast<int>(name)]);
			PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK);
		}


		// SEを流しているとする
		se_playFlag[static_cast<int>(name)] = true;
	} /// void DoSound(ESOUNDNAME_SE name, VECTOR area, int volume)



	/// --------------------------------------------------------------------------------------------------
	void BGMEnd()
	{
		for (int i = 0; i != bgmArray; ++i)
		{
			if (bgm_name[i] == ESOUNDNAME_BGM::none) continue;
			SetBGMVolume(bgm_name[i], 0, 0);
		}
	}



	/// --------------------------------------------------------------------------------------------------
	void BGMTrans(ESOUNDNAME_BGM nextName, int volume)
	{
		// BGMが流れていなかったら
		if (!bgm_soundFlag) bgm_soundFlag = true;


		// 一つしかBGMが流れていなかったら
		if (bgm_name[1] == ESOUNDNAME_BGM::none)
		{
			// 0番目の音量を下げて、要求BGM1番目で流す
			SetBGMVolume(bgm_name[0], 0, 0);
			SetBGMVolume(nextName, volume, volume);
			return;
		}


		// 既に流しているBGMだったら
		if (bgm_name[1] == nextName)
		{
			// リクエスト音量を再設定する
			SetBGMVolume(bgm_name[0], 0, 0);
			SetBGMVolume(nextName, volume, volume);
			return;
		}
		if (bgm_name[0] == nextName)
		{
			// リクエスト音量を再設定する
			SetBGMVolume(bgm_name[1], 0, 0);
			SetBGMVolume(nextName, volume, volume);
			return;
		}


		// 0番目より1番目の方がリクエスト音量大きかったら
		if (bgm_requestVolume[0] < bgm_requestVolume[1])
		{
			// 0番目をリセットする
			ChangeVolumeSoundMem(0, bgm_sound[static_cast<int>(bgm_name[0])]);
			StopSoundMem(bgm_sound[static_cast<int>(bgm_name[0])]);
			bgm_nowVolume[0] = 0;
			bgm_nextVolume[0] = 0;
			bgm_requestVolume[0] = 0;
			bgm_name[0] = ESOUNDNAME_BGM::none;


			// 1番目の音量を下げて、要求BGMを0番目で流す
			SetBGMVolume(nextName, volume, volume);
			SetBGMVolume(bgm_name[1], 0, 0);
			return;
		}


		// 1番目より0番目の方がリクエスト音量大きかったら
		if (bgm_requestVolume[0] > bgm_requestVolume[1])
		{
			// 1番目をリセットする
			ChangeVolumeSoundMem(0, bgm_sound[static_cast<int>(bgm_name[1])]);
			StopSoundMem(bgm_sound[static_cast<int>(bgm_name[1])]);
			bgm_nowVolume[1] = 0;
			bgm_nextVolume[1] = 0;
			bgm_requestVolume[1] = 0;
			bgm_name[1] = ESOUNDNAME_BGM::none;


			// 0番目の音量を下げて、要求BGMを1番目で流す
			SetBGMVolume(bgm_name[0], 0, 0);
			SetBGMVolume(nextName, volume, volume);
			return;
		}
	} /// void BGMTrans(ESOUNDNAME_BGM nextName, int volume)




	/// --------------------------------------------------------------------------------------------------
	void BGMProcess()
	{
		BGMFeed();		// フェードを呼ぶ
		// 他にも何かするかもしれないので一応別処理として
	}


	/// --------------------------------------------------------------------------------------------------
	void BGMFeed()
	{
		// BGMが流れていなかったら流れているとする
		if (!bgm_soundFlag) bgm_soundFlag = true;


		for (int i = 0; i != bgmArray; ++i)
		{
			// 今の音量よりリクエスト音量の方が高かったら
			if (bgm_nowVolume[i] < bgm_nextVolume[i])
			{
				// ユーザーのBGM最大音量をかみして、音量を上げていく
				ChangeVolumeSoundMem(static_cast<int>((bgm_nowVolume[i] + static_cast<int>((sin(-M_PI / 2 + M_PI / bgmFeedValue * bgm_feedCount[i]) + 1) / 2 * (bgm_nextVolume[i] - bgm_nowVolume[i]))) * bgm_volumeAdjustment * optionNowVolume)
					, bgm_sound[static_cast<int>(bgm_name[i])]);
			}
			// 今の音量よりリクエスト音量が小さかったら
			else
			{
				// ユーザーのBGM最大音量をかみして、音量を下げていく
				ChangeVolumeSoundMem(static_cast<int>((bgm_nowVolume[i] - static_cast<int>((sin(-M_PI / 2 + M_PI / bgmFeedValue * bgm_feedCount[i]) + 1) / 2 * (bgm_nowVolume[i] - bgm_nextVolume[i]))) * bgm_volumeAdjustment * optionNowVolume)
					, bgm_sound[static_cast<int>(bgm_name[i])]);
			}


			// フェード値まで上げていく
			if (bgm_feedCount[i] <= bgmFeedValue)
			{
				bgm_feedCount[i]++;
			}
			// フェードが満たされたら(つまりリクエスト音量に達する
			else
			{
				bgm_nowVolume[i] = bgm_nextVolume[i];
			}
		} /// for (int i = 0; i != bgmArray; ++i)
	} /// void BGMFeed()



	/// --------------------------------------------------------------------------------------------------
	void SetBGMVolume(ESOUNDNAME_BGM name, int volume, int maxVolume)
	{
		// BGMが流れていなかったら
		if (!bgm_soundFlag) bgm_soundFlag = true;


		for (int i = 0; i != bgmArray; ++i)
		{
			// 今流しているBGMもしくはBGMを全く流していなかったら
			if (bgm_name[i] == ESOUNDNAME_BGM::none || bgm_name[i] == name)
			{
				// BGMを設定する
				bgm_name[i] = name;
				bgm_requestVolume[i] = volume;
				bgm_maxVolume[i] = maxVolume;


				// リクエスト音量に達していたら
				if (bgm_nextVolume[i] != bgm_requestVolume[i])
				{
					// 音量を取得して渡す
					bgm_nowVolume[i] = GetVolumeSoundMem2(bgm_sound[static_cast<int>(bgm_name[i])]);
					bgm_nextVolume[i] = bgm_requestVolume[i];
				}


				bgm_feedCount[i] = 0;		// 音量フェードを開始させる


				// 今の音量が止まっているか限りなく0に近かったら
				if (bgm_nowVolume[i] <= 2
					|| !(CheckSoundMem(bgm_sound[static_cast<int>(bgm_name[i])])))
				{
					// 止めて終了させる
					StopSoundMem(bgm_sound[static_cast<int>(bgm_name[i])]);
					PlaySoundMem(bgm_sound[static_cast<int>(bgm_name[i])], DX_PLAYTYPE_LOOP);
					ChangeVolumeSoundMem(0, bgm_sound[static_cast<int>(bgm_name[i])]);
					bgm_nowVolume[i] = 0;
				}
				return;
			} /// if (bgm_name[i] == ESOUNDNAME_BGM::none	|| bgm_name[i] == name)
		} /// for (int i = 0; i != bgmArray; ++i)
	} /// void SetBGMVolume(ESOUNDNAME_BGM name, int volume, int maxVolume)



	/// --------------------------------------------------------------------------------------------------
	void Release()
	{
		// SEを消す
		for (int i = 0; i != seNum; ++i)
		{
			if (!se_loadFlag[i]) continue;
			DeleteSoundMem(se_sound[i]);
			se_sound[i] = -1;
		}


		// BGMを消す
		for (int i = 0; i != bgmNum; ++i)
		{
			if (!bgm_loadFlag[i]) continue;
			StopSoundMem(bgm_sound[i]);
			DeleteSoundMem(bgm_sound[i]);
			bgm_sound[i] = -1;
		}
	} /// void Release()



	/// --------------------------------------------------------------------------------------------------
	void SetCharaArea(VECTOR area)
	{
		charaArea = area;
	}



	/// --------------------------------------------------------------------------------------------------
	void SetLisnerArea(VECTOR area)
	{
		listenerArea = area;
	}



	/// --------------------------------------------------------------------------------------------------
	void SetLisnerViewArea(VECTOR area)
	{
		listenerViewArea = area;
	}



	/// --------------------------------------------------------------------------------------------------
	void Set3DRadius(float radius)
	{
		volume3DRadius = radius;
	}



	/// --------------------------------------------------------------------------------------------------
	void SetSEVolumeEntire(float volumeEntire)
	{
		se_volumeAdjustment = volumeEntire;
	}



	/// --------------------------------------------------------------------------------------------------
	float GetSEVolumeEntire()
	{
		return se_volumeAdjustment;
	}



	/// --------------------------------------------------------------------------------------------------
	void SetBGMVolumeEntire(float volumeEntire)
	{
		bgm_volumeAdjustment = volumeEntire;
	}



	/// --------------------------------------------------------------------------------------------------
	float GetBGMVolumeEntire()
	{
		return bgm_volumeAdjustment;
	}



	/// --------------------------------------------------------------------------------------------------
	void SetOptionMenuNow(bool nowTrue)
	{
		nowTrue ? optionNowVolume = 0.5f : optionNowVolume = 1.0f;
	}
} /// namespace SoundProcess
