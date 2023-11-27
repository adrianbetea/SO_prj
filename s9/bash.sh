#!/bin/bash
# verifica daca primeste exact un argument
if [ "$#" -ne 1 ];
then echo "eroare, numarul de argumente nu este bun!"
     exit 1
fi

c="$1"
#initializeaza contorul
count=0

function propozitie_corecta {
    prop="$1"

    #daca propozitia nu incepe cu litera mare nu e corecta
    if [[ ! "$prop" =~ ^[A-Z] ]]; then
        return 1
    fi

    #daca propozitia nu contine caracterul dat ca argument nu e corecta
    if [[ ! "$prop" =~ $c ]]; then
        return 1
    fi

    #daca propozitia are virgula inainte de si nu e corecta
    if [[ "$prop" =~ ", si" || "$prop" =~ ",si" ]]; then
        return 1
    fi

    #daca propozitia nu se termina cu ?, ! sau . nu e corecta
    if [[ ! "$prop" =~ [!?\.]$ ]]; then
        return 1
    fi

    #altfel return 0, propozitia este corecta
    return 0


}

# citeste de la stdin pana la EOF (Ctrl+D)
while IFS= read prop; do
    #verifica daca propozitia e corecta si contine caracterul c
    if propozitie_corecta "$prop"; then
        count=$((count+1))
    fi
done

echo $count

