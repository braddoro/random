<?php

/*** make sure the file exists ***/
$fname = 'Book2.csv';
$cols = 55; // zero base

$fp = fopen($fname,'r') or die("can't open file");
//print "<table>\n";
$curr = 0;
while($csv_line = fgetcsv($fp,1024)) {
    //print '<tr>';
    for ($i = 0, $j = count($csv_line); $i < $j; $i++) {
        //print '<td>'.$csv_line[$i].'</td>';
       
    }
    if ($i != $cols) {
    	echo "$curr $i $cols \n";	
  	}
    //print "</tr>\n";
    $curr++;
}
//print '</table>';
fclose($fp) or die("can't close file");


?>