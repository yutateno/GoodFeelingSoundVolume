#pragma once
#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <cmath>

#include <vector>

namespace SoundProcess
{
	/// �T�E���h�̖��OID
	enum class ESOUNDNAME
	{
		saveOn, saveMiss, pianoAttack3, pianoAttack2, pianoAttack1, pianoAttack
		, jump, pianoAttackMiss, landing, landing2, footFloor, foot
		, ballPickUp, strikeBomb, ballPawnHigh, ballPawn, lightOneFoot, lightFoot
		, crayonDie, titleBurnLow, titleBurnLowMagni, titleBurn, titleMusicBox, normalBGM
		, battleBGM, lightBGM, noise3, noise2, noise, noise_2
		, noise_1, ending, boss
	};

	/// �T�E���h�^�C�v
	enum class ESOUNDTYPE
	{
		soundMem, soundStreem, sound3DSourceChara, sound3DSourcePartner, none
	};


	/// ������
	void Init();

	/// ���[�h
	void Load(int loadFile, ESOUNDNAME name, ESOUNDTYPE type);

	/// 3D�T�E���h�̃��[�h
	void Load(int loadFile, ESOUNDNAME name, ESOUNDTYPE type, VECTOR partnerArea);

	/// �Đ�����
	void DoSound(ESOUNDNAME name);

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



	void Test();
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