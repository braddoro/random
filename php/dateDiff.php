#!/usr/bin/php
<?php
$options = getopt("e:s:");
$end = isset($options['e']) ? $options['e'] : date("Y-m-d");
$start = isset($options['s']) ? $options['s'] : date("Y-m-d");
function dateDiff($d1, $d2) {
  return round(abs(strtotime($d1)-strtotime($d2))/86400);
}
$days = dateDiff($start,$end);
echo "$days days between $start and $end.";
?>