#!/usr/bin/php
<?php
function cleanString($in){
    $clean = trim($in);
    $clean = str_replace('  ', '',$clean);
    $clean = str_replace('  ', '',$clean);
    $clean = str_replace('  ', '',$clean);
    $clean = str_replace('...', ': ',$clean);
    $clean = substr($clean,1,strlen($clean));
    $clean = ucfirst(strtolower($clean));
    return $clean;
}
exec('weather -f 28027', $result, $return);
$day = '';
foreach($result as $line) {
    if(substr(trim($line),0,1) == '.') {
        if(strpos($day,'*****') > 0) { // Throw away the header.
            $day = $line;
            continue;
        }
        $clean = cleanString($day);
        echo($clean . PHP_EOL);
        $day = '';
    }
    $day .= $line;
}
$clean = cleanString($day);
print_r($clean . PHP_EOL);
?>