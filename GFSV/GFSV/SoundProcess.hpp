#pragma once
#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <cmath>

#include <vector>

namespace SoundProcess
{
	/// �T�E���h�̖��OID_SE
	enum class ESOUNDNAME_SE
	{
		saveOn, saveMiss, pianoAttack3, pianoAttack2, pianoAttack1, pianoAttack
		, jump, pianoAttackMiss, landing, landing2, footFloor, foot
		, ballPickUp, strikeBomb, ballPawnHigh, ballPawn, lightOneFoot, lightFoot
		, crayonDie, titleBurnLow, titleBurnLowMagni, titleBurn
	};

	/// �T�E���h�̖��OID_BGM
	enum class ESOUNDNAME_BGM
	{
		titleMusicBox, normalBGM, battleBGM, lightBGM, noise3, noise2
		, noise, noise_2, noise_1, ending, boss, none
	};

	/// �T�E���h�^�C�v
	enum class ESOUNDTYPE
	{
		soundMem, soundStreem, sound3DSourceChara, sound3DSourcePartner, none
	};


	/// ������
	void Init();

	/// ���[�h
	void Load(int loadFile, ESOUNDNAME_SE name, ESOUNDTYPE type);
	void Load(int loadFile, ESOUNDNAME_BGM name);
	void Load(int loadFile, ESOUNDNAME_SE name, ESOUNDTYPE type, VECTOR partnerArea);

	/// �v���Z�X
	void Process();

	/// SE�̍Đ�
	void DoSound(ESOUNDNAME_SE name, int volume = 255);

	/// BGM�̏I���
	void BGMEnd();

	/// BGM�؂�ւ�
	void BGMTrans(ESOUNDNAME_BGM nextName, int volume = 255);

	/// BGM�v���Z�X
	void BGMProcess();

	/// BGM�t�F�[�h
	void BGMFeed();

	/// BGM���ʐݒ�
	void SetBGMVolume(ESOUNDNAME_BGM name, int volume);

	/// ���
	void Release();


	void Test();

	/*/// ������
	void Init();

	/// ���[�h
	void Load(int loadFile, ESOUNDNAME_SE name, ESOUNDTYPE type);

	/// ���[�h
	void Load(int loadFile, ESOUNDNAME_BGM name, ESOUNDTYPE type);

	/// 3D�T�E���h�̃��[�h
	void Load(int loadFile, ESOUNDNAME_SE name, ESOUNDTYPE type, VECTOR partnerArea);

	/// �Đ�����
	void DoSound(ESOUNDNAME_SE name);

	/// �������Ă���
	void BGMVolumeIn(ESOUNDNAME name, int volume = 255);

	/// ����ς���
	void BGMVolumeTrans(ESOUNDNAME nextName, int volume = 255);

	/// ��������
	void BGMVolumeEnd(ESOUNDNAME name);

	/// �������
	void Release();

	/// �v���Z�X
	void Process();

	/// �X�e�[�W1����T�E���h
	void StageOneSound(int touchNum);
	void StageOneBallCatch();

	/// �v���C���[�̍��W��ݒ�
	void SetCharaArea(VECTOR area);

	/// ���X�i�[�̍��W��ݒ�
	void SetLisnerArea(VECTOR area);



	void Test();*/
}


/*
���C�������Ďv������

/// ����BGM���玟��BGM�ɑJ�ڂ���
void Volume�J��(ESOUNDNAME nowName, int nowVolume, ESOUNDNAME nextName, int nextVolume);

/// 3D�����̓{�����[���̒����͂��Ȃ�

/// SE��BGM�ŗD��x���特�ʂ𒲐�����
/// �Ԃ����ႯSE�͂��������邩�特�ʂ��t�F�[�h������悤�؂�ւ��Ȃ��Ă��ǂ��C������
/// �ł���ꂽ��SE�p�̃t�F�[�h�J�ڍ��

/// ����BGM�̉��ʂ����̉��ʂɕς���
void VolumeFeed(ESOUNDNAME name, int nowVolume, int nextVolume);
*/