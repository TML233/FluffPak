#pragma once

#include "Engine/Application/AppLoop.h"
#include "Engine/Application/Node/Node.h"

namespace Engine{
	/// @brief Default AppLoop of the engine. Manages a tree of game nodes.\n
	/// Only the nodes that are joined in the tree are active.
	class NodeTree final:public AppLoop {
		REFLECTION_CLASS(::Engine::NodeTree, ::Engine::AppLoop) {}

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

		bool stopWhenNoWindow = true;

		UniquePtr<RootType> root = UniquePtr<RootType>::Create();
	};
}