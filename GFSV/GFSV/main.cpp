#include "SoundProcess.hpp"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);

	// �c�w���C�u�����̏�����
	if (DxLib_Init() < 0)
	{
		// �G���[�����������璼���ɏI��
		return -1;
	}

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	SoundProcess::Init();

	int bgm1 = LoadSoundMem("media\\�ύX��\\�ʏ�bgm.wav");
	SoundProcess::Load(bgm1, SoundProcess::ESOUNDNAME::normalBGM, SoundProcess::ESOUNDTYPE::soundStreem);

	int bgm2 = LoadSoundMem("media\\�ύX��\\�퓬BGM.wav");
	SoundProcess::Load(bgm2, SoundProcess::ESOUNDNAME::battleBGM, SoundProcess::ESOUNDTYPE::soundStreem);

	int bgm3 = LoadSoundMem("media\\�ύX��\\boss.wav");
	SoundProcess::Load(bgm3, SoundProcess::ESOUNDNAME::boss, SoundProcess::ESOUNDTYPE::soundStreem);

	int se1 = LoadSoundMem("media\\�ύX��\\piano�U����.wav");
	SoundProcess::Load(se1, SoundProcess::ESOUNDNAME::pianoAttack, SoundProcess::ESOUNDTYPE::soundMem);
	int se2 = LoadSoundMem("media\\�ύX��\\piano�U����1.wav");
	SoundProcess::Load(se2, SoundProcess::ESOUNDNAME::pianoAttack1, SoundProcess::ESOUNDTYPE::soundMem);
	int se3 = LoadSoundMem("media\\�ύX��\\piano�U����2.wav");
	SoundProcess::Load(se3, SoundProcess::ESOUNDNAME::pianoAttack2, SoundProcess::ESOUNDTYPE::soundMem);

	// �d�r�b�L�[��������邩�E�C���h�E��������܂Ń��[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ��ʂ��N���A
		ClearDrawScreen();

		SoundProcess::Process();

		if (CheckHitKey(KEY_INPUT_Z) == 1)
		{
			SoundProcess::BGMVolumeIn(SoundProcess::ESOUNDNAME::normalBGM);
		}

		if (CheckHitKey(KEY_INPUT_X) == 1)
		{
			SoundProcess::BGMVolumeEnd(SoundProcess::ESOUNDNAME::normalBGM);
		}

		if (CheckHitKey(KEY_INPUT_C) == 1)
		{
			SoundProcess::BGMVolumeTrans(SoundProcess::ESOUNDNAME::battleBGM);
		}

		if (CheckHitKey(KEY_INPUT_V) == 1)
		{
			SoundProcess::BGMVolumeTrans(SoundProcess::ESOUNDNAME::boss);
		}

		if (CheckHitKey(KEY_INPUT_A) == 1)
		{
			SoundProcess::DoSound(SoundProcess::ESOUNDNAME::pianoAttack);
		}
		if (CheckHitKey(KEY_INPUT_S) == 1)
		{
			SoundProcess::DoSound(SoundProcess::ESOUNDNAME::pianoAttack1);
		}
		if (CheckHitKey(KEY_INPUT_D) == 1)
		{
			SoundProcess::DoSound(SoundProcess::ESOUNDNAME::pianoAttack2);
		}

		printfDx("%d\t%d\t%d\n", GetVolumeSoundMem2(bgm1), GetVolumeSoundMem2(bgm2), GetVolumeSoundMem2(bgm3));

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}

	SoundProcess::Release();

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}