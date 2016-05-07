<?php
$lang = "en";
$path = "lang/$lang/cmr/";
function dupe_check($lang, $path) {
    $dir = rtrim($path, '\\/');
    $result = array();
    foreach (scandir($dir) as $f) {
        if ($f !== '.' and $f !== '..') {
            if (is_dir("$dir/$f")) {
                $result = array_merge($result, ListIn("$dir/$f", "$prefix$f/"));
            } else {
                $file = $path . $prefix.$f;
                //$html .= "<SCRIPT>" . file_get_contents($file) . "</SCRIPT>\n";
            }
        }
    }
}
dupe_check($lang, $path);
?>
