#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// directX
#include <xaudio2.h>

// c++
#include <fstream>
#include <cassert>
#include <unordered_map>
#include <string>
#include <filesystem>

//============================================================================*/
//	Audio class
//============================================================================*/
class Audio {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Audio() = default;
	~Audio() = default;

	static Audio* GetInstance();

	void LoadWave(const std::string& filename);

	void PlayWave(const std::string& name, bool loop = false);
	void StopWave(const std::string& name);

	void PlaySE(const std::string& name);

	void PauseWave(const std::string& name);
	void ResumeWave(const std::string& name);

	void SetVolume(const std::string& name, float volume);

	bool IsPlayWave(const std::string& name);

	void Init();

	void Unload();
	void Finalize();

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* structures

	// チャンク
	struct ChunkHeader {

		char id[4];
		int32_t size;
	};

	// RIFFチャンク
	struct RiffHeader {
		ChunkHeader chunk;
		char type[4];
	};

	// FMTチャンク
	struct FormatChunk {

		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

	// 音声データ
	struct SoundData {

		WAVEFORMATEX wfex;
		BYTE* pBuffer;
		uint32_t bufferSize;
	};

	//========================================================================*/
	//* variables

	static Audio* instance_;

	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_{};

	std::unordered_map<std::string, SoundData> soundData_;
	std::unordered_map<std::string, IXAudio2SourceVoice*> activeVoices_;

	//========================================================================*/
	//* functions

	SoundData LoadWaveInternal(const std::string& filename);
	std::string GetFileNameWithoutExtension(const std::string& filename);

};