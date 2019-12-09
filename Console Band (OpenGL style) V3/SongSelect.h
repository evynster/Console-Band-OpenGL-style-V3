#pragma once

#include <GameEmGine.h>
#include <EmGineAudioPlayer.h>
#include <filesystem>
#include <string>
#include "Difficulty.h"


#pragma region using / Namespace Stuff
using std::vector;
using std::string;
using std::wstring;
using std::pair;
using std::function;
using std::thread;
using std::stoi;
namespace fs = std::filesystem;
#pragma endregion

#define select (InputManager::keyStroke('A') || XinputManager::getController(0)->isButtonStroked(GUITAR_INPUT_BUTTONS::GUITAR_FRET_GREEN))
#define deselect (InputManager::keyStroke('S') || XinputManager::getController(0)->isButtonStroked(GUITAR_INPUT_BUTTONS::GUITAR_FRET_RED)\
				||InputManager::mouseStroke(MOUSE_RIGHT_BUTTON))

wstring cDir(wstring dir)
{
	for(auto& a : dir)
		if(a == L'\\')
			a = L'/';

	const wchar_t* tmp;
	if(dir.length() > 1)
		while(bool(tmp = wcsstr(dir.c_str(), L"//")))
		{
			memmove_s((wchar_t*)tmp, sizeof(wchar_t) * wcslen(tmp), tmp + 1, sizeof(wchar_t) * wcslen(tmp + 1));
			dir[wcslen(dir.c_str()) - 1] = 0;
		}

	return dir;
}


class SongSelect:public Scene
{
	void init()
	{
		Game::setFPSLimit(10);



	}
	void songChoiceMovement(int size)
	{

		static float delay = .2f;
		static clock_t lastClockT;
		if(float(std::clock() - lastClockT) / CLOCKS_PER_SEC > delay)
		{
			lastClockT = std::clock();

			if(InputManager::keyDown(VK_DOWN) || InputManager::getController(0)->isButtonPressed(CONTROLLER_DPAD_DOWN))

			{
				AudioPlayer::setMasterVolume(1);
				if(songChoice < size - 1)
					songChoice++;
				else
					songChoice = 0;
				AudioPlayer::createAudioStream("sfx/Guitar_Miss_2.wav");
				AudioPlayer::setVolume(2);
				AudioPlayer::play();
			}
			else if(InputManager::keyDown(VK_UP) || XinputManager::getController(0)->isButtonPressed(GUITAR_INPUT_BUTTONS::GUITAR_STRUM_UP))
			{
				AudioPlayer::setMasterVolume(1);
				if(songChoice > 0)
					songChoice--;
				else
					songChoice = size - 1;
				AudioPlayer::createAudioStream("sfx/Guitar_Miss_2.wav");
				AudioPlayer::setVolume(2);
				AudioPlayer::play();
			}
		}
		if(XinputManager::getController(0)->isButtonReleased(GUITAR_INPUT_BUTTONS::GUITAR_STRUM_DOWN) && InputManager::keyUp(VK_DOWN) &&
			XinputManager::getController(0)->isButtonReleased(GUITAR_INPUT_BUTTONS::GUITAR_STRUM_UP) && InputManager::keyUp(VK_UP))
			lastClockT = std::clock() - uint(CLOCKS_PER_SEC * delay);
	}

	void update(double dt)
	{




		//guitarstart;//eat this input;
		InputManager::keyDown('Q');//eat this input;
		XinputManager::update();
		bool pass = select;



		songChoiceMovement(count);

		static float startHeight = 3, endHeight = -3, startWidth = 0;

		if(lastSongChoice != songChoice)
			lastClockT = std::clock();

		songs.clear();
		songPath.clear();
		count = 0;
		for(auto& a : fs::directory_iterator(path))
		{
			if(count < movement || count - movement >(endHeight - startHeight))
			{
				count++;
				continue;
			}
			p = cDir(a.path());

			if(songChoice == count)
			{
				lastSongChoice = songChoice;
				playing = wstring(p.begin(), p.end());
				playing = cDir(playing);
				playing += L'/';

				if(float(std::clock() - lastClockT) / CLOCKS_PER_SEC > 1.5f)
				{
					bool isSong = false;
					for(auto& b : fs::directory_iterator(a.path()))
					{
						wstring tmp = b.path();
						if(tmp.substr(tmp.find_last_of('.') + 1) == L"ini")
							isSong = true;
					}
					if(isSong)
						if(lastPlayed != playing)
						{

							AudioPlayer::stopAll();
							uint started = 5000;
							for(auto& b : fs::directory_iterator(playing))
							{
								wstring ogg = cDir(b.path());
								if(ogg.substr(ogg.find_last_of('.') + 1) == L"ogg")
									if(ogg.substr(ogg.find_last_of('/') + 1, ogg.find_last_of('.') - ogg.find_last_of('/') - 1) != L"preview")
										AudioPlayer::createAudioStream(string(ogg.begin(), ogg.end()).c_str());

								if(ogg.substr(ogg.find_last_of('.') + 1) == L"ini")
								{
									FILE* f;
									fopen_s(&f, string(ogg.begin(), ogg.end()).c_str(), "r");

									char* str2 = new char[255];

									while(str2 = fgets(str2, 255, f))
									{
										if(strstr(str2, "preview_start_time"))
										{
											str2[strlen(str2) - 1] = (str2[strlen(str2) - 1] == '\n' ? '\0' : str2[strlen(str2) - 1]);
											started = stoi(strstr(str2, "=") + 1) + 1;
											fclose(f);
											break;
										}
									}
									delete str2;
									fclose(f);
								}
							}

							EmGineAudioPlayer::getAudioSystem()->getSoftwareFormat(&sample, nullptr, nullptr);
							AudioPlayer::getMasterChannelGroup()->removeFadePoints(clock, clock + (sample * 25));

							AudioPlayer::getMasterChannelGroup()->getDSPClock(nullptr, &clock);
							AudioPlayer::getMasterChannelGroup()->addFadePoint(clock, 0);


							uint
								starting = started,
								ending = started + (25 * 1000);
							AudioPlayer::playAll(true, starting, ending, FMOD_TIMEUNIT_MS);

							AudioPlayer::getMasterChannelGroup()->getDSPClock(nullptr, &clock);
							AudioPlayer::getMasterChannelGroup()->addFadePoint(clock, 0);
							AudioPlayer::getMasterChannelGroup()->addFadePoint(clock + (sample * 2), 1);
							AudioPlayer::getMasterChannelGroup()->addFadePoint(clock + (sample * 23), 1);
							AudioPlayer::getMasterChannelGroup()->addFadePoint(clock + (sample * 25), 0);
							lastClock = clock;
							lastPlayed = playing;


						}
				}
			}

			songPath.push_back(p + L'/');

			bool isSong = false;
			for(auto& b : fs::directory_iterator(a.path()))
			{
				wstring tmp = b.path();
				if(tmp.substr(tmp.find_last_of('.') + 1) == L"ini")
					isSong = true;
			}

			if(isSong)
			{
				for(auto& b : fs::directory_iterator(songPath.back()))
					if(b.path().extension() == L".ini")
					{
						FILE* f;
						wstring s1(b.path());
						fopen_s(&f, string(s1.begin(), s1.end()).c_str(), "r");
						char* str2 = new char[255];

						p = L"";
						while(str2 = fgets(str2, 255, f))
						{
							if(strstr(str2, "name"))
							{
								bool end = p.size();
								str2[strlen(str2) - 1] = '\0';
								string s = strstr(str2, "=") + 1;
								p = wstring(s.begin(), s.end()) + L" -" + p;
								if(end)
									break;
							}
							else
								if(strstr(str2, "artist"))
								{
									bool end = p.size();
									str2[strlen(str2) - 1] = '\0';
									string s = strstr(str2, "=") + 1;
									p += wstring(s.begin(), s.end());
									if(end)
										break;
								}
						}
						delete str2;
						fclose(f);
						songs.push_back(p);
						count++;
						break;
					}
			}
			else
			{
				songs.push_back(p.substr(p.find_last_of('/') + 1));
				count++;
			}
		}

		if(pass || (InputManager::mouseStroke(MOUSE_LEFT_BUTTON)))
		{

			if(![&]()->bool
				{
					count = 0;
						for(auto& a : fs::directory_iterator(path))
						{
							if(songChoice == count++)
								for(auto& b : fs::directory_iterator(a.path()))
								{
									wstring tmp = b.path();
										if(tmp.substr(tmp.find_last_of('.') + 1) == L"ini")
											return true;
								}
						}
					return false;
				}())
			{
				count = 0;
				for(auto& a : fs::directory_iterator(path))
					if(songChoice == count++)
					{
						path = cDir(a.path());
						break;
					}
				songChoice = 0;
			}
			else
			{
				AudioPlayer::getMasterChannelGroup()->getDSPClock(nullptr, &clock);
				AudioPlayer::getMasterChannelGroup()->removeFadePoints(0, clock);
				AudioPlayer::getMasterChannelGroup()->removeFadePoints(clock, clock + (sample * 25));
				AudioPlayer::getMasterChannelGroup()->addFadePoint(0, 1);
				AudioPlayer::stopAll();

				lastPlayed = L"";
				static Difficulty difficultyMenu;
				Game::setScene(&difficultyMenu);
			}

		}
		if(deselect)
		{
			if(path == L"songs/")
			{
				AudioPlayer::getMasterChannelGroup()->getDSPClock(nullptr, &clock);
				AudioPlayer::getMasterChannelGroup()->removeFadePoints(0, clock);
				AudioPlayer::getMasterChannelGroup()->removeFadePoints(clock, clock + (sample * 25));
				AudioPlayer::getMasterChannelGroup()->addFadePoint(0, 1);
				AudioPlayer::stopAll();
				
				Game::setScene(parent);
			}
			else
			{
				songChoice = 0;
				path = path.substr(0, path.substr(0, path.size() - 1).find_last_of('/') + 1);
			}

		}

		AudioPlayer::getMasterChannelGroup()->getDSPClock(nullptr, &clock);
		EmGineAudioPlayer::getAudioSystem()->getSoftwareFormat(&sample, nullptr, nullptr);

		//Loops the song fade in/out
		if(clock >= lastClock + (sample * 25))
		{
			AudioPlayer::getMasterChannelGroup()->removeFadePoints(lastClock, clock);
			AudioPlayer::getMasterChannelGroup()->addFadePoint(clock, 0);
			AudioPlayer::getMasterChannelGroup()->addFadePoint(clock + (sample * 2), 1);
			AudioPlayer::getMasterChannelGroup()->addFadePoint(clock + (sample * 23), 1);
			AudioPlayer::getMasterChannelGroup()->addFadePoint(clock + (sample * 25), 0);
			lastClock = clock;
		}

		if(songChoice >= (endHeight - startHeight) / 3 && songChoice < count - (endHeight - startHeight) / 3)
			movement =
			(
				startHeight + songChoice - movement > startHeight + (endHeight - startHeight) / 3 ?
				(startHeight + songChoice - movement > endHeight - (endHeight - startHeight) / 3 ?
					songChoice - (endHeight - (endHeight - startHeight) / 3 - startHeight) : movement)
				:
				songChoice - (endHeight - startHeight) / 3
				);

		else if(songChoice < (endHeight - startHeight) / 3)
			movement = 0;

		else if(songChoice > count - (endHeight - startHeight) / 3)
			movement = (count - 1) - int((endHeight - startHeight) / 2.f) - int((endHeight - startHeight) / 2.f);

		uint tmp;
		if((tmp = InputManager::getMousePosition().y - startHeight) >= 0 && InputManager::getMousePosition().y >= startHeight && InputManager::getMousePosition().y <= endHeight && tmp < songs.size())
			if(Sprite(vector<wstring> {songs[tmp]}).mouseCollision({short(startWidth - (songs[tmp].size() / 2)), short(startHeight + tmp)}, InputManager::getMousePosition()))
				songChoice = InputManager::getMousePosition().y - startHeight + movement;

		//draw song info
		for(uint a = 0; a < songs.size(); a++)
			con->toConsoleBuffer(songs[a], startWidth - (songs[a].size() / 2), startHeight + a, ((uint)songChoice - movement == a ? colours[1] : colours[0]));


		con->toConsoleBuffer(L"Song List", startWidth - 4, startHeight - 2);
		con->toConsoleBuffer(L"----------------------------", startWidth - 14, startHeight - 1);
		con->toConsoleBuffer(L"----------------------------", startWidth - 14, endHeight + 1);

		for(int a = 0; a < 2; a++)
			con->toConsoleBuffer(notes[0][0], int(con->getWidth() * .1f) + 12 * a, con->getHeight() - 4, fretNoteColour[0][a]),
			con->toConsoleBuffer(noteMsg[a], int(con->getWidth() * .1f) + 12 * a + notes[0][0].getWidth() / 2 - noteMsg[a].size() / 2, con->getHeight() - 3, fretNoteColour[0][a]),
			con->toConsoleBuffer(noteKey[a], int(con->getWidth() * .1f) + 12 * a + notes[0][0].getWidth() / 2 - noteKey[a].size() / 2, con->getHeight() - 5, fretNoteColour[0][a]);

		if(songChoice - movement >= 0 && (uint)songChoice - movement < songs.size())
			* songName = string(songs[songChoice - movement].begin(), songs[songChoice - movement].end());


	}

	static wstring path;
	wstring p, playing, lastPlayed;
	vector<wstring>songs, songPath;
	wstring noteMsg[2]{L"Select",L"Back"},
		noteKey[2]{L"A",L"S"};
	clock_t lastClockT = std::clock();
	static int count, movement, lastSongChoice, songChoice;
	ColourRGBA colours[2]{{255,255,255},{0,255,0}};
	int sample = 0, startHeight = 0, endHeight = 0, startWidth = 0;
	unsigned long long lastClock = 0, clock = 0;

};
wstring SongSelect::path = L"songs/";
int SongSelect::count, SongSelect::movement, SongSelect::lastSongChoice, SongSelect::songChoice;
