<?php
echo(date('H:i:s', time()) . "\n");
// php prx.php -l 438008 -o 'ATD Walmart'
$options = getopt("l:o:");
$location_number = isset($options['l']) ? $options['l'] : '';
$orderSourceName = isset($options['o']) ? $options['o'] : '';
try {
    ini_set('default_socket_timeout', 800);
    echo('Request.: ' . $location_number . "\t" . $orderSourceName . "\n");
    $client = new SoapClient("http://dbosb.atd-us.icd/PrxService/ProxyServices/PrxServicePS?WSDL",array('trace'=>1));
    $response = $client->getprxDetails(array('location_number' => $location_number, 'orderSourceName' => $orderSourceName));
    echo(date('H:i:s', time()) . "\n");
    echo('Response: ' . $response->location_number . "\t" . $response->orderSourceName . "\n");
    echo("Group\tPrec\tRule\n");
    echo("-----\t-----\t-----\n");
    foreach($response as $key => $productGroup){
        if (is_array($productGroup)) {
            foreach($productGroup as $node){
                if (is_object($node)) {
                    echo($node->productGroupId . "\t" . $node->productPrecedence . "\t" . $node->ruleType . "\n");
                }
            }
        }
    }
} catch (Exception $e) {
    echo $e->getMessage();
}
unset($response);
?>