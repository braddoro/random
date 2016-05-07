#!/usr/bin/php
<?php
$datetime1 = new DateTime('now');
function isPrime($num) {
    //1 is not prime. See: http://en.wikipedia.org/wiki/Prime_number#Primality_of_one
    if($num == 1)
        return false;

    //2 is prime (the only even number that is prime)
    if($num == 2)
        return true;

    /**
     * if the number is divisible by two, then it's not prime and it's no longer
     * needed to check other even numbers
     */
    if($num % 2 == 0) {
        return false;
    }

    /**
     * Checks the odd numbers. If any of them is a factor, then it returns false.
     * The sqrt can be an aproximation, hence just for the sake of
     * security, one rounds it to the next highest integer value.
     */
    for($i = 3; $i <= ceil(sqrt($num)); $i = $i + 2) {
        if($num % $i == 0)
            return false;
    }

    return true;
}
$options = getopt("l:");
$loops = isset($options['l']) ? $options['l'] : 15;
$sequence = 1;
$current = 1;
$last = 0;
$primeCount = 0;
for ($l = 1; $l <= $loops; $l++) {
    $prime2 = isPrime($sequence);
    ($prime2) ? $primeCount++ : $primeCount;
    echo "$l\t$prime2\t" . number_format($sequence,0,'.',',') . PHP_EOL;
    $last = $sequence;
    $sequence = $sequence + $current;
    $current = $last;
}
$datetime2 = new DateTime('now');
$diff = $datetime2->diff($datetime1);
echo 'Elapsed: ' . $diff->s . PHP_EOL;
echo 'Prime count: ' . $primeCount . PHP_EOL;
//print_r($diff);
?>