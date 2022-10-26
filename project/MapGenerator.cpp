/*
File name: MapGenerator.cpp
Project name: Phone! Pong!
Primary Author: Shinil Kang
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#include "MapGenerator.h"
#include "MyObject.h"
#include <functional>

Map::Map()
{
}

Map::~Map()
{
	while (objects.empty() == false)
	{
		MyObject* tmp = objects.back();
		objects.pop_back();
		delete tmp;
	}
	while (scoreObjects.empty() == false)
	{
		ScoreObject* tmp = scoreObjects.back();
		scoreObjects.pop_back();
		delete tmp;
	}
}

Map::Map(const Map& value)
{
	Map* tmp = new Map;
	for (auto element : value.objects)
	{
		tmp->AddObject(element);
	}
	objects = tmp->objects;
}

Map::Map(Map* value)
{
	Map* tmp = new Map;
	for (auto element : value->objects)
	{
		tmp->AddObject(element);
	}
	SetNumOfCollisionBox(value->GetNumOfCollisionBox());
	objects = tmp->objects;
}

Map::Map(Map && value) noexcept
{
	Map* tmp = new Map;
	for (auto element : value.objects)
	{
		tmp->AddObject(element);
	}
	objects = tmp->objects;
}

Map& Map::operator=(const Map& source)
{
	Map* tmp = new Map;
	for (const auto & element : source.objects)
	{
		tmp->objects.push_back(element);
	}
	objects = tmp->objects;
	return *this;
}

void Map::AddObject(MyObject* obj)
{
	objects.push_back(obj);
}

void Map::AddScoreObject(ScoreObject* obj)
{
	scoreObjects.push_back(obj);
}

MyObject* Map::GetObject(int index)
{
	return objects.at(index);
}

ScoreObject* Map::GetScoreObject(int index)
{
	return scoreObjects.at(index);
}
int Map::GetObjectSize() const
{
	return static_cast<int>(objects.size());
}

int Map::GetScoreObjectSize() const
{
	return static_cast<int>(scoreObjects.size());
}

void Map::Calculate()
{
	startPosition = objects.at(0)->transform.position.x - objects.at(0)->GetScale().x / 2.f;
	endPosition = objects.at(GetObjectSize() - 1)->transform.position.x + objects.at(GetObjectSize() - 1)->GetScale().x / 2.f;
}

float Map::getStartPosition() const
{
	return startPosition;
}

float Map::getEndPosition() const
{
	return endPosition;
}

void Map::SetNumOfCollisionBox(int num)
{
	numOfCollisionBox = num;
}

int Map::GetNumOfCollisionBox()
{
	return numOfCollisionBox;
}

MyObject* Map::GetJointBox(int index)
{
	if (index >= numOfCollisionBox)
	{
		return nullptr;
	}
	return objects.at(index + numOfCollisionBox);
}

void Map::EraseScoreObject(int index)
{
	auto it = scoreObjects.begin();

	for (int i = 0; i < index; ++i)
	{
		++it;
	}

	scoreObjects.erase(it);
}

void Map::ToggleRopeAvailable(void)
{
    isRopeAvailable = !isRopeAvailable;
}

bool Map::getIsRopeAvailable(void)
{
    return isRopeAvailable;
}

MapGenerator::MapGenerator()
{
}

MapGenerator::~MapGenerator()
{
	//mapList.clear();
}

void MapGenerator::AddMap(Map* map)
{
	mapList.push_back(map);
}

Map* MapGenerator::GetMap(int index)
{
	// 0 need to be changed as a random number
	return mapList.at(index);
}

void MapGenerator::AddMapToGame(int index)
{
	Map* beingCopied = (mapList.at(index));
	Map* storage = new Map;
	// Update platforms
	for (int i = 0; i<beingCopied->GetObjectSize(); ++i)
	{
		storage->AddObject(new MyObject(beingCopied->GetObject(i)));
	}
	// Update Score Objects
	for (int i = 0; i<beingCopied->GetScoreObjectSize(); ++i)
	{
		storage->AddScoreObject(new ScoreObject(beingCopied->GetScoreObject(i)));
	}
	storage->SetNumOfCollisionBox(beingCopied->GetNumOfCollisionBox());
	gameMap.push_back(storage);
}

Map* MapGenerator::GetMapFromGame(int index)
{
	return gameMap.at(index);
}

int MapGenerator::StorageSize()
{
	return static_cast<int>(mapList.size());
}

int MapGenerator::GameMapSize()
{
	return static_cast<int>(gameMap.size());
}

void MapGenerator::RemoveFirstGameMap()
{
	// I need to handle memory leak issue.
	// delete gameMap.at(0);
	 Map* tmp = gameMap.front();
	gameMap.erase(gameMap.begin());

	 delete tmp;
}

Map* MapGenerator::GetBackFromGame()
{
	return gameMap.back();
}

void MapGenerator::RemoveMap()
{
	while (mapList.empty() == false)
	{
		Map* tmp = mapList.front();
        if (tmp != nullptr)
        {
            mapList.erase(mapList.begin());

            delete tmp;
        }
	}
}

void MapGenerator::RemoveGameMap()
{
	RemoveFirstGameMap();
}


// Clean up only map storage
void MapGenerator::mapGeneratorCleanUp()
{
	RemoveMap();

}