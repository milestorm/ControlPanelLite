#include <ToneSfx.h>

/*
Sound effect definitions
*/

// intro melody
const char *melodyStart[] = {"I:523,659,15,200", "I:659,784,15,200", "P:100", "I:698,1047,15,100", "P:50", "I:784,988,15,100", "P:50","I:784,1047,15,100", SFX_END};


// BANK 0 - retro sounds
const char *sfxComputerSoundCmd[] = {"N:10,2000,50,120,1000", SFX_END};
const char *sfxBombCmd[] = {"S:2200,1000,25,40", "N:100,200,5,15,1300", SFX_END};
const char *sfxSirenCmd[] = {"S:880,1650,50,10", "S:1650,880,50,10", SFX_REPEAT};
const char *sfxGunCmd[] = {"N:100,200,5,15,200", "S:2200,900,50,12", SFX_END};

// BANK 1 - wolf 3D
const char *sfxWolfAmmoCmd[] = {"T:587,170", "P:30", "T:988,50", "P:30", "T:988,50", "P:30", "T:988,50", SFX_END};
const char *sfxWolfOneUpCmd[] = {"T:1047,50", "P:50", "T:1245,50", "P:50", "T:1480,50", "P:50", "S:2093,131,100,20", SFX_END};
const char *sfxWolfGobletPickupCmd[] = {"S:294,440,10,5", "S:440,294,10,5", "S:440,554,10,5", "S:554,440,10,5", "S:554,831,10,5", "T:831,80", "P:140", "T:1047,45", "P:20", "T:1047,60", "P:20", "T:1047,45", "P:35", "T:1245,80", "P:20", "T:1245,60", SFX_END};
const char *sfxWolfGunshot[] = {"T:784,7", "T:523,22", "P:7", "T:587,29", "P:7", "T:523,29", "T:123,15", "S:392,294,80,10", "T:123,100", "P:14", "T:123,7", "P:20", "T:123,7", "P:14", "T:123,14", "P:40", "T:123,35", "P:40", "T:123,7", SFX_END};

// BANK 3 - melody tones
const char *toneAC[] = {"I:440,523,15,200", SFX_END};
const char *toneBD[] = {"I:494,587,15,200", SFX_END};
const char *toneCE[] = {"I:523,659,15,200", SFX_END};
const char *toneEG[] = {"I:659,784,15,200", SFX_END};

// BANK 4 - Todo :)

// misc
const char *testCmd[] = {"N:10,2000,50,120,1000", SFX_REPEAT};
const char *sfxBlastCmd[] = {"N:400,500,5,15,50", "N:300,400,5,15,75", "N:200,300,5,15,100", "N:100,200,5,15,125", SFX_END};
