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
	SoundProcess::Load(bgm1, SoundProcess::ESOUNDNAME::battleBGM, SoundProcess::ESOUNDTYPE::soundStreem);

	int se1 = LoadSoundMem("media\\�ύX��\\piano�U����.wav");
	SoundProcess::Load(se1, SoundProcess::ESOUNDNAME::pianoAttack, SoundProcess::ESOUNDTYPE::soundMem);

	SoundProcess::DoSound(SoundProcess::ESOUNDNAME::normalBGM);

	// �d�r�b�L�[��������邩�E�C���h�E��������܂Ń��[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ��ʂ��N���A
		ClearDrawScreen();

		SoundProcess::Process();

		if (CheckHitKey(KEY_INPUT_Z) == 1)
		{
			SoundProcess::VolumeIn(SoundProcess::ESOUNDNAME::normalBGM);
		}

		if (CheckHitKey(KEY_INPUT_X) == 1)
		{
			SoundProcess::VolumeReset(SoundProcess::ESOUNDNAME::normalBGM);
		}

		if (CheckHitKey(KEY_INPUT_A) == 1)
		{
			SoundProcess::DoSound(SoundProcess::ESOUNDNAME::pianoAttack);
		}

		printfDx("%d\n", GetVolumeSoundMem2(bgm1));

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}

	SoundProcess::Release();

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}