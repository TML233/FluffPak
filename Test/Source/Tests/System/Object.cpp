#include "doctest.h"
#include "Engine/System/Object/Object.h"

using namespace Engine;

class SignalTest :public ManualObject {
	REFLECTION_CLASS(::SignalTest, ::Engine::ManualObject) {
		REFLECTION_SIGNAL(STRL("TestSignal"), { SIGARG(STRL("value"), Variant::Type::Int64) });
	}
};

class SignalHandler :public ManualObject {
	REFLECTION_CLASS(::SignalHandler, ::Engine::ManualObject) {
		REFLECTION_METHOD(STRL("OnTestSignal"),
			SignalHandler::OnTestSignal,
			{ STRL("value") },
			{}
		);
		REFLECTION_METHOD(STRL("OnTestSignalWithExtra"),
			SignalHandler::OnTestSignalWithExtra,
			ARGLIST(STRL("value"), STRL("add"), STRL("minus")),
			ARGLIST()
		);
	}
public:
	void OnTestSignal(int32 v) {
		value += v;
	}
	void OnTestSignalWithExtra(int32 v, int32 add, int32 minus) {
		value += v + add - minus;
	}
	int32 value = 0;
};

TEST_CASE("Object") {
	String signame = STRL("TestSignal");
	String recvname = STRL("OnTestSignal");
	String recvname2 = STRL("OnTestSignalWithExtra");
	UniquePtr<SignalTest> sig = UniquePtr<SignalTest>::Create();

	UniquePtr<SignalHandler> hd1 = UniquePtr<SignalHandler>::Create();
	CHECK(sig->ConnectSignal(signame, Invokable(hd1.GetRaw(), recvname)) == ResultCode::OK);
	UniquePtr<SignalHandler> hd2 = UniquePtr<SignalHandler>::Create();
	CHECK(sig->ConnectSignal(signame, Invokable(hd2.GetRaw(), recvname)) == ResultCode::OK);
	UniquePtr<SignalHandler> hd3 = UniquePtr<SignalHandler>::Create();
	CHECK(sig->ConnectSignal(signame, Invokable(hd3.GetRaw(), recvname)) == ResultCode::OK);
	Variant argAdd = 10;
	Variant argMinus = 5;
	const Variant* extraArgs[] = {&argAdd,&argMinus};
	CHECK(sig->ConnectSignal(signame, Invokable(hd3.GetRaw(), recvname2), extraArgs, 2) == ResultCode::OK);

	CHECK(sig->IsSignalConnected(signame, Invokable(hd1.GetRaw(), recvname)));
	CHECK(sig->IsSignalConnected(signame, Invokable(hd2.GetRaw(), recvname)));
	CHECK(sig->IsSignalConnected(signame, Invokable(hd3.GetRaw(), recvname)));
	CHECK(sig->IsSignalConnected(signame, Invokable(hd3.GetRaw(), recvname2)));

	CHECK(!sig->IsSignalConnected(signame, Invokable(hd1.GetRaw(), STRL("SB"))));
	CHECK(!sig->IsSignalConnected(STRL("SB"), Invokable(hd2.GetRaw(), recvname)));

	Variant v = 3;
	const Variant* args[] = { &v };
	CHECK(sig->EmitSignal(signame, (const Variant**)args, 1));

	CHECK(hd1->value == 3);
	CHECK(hd2->value == 3);
	CHECK(hd3->value == 3 + 3 + 10 - 5);

	CHECK(sig->DisconnectSignal(signame, Invokable(hd2.GetRaw(), recvname)));
	CHECK(sig->DisconnectSignal(signame, Invokable(hd3.GetRaw(), recvname)));

	CHECK(sig->IsSignalConnected(signame, Invokable(hd1.GetRaw(), recvname)));
	CHECK(!sig->IsSignalConnected(signame, Invokable(hd2.GetRaw(), recvname)));
	CHECK(!sig->IsSignalConnected(signame, Invokable(hd3.GetRaw(), recvname)));
	CHECK(sig->IsSignalConnected(signame, Invokable(hd3.GetRaw(), recvname2)));

	v = -1;
	CHECK(sig->EmitSignal(signame, (const Variant**)args, 1));

	CHECK(hd1->value == 2);
	CHECK(hd2->value == 3);
	CHECK(hd3->value == 3 + 3 + 10 - 5 - 1 + 10 - 5);
}
