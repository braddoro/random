<?php
// https://www.google.com/advanced_search
// https://moz.com/ugc/the-ultimate-guide-to-the-google-search-parameters
$options = getopt("p:n:");
$parms = isset($options['p']) ? $options['p'] : '';
$num = isset($options['n']) ? $options['n'] : 10;
require_once('simple_html_dom.php');
//$url  = 'http://www.google.com/search?num=10&hl=en&safe=active&tbo=d&site=&source=hp&q=Beautiful+Bangladesh&oq=Beautiful+Bangladesh';
$parms = str_replace(" ", "+", $parms);
$url  = "http://www.google.com/search?hl=en&safe=active&num=$num&q=$parms";
$html = file_get_html($url);
$linkObjs = $html->find('h3.r a');
$loop = 1;
echo PHP_EOL;
foreach ($linkObjs as $linkObj) {
    $title = trim($linkObj->plaintext);
    $link  = trim($linkObj->href);
    // if it is not a direct link but url reference found inside it, then extract
    if (!preg_match('/^https?/', $link) && preg_match('/q=(.+)&amp;sa=/U', $link, $matches) && preg_match('/^https?/', $matches[1])) {
        $link = $matches[1];
    } else if (!preg_match('/^https?/', $link)) { // skip if it is not a valid link
        continue;
    }
//    echo "$loop.\tTitle: $title" . PHP_EOL;
//    echo "\tLink.: $link" . PHP_EOL . PHP_EOL;
    echo html_entity_decode("Title: $title") . PHP_EOL;
    echo "$link" . PHP_EOL . PHP_EOL;
    $loop++;
}
?>