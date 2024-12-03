#pragma once

#include "CoreMinimal.h"

class FIniObject;

struct FMusicToPlay
{
	FMusicToPlay(std::string InMusicPath = "", const bool bLoadNow = false);
	virtual ~FMusicToPlay();

	void Play();
	void Load();
	void UnLoad();

private:
	std::string MusicPath;

	Mix_Music* MusicObject;

	bool bIsLoaded;

};

/**
 * Audio player entity
 */
class FAudioPlayer : public EEntity
{
public:
	FAudioPlayer(FEntityManager* InEntityManager);

	/** Begin EEntity */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	/** End EEntity */

	void SetPlayAudioAutomatic(const bool bInPlayAutomatically);
	void SetMasterVolume(const int32 IntValue);
	void SetBackgroundMusicVolume(const int32 IntValue);
	void SetNumberOfSupportedAudioChannels(const int32 NewNumberOfChannels);

	CArray<FMusicToPlay>& GetMusicToPlayArray() { return BackgroundMusicToPlayArray; };

protected:
	void ReadIniFiles();

	/** Override in case of adding additional audio settings */
	virtual void GetAudioSettings(const std::shared_ptr<FIniObject>& InAudioSettingsConfigObject);

	/** Override to change how music is collected into an array. By default, it will search for ini called 'AudioList.ini'. */
	virtual void CollectMusic(const std::shared_ptr<FIniObject>& InAudioListConfigObject);

	/** Array with music ready to play */
	CArray<FMusicToPlay> BackgroundMusicToPlayArray;

private:
	/** Engine asset manager (for converting paths) */
	FAssetsManager* AssetManager;

	/** Should play songs automatically. If not  */
	bool bPlayAudioAutomatically;

};
