// EN LA CLASE
std::ostream& operator << (std::ostream& os, const Complejos& complejo) {
    os << complejo.real() << " " << complejo.imaginario() << "i";
    return os;
}

// EN EL MAIN
double real_1, real_2;
double imaginario_1, imaginario_2;
std::cin >> real_1 >> imaginario_1 >> real_2 >> imaginario_2;
Complejos complejo1(real1,imaginario_1), complejo2(real2,imaginario_2);

std::cout << complejo1 + complejo2 << std::endl; //imprime la suma
std::cout << complejo1 - complejo2 << std::endl; //imprime la resta