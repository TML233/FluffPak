#include "doctest.h"
#include "Engine/System/Object/Object.h"

using namespace Engine;

class SignalTest :public ManualObject {
	REFLECTION_CLASS(::SignalTest, ::Engine::ManualObject) {
		REFLECTION_SIGNAL(STRL("TestSignal"), { SIGARG(STRL("test"), Variant::Type::Int64) });
	}
};

class SignalHandler :public ManualObject {
	REFLECTION_CLASS(::SignalHandler, ::Engine::ManualObject) {
		REFLECTION_METHOD(STRL("OnTestSignal"), SignalHandler::OnTestSignal, { STRL("v") }, {});
	}
public:
	void OnTestSignal(int32 v) {
		value += v;
	}
	int32 value = 0;
};

TEST_CASE("Object") {
	UniquePtr<SignalTest> sig = UniquePtr<SignalTest>::Create();
	UniquePtr<SignalHandler> hd = UniquePtr<SignalHandler>::Create();
	sig->ConnectSignal(STRL("TestSignal"), Invokable(hd.GetRaw(), STRL("OnTestSignal")));

	Variant v = 3;
	const Variant* args[] = { &v };
	sig->EmitSignal(STRL("TestSignal"), (const Variant**)args, 1);

	CHECK(hd->value == 3);
}
