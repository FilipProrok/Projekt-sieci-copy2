// Includy:
#include <iostream>
#include <iomanip>
#include "ProstyUAR.h"
#include "Services.h"

#include <vector>
#include <cmath>
#include <queue>
#include <functional>

#define MAIN // ustaw na MAIN aby skompilować program docelowy / ustaw na DEBUG aby skompilować program testujacy

#ifdef DEBUG

//Funkcje pomocnicze dla testów:

void raportBleduSekwencji(std::vector<double>& spodz, std::vector<double>& fakt)
{
	constexpr size_t PREC = 3;
	std::cerr << std::fixed << std::setprecision(PREC);
	std::cerr << "  Spodziewany:\t";
	for (auto& el : spodz)
		std::cerr << el << ", ";
	std::cerr << "\n  Faktyczny:\t";
	for (auto& el : fakt)
		std::cerr << el << ", ";
	std::cerr << std::endl << std::endl;
}

bool porownanieSekwencji(std::vector<double>& spodz, std::vector<double>& fakt)
{
	constexpr double TOL = 1e-3;	// tolerancja dla porównań zmiennoprzecinkowych
	bool result = fakt.size() == spodz.size();
	for (int i = 0; result && i < fakt.size(); i++)
		result = fabs(fakt[i] - spodz[i]) < TOL;
	return result;
}

void myAssert(std::vector<double>& spodz, std::vector<double>& fakt)
{
	if (porownanieSekwencji(spodz, fakt))
		std::cerr << "OK!\n";
	else
	{
		std::cerr << "FAIL!\n";
		raportBleduSekwencji(spodz, fakt);
	}
}

// testy dla samego ARX:

namespace TESTY_ModelARX
{
	void wykonaj_testy();
	void test_brakPobudzenia();
	void test_skokJednostkowy_1();
	void test_skokJednostkowy_2();
	void test_skokJednostkowy_3();
}

void TESTY_ModelARX::wykonaj_testy()
{
	test_brakPobudzenia();
	test_skokJednostkowy_1();
	test_skokJednostkowy_2();
	test_skokJednostkowy_3();
}

void TESTY_ModelARX::test_brakPobudzenia()
{
	//Sygnatura testu:
	std::cerr << "ModelARX (-0.4 | 0.6 | 1 | 0 ) -> test zerowego pobudzenia: ";
	try
	{
		// Przygotowanie danych:
		ARX instancjaTestowa({ -0.4 }, { 0.6 }, 1, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja modelu:

		for (int i = 0; i < LICZ_ITER; i++) {
			faktSygWy[i] = instancjaTestowa.simulate(sygWe[i]); //i aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
		}
		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void TESTY_ModelARX::test_skokJednostkowy_1()
{
	//Sygnatura testu:
	std::cerr << "ModelARX (-0.4 | 0.6 | 1 | 0 ) -> test skoku jednostkowego nr 1: ";

	try
	{
		// Przygotowanie danych:
		ARX instancjaTestowa({ -0.4 }, { 0.6 }, 1, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362,
			           0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999,
			           1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++) {
			faktSygWy[i] = instancjaTestowa.simulate(sygWe[i]); //i aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
		}
		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}

}

void TESTY_ModelARX::test_skokJednostkowy_2()
{
	//Sygnatura testu:
	std::cerr << "ModelARX (-0.4 | 0.6 | 2 | 0 ) -> test skoku jednostkowego nr 2: ";

	try
	{
		// Przygotowanie danych:
		ARX instancjaTestowa({ -0.4 }, { 0.6 }, 2, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu, 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++) {
			faktSygWy[i] = instancjaTestowa.simulate(sygWe[i]);
		}
			

		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void TESTY_ModelARX::test_skokJednostkowy_3()
{
	//Sygnatura testu:
	std::cerr << "ModelARX (-0.4, 0.2 | 0.6, 0.3 | 2 | 0 ) -> test skoku jednostkowego nr 3: ";
	try
	{
		// Przygotowanie danych:
		ARX instancjaTestowa({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu, 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0, 0, 0.6, 1.14, 1.236, 1.1664, 1.11936, 1.11446, 1.12191, 1.12587, 1.12597, 1.12521, 1.12489, 1.12491, 1.12499, 1.12501, 1.12501, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++){
			faktSygWy[i] = instancjaTestowa.simulate(sygWe[i]);
		}

		// Weryfikacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}


// testy dla samego Regulatora PID:

namespace TESTY_RegulatorPID
{
	void wykonaj_testy();
	void test_P_brakPobudzenia();
	void test_P_skokJednostkowy();
	void test_PI_skokJednostkowy_1();
	void test_PI_skokJednostkowy_2();
	void test_PID_skokJednostkowy();
	void test_PI_skokJednostkowy_3();
}

void TESTY_RegulatorPID::wykonaj_testy()
{
	test_P_brakPobudzenia();
	test_P_skokJednostkowy();
	test_PI_skokJednostkowy_1();
	test_PI_skokJednostkowy_2();
	test_PID_skokJednostkowy();
	test_PI_skokJednostkowy_3();
}

void TESTY_RegulatorPID::test_P_brakPobudzenia()
{
	//Sygnatura testu:
	std::cerr << "RegP (k = 0.5) -> test zerowego pobudzenia: ";
	try
	{
		// Przygotowanie danych:
		RegulatorPID instancjaTestowa(0.5);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja modelu:

		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.calculate_u(sygWe[i]);

		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}



void TESTY_RegulatorPID::test_P_skokJednostkowy()
{
	//Sygnatura testu:
	std::cerr << "RegP (k = 0.5) -> test skoku jednostkowego: ";

	try
	{
		// Przygotowanie danych:
		RegulatorPID instancjaTestowa(0.5);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
			           0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.calculate_u(sygWe[i]);

		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void TESTY_RegulatorPID::test_PI_skokJednostkowy_1()
{
	//Sygnatura testu:
	std::cerr << "RegPI (k = 0.5, TI = 1.0) -> test skoku jednostkowego nr 1: ";

	try
	{
		// Przygotowanie danych:
		RegulatorPID instancjaTestowa(0.5, 1.0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 11.5, 12.5,
			           13.5, 14.5, 15.5, 16.5, 17.5, 18.5, 19.5, 20.5, 21.5, 22.5, 23.5,
			           24.5, 25.5, 26.5, 27.5, 28.5, 29.5 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.calculate_u(sygWe[i]);

		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void TESTY_RegulatorPID::test_PI_skokJednostkowy_2()
{
	//Sygnatura testu:
	std::cerr << "RegPI (k = 0.5, TI = 10.0) -> test skoku jednostkowego nr 2: ";

	try
	{
		// Przygotowanie danych:
		RegulatorPID instancjaTestowa(0.5, 10.0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,
			           2, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3, 3.1, 3.2, 3.3, 3.4 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.calculate_u(sygWe[i]);

		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void TESTY_RegulatorPID::test_PID_skokJednostkowy()
{
	//Sygnatura testu:
	std::cerr << "RegPID (k = 0.5, TI = 10.0, TD = 0.2) -> test skoku jednostkowego: ";

	try
	{
		// Przygotowanie danych:
		RegulatorPID instancjaTestowa(0.5, 10.0, 0.2);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0.8, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.1,
			           2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3, 3.1, 3.2, 3.3, 3.4 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.calculate_u(sygWe[i]);

		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void TESTY_RegulatorPID::test_PI_skokJednostkowy_3() 
{
	//Sygnatura testu:
	std::cerr << "RegPI (k = 0.5, TI = 10.0 -> 5.0 -> 10.0) -> test skoku jednostkowego nr 3: ";

	try
	{
		// Przygotowanie danych:
		RegulatorPID instancjaTestowa(0.5, 10.0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.8, 2, 2.2, 2.4,
			           2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 2.35, 2.45, 2.55, 2.65, 2.75, 2.85 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
		{
			if (i == LICZ_ITER * 1 / 5) // przelaczenie na wew. liczenie calki - nie powinno być zauważalane
				instancjaTestowa.setLiczCalk(RegulatorPID::LiczCalk::Wew);
			if (i == LICZ_ITER * 2 / 5) // zmiana stalej calkowania - powinna byc tylko zmiana nachylenia 
				instancjaTestowa.setStalaCalk(5.0);
			if (i == LICZ_ITER * 3 / 5) // przelaczenie na zew. liczenie calki - nie powinno być zauważalane
				instancjaTestowa.setLiczCalk(RegulatorPID::LiczCalk::Zew); //i=18 wywołanie, tam jest błąd (mamy 4.1 zamiast 3.0) 
			if (i == LICZ_ITER * 4 / 5) // zmiana stalej calkowania - powinien wsytapic skok wartosci i zmiana nachylenia 
				instancjaTestowa.setStalaCalk(10.0);
			faktSygWy[i] = instancjaTestowa.calculate_u(sygWe[i]);
		}
		// Uwaga przy poprawnej implementacji zmiany sposobu liczenia całki, nie powinno dość do sytuacji, gdy
		// zmiana sposobu liczenia powoduje skokową zmianę wartości sterowania. dla liczenia całki zwenetrznie
		// zmiana stalej calkowania powinna powodować skok. dla liczenia wewnętrznego, powinno sie tylko zmienic
		// nachylenie.

		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}


// testy dla pelnego UAR:

namespace TESTY_ProstyUAR
{
	void wykonaj_testy();
	void test_UAR_1_brakPobudzenia();
	void test_UAR_1_skokJednostkowyPID();
	void test_UAR_2_skokJednostkowyPID();
	void test_UAR_3_skokJednostkowyPID();
}

void TESTY_ProstyUAR::wykonaj_testy()
{
	test_UAR_1_brakPobudzenia();
	test_UAR_1_skokJednostkowyPID();
	test_UAR_2_skokJednostkowyPID();
	test_UAR_3_skokJednostkowyPID();
}

void TESTY_ProstyUAR::test_UAR_1_brakPobudzenia()
{
	//Sygnatura testu:
	std::cerr << "UAR_1 -> test zerowego pobudzenia: ";
	try
	{
		// Przygotowanie danych:
		RegulatorPID testPID(0.5, 5.0, 0.2);
		ARX testARX({ -0.4 }, { 0.6 });
		ProstyUAR instancjaTestowa(testARX, testPID);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja UAR:

		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void TESTY_ProstyUAR::test_UAR_1_skokJednostkowyPID()
{
	//Sygnatura testu:
	std::cerr << "UAR_1 PID -> test skoku jednostkowego: ";
	try
	{
		// Przygotowanie danych:
		RegulatorPID testPID(0.5, 5.0, 0.2);
		ARX testARX({ -0.4 }, { 0.6 });
		ProstyUAR instancjaTestowa(testARX, testPID);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0.0, 0.0, 0.54, 0.756, 0.6708, 0.64008, 0.729, 0.810437, 0.834499,
			           0.843338, 0.8664, 0.8936, 0.911886, 0.923312, 0.93404, 0.944929,
			           0.954065, 0.961042, 0.966815, 0.971965, 0.97642, 0.980096, 0.983143,
			           0.985741, 0.987964, 0.989839, 0.991411, 0.992739, 0.993865, 0.994818 
		             };

		// Symulacja UAR:

		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void TESTY_ProstyUAR::test_UAR_2_skokJednostkowyPID()
{
	//Sygnatura testu:
	std::cerr << "UAR_2 PID (k = 2) -> test skoku jednostkowego: ";
	try
	{
		// Przygotowanie danych:
		RegulatorPID testPID(0.5, 5.0, 0.2);
		ARX testARX({ -0.4 }, { 0.6 }, 2,0);
		ProstyUAR instancjaTestowa(testARX, testPID);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0.0, 0.0, 0.0, 0.54, 0.756, 0.9624, 0.87336, 0.841104, 0.771946, 0.821644,
			           0.863453, 0.93272, 0.952656, 0.965421, 0.954525, 0.955787, 0.957472,
			           0.969711, 0.978075, 0.985968, 0.987821, 0.989149, 0.989053, 0.990645,
			           0.992248, 0.994403, 0.995707, 0.996677, 0.997024, 0.997388
		             };
		// Symulacja UAR:

		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void TESTY_ProstyUAR::test_UAR_3_skokJednostkowyPID()
{
	//Sygnatura testu:
	std::cerr << "UAR_3 PID (kP=1.0,Ti=2.0) -> test skoku jednostkowego: ";
	try
	{
		// Przygotowanie danych:
		RegulatorPID testPID(1.0, 2.0, 0.2);
		ARX testARX({ -0.4 }, { 0.6 }, 1, 0);
		ProstyUAR instancjaTestowa(testARX, testPID);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0.0, 0.0, 1.02, 1.608, 1.1028, 0.41736, 0.546648, 1.20605, 1.43047,
			           0.999176, 0.615056, 0.799121, 1.21304, 1.26025, 0.939289, 0.748507,
			           0.927166, 1.17292, 1.14155, 0.921616, 0.843258, 0.990018, 1.12577,
			           1.068, 0.927024, 0.908125, 1.01702, 1.08484, 1.02618, 0.941508
		             };
		// Symulacja UAR:

		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

		// Walidacja poprawności i raport:
		myAssert(spodzSygWy, faktSygWy);
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

bool test_powtarzalnosc_wynikow_UAR() {

	try {
		//sprawdzenie czy dwie niezależne symulacje dają te same wyniki

		ARX modelARX_1({ -0.4 }, { 0.6 }, 1, 0);
		ARX modelARX_2({ -0.4 }, { 0.6 }, 1, 0);
		RegulatorPID regulator_1(0.5, 5.0, 0.2);
		RegulatorPID regulator_2(0.5, 5.0, 0.2);
		ProstyUAR uar_1(modelARX_1, regulator_1);
		ProstyUAR uar_2(modelARX_2, regulator_2);

		constexpr size_t LICZ_ITER = 30;

		std::vector<double> sygWe(LICZ_ITER);

		for (int i = 0; i < LICZ_ITER; i++) //1 od i=1
			sygWe[i] = !!i;

		for (int i = 0; i < LICZ_ITER; i++)
		{
			if (uar_1.symuluj(sygWe[i]) != uar_2.symuluj(sygWe[i]))
			{
				std::cerr << "TEST POWTARZALNOSC WYNIKOW UAR FAIL";
				return false;
			}
		}
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "TEST POWTARZALNOSC WYNIKOW UAR FAIL, NIEOCZEKIWANY WYJATEK " << e.what() << std::endl;
		return false;
	}
}

bool test_granice_wynikow_ARX() {

	try {
		//sprawdzenie czy model ARX poprawnie ogranicza wyniki do ustawionych granic
		ARX model({ 0.0 }, { 100.0 }, 1, 0.0);

		double topBorder = 5.0;
		model.setYBorderTop(topBorder);

		model.simulate(1.0);
		double result = model.simulate(1.0);

		if (result > topBorder)
		{
			std::cerr << "TEST GRANICE WYNIKOW ARX FAIL" << std::endl;
			return false;
		}
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "TEST GRANICE WYNIKOW ARX FAIL, NIEOCZEKIWANY WYJATEK " << e.what() << std::endl;
		return false;
	}
	
}


bool test_obliczenia_ARX() {
	try
	{
		// Test sprawdzający poprawność obliczeń modelu ARX dla prostego przypadku.

		std::vector<double> A = { -0.5 };
		std::vector<double> B = { 0.5 };
		int delay = 1;

		ARX model(A, B, delay, 0.0);

		// Krok 1 (t=1): u(0)=1. y(0) wciąż zależy od historii zerowej.
		// y(1) = 0.5*0 + 0.5*0 = 0 (zależnie od implementacji opóźnienia)
		model.simulate(1.0);

		// Krok 2 (t=2): u(1)=1. y(1)=0.
		// y(2) = 0.5*y(1) + 0.5*u(0) = 0 + 0.5 = 0.5
		model.simulate(1.0);

		// Krok 3 (t=3): u(2)=1. y(2)=0.5
		// y(3) = 0.5*y(2) + 0.5*u(1) = 0.5*0.5 + 0.5*1.0 = 0.25 + 0.5 = 0.75
		double wyjscie = model.simulate(1.0);

		double expectedY3 = 0.75;

		if (wyjscie != expectedY3)
		{
			std::cerr << "TEST OBLICZENIA ARX FAIL" << std::endl;
			return false;
		}
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "TEST OBLICZENIA ARX FAIL, NIEOCZEKIWANY WYJATEK " << e.what() << std::endl;
		return false;
	}

}

bool test_zerowe_wzmocnienie_PID() {
	//wszystkie parametry 0, wyjście powinno być zawsze 0
	try {
		RegulatorPID regulator(0.0, 0.0, 0.0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 1)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy
		// Symulacja skoku jednostkowego w chwili 0.
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = 1.0;
		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = regulator.calculate_u(sygWe[i]);
		// Walidacja poprawności i raport:
		for (size_t i = 0; i < LICZ_ITER; ++i) {
			if (faktSygWy[i] != spodzSygWy[i]) {
				std::cerr << "TEST ZEROWE WZMOCNIENIE PID FAIL" << std::endl;
				return false;
			}
		}
		return true;
	}
	catch (...)
	{
		std::cerr << "TEST ZEROWE WZMOCNIENIE PID FAIL (nieoczekiwany wyjątek)" << std::endl;
		return false; // Inny wyjątek niż oczekiwany
	}
	
}

bool test_dla_Ti_zero_PID() {
	try {
		RegulatorPID regulator(1.0, 0.0, 0.2);
		if (regulator.calculate_u(1.0) - 1.2 == 0) {
			return true;
		}
		else {
			std::cerr << "TEST DLA Ti=0 PID FAIL" << std::endl;
			return false;
		} 
	}
	catch (...) {
		std::cerr << "TEST DLA Ti=0 PID FAIL (nieoczekiwany wyjątek)" << std::endl;
		return false;
	}
}

bool test_generator_okres_T() {
	try {
		//sprawdzenie czy generator nie ustawi okresu T na 0
		Generator test_gen(0,0,0,0);
		if (test_gen.getPeriodT() == 0) {
			std::cerr << "TEST GENERATOR OKRES T FAIL, NIEPOPRAWNA WARTOSC POCZATKOWA" << std::endl;
			return false;
		}

		return true;
	}
	catch (std::exception& e) {
		std::cerr << "TEST GENERATOR OKRES T FAIL, NIEOCZEKIWANY WYJATEK " << e.what() << std::endl;
		return false;
	}
}

bool test_generator_symulowanie_sygnalu_sinusoidalnego() {

	try {
		Generator test_gen(10.0, 125, 1.0, 0.0);

		int LICZ_ITER = 125;

		std::vector<double> oczekiwane_dane = {0, 7.07, 10, 7.07, 0, -7.07, 0, -7.07};

		double eps = 0.01;

		for (int i = 0; i < LICZ_ITER; i++)
		{
			if ((test_gen.generateSignal(i) - oczekiwane_dane[i%oczekiwane_dane.size()])>eps)
			{
				std::cerr << "TEST GENERATOR SYMULOWANIE SYGNALU SINUSOIDALNEGO FAIL" << std::endl;
				return false;
			}
		}

		return true;
	}catch (std::exception& e) {
		std::cerr << "TEST GENERATOR SKOK JEDNOSTKOWY FAIL, NIEOCZEKIWANY WYJATEK " << e.what() << std::endl;
		return false;
	}
}

bool test_tozsamosc_sygnalu_z_services_i_uar() {
	try {
		ARX modelARX({ -0.4 }, { 0.6 }, 1, 0);
		RegulatorPID regulator(0.5, 5.0, 0.2);
		ProstyUAR uar(modelARX, regulator);
		Services services(&uar);

		if (services.getSimulationFromUAR(1.0) == uar.getYPrev()) {
			return true;
		}
		else {
			std::cerr << "TEST TOZSAMOSC SYGNALU Z SERVICES I UAR FAIL" << std::endl;
			return false;
		}
	}
	catch (...)
	{
		std::cerr << "TEST TOZSAMOSC SYGNALU Z SERVICES I UAR FAIL (nieoczekiwany wyjątek)" << std::endl;
		return false;
	}
}

bool test_symulacji_przez_services() {
	
	try
	{
		RegulatorPID testPID(1.0, 2.0, 0.2);
		ARX testARX({ -0.4 }, { 0.6 }, 1, 0);
		ProstyUAR instancjaTestowa(testARX, testPID);
		Services services(&instancjaTestowa);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      
		std::vector<double> spodzSygWy(LICZ_ITER); 
		std::vector<double> faktSygWy(LICZ_ITER);  

		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0.0, 0.0, 1.02, 1.608, 1.1028, 0.41736, 0.546648, 1.20605, 1.43047,
					   0.999176, 0.615056, 0.799121, 1.21304, 1.26025, 0.939289, 0.748507,
					   0.927166, 1.17292, 1.14155, 0.921616, 0.843258, 0.990018, 1.12577,
					   1.068, 0.927024, 0.908125, 1.01702, 1.08484, 1.02618, 0.941508
		};
		
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = services.getSimulationFromUAR(sygWe[i]);

		for (int i = 0; i < LICZ_ITER; i++) {
			if (fabs(faktSygWy[i] - spodzSygWy[i]) > 0.0001) {
				std::cerr << "TEST SYMULACJI PRZEZ SERVICES: FAIL" << std::endl;
				return false;
			}
		}
		return true;
	}
	catch (...)
	{
		std::cerr << "TEST SYMULACJI PRZEZ SERVICES: FAIL" << std::endl;
	}
}

bool test_zmiany_parametrow_PID_przez_services() {

	try {
		ARX modelARX({ -0.4 }, { 0.6 }, 1, 0);
		RegulatorPID regulator(0.5, 5.0, 0.2);
		ProstyUAR uar(modelARX, regulator);

		Services services(&uar);

		services.set_PID_parameters(1.0, 10.0, 0.5, RegulatorPID::LiczCalk::Zew);

		if (services.get_PID_Wzmocnienie() != 1.0 ||services.get_PID_StalaCalk() != 10.0 || services.get_PID_StalaRoz() != 0.5 || services.get_PID_TypCalkowaniaPID() != RegulatorPID::LiczCalk::Zew)
		{
			std::cerr << "TEST ZMIANY PARAMETROW PID PRZEZ SERVICES FAIL" << std::endl;
			return false;
		}
		return true;
	}
	catch (...) {
		std::cerr << "TEST ZMIANY PARAMETROW PID PRZEZ SERVICES FAIL (nieoczekiwany wyjątek)" << std::endl;
		return false;
	}
}


void wykonaj_testy_dodatkowe() {
	std::vector<std::function<bool()>> dodatkowe_testy = {
		test_powtarzalnosc_wynikow_UAR,
		test_granice_wynikow_ARX,
		test_obliczenia_ARX,
		test_zerowe_wzmocnienie_PID,
		test_dla_Ti_zero_PID,
		test_generator_okres_T,
		test_generator_symulowanie_sygnalu_sinusoidalnego,
		test_tozsamosc_sygnalu_z_services_i_uar,
		test_symulacji_przez_services,
		test_zmiany_parametrow_PID_przez_services
	};

	int attempted = 0;
	int passed = 0;

	for (const auto& test : dodatkowe_testy) {
		attempted++;
		if (test()) passed++;
	}

	std::cerr << "Dodatkowe testy: " << passed << " / " << attempted;
}

int main()
{
	TESTY_ModelARX::wykonaj_testy();
	TESTY_RegulatorPID::wykonaj_testy();
	TESTY_ProstyUAR::wykonaj_testy();
	std::cout << "\n";
	wykonaj_testy_dodatkowe();
}

#endif


// #ifdef MAIN


// int main()
// {
// 	ARX model({ -0.4 }, { 0.6 }, 1, 0.0);
// 	std::cout << model.simulate(0.0) << std::endl;
// 	std::cout << model.simulate(1.0) << std::endl;
// 	std::cout << model.simulate(1.0) << std::endl;
// 	std::cout << model.simulate(1.0) << std::endl;
// 	std::cout << model.simulate(1.0) << std::endl;
// 	std::cout << model.simulate(1.0) << std::endl;
// 	std::cout << model.simulate(1.0) << std::endl;

// }

// #endif

