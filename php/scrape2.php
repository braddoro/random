<?php
// http://simplehtmldom.sourceforge.net/
// https://www.google.com/?q=php%20array
require('simple_html_dom.php');
// Create DOM from URL or file
//$html = file_get_html('https://www.informatica.com/products/data-quality/data-as-a-service/address-verification/interactive-country-map.html?#fbid=xcpGSL8gyt_?hashlink=country-list');
//$html = file_get_html('http://www.imdb.com/title/tt4126304/');

$ret = $html->find('#basicInfo')->plaintext;
var_dump($ret);

// Find all images
foreach($html->find('img') as $element) {
    //echo $element->src . PHP_EOL;
}

// Find all links
foreach($html->find('a') as $element) {
    //echo $element->href . PHP_EOL;
}

?>