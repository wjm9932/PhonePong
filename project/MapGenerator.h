/*
File name: MapGenerator.h
Project name: Phone! Pong!
Primary Author: Shinil Kang
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once
#include <engine/Object.h>
#include <engine/State.h>
#include "ScoreObject.h"

class MyObject;

// Big problem is it works only one Object.
// I need to change it works even if there are a lot of objects.

class Map
{
public:
	// default constructor
	Map();
	~Map();

	Map(const Map&);

	// Reference Copy Constructor
	Map(Map* value);
	Map(Map &&value) noexcept;
	Map& operator=(const Map& source);

	void AddObject(MyObject* obj);
	void AddScoreObject(ScoreObject* obj);

	MyObject* GetObject(int index);
	ScoreObject* GetScoreObject(int index);
	int GetObjectSize(void) const;
	int GetScoreObjectSize(void) const;

	void Calculate(void);

	float getStartPosition() const;
	float getEndPosition() const;

	void SetNumOfCollisionBox(int num);
	int GetNumOfCollisionBox(void);

	MyObject* GetJointBox(int index);

	void EraseScoreObject(int index);

    void ToggleRopeAvailable(void);
    bool getIsRopeAvailable(void);

private:
	std::vector<MyObject*> objects;
	std::vector<ScoreObject*> scoreObjects;

	int numOfCollisionBox = 0;
	float startPosition;
	float endPosition;

    bool isRopeAvailable = true;
};

class MapGenerator
{
public:
	MapGenerator();
	~MapGenerator();

	void AddMap(Map* map);
	Map* GetMap(int index);

	void AddMapToGame(int index);
	Map* GetMapFromGame(int index);

	int StorageSize(void);
	int GameMapSize(void);
	void RemoveFirstGameMap(void);

	Map* GetBackFromGame(void);

	void mapGeneratorCleanUp();
	void RemoveMap(void);
	void RemoveGameMap(void);

private:
	// storage for all map style
	std::vector<Map*> mapList;

	// present vector that currently added Maps in game
	std::vector<Map*> gameMap;
};