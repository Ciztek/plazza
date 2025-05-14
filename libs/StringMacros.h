#pragma once

#define lengthof(sstr) (sizeof (sstr) - (sizeof (char)))
#define sstr_unpack(sstr) (sstr), lengthof((sstr))
#define sstr_runpack(sstr) lengthof((sstr)), (sstr)
