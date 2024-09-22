#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{
    // Constructor
}

void AudioPlayer::begin(Stream& serial, bool debug)
{
    _player.begin(serial, debug);
}

void AudioPlayer::setVolume(int volume)
{
    if (volume < 0 || volume > 30) {
        return;
    }

    _player.volume(volume);
}

void AudioPlayer::playTrack(int trackNo)
{
    _player.playFolder(1, trackNo);
}

void AudioPlayer::playTrackInFolder(int folderNo, int trackNo)
{
    _player.playFolder(folderNo, trackNo);
}
void AudioPlayer::playMp3Folder(int folderNo)
{
    //_player.repeatFolder(folderNo);
    _currentTrack = 0;
    _lastTrack = _player.numTracksInFolder(folderNo);
    nextTrack(folderNo);
}

void AudioPlayer::stop()
{
    _player.stop();
}

void AudioPlayer::nextTrack(int folderNo)
{
    _currentTrack++;
    if (_currentTrack > _lastTrack) {
        // Back to first track
        _currentTrack = 1;
    }

    _player.playFolder(folderNo, _currentTrack);

    Serial.print(F("Playing track #"));
    Serial.print(_currentTrack);
    Serial.print(F(" of"));
    Serial.print(_lastTrack);
    Serial.print(F("tracks in folder "));
    Serial.println(folderNo);
}

bool AudioPlayer::isPlaying()
{
    return _player.isPlaying();
}