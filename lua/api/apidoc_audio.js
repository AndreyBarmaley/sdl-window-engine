SWE.Audio.MixerVolume
/**
    @api	{set mixer volume} SWE.Audio.MixerVolume(volume) SWE.Audio.MixerVolume
    @apiGroup	SWE.Audio
    @apiName	MixerVolume
    @apiParam	{number}			volume		new volume setting, 128 max
    @apiSuccess (Return) {number}		volume		previous volume setting
*/
SWE.Audio.MixerReset
/**
    @api	{reset mixer} SWE.Audio.MixerReset() SWE.Audio.MixerReset
    @apiGroup	SWE.Audio
    @apiName	MixerReset
*/
SWE.Audio.MusicIsPlaying
/**
    @api {check mixer is playing music} SWE.Audio.MusicIsPlaying() SWE.Audio.MusicIsPlaying
    @apiGroup SWE.Audio
    @apiName MusicIsPlaying

    @apiSuccess (Return) {boolean}		result		true if playing
*/
SWE.Audio.MusicPause
/**
    @api {set pause} SWE.Audio.MusicPause() SWE.Audio.MusicPause
    @apiGroup SWE.Audio
    @apiName MusicPause
*/
SWE.Audio.MusicResume
/**
    @api {resume play music} SWE.Audio.MusicResume() SWE.Audio.MusicResume
    @apiGroup SWE.Audio
    @apiName MusicResume
*/
SWE.Audio.MusicPlay
/**
    @api {play music from filename} SWE.Audio.MusicPlay(filename) SWE.Audio.MusicPlay
    @apiGroup SWE.Audio
    @apiName MusicPlay

    @apiParam {string}				filename	filename music format
    @apiSuccess (Return) {boolean}		result		true if playing
*/
SWE.Audio.MusicPlayBuf
/**
    @api {play music from binarybuf} SWE.Audio.MusicPlayBuf(inputbuf) SWE.Audio.MusicPlayBuf
    @apiGroup SWE.Audio
    @apiName MusicPlayBuf

    @apiParam {SWE.BinaryBuf}			inputbuf	binarybuf music format
    @apiSuccess (Return) {boolean}		result		true if playing
*/
SWE.Audio.SoundIsPlaying
/**
    @api {check mixer is playing sound} SWE.Audio.SoundIsPlaying() SWE.Audio.SoundIsPlaying
    @apiGroup SWE.Audio
    @apiName SoundIsPlaying

    @apiSuccess (Return) {boolean}		result		true if playing
*/
SWE.Audio.SoundPlay
/**
    @api {play sound from filename} SWE.Audio.SoundPlay(filename) SWE.Audio.SoundPlay
    @apiGroup SWE.Audio
    @apiName SoundPlay

    @apiParam {string} 				filename	filename sound format
    @apiSuccess (Return) {boolean}		result		true if playing
*/
SWE.Audio.SoundPlayBuf
/**
    @api {play sound from binarybuf} SWE.Audio.SoundPlayBuf(inputbuf) SWE.Audio.SoundPlayBuf
    @apiGroup SWE.Audio
    @apiName SoundPlayBuf

    @apiParam {SWE.BinaryBuf} 			inputbuf	binarybuf sound format
    @apiSuccess (Return) {boolean}		result		true if playing
*/
