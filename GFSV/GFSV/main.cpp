#include "SoundProcess.hpp"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);

	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0)
	{
		// エラーが発生したら直ちに終了
		return -1;
	}

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	SoundProcess::Init();

	int bgm1 = LoadSoundMem("media\\変更後\\通常bgm.wav");
	SoundProcess::Load(bgm1, SoundProcess::ESOUNDNAME_BGM::normalBGM);

	int bgm2 = LoadSoundMem("media\\変更後\\戦闘BGM.wav");
	SoundProcess::Load(bgm2, SoundProcess::ESOUNDNAME_BGM::battleBGM);

	int bgm3 = LoadSoundMem("media\\変更後\\boss.wav");
	SoundProcess::Load(bgm3, SoundProcess::ESOUNDNAME_BGM::boss);

	int se1 = LoadSoundMem("media\\変更後\\piano攻撃音.wav");
	SoundProcess::Load(se1, SoundProcess::ESOUNDNAME_SE::pianoAttack, SoundProcess::ESOUNDTYPE::soundMem);
	int se2 = LoadSoundMem("media\\変更後\\piano攻撃音1.wav");
	SoundProcess::Load(se2, SoundProcess::ESOUNDNAME_SE::pianoAttack1, SoundProcess::ESOUNDTYPE::soundMem);
	int se3 = LoadSoundMem("media\\変更後\\piano攻撃音2.wav");
	SoundProcess::Load(se3, SoundProcess::ESOUNDNAME_SE::pianoAttack2, SoundProcess::ESOUNDTYPE::soundMem);

	// ＥＳＣキーが押されるかウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面をクリア
		ClearDrawScreen();

		SoundProcess::Process();

		if (CheckHitKey(KEY_INPUT_Z) == 1)
		{
			SoundProcess::SetBGMVolume(SoundProcess::ESOUNDNAME_BGM::normalBGM, 50, 50);
			//SoundProcess::BGMVolumeIn(SoundProcess::ESOUNDNAME::normalBGM);
		}

		if (CheckHitKey(KEY_INPUT_X) == 1)
		{
			SoundProcess::SetBGMVolume(SoundProcess::ESOUNDNAME_BGM::normalBGM, 125, 125);
			//SoundProcess::BGMVolumeEnd(SoundProcess::ESOUNDNAME::normalBGM);
		}

		if (CheckHitKey(KEY_INPUT_C) == 1)
		{
			SoundProcess::SetBGMVolume(SoundProcess::ESOUNDNAME_BGM::normalBGM, 230, 230);
			//SoundProcess::BGMVolumeTrans(SoundProcess::ESOUNDNAME::battleBGM);
		}

		if (CheckHitKey(KEY_INPUT_V) == 1)
		{
			SoundProcess::SetBGMVolume(SoundProcess::ESOUNDNAME_BGM::normalBGM, 0, 0);
			//SoundProcess::BGMVolumeTrans(SoundProcess::ESOUNDNAME::boss);
		}

		if (CheckHitKey(KEY_INPUT_Q) == 1)
		{
			SoundProcess::DoSound(SoundProcess::ESOUNDNAME_SE::pianoAttack);
			//SoundProcess::DoSound(SoundProcess::ESOUNDNAME::pianoAttack);
		}
		if (CheckHitKey(KEY_INPUT_W) == 1)
		{
			SoundProcess::DoSound(SoundProcess::ESOUNDNAME_SE::pianoAttack1);
			//SoundProcess::DoSound(SoundProcess::ESOUNDNAME::pianoAttack);
		}
		if (CheckHitKey(KEY_INPUT_E) == 1)
		{
			SoundProcess::DoSound(SoundProcess::ESOUNDNAME_SE::pianoAttack2);
			//SoundProcess::DoSound(SoundProcess::ESOUNDNAME::pianoAttack);
		}

		if (CheckHitKey(KEY_INPUT_S) == 1)
		{
			SoundProcess::BGMTrans(SoundProcess::ESOUNDNAME_BGM::battleBGM);
			//SoundProcess::DoSound(SoundProcess::ESOUNDNAME::pianoAttack1);
		}
		if (CheckHitKey(KEY_INPUT_D) == 1)
		{
			SoundProcess::BGMTrans(SoundProcess::ESOUNDNAME_BGM::normalBGM);
			//SoundProcess::DoSound(SoundProcess::ESOUNDNAME::pianoAttack2);
		}
		if (CheckHitKey(KEY_INPUT_F) == 1)
		{
			SoundProcess::BGMTrans(SoundProcess::ESOUNDNAME_BGM::boss);
			//SoundProcess::DoSound(SoundProcess::ESOUNDNAME::pianoAttack2);
		}
		if (CheckHitKey(KEY_INPUT_G) == 1)
		{
			SoundProcess::BGMTrans(SoundProcess::ESOUNDNAME_BGM::normalBGM);
			//SoundProcess::DoSound(SoundProcess::ESOUNDNAME::pianoAttack2);
		}
		if (CheckHitKey(KEY_INPUT_O) == 1)
		{
			SoundProcess::BGMEnd();
			//SoundProcess::DoSound(SoundProcess::ESOUNDNAME::pianoAttack2);
		}
		if (CheckHitKey(KEY_INPUT_T) == 1)
		{
			SoundProcess::SetBGMVolumeEntire(0.5);
		}


		printfDx("%d\t%d\t%d\t%d\t%d\t%d\n", GetVolumeSoundMem2(bgm1), GetVolumeSoundMem2(bgm2), GetVolumeSoundMem2(bgm3), GetVolumeSoundMem2(se1), GetVolumeSoundMem2(se2), GetVolumeSoundMem2(se3));

		//SoundProcess::Test();

		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	SoundProcess::Release();

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}