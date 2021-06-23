# DF-Gladiator-Placer
Takes a .txt file of Dwarf Fortress race/skill/equipment combos and creates an AutoHotKey program that creates them in-game, two-by-two, to enable duels.

Formatting should be

Race Sex (sex not necessary if the race has man or woman at the end)
FULL equipment name(order may be swapped with skills or intermingled between several; note that gauntlets don't count as pairs)
general skill name skill value
whitespace

So a pair of gladiators would look something like like

Dwarf M
Dodger 1
Axeman 2
Iron helm
Discipline 1
Steel battle axe

Adder Woman
Fighter 2
Wrestler 3
Steel helm
Iron Gauntlet
Iron Gauntlet

Comments can be added by adding // to the start of a line, and act as whitespace. They are unfortunately not entirely true to what comments are due to acting as whitespace, so adding a comment in the middle of a gladiator like

Dwarf M
Dodger 1
Axeman 2
//comment here
Iron helm
Discipline 1
Steel battle axe

would still break the code. However, you can have multiple lines of whitespace between gladiators, so


//Urist the Dwarf, not updated yet!
Dwarf M
Dodger 1
Axeman 2
Iron helm
Discipline 1
Steel battle axe


//Snake Queen the third, Updated for next round
Adder Woman
Fighter 2
Wrestler 3
Steel helm
Iron Gauntlet

would be perfectly fine. Also note that nothing is case sensitive.

After you have put your text into gladiators.txt, run the program and it will place the first two gladiators, run the fight, and then pause. Press d to place the next set, and it will run that fight before pausing again.


