#pragma once

#include "Engine/Application/AppLoop.h"
#include "Engine/Application/Node/Node.h"

namespace Engine{
	class NodeTree final:public AppLoop {
		REFLECTION_CLASS(NodeTree, AppLoop) {}

	public:
		using RootType = Node;
		NodeTree();

		void OnStart() override;
		void OnUpdate(const Time& time) override;
		void OnPhysicsUpdate(const Time& time) override;

		void OnStop() override;
		bool IsRunning() const override;

		RootType* GetRoot() const;

		void RequestStop();

	private:
		bool running = false;

		UniquePtr<RootType> root = UniquePtr<RootType>::Create();
	};
}