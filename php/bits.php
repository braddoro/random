<?php
/* http://www.litfuel.net/tutorials/bitwise.htm */
$i_testmin = 0;
if (isset($argv[1])) {$i_testmin = $argv[1];}
$i_testmax = 0;
if (isset($argv[2])) {$i_testmax = $argv[2];}
for ($x=$i_testmin;$x<=$i_testmax;$x++)
	{
	echo "Parm: $x \t";
	for ($y=1;$y<=$x;$y++)
		{
		if($x & $y)
			{
			echo "$y ";
			}
		}
	echo "\n";
	}
?>