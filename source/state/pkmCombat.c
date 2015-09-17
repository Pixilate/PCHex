#include <stdlib.h>
#include <stdio.h>
#include <3ds.h>

#include "../pchex.h"
#include "state.h"

char helpstringsCom[20][3][50] = {
  { "Up/Down/Left/Right : Choose field",
    "A : Select field ",
    "Select : Save  | Start : Back" },
  {"","","B : Leave field"},
  {"","Up/Down : +/- 1 | Left/Right : Min/Max","B : Leave field"},
  {"X : +1","Up/Down : +/- 4 | Left/Right : Min/Max","B : Leave field"},
  {"","Up/Down : +/- 1 | Left/Right : Min/Max","B : Leave field"},
  {"X : +1","Up/Down : +/- 4 | Left/Right : Min/Max","B : Leave field"},
  {"","Up/Down : +/- 1 | Left/Right : Min/Max","B : Leave field"},
  {"X : +1","Up/Down : +/- 4 | Left/Right : Min/Max","B : Leave field"},
  {"","Up/Down : +/- 1 | Left/Right : Min/Max","B : Leave field"},
  {"X : +1","Up/Down : +/- 4 | Left/Right : Min/Max","B : Leave field"},
  {"","Up/Down : +/- 1 | Left/Right : Min/Max","B : Leave field"},
  {"X : +1","Up/Down : +/- 4 | Left/Right : Min/Max","B : Leave field"},
  {"","Up/Down : +/- 1 | Left/Right : Min/Max","B : Leave field"},
  {"X : +1","Up/Down : +/- 4 | Left/Right : Min/Max","B : Leave field"},
  {"","","B : Leave field"},
};

void 	pkmCombatHelp(t_stinf *state)
{
  u8	sel = state->inState;

  for (int i = 0; i < 40; i++)
    printf("-");
  if (!state->inSel)
    sel = 0;
  printf("%-40s", helpstringsCom[sel][0]);
  printf("%-40s", helpstringsCom[sel][1]);
  printf("%-40s", helpstringsCom[sel][2]);
}

void 	pkmCombatInit(t_stinf *state)
{
  state->inState = 1;
  state->inSel = 0;
  consoleClear();
}

void 	statField(t_stinf *state, s8 tstate, s8 stat)
{
  char	str[6][4] = {"HP", "ATK", "DEF", "SPE", "SPA", "SPD"};

  resetColor();
  printf("%-3s: %-3d\n", str[stat], state->pkm.stat[stat]);
  printf("    ");
  selectColor(tstate, state->inState, state->inSel);
  printf("IV : %-2d", getPkmIV(state->pkm.pkx.individualValues, stat));

  resetColor();printf("    ");
  selectColor(tstate + 1, state->inState, state->inSel);
  printf("EV : %-3d\n", state->pkm.pkx.effortValues[stat]);
}


void 	pkmCombatDisplay(t_stinf *state)
{
  struct s_pkx *pkx = &state->pkm.pkx;
  u8	ist = state->inState, sel = state->inSel;

  printf("\x1B[0;0H");
  printf("\x1B[2mGeneral\x1B[0m\x1B[0;17HCombat\n");
  printf("\x1B[1;17H<<L R>>\n");
  printf("Box %-2d Slot %-2d\n", state->pkmSlot / 30 + 1, state->pkmSlot % 30 + 1);
  if (state->modded) printf("\x1B[31mModified\x1B[0m");
  else printf("%-8s", "");
  printf("\n");
  
  selectColor(1, ist, sel);
  printf("Ability : %14s (%d)\n", pkData.abilities[pkx->ability], pkx->abilityNum);
  printf("\n");

  for (int i = 0; i < 6; i++)
    statField(state, (i + 1) * 2, i);

  resetColor();
  printf("\x1B[26;0H");
  pkmCombatHelp(state);
}

void 	pkmCombatInput(t_stinf *state)
{
  u32 	kPressed = state->kPressed;

  if (kPressed & KEY_START)
  {	
    switchState(state, pkmSelectState);
    return;
  }
  if (kPressed & KEY_SELECT)
  {
    savePokemon(state, state->pkmSlot, (u8 *)&state->pkm.pkx);
    state->modded = 0;
  }
  if (kPressed & KEY_L)
  {
    switchState(state, pkmGeneralState);
    return;
  }
  pkmComInputField(state);
}

struct s_UIState pkmCombatState = {&pkmCombatInit, &pkmCombatDisplay, &pkmCombatInput};
