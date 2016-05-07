#!/usr/bin/php
<?php
//http://tinsology.net/scripts/php-lorem-ipsum-generator/
$options = getopt("l:");
$loops = isset($options['l']) ? $options['l'] : 10;

require('LoremIpsum.class.php');
$lig = new LoremIpsumGenerator;

for ($l = 0; $l <= $loops; $l++) {
    echo $lig->getContent(rand(10,50), 'plain', false) . PHP_EOL . PHP_EOL;
}

function RandomString($rand) {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $randstring = '';
    for ($i = 0; $i <= $rand; $i++) {
        $randstring .= $characters[rand(0, strlen($characters))];
    }
    $randstring .= PHP_EOL;
    return $randstring;
}
