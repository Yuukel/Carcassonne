#include <stdio.h>
#include "game_structures.h"
#include "lecteur_csv.h"

void AfficheTuileDbg(TuileStruct t);

int main(int argc, char * argv[])
{
    TuileStruct t = { 0, {"V", "R", "P", "R"}, "R" };

    AfficheTuileDbg(t);
    return 0;
    //parseur_csv();
}

void AfficheTuileDbg(TuileStruct t){
    printf("\t%s\t\n",t.cotes[0]);
    printf("%s\t%s\t%s\n", t.cotes[1], t.centre, t.cotes[2]);
    printf("\t%s\t\n",t.cotes[3]);
}