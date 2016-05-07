<?php
// http://simplehtmldom.sourceforge.net/
// https://www.google.com/?q=php%20array
require('simple_html_dom.php');
// Create DOM from URL or file
$html = file_get_html('http://www.google.com/');
//$html = file_get_html('http://www.imdb.com/title/tt4126304/');

//echo $html . PHP_EOL;

// Find all images
foreach($html->find('img') as $element) {
    echo $element->src . PHP_EOL;
}

// Find all links
foreach($html->find('a') as $element) {
    echo $element->href . PHP_EOL;
}

?>