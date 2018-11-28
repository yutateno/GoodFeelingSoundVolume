#include "SoundProcess.hpp"

namespace SoundProcess
{
	// SE�Ɋւ���
	/// �ۑ�����
	int se_sound[22];
	/// ���[�h�������ǂ���
	bool se_loadFlag[22];
	/// �Đ����Ă��邩�ǂ���
	bool se_playFlag[22];

	// BGM�Ɋւ���
	/// �ۑ�����
	int bgm_sound[11];
	/// ���[�h�������ǂ���
	bool bgm_loadFlag[11];
	/// �{�����[��
	int bgm_nowVolume[2];
	/// BGM�̖��O
	ESOUNDNAME_BGM bgm_name[2];
	/// �v�����ꂽBGM�̃{�����[��
	int bgm_requestVolume[2];
	/// �ύX���̖ړI�{�����[��
	int bgm_nextVolume[2];
	/// �t�F�[�h�J�E���g
	int bgm_feedCount[2];
	/// BGM������Ă��邩�ǂ���
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
		/// 3D�֘A
	}

	void Process()
	{
		/// �Đ���
		int count = 0;

		// �Đ����Ă��邩�ǂ������f
		for (int i = 0; i != 22; ++i)
		{
			// ��������Ă��Ȃ�������
			if (!CheckSoundMem(se_sound[i]))
			{
				se_playFlag[i] = false;
				continue;
			}

			// �Đ����Ă�����
			if (se_playFlag[i])
			{
				count++;		// �Đ��������Z
			}
		}

		if (bgm_soundFlag) count++;


		/// ������l�����炵�Ȃ���Đ����ʒ���
		for (int i = 21; i >= 0; --i)
		{
			// �Đ����Ă��Ȃ�������
			if (!se_playFlag[i]) continue;

			if (bgm_soundFlag)
			{
				bgm_requestVolume[0] = 255 - (10 * count);
				continue;
			}

			// ���ʂ�������
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


	///// SE�Ɋւ���----------------------
	///// ID�̂�����[�h�������ǂ���
	//bool loadFlag[22];
	///// �Đ����Ă��邩�ǂ���
	//bool playFlag[22];
	///// ���[�h�����T�E���h
	//int soundLoad[22];
	///// ���[�h�����T�E���h�̃^�C�v
	//ESOUNDTYPE soundType[22];

	///// �v���C���[�̍��W
	//VECTOR charaArea;
	///// ���X�i�[�̍��W
	//VECTOR lisnerArea;

	///// BGM�Ɋւ���----------------------
	///// BGM�{�����[���̃t�F�[�h���l
	//int bgmVolumeCount = 0;

	///// �{�����[���グ��
	//bool volumeUpFlag = false;

	///// �{�����[��������
	//bool volumeDownFlag = false;

	///// BGM��ς���
	//bool bgmTransFlag = false;

	///// BGM�̖��O
	//ESOUNDNAME bgmName;

	///// BGM�؂�ւ����̖��O
	//ESOUNDNAME nextBGMName = bgmName;

	///// BGM�r���؂�ւ����̖��O
	//ESOUNDNAME tempBGMName = bgmName;

	///// BGM�{�����[��
	//int bgmVolume = 0;

	///// ���O��BGM�{�����[��
	//int preBGMVolume = bgmVolume;

	///// ����BGM�{�����[��
	//int nextBGMVolume = bgmVolume;
	///// ---------------------------------

	//void VolumeInProcess();
	//void VolumeResetProcess();
	//void BGMVolumeTransProcess();


	///// ������
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


	///// �ǂݍ���
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


	///// �ǂݍ���
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


	///// �v���Z�X
	//void Process()
	//{
	//	/// �Đ���
	//	int count = 0;

	//	// �Đ����Ă��邩�ǂ������f
	//	for (int i = 0; i != 33; ++i)
	//	{
	//		// ��������Ă��Ȃ�������
	//		if (!CheckSoundMem(soundLoad[i]))
	//		{
	//			playFlag[i] = false;
	//			continue;
	//		}

	//		// 3D������������
	//		if (soundType[i] == ESOUNDTYPE::sound3DSourceChara)
	//		{
	//			Set3DPositionSoundMem(charaArea, soundLoad[i]);
	//			continue;
	//		}

	//		// �Đ����Ă�����
	//		if (playFlag[i])
	//		{
	//			count++;		// �Đ��������Z
	//		}
	//	}

	//	// ��ȉ��Ȃ���ɉ������Ȃ�
	//	//if (count <= 1)return;

	//	// �ŏ����{�����[��0�ɂȂ�Ȃ��悤��
	//	count--;
	//	//printfDx("%d\n", count);
	//	/// ������l�����炵�Ȃ���Đ����ʒ���
	//	for (int i = 32; i >= 0; --i)
	//	{
	//		// �Đ����Ă��Ȃ�������
	//		if (!playFlag[i]) continue;

	//		if (i >= 22)
	//		{
	//			nextBGMVolume = 255 - (6 * count);
	//			continue;
	//		}

	//		// ���ʂ�������
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


	///// �Đ�������
	//void DoSound(ESOUNDNAME name)
	//{
	//	if (playFlag[static_cast<int>(name)]) return;
	//	PlaySoundMem(soundLoad[static_cast<int>(name)], DX_PLAYTYPE_BACK);
	//	ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(name)]);

	//	playFlag[static_cast<int>(name)] = true;
	//}


	///// �������Ă�������
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


	///// �������Ă���
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


	///// BGM��؂�ւ��鏈��
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
	///// BGM��؂�ւ���
	//void BGMVolumeTrans(ESOUNDNAME nextName, int volume)
	//{
	//	PlaySoundMem(soundLoad[static_cast<int>(nextName)], DX_PLAYTYPE_LOOP);
	//	ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(nextName)]);

	//	// BGM�؂�ւ������łɍs���Ă��ĕύX���BGM���قȂ�ꍇ
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


	///// ������������
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


	///// ��������
	//void BGMVolumeEnd(ESOUNDNAME name)
	//{
	//	volumeUpFlag = false;
	//	bgmVolume = GetVolumeSoundMem2(soundLoad[static_cast<int>(name)]);
	//	volumeDownFlag = true;
	//	bgmVolumeCount = 120;
	//	bgmName = name;
	//}


	///// ���
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


	///// �L�����N�^�[�̃G���A
	//void SetCharaArea(VECTOR area)
	//{
	//	charaArea = area;
	//}



///// ���X�i�[�̃G���A
	//void SetLisnerArea(VECTOR area)
	//{
	//	lisnerArea = area;
	//}
	//void Test()
	//{
	//	//printfDx("loadF: %d\tfile: %d\tPlayF: %d\n", loadFlag[static_cast<int>(ESOUNDNAME::titleMusicBox)], soundLoad[static_cast<int>(ESOUNDNAME::titleMusicBox)], playFlag[static_cast<int>(ESOUNDNAME::titleMusicBox)]);
	//}
}
