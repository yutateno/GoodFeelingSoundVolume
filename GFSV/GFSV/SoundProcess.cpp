#include "SoundProcess.hpp"

namespace SoundProcess
{
	/// ID�̂�����[�h�������ǂ���
	bool loadFlag[33];
	/// �Đ����Ă��邩�ǂ���
	bool playFlag[33];
	/// ���[�h�����T�E���h
	int soundLoad[33];
	/// ���[�h�����T�E���h�̃^�C�v
	ESOUNDTYPE soundType[33];

	/// �v���C���[�̍��W
	VECTOR charaArea;
	/// ���X�i�[�̍��W
	VECTOR lisnerArea;

	std::vector<ESOUNDNAME> seVector;

	/// BGM�Ɋւ���----------------------
	/// BGM�{�����[���̃t�F�[�h���l
	int bgmVolumeCount = 0;

	/// �{�����[���グ��
	bool volumeUpFlag = false;

	/// �{�����[��������
	bool volumeDownFlag = false;

	/// BGM��ς���
	bool bgmTransFlag = false;

	/// BGM�̖��O
	ESOUNDNAME bgmName;

	/// BGM�؂�ւ����̖��O
	ESOUNDNAME nextBGMName = bgmName;

	/// BGM�r���؂�ւ����̖��O
	ESOUNDNAME tempBGMName = bgmName;

	/// BGM�{�����[��
	int bgmVolume = 0;

	/// ���O��BGM�{�����[��
	int preBGMVolume = bgmVolume;

	/// ����BGM�{�����[��
	int nextBGMVolume = bgmVolume;
	/// ---------------------------------

	void VolumeInProcess();
	void VolumeResetProcess();
	void BGMVolumeTransProcess();


	/// ������
	void Init()
	{
		ZeroMemory(loadFlag, sizeof(loadFlag));
		ZeroMemory(playFlag, sizeof(playFlag));
		volumeUpFlag = false;
		volumeDownFlag = false;
		bgmTransFlag = false;
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


	/// �ǂݍ���
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


	/// �ǂݍ���
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


	/// �v���Z�X
	void Process()
	{
		/// �Đ���
		int count = 0;

		// �Đ����Ă��邩�ǂ������f
		for (int i = 0; i != 33; ++i)
		{
			// ��������Ă��Ȃ�������
			if (!CheckSoundMem(soundLoad[i]))
			{
				playFlag[i] = false;
				continue;
			}

			// 3D������������
			if (soundType[i] == ESOUNDTYPE::sound3DSourceChara)
			{
				Set3DPositionSoundMem(charaArea, soundLoad[i]);
				continue;
			}

			// �Đ����Ă�����
			if (playFlag[i])
			{
				count++;		// �Đ��������Z
			}
		}

		// ��ȉ��Ȃ���ɉ������Ȃ�
		//if (count <= 1)return;

		// �ŏ����{�����[��0�ɂȂ�Ȃ��悤��
		count--;
		//printfDx("%d\n", count);
		/// ������l�����炵�Ȃ���Đ����ʒ���
		for (int i = 32; i >= 0; --i)
		{
			// �Đ����Ă��Ȃ�������
			if (!playFlag[i]) continue;

			if (i >= 22)
			{
				nextBGMVolume = 255 - (6 * count);
				continue;
			}

			// ���ʂ�������
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

		//printfDx("%d\tDown: %d\tUp: %d\tTrans: %d\n", preBGMVolume, volumeDownFlag, volumeUpFlag, bgmTransFlag);
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


	/// �Đ�������
	void DoSound(ESOUNDNAME name)
	{
		if (playFlag[static_cast<int>(name)]) return;
		PlaySoundMem(soundLoad[static_cast<int>(name)], DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(name)]);

		playFlag[static_cast<int>(name)] = true;
	}


	/// �������Ă�������
	void VolumeInProcess()
	{
		ChangeVolumeSoundMem(static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * bgmVolume), soundLoad[static_cast<int>(bgmName)]);
		if (bgmVolumeCount <= 120)
		{
			bgmVolumeCount++;
		}
		
		if (bgmVolumeCount > 120 && volumeUpFlag)
		{
			preBGMVolume = GetVolumeSoundMem2(soundLoad[static_cast<int>(bgmName)]);
			volumeUpFlag = false;
		}
	}


	/// �������Ă���
	void BGMVolumeIn(ESOUNDNAME name, int volume)
	{
		PlaySoundMem(soundLoad[static_cast<int>(name)], DX_PLAYTYPE_LOOP);
		ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(name)]);
		playFlag[static_cast<int>(name)] = true;

		volumeUpFlag = true;
		bgmName = name;
		nextBGMName = bgmName;
		bgmVolume = volume;
		bgmVolumeCount = 0;
	}


	/// BGM��؂�ւ��鏈��
	void BGMVolumeTransProcess()
	{
		ChangeVolumeSoundMem(static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * bgmVolume), soundLoad[static_cast<int>(bgmName)]);

		ChangeVolumeSoundMem(static_cast<int>(nextBGMVolume - (sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * nextBGMVolume), soundLoad[static_cast<int>(nextBGMName)]);
		if (bgmVolumeCount >= 0)
		{
			bgmVolumeCount--;
		}
		
		if (bgmVolumeCount < 0 && bgmTransFlag)
		{
			bgmName = nextBGMName;
			//preBGMVolume = GetVolumeSoundMem2(soundLoad[static_cast<int>(bgmName)]);
			bgmVolume = GetVolumeSoundMem2(soundLoad[static_cast<int>(bgmName)]);
			bgmTransFlag = false;
		}
	}
	
	/// BGM��؂�ւ���
	void BGMVolumeTrans(ESOUNDNAME nextName, int volume)
	{
		PlaySoundMem(soundLoad[static_cast<int>(nextName)], DX_PLAYTYPE_LOOP);
		ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(nextName)]);

		// BGM�؂�ւ������łɍs���Ă��ĕύX���BGM���قȂ�ꍇ
		if (bgmTransFlag && nextName != tempBGMName)
		{
			if (bgmName != nextName)
			{
				StopSoundMem(soundLoad[static_cast<int>(bgmName)]);
				ChangeVolumeSoundMem(0, soundLoad[static_cast<int>(bgmName)]);
			}
			bgmName = nextBGMName;
		}

		bgmVolumeCount = 120;
		bgmVolume = GetVolumeSoundMem2(soundLoad[static_cast<int>(bgmName)]);
		preBGMVolume = bgmVolume;
		nextBGMName = nextName;
		tempBGMName = nextBGMName;
		nextBGMVolume = volume;
		bgmTransFlag = true;
	}


	/// ������������
	void VolumeResetProcess()
	{
		ChangeVolumeSoundMem(static_cast<int>((sin(-M_PI / 2 + M_PI / 120 * bgmVolumeCount) + 1) / 2 * bgmVolume), soundLoad[static_cast<int>(bgmName)]);
		if (bgmVolumeCount >= 0)
		{
			bgmVolumeCount--;
		}
		
		if (bgmVolumeCount < 0 && volumeDownFlag)
		{
			volumeDownFlag = false;
		}
	}


	/// ��������
	void BGMVolumeEnd(ESOUNDNAME name)
	{
		volumeUpFlag = false;
		bgmVolume = GetVolumeSoundMem2(soundLoad[static_cast<int>(name)]);
		volumeDownFlag = true;
		bgmVolumeCount = 120;
		bgmName = name;
	}


	/// ���
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


	/// �L�����N�^�[�̃G���A
	void SetCharaArea(VECTOR area)
	{
		charaArea = area;
	}


	/// ���X�i�[�̃G���A
	void SetLisnerArea(VECTOR area)
	{
		lisnerArea = area;
	}
	void Test()
	{
		//printfDx("loadF: %d\tfile: %d\tPlayF: %d\n", loadFlag[static_cast<int>(ESOUNDNAME::titleMusicBox)], soundLoad[static_cast<int>(ESOUNDNAME::titleMusicBox)], playFlag[static_cast<int>(ESOUNDNAME::titleMusicBox)]);
	}
}
