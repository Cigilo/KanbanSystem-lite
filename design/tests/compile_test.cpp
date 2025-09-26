// Este arquivo inclui apenas os headers para garantir que os .h compilam.
#include "../include/domain/ActivityLog.h"
#include "../include/domain/Board.h"
#include "../include/domain/Card.h"
#include "../include/domain/Column.h"
#include "../include/domain/User.h"
#include "../include/interfaces/IFilter.h"
#include "../include/interfaces/IRepository.h"
#include "../include/interfaces/IService.h"
#include "../include/interfaces/IView.h"
#include "../include/persistence/FileRepository.h"
#include <iostream>


int main() {
// Não criamos objetos complexos aqui — apenas garantimos que os headers são válidos.
int x ;
std::cout << "Headers included successfully\n";
std::cout<<"\n digite qualquer numero: \n\n";
std::cin >> x ;
return 0;
}