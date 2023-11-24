# verifica daca primeste exact un argument
if [ "$#" -ne 1 ]
then echo "eroare, numarul de argumente nu este bun!"
     exit 1
fi

#initializeaza contorul
count=0

# citeste de la stdin pana la EOF (Ctrl+D)
while read line
do
    echo $line | grep -E '^[A-Z][A-Za-z?!,\. ]*' | grep -v ', si' | grep -v ',si' | grep -E '.*(\?|\!|\.)$'
    if [ "$?" -eq 0 ]
    then echo $line | grep -E $1
	 if [ "$?" -eq 0 ]
	 then count=$((count+1))
	 fi
    fi
done

echo $count

