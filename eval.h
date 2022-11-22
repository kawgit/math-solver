#include "stdint.h"
#include "system.h"

typedef int Eval;

int count_funcs(const Node* node);

Eval simp_eval(Equation& equ);