#include "Audio.h"

//============================================================================*/
//	include
//============================================================================*/

#pragma comment(lib,"xaudio2.lib")

//============================================================================*/
//	Audio classMethods
//============================================================================*/

Audio* Audio::instance_ = nullptr;

Audio* Audio::GetInstance() {

	if (instance_ == nullptr) {
		instance_ = new Audio();
	}
	return instance_;
}

void Audio::LoadWave(const std::string& filename) {

	// 識別子を除いた名前を取得
	std::string identifier = GetFileNameWithoutExtension(filename);
	SoundData soundData = LoadWaveInternal(filename);

	// サウンドデータ追加
	soundData_.emplace(identifier, soundData);
}
Audio::SoundData Audio::LoadWaveInternal(const std::string& filename) {

	std::ifstream file(filename, std::ios_base::binary);
	assert(file.is_open());

	RiffHeader riff;
	file.read(reinterpret_cast<char*>(&riff), sizeof(riff));
	assert(strncmp(riff.chunk.id, "RIFF", 4) == 0);
	assert(strncmp(riff.type, "WAVE", 4) == 0);

	FormatChunk format{};
	bool formatFound = false;
	bool dataFound = false;
	ChunkHeader chunkHeader;

	while (!dataFound && file.read(reinterpret_cast<char*>(&chunkHeader), sizeof(chunkHeader))) {
		if (strncmp(chunkHeader.id, "fmt ", 4) == 0) {

			file.read(reinterpret_cast<char*>(&format.fmt), chunkHeader.size);
			formatFound = true;
		} else if (strncmp(chunkHeader.id, "data", 4) == 0) {

			BYTE* pBuffer = new BYTE[chunkHeader.size];
			file.read(reinterpret_cast<char*>(pBuffer), chunkHeader.size);

			SoundData soundData{};
			soundData.wfex = format.fmt;
			soundData.pBuffer = pBuffer;
			soundData.bufferSize = chunkHeader.size;

			dataFound = true;
			file.close();
			return soundData;
		} else {

			// 読み込んだチャンクをスキップ
			file.seekg(chunkHeader.size, std::ios_base::cur);
		}
	}

	// ファイルを閉じる
	file.close();
	// 必要なチャンクが見つからなかった場合はエラー
	assert(formatFound && dataFound);

	// エラー時にはデフォルトのSoundDataを返す とりあえずreturnしとく
	return SoundData{};
}

std::string Audio::GetFileNameWithoutExtension(const std::string& filename) {

	std::filesystem::path path(filename);

	return path.stem().string();
}

void Audio::Unload() {

	for (auto& [name, soundData] : soundData_) {
		if (soundData.pBuffer) {

			delete[] soundData.pBuffer;
			soundData.pBuffer = nullptr;
		}
	}
	soundData_.clear();

}

void Audio::PlayWave(const std::string& name, bool loop) {

	auto it = soundData_.find(name);
	if (it == soundData_.end()) return;

	// 再生中の音声がある場合、何もしない（ループ中の音声を再生し直さない）
	auto voiceIt = activeVoices_.find(name);
	if (voiceIt != activeVoices_.end()) {
		XAUDIO2_VOICE_STATE state;
		voiceIt->second->GetState(&state);

		// 再生中かつバッファが残っている場合は何もしない
		if (state.BuffersQueued > 0) {
			return;
		}

		// 再生終了済みの場合はリストから削除
		voiceIt->second->DestroyVoice();
		activeVoices_.erase(voiceIt);
	}

	const SoundData& soundData = it->second;

	// 新しい SourceVoice を作成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	HRESULT hr = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(hr));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	if (loop) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE; // 無限ループ設定
	} else {
		buf.LoopCount = 0;
	}

	hr = pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(hr));

	hr = pSourceVoice->Start();
	assert(SUCCEEDED(hr));

	// 再生中の SourceVoice を登録
	activeVoices_[name] = pSourceVoice;
}

void Audio::StopWave(const std::string& name) {

	auto it = activeVoices_.find(name);
	if (it != activeVoices_.end()) {
		IXAudio2SourceVoice* pSourceVoice = it->second;
		if (pSourceVoice) {

			pSourceVoice->Stop();
			pSourceVoice->DestroyVoice();
			pSourceVoice = nullptr;
		}
		activeVoices_.erase(it);
	}
}

void Audio::PlaySE(const std::string& name) {

	auto it = soundData_.find(name);
	if (it == soundData_.end()) return;

	// 再生中の音声があるか確認
	auto voiceIt = activeVoices_.find(name);
	if (voiceIt != activeVoices_.end()) {
		// 音声の再生状態を確認
		XAUDIO2_VOICE_STATE state;
		voiceIt->second->GetState(&state);

		// 再生中であれば一旦停止し、リストから削除
		voiceIt->second->Stop();
		voiceIt->second->DestroyVoice();
		activeVoices_.erase(voiceIt);
	}

	const SoundData& soundData = it->second;

	// 新しい SourceVoice を作成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	HRESULT hr = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(hr));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	buf.LoopCount = 0;

	hr = pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(hr));

	hr = pSourceVoice->Start();
	assert(SUCCEEDED(hr));

	// 再生中の SourceVoice を登録
	activeVoices_[name] = pSourceVoice;
}

void Audio::PauseWave(const std::string& name) {

	std::string identifier = GetFileNameWithoutExtension(name);

	auto it = activeVoices_.find(identifier);
	if (it != activeVoices_.end()) {

		it->second->Stop(0);
	}
}

void Audio::ResumeWave(const std::string& name) {

	std::string identifier = GetFileNameWithoutExtension(name);

	auto it = activeVoices_.find(identifier);
	if (it != activeVoices_.end()) {

		it->second->Start(0);
	}
}

void Audio::SetVolume(const std::string& name, float volume) {

	std::string identifier = GetFileNameWithoutExtension(name);

	auto it = activeVoices_.find(identifier);
	if (it != activeVoices_.end()) {

		it->second->SetVolume(volume);
	}
}

bool Audio::IsPlayWave(const std::string& name) {

	auto it = activeVoices_.find(name);
	if (it != activeVoices_.end()) {

		XAUDIO2_VOICE_STATE state;
		it->second->GetState(&state);
		return (state.BuffersQueued > 0);
	}
	return false;
}

void Audio::Init() {

	HRESULT hr;

	// XAudioエンジンのインスタンスを生成
	hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイスを生成
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
}

void Audio::Finalize() {

	for (auto& [name, pSourceVoice] : activeVoices_) {
		if (pSourceVoice) {

			pSourceVoice->Stop();
			pSourceVoice->DestroyVoice();
			pSourceVoice = nullptr;
		}
	}
	activeVoices_.clear();

	if (masterVoice_) {
		masterVoice_->DestroyVoice();
		masterVoice_ = nullptr;
	}

	Unload();

	if (instance_ != nullptr) {

		delete instance_;
		instance_ = nullptr;
	}

}