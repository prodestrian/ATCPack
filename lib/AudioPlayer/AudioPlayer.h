#ifndef PROTONPACKV3_AUDIOPLAYER_H
#define PROTONPACKV3_AUDIOPLAYER_H

#include <DFPlayerMini_Fast.h>






class AudioPlayer
{
    public:
        // Constructor
        AudioPlayer();

        void begin(Stream& serial, bool debug);
        void setVolume(int volume);
        void playTrack(int trackNo);
        void playTrackInFolder(int folderNo, int trackNo);
        void playMp3Folder(int folderNo);
        void nextTrack(int folderNo);
        void stop();
        bool isPlaying();

    private:
        DFPlayerMini_Fast _player;
        int _currentTrack = 0;
        int _lastTrack = 0;
};


#endif //PROTONPACKV3_AUDIOPLAYER_H
