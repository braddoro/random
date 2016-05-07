<?php
// Recursion.
//
function drink($sip) {
    if ($sip <= 0) {die("empty\n");}
    print "$sip\n";
    $sip--;
    drink($sip);
}
drink(22);
?>