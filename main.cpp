//NordicNooob's Gladiator Placer
//Takes a semi-ordred list of gladiators and places them in the arena in pairs.


#include <string> //Gives us access to strings
#include <fstream> //Reading and writing to files
#include <iostream> //Input and output. Do we even use this, actually?
#include <sstream> //For stringstreams
#include <locale> //Gives us tolower, which makes us not worry about case when reading gladiator info
#include <algorithm> //Gives us transform

using namespace std;

int GladiatorToCode(ifstream& fin, ofstream& fout); //This function takes the input gladiator file, the output ahk file, and the input placeholder variable (which already has a line in it) and uses them to turn one gladiator into a set of key presses to make that gladiator in Dwarf Fortress.

int LineDeterminer (const string& line);

int GladiatorFinalizer(ofstream& fout);

void GladiatorInitializer (ofstream& fout, string& line);

void SkillAssembler (ofstream& fout, string& line);

int FindSkill(string skill_type); //Input a lowercase string and see if it matches any of the below skills. If it does, return a number corresponding to the number of down key presses needed to reach it in the creature creator.

void EquipmentAssembler(ofstream& fout, string& line);

void MakeHeader (ofstream& fout);

void Refresh (ofstream& fout);

void ToAHK(string& convert);

const string skills[29] {"fighter", "archer", "axeman", "swordsman", "knife user", "maceman", "hammerman", "spearman", "pikeman", "lasher", "crossbowman", "bowman", "blowgunner", "wrestler", "striker", "kicker", "biter", "dodger", "shield user", "armor user", "thrower", "ambusher", "swimmer", "observer", "discipline", "tracker", "climber", "rider", "misc object user"};

int main () {
    ifstream fin;
    fin.open("gladiators.txt"); //Open file with gladiators list
    if (!fin.is_open()) return -1; //Return an error if we couldn't open the file
    bool quit = false;
    ofstream fout("placegladiators.ahk"); //Create the ahk file
    MakeHeader(fout);
    while (!quit) {
        if (GladiatorToCode(fin, fout) == -1) break; //With the open file, read the first gladiator. If we reach eof before we can read two gladiators, we got problems. Quit the loop so we don't add extra commands to the AHK file.
        else fout << "Send +{Down}{Down}{Down} \n"
        "Sleep, 100 \n";
        if (GladiatorToCode(fin, fout) == -1) quit = true;
        fout << "Send {Space} \n"
        "Sleep, 5000 \n"
        "Send {Space} \n"
        "Sleep, 100 \n"
        "KeyWait, n, D \n"; //Start the fight, pause after five seconds, and wait for the user's prompt to create the next gladiator.
        
        if (fin.eof()) quit = true; //Probably redundant, but just double check and make sure we're not at eof somehow
        if (!quit) Refresh(fout);
    }
    //system("placegladiators.ahk");
    fout.close();
    fin.close();
    return 0;
}

int GladiatorToCode(ifstream& fin, ofstream& fout) {
    bool lcr = false; //last-case-real: if the previous line had info on it, then it was the last line of a gladiator. Create the gladiator.
    string line; //this is a placeholder for getline to store each line of text in
    while (getline(fin, line)) {
        if (fin.eof()) {
            GladiatorFinalizer(fout);
            return -1;
        }; //If we've encountered the end of file, 
        transform(line.begin(), line.end(), line.begin(), ::tolower); //Set line to lowercase; keeps things from being case sensitive
        int line_type = LineDeterminer(line);
        switch(line_type) {
            case 0:
                if (lcr) return GladiatorFinalizer(fout);
                lcr = false;
                break;
            case 1:
                SkillAssembler(fout, line);
                lcr = true;
                break;
            case 2:
                GladiatorInitializer(fout, line);
                lcr = true;
                break;
            case 3:
                EquipmentAssembler(fout, line);
                lcr = true;
                break;
        }

    }
    return -1;
}
/* CODE DUMP!!! I'll use this eventually. Maybe.
system("placegladiators.ahk"); //This runs the autohotkey script we've been building; system is the command to write something into the command line.
if (line[0] == '/' && line[1] == '/') continue; //Skip the line if it's a comment, which I've decided should be //, the same as it is in c++. I'm too lazy to implement proper comments that can be placed at the ends of lines, but we don't need them.
        else { //For leaving and re-entering the game
            fout << "
            Send, {Esc}{Up}{Enter} \n
            Send, {Down}{Down}{Enter}{Space}k+{Up}{Up}{Right}{Right}{Right}{Right}{Right} \n
            ";

*/
int LineDeterminer (const string& line) { //0 for empty, 1 for skill, 2 for race, 3 for equipment
    if ((line[0] == '/' && line[1] == '/') || line == "\0") return 0; //If the line is empty or a comment, return 0.
    stringstream ss;
    ss << line;
    string first_word, second_word;
    ss >> first_word >> second_word;
    if (FindSkill(first_word) != -1) return 1; //If the line is a match for a skill, return 1.
    if ((second_word == "man") || (second_word == "woman") || (second_word == "m") || (second_word == "f")) return 2; //If the second word is a gender, then the line must be a race
    else return 3; //Barring all other options, the line is an equipment. Return 3.
}

int GladiatorFinalizer(ofstream& fout) {
    fout << "Send, {Enter} \n"
    "Sleep, 100 \n";
    return 0;
}

void GladiatorInitializer (ofstream& fout, string& line) {
    stringstream ss;
    string species, gender; //Placeholders for species and gender
    ss << line;
    ss >> species >> gender; //The species of each gladiator is split into two components.
    fout << "Send, c{Right}b{Right}bfewe{Backspace}{Backspace}{Backspace} \n"
    "Sleep, 100 \n"; //Clear the existing gladiator's stuff and reset cursor position
    fout << "Send, " << species << " \n"
    "Sleep, 100 \n";
    if (gender == "woman" || gender == "man") { //Animal men just have woman or man typed into seach bar.
        fout << "Send, {Space}" << gender << " \n"
        "Sleep, 100 \n";
    }
    else if (gender == "m") { //Males always one down press from females where not differentiable
        fout << "Send, {Enter}{Down} \n"
        "Sleep, 100 \n";
    }
    if (species == "elf") { //Elves do not show up first when elf is searched. Edge case. Fuck elves.
        if (gender == "f") fout << "Send, {Enter} \n";
        fout << "Send, {Down}{Down} \n"
        "Sleep, 100 \n";
    } 
    fout << "Send, {Enter}{Right} \n"
    "Sleep, 100 \n";
}

void SkillAssembler (ofstream& fout, string& line) {
    stringstream ss;
    int skill_value;
    string skill_type;
    ss << line;
    ss >> skill_type >> skill_value;
    int down_presses = FindSkill(skill_type);
    fout << "Loop " << down_presses << " \n"
    "{ \n"
    "   Send, {Down} \n"
    "   Sleep, 100 \n"
    "} \n"
    "Sleep, 100 \n"; //This loop brings the cursor to the right skill.
    fout << "Loop " << skill_value << " \n"
    "{ \n"
    "    Send, {+} \n"
    "   Sleep, 100 \n"
    "} \n"
    "Sleep, 100 \n"; //This loop increments the skill a number of times equal to the skill value.
    fout << "Loop " << down_presses << " \n"
    "{ \n"
    "     Send, {Up} \n"
    "   Sleep, 100 \n"
    "} \n"
    "Sleep, 100 \n"; //This brings the cursor back to the top of the skill list
}

int FindSkill(string skill_type) {
    for (int i = 0; i < 29; i++) { //There are 29 skills. Look at each one, comparing them with our string.
        if (skill_type == skills[i]) return i;
    }
    return -1; //If the skill type is none of the possible skills, it must be an equipment.
}

void EquipmentAssembler(ofstream& fout, string& line) {
    ToAHK(line); //Convert line to proper format to be searched
    fout << "Send, n" << line << "{Right}{Enter} \n"
    "Sleep, 100 \n"; //Open search bar, write the equipment name, select what pops up.
}

void MakeHeader (ofstream& fout) {
    fout << "^!j:: ; Ctrl+Alt+j starts the script \n"
    "SendMode Input \n"
    "SetWorkingDir %A_ScriptDir%  \n"
    "Send, {Down}{Down}{Enter} \n"
    "Sleep, 10000 \n"
    "Send, k+{Left}+{Left}+{Left}{Left}+{Up}+{Up}+{Up} \n"
    "Loop 15 \n"
    "{ \n"
    "   Send, {Alt}{+} \n"
    "   Sleep, 100 \n"
    "} \n"; //Header stuff: start the game by entering the arena, moving to the proper location, and increasing FPS cap
}

void Refresh (ofstream& fout) {
    fout << "Send, {Esc}{Up}{Enter} \n"
    "Send, {Down}{Down}{Enter}{Space} \n"
    "Sleep, 10000 \n"
    "Send, {Space}k+{Up} \n"
    "Sleep, 100 \n"
    "Send, {Up}{Right}{Right} \n"
    "Sleep, 100 \n"
    "Send, {Right}{Right}{Right} \n"; //Leave the arena, re-enter, and re-position the cursor.
}

void ToAHK(string& convert) {
    for (int i = 0; i < convert.length(); i++) { //As long as we haven't reached the end of the string we're converting
        if (convert[i] == ' ') {
            convert.replace(i,1,"{Space}"); //check each character, and if it's a space, replace it with {Space}
            i += 6; //Iterate i by 6, so we don't uselessly iterate through the newly added characters that spell out {Space}
        }
    }
}

/*TODO:
Loop properly for play/pause feature and make sure you place two gladiators.

Make code read gladiators based off of if there is a race, not whitespace-based like it is currently

Multiples-of equipment problem: implement numbers at the end for qty

Make comments able to be end-of-line things

Make alternate skill names able to be picked up; ie axedwarf, maceaxoltol, etc...

Possibly get the program to go through the motions of copying combat logs and renaming, saving each as their own .txt file

Error messages for eof when gladiator expected

Can I detect if you're already in the arena somehow?

Arena 10 second timer is unreliable because it depends on computer power

Change unpause key from d

Script has trouble ending itself?

Gladiator 2 placed one too far left?!?!
*/