#pragma once

#include "Engine/Application/AppLoop.h"
#include "Engine/Application/Node/Node.h"

namespace Engine{
	class NodeTree final:public AppLoop {
	public:
		using RootType = Node;

		void OnStart() override;
		void OnUpdate(const Time& time) override;
		void OnPhysicsUpdate(const Time& time) override;

		void OnStop() override;
		bool IsRunning() const override;

		RootType* GetRoot() const;
	private:
		bool running = false;

		UniquePtr<RootType> root = UniquePtr<RootType>::Create();
	};
}