#!/usr/bin/php
<?php
try {
    $datetime1 = new DateTime('now');
    $dow = $datetime1->format('weekday');
    $hour = $datetime1->format('H');
    $minute = $datetime1->format('i');
    if ($dow == 0 && $hour < 18 || ($hour == 18 && $minute < 30) ) {
        $datetime2 = new DateTime('today 18:30');
    }else{
        $datetime2 = new DateTime('next monday 18:30');
    }
    $interval = $datetime1->diff($datetime2);
    $d = $interval->format('%d');
    $h = $interval->format('%H');
    $m = $interval->format('%I');
    $s = $interval->format('%S');
    $daysecs = $d*(24*3600);
    $seconds = $daysecs + $s + ($m*60) + ($h*3660);
    $minutes = $seconds/60;
    $hours = $minutes/60;
    $prefix = 'Only ';
    $suffix = 'until Stella time!';
    echo $prefix . number_format($d)        . ' days '    . $suffix . PHP_EOL;
    echo $prefix . number_format($hours,2)  . ' hours '   . $suffix . PHP_EOL;
    echo $prefix . number_format($minutes)  . ' minutes ' . $suffix . PHP_EOL;
    echo $prefix . number_format($seconds)  . ' seconds ' . $suffix . PHP_EOL;
} catch (Exception $e) {
    echo PHP_EOL . ' - Caught exception: ',  $e->getMessage(), PHP_EOL;
}
?>