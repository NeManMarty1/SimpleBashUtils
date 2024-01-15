SUCCESS=0
FAIL=0
VERDICT=""

for flag in -b -n -e -s -t -v
do
	./s21_cat $flag test.txt > s21_cat.txt
	cat $flag test.txt > cat.txt
	VERDICT="$(diff -s s21_cat.txt cat.txt)"
	if [ "$VERDICT" == "Files s21_cat.txt and cat.txt are identical" ]
	  then
	    (( SUCCESS++ ))
	  else
	    echo "flag $flag not passed"
	    (( FAIL++))
	fi
	rm s21_cat.txt cat.txt
done

echo "passed $SUCCESS tests"
echo "failed $FAIL tests"