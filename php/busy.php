#!/usr/bin/php
<?php
require('LoremIpsum.class.php');
//http://tinsology.net/scripts/php-lorem-ipsum-generator/
$options = getopt("l:c:");
$loops = isset($options['l']) ? $options['l'] : 100;
$chars = isset($options['c']) ? $options['c'] : 10;
$versi = isset($options['v']) ? $options['v'] : '';
$hash = round($loops*.1);
function RandomString($rand) {
    //$characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $characters = '0123456789abcdefghijklmnopqrstuvwxyz';
    $randstring = '';
    for ($i = 0; $i <= $rand; $i++) {
        $foo = rand(0,strlen($characters)-1);
        $randstring .= $characters[$foo];
    }
    $randstring .= PHP_EOL;
    return $randstring;
}
$lig = new LoremIpsumGenerator;
$file = basename(__FILE__,'.php')."$versi.data";
$fp = fopen($file, 'w');
$list = '';
for ($l = 0; $l <= $loops; $l++) {
    if($l % $hash == 0) {
        echo 'Generating... ' . $l . PHP_EOL;
    }
    //$text = $lig->getContent(rand($chars,$chars), 'plain', false);
    $text = RandomString(rand($chars,$chars)) . ' ';
    $list .= $text;
}
$alist = explode(' ',$list);
$aword = array();
$l = 0;
$hash = round(count($alist)*.1);
foreach($alist as $word) {
    if($l % $hash == 0) {
        echo 'Parsing... ' . $l . PHP_EOL;
    }
    $word = trim($word);
    $word = str_replace('.', '', $word);
    $word = str_replace(',', '', $word);
    $word = str_replace('"', '', $word);
    if (strlen($word) > $chars) {
        if (array_key_exists($word, $aword)){
            $aword[$word]++;
            //echo $word . "\t" . $aword[$word] . PHP_EOL;
        } else {
            $aword[$word] = 1;
        }
    }
    $l++;
}
asort($aword);
$l = 0;
$hash = round(count($aword)*.1);
foreach($aword as $word => $value) {
    if($l % $hash == 0) {
        echo 'Looping... ' . $l . PHP_EOL;
    }
    fwrite($fp, "$word : $value" . PHP_EOL);
    $l++;
}
fclose($fp);