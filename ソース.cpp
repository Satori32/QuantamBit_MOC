#include <iostream>
#include <random>
#include <future>
#include <chrono>
#include <thread>
#include <memory>
#include <mutex>
#include <string>
//#include <unordered_map>
#include <vector>

#include <conio.h>

int KeyIn() {
	int K = -1;

	if (_kbhit()) {
		K = _getch();
	}

	return K;
}

template<class Float>
class QuantamBit {
public:

	typedef std::shared_ptr<QuantamBit> ShardQuantamBit;
	typedef Float FTYPE;
	bool Update() {
		bool F = false;
		std::uniform_real_distribution<Float> ui(0, 1);
		if (!IsObserve) { B = ui(mt); F = true; }

		return F;
	}
	bool UpdateII() {
		bool F = false;
		std::uniform_real_distribution<Float> ui(0, 1);
		if (!IsObserve) {
			Float X = ui(mt);
			if (X < 0.5) {
				B+= X;
			}
			else {
				B -= X;
			}
			F = true; 
			B=std::fmod(B+16, 1);
		}

		return F;
	}
	bool UpdateIII() {
		std::uniform_real_distribution<Float> ui(0, 1);
		bool F = false;
		if (!IsObserve) { std::shuffle(IN.begin(), IN.end(), mt); B = ui(mt); F = true; }

		return F;
	}
	bool IsZero() {
		return B == 0.0;
	}
	bool IsOne() {
		return B == 1.0;
	}

	Float Observe() {
		Update();
		IsObserve = true;
		return B;
	}

	bool Free() {//inluding forgot.
		IsObserve = false;
		Update();
		return true;
	}

	operator Float&() {
		return B;
	}

	Float Get() { return B; }

	bool Install(const std::string& S) {
		IN.push_back(S);
		return true;
	}

	bool UnIntall(const std::string& S) {
		
		for (auto it = IN.begin(); it != IN.end(); it++) {
			if (S == *it) { auto X = it; it--; IN.erase(X); }
		}
		return 0;
	}
	const std::vector<std::string>& GetStrings(){
		return IN;
	}

protected:
	//std::random_device rd();
	//std::mt19937 mt{ rd() };
	std::mt19937 mt;
	Float B=0;
	bool IsObserve = false;
	std::vector<std::string> IN;
};
typedef std::shared_ptr<bool> SBool;

/** /
template<class F ,class AB>
auto NewAsync(typename QuantamBit<F>::SharedQuantamBit In,SBool B) {
	auto X = [](auto In, auto BB) {while (*BB) { In->Update(); }return BB; };

	auto A = std::async(std::launch::async, X, In, B);
	//return std::move(A);
	auto C = std::forward<std::future<AB>&&>(A);
	return C;
}
/* */ 
int main() {
	QuantamBit<double>::ShardQuantamBit B = std::make_shared<QuantamBit<double>>();
	SBool SB = std::make_shared<bool>(true);

	B->Free();

	B->Install("System");
	B->Install("SubSystem");
	B->Install("Minion");
	B->Install("DA");
	B->Install("DB");
	B->Install("DC");
	auto F = [](auto In, auto BB) {static std::mutex M; while (*BB) { std::lock_guard<std::mutex> L(M); In->Update(); }return *BB; };
	auto F2 = [](auto In, auto BB) {static std::mutex M; while (*BB) { std::lock_guard<std::mutex> L(M); In->UpdateII(); }return *BB; };
	auto F3 = [](auto In, auto BB) {static std::mutex M; while (*BB) { std::lock_guard<std::mutex> L(M); In->UpdateIII(); /**/for (const auto& o : In->GetStrings()) { std::cout << o << ","; } /**/ }return *BB;
};
	auto A = std::async(std::launch::async, F3, B, SB);

	std::this_thread::sleep_for(std::chrono::seconds(3));

	while (*SB) {
		if (KeyIn() == ' ') { *SB = false; }
		std::cout << B->Get() << std::endl;
	}

	//*SB = false;

	return 0;
}
/** /
int main(){
	QuantamBit<double>::ShardQuantamBit B = std::make_shared<QuantamBit<double>>();
	SBool SB = std::make_shared<bool>(true);
	auto A = NewAsync<double,bool>(B, SB);//compile error. why??
}
/**/
/** /
int main() {
	QuantamBit<double> B;
	B.Update();
	B.Update();
	B.Update();

	bool F = true;

	auto FF = [&]() {
		//QuantamBit<double> B;
		while (F) { B.Update(); }
		return B; };

	auto A = std::async(std::launch::async, FF);

	std::this_thread::sleep_for(std::chrono::seconds(3));
	F = false;

}
/**/