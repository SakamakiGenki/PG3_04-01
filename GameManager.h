#pragma once

#include <memory>

class IScene;

class GameManager {

private:

	// シーン配列
	std::unique_ptr<IScene> sceneArr_[3];

	int currentSceneNo_;

	int prevSceneNo_;

public:

	GameManager();

	~GameManager();

	int Run();
};