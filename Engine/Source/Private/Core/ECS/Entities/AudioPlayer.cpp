#include "CoreEngine.h"
#include "Core/ECS/Entities/AudioPlayer.h"

#include "Assets/IniReader/IniManager.h"
#include "Assets/IniReader/IniObject.h"

static const std::string DefaultAudioListConfigFile = "AudioList";
static const std::string DefaultAudioSettingsConfigFile = "AudioSettings";
static const std::string BackgroundMusicFieldName = "BackgroundMusic";
static const std::string MasterMusicVolumeFieldName = "MasterMusicVolume";
static const std::string BackgroundMusicVolumeFieldName = "BackgroundMusicVolume";
static constexpr int32 MaxNumberOfMusicToLoad = 1024;
static const FVector2D<int32> MusicVolumeInRange = { 0, 100 };
static const FVector2D<int32> MusicVolumeOutRange = { 0, MIX_MAX_VOLUME };

FMusicToPlay::FMusicToPlay(std::string InMusicPath, const bool bLoadNow)
	: MusicPath(std::move(InMusicPath))
	, bIsLoaded(false)
{
	if (bLoadNow)
	{
		Load();
	}
}

FMusicToPlay::~FMusicToPlay()
{
	UnLoad();
}

void FMusicToPlay::Play()
{
	if (!bIsLoaded)
	{
		Load();
	}

	Mix_FadeInMusic(MusicObject, 0, 500);
}

void FMusicToPlay::Load()
{
	if (!bIsLoaded)
	{
		bIsLoaded = true;

		MusicObject = Mix_LoadMUS(MusicPath.c_str());
	}
}

void FMusicToPlay::UnLoad()
{
	if (bIsLoaded)
	{
		bIsLoaded = false;

		Mix_FreeMusic(MusicObject);
	}
}

FAudioPlayer::FAudioPlayer(FEntityManager* InEntityManager)
	: EEntity(InEntityManager)
	, AssetManager(nullptr)
	, bPlayAudioAutomatically(true)
{
}

void FAudioPlayer::BeginPlay()
{
	Super::BeginPlay();

	ReadIniFiles();
}

void FAudioPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPlayAudioAutomatically && BackgroundMusicToPlayArray.Size() > 0)
	{
		if (Mix_PlayingMusic() == 0)
		{
			FMusicToPlay& RandomMusic = BackgroundMusicToPlayArray[BackgroundMusicToPlayArray.GetRandomIndex()];
			RandomMusic.Play();
		}
	}
}

void FAudioPlayer::SetPlayAudioAutomatic(const bool bInPlayAutomatically)
{
	bPlayAudioAutomatically = bInPlayAutomatically;
}

void FAudioPlayer::SetMasterVolume(const int32 IntValue)
{
	const int32 NewBackgroundMusicVolume = FMath::Clamp(IntValue, MusicVolumeInRange.X, MusicVolumeInRange.Y);
	const int32 NewBackgroundMusicVolumeForMixer = FMath::MapRange(NewBackgroundMusicVolume, MusicVolumeInRange, MusicVolumeOutRange);

	Mix_MasterVolume(NewBackgroundMusicVolumeForMixer);

	LOG_DEBUG("Changed background music volume from input: '" << NewBackgroundMusicVolumeForMixer << "'.");
}

void FAudioPlayer::SetBackgroundMusicVolume(const int32 IntValue)
{
	// Value should be between 0 and 100
	const int32 NewBackgroundMusicVolume = FMath::Clamp(IntValue, MusicVolumeInRange.X, MusicVolumeInRange.Y);
	const int32 NewBackgroundMusicVolumeForMixer = FMath::MapRange(NewBackgroundMusicVolume, MusicVolumeInRange, MusicVolumeOutRange);

	Mix_VolumeMusic(NewBackgroundMusicVolumeForMixer);

	LOG_DEBUG("Changed background music volume from input: '" << NewBackgroundMusicVolumeForMixer << "'.");
}

void FAudioPlayer::SetNumberOfSupportedAudioChannels(const int32 NewNumberOfChannels)
{
	const int32 NewNumberOfChannelsGot = Mix_AllocateChannels(NewNumberOfChannels);

	LOG_DEBUG("Changing number of audio channels, reguested: '" << NewNumberOfChannels << "', mixer returned: '" << NewNumberOfChannelsGot << "'.");
}

void FAudioPlayer::ReadIniFiles()
{
	AssetManager = GEngine->GetAssetsManager();
	FIniManager* IniManager = AssetManager->GetIniManager();
	const std::shared_ptr<FIniObject> AudioSettingsConfigObject = IniManager->GetIniObject(DefaultAudioSettingsConfigFile);
	if (AudioSettingsConfigObject->DoesIniExist())
	{
		AudioSettingsConfigObject->LoadIni();

		GetAudioSettings(AudioSettingsConfigObject);
	}
	else
	{
		LOG_WARN("AudioPlayer is missing config file for audio settings: '" << DefaultAudioSettingsConfigFile << "' audio may still be played.");
	}

	const std::shared_ptr<FIniObject> AudioListConfigObject = IniManager->GetIniObject(DefaultAudioListConfigFile);
	if (AudioListConfigObject->DoesIniExist())
	{
		AudioListConfigObject->LoadIni();

		CollectMusic(AudioListConfigObject);
	}
	else
	{
		LOG_WARN("AudioPlayer is missing required config file: '" << DefaultAudioSettingsConfigFile << "'. Audio will not be played.");
	}
}

void FAudioPlayer::GetAudioSettings(const std::shared_ptr<FIniObject>& InAudioSettingsConfigObject)
{
	FIniField BackgroundMusicVolumeField = InAudioSettingsConfigObject->FindFieldByName(BackgroundMusicVolumeFieldName);
	if (BackgroundMusicVolumeField.IsValid())
	{
		const int32 BackgroundMusicVolume = BackgroundMusicVolumeField.GetValueAsInt();

		SetBackgroundMusicVolume(BackgroundMusicVolume);
	}

	FIniField MasterMusicVolumeField = InAudioSettingsConfigObject->FindFieldByName(MasterMusicVolumeFieldName);
	if (MasterMusicVolumeField.IsValid())
	{
		const int32 MasterMusicVolume = MasterMusicVolumeField.GetValueAsInt();

		SetMasterVolume(MasterMusicVolume);
	}
}

void FAudioPlayer::CollectMusic(const std::shared_ptr<FIniObject>& InAudioListConfigObject)
{
	for (int32 i = 0; i < MaxNumberOfMusicToLoad; i++)
	{
		std::string CurrentMusicName = BackgroundMusicFieldName + std::to_string(i + 1);
		FIniField Field = InAudioListConfigObject->FindFieldByName(CurrentMusicName);
		if (Field.IsValid())
		{
			const std::string RelativeAssetPath = AssetManager->GetAssetsPathRelative() + AssetManager->GetPlatformSlash() + Field.GetValueAsString();
			const std::string AbsoluteAssetPath = AssetManager->ConvertRelativeToFullPath(RelativeAssetPath);

			FMusicToPlay NewMusicToPlay(AbsoluteAssetPath);

			BackgroundMusicToPlayArray.Push(NewMusicToPlay);
		}
		else
		{
			LOG_INFO("Found audio files to play in background: '" << i << "'.");

			break;
		}
	}
}
