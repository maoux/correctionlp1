export LP7=/sys/kernel/debug/fortytwo
echo "cat $LP7/id: ";cat $LP7/id;echo ""
echo "cat $LP7/foo: ";cat $LP7/foo;echo ""
echo "cat $LP7/jiffies: ";cat $LP7/jiffies;echo ""
echo "echo -n \"toto\" > $LP7/id: ";echo -n "toto" > $LP7/id
echo "echo -n \"tototot\" > $LP7/id: ";echo -n "tototot" > $LP7/id
echo -n "heynard" > $LP7/id
echo "echo -n \"heynard\" > $LP7/id :";echo $?
echo "echo -n \"titi\" > $LP7/jiffies: ";echo -n "titi" > $LP7/jiffies
echo -n "titi" > $LP7/foo
echo "cat $LP7/foo: ";cat $LP7/foo;echo ""
echo -n "tutu" > $LP7/foo
echo "cat $LP7/foo: ";cat $LP7/foo;echo ""
