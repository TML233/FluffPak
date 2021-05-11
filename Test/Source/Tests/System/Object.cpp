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
	String signame = STRL("TestSignal");
	String recvname = STRL("OnTestSignal");
	UniquePtr<SignalTest> sig = UniquePtr<SignalTest>::Create();

	UniquePtr<SignalHandler> hd1 = UniquePtr<SignalHandler>::Create();
	CHECK(sig->ConnectSignal(signame, Invokable(hd1.GetRaw(), recvname)) == ReflectionSignal::ConnectResult::OK);
	UniquePtr<SignalHandler> hd2 = UniquePtr<SignalHandler>::Create();
	CHECK(sig->ConnectSignal(signame, Invokable(hd2.GetRaw(), recvname)) == ReflectionSignal::ConnectResult::OK);
	UniquePtr<SignalHandler> hd3 = UniquePtr<SignalHandler>::Create();
	CHECK(sig->ConnectSignal(signame, Invokable(hd3.GetRaw(), recvname)) == ReflectionSignal::ConnectResult::OK);

	CHECK(sig->IsSignalConnected(signame, Invokable(hd1.GetRaw(), recvname)));
	CHECK(sig->IsSignalConnected(signame, Invokable(hd2.GetRaw(), recvname)));
	CHECK(sig->IsSignalConnected(signame, Invokable(hd3.GetRaw(), recvname)));

	CHECK(!sig->IsSignalConnected(signame, Invokable(hd1.GetRaw(), STRL("SB"))));
	CHECK(!sig->IsSignalConnected(STRL("SB"), Invokable(hd2.GetRaw(), recvname)));

	Variant v = 3;
	const Variant* args[] = { &v };
	CHECK(sig->EmitSignal(signame, (const Variant**)args, 1));

	CHECK(hd1->value == 3);
	CHECK(hd2->value == 3);
	CHECK(hd3->value == 3);

	CHECK(sig->DisconnectSignal(signame, Invokable(hd2.GetRaw(), recvname)));

	v = -1;
	CHECK(sig->EmitSignal(signame, (const Variant**)args, 1));

	CHECK(hd1->value == 2);
	CHECK(hd2->value == 3);
	CHECK(hd3->value == 2);
}
