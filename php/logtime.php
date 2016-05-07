#!/usr/bin/php
<?php
$options = getopt("u:p:t:h:d:gi");
$user = isset($options['u']) ? $options['u'] : 'bhughes';
$pass = isset($options['p']) ? $options['p'] : '';
$date = isset($options['d']) ? $options['d'] : time();
$tick = isset($options['t']) ? $options['t'] : 'NDT-431';
$hour = isset($options['h']) ? $options['h'] : 8;
$disp = isset($options['g']) ? true : false;
$info = isset($options['i']) ? true : false;
if($info) {
    $params = "param help:
    u : username
    p : password
    d : date (01/01/2015)
    t : ticket number
    h : hours in a day
    g : go mode (non test)
    i : this output
    example call: php logtime.php -u bhughes -p password -d 04/10/2015 -t EAS-1234 -g
    ";
 echo($params);
 exit();
}
try {
    $tickets =  explode(',',$tick);
    $workday = strtotime($date);
    $time =  round((($hour*60)/count($tickets)));
    $timeSpent = $time . 'm';
    $timeInSeconds = $time * 60;
    $wsdl = 'http://jira.prod.icd/rpc/soap/jirasoapservice-v2?wsdl';
    $soapClient = new SoapClient($wsdl);
    $auth = $soapClient->login($user, $pass);
    $worklog = new StdClass;
    $worklog->comment = '';
    $worklog->timeSpent = $timeSpent;
    $worklog->timeSpentInSeconds = $timeInSeconds;
    $worklog->startDate = $workday;
    $file = basename(__FILE__,'.php').'.log';
    $fp = fopen($file, 'a');
    foreach ($tickets as $ticket) {
        $output = "$ticket\t" . date("Y-m-d", $workday) . "\t$timeSpent" . PHP_EOL;
        if($disp) {
            try {
                $logWork = $soapClient->addWorklogAndAutoAdjustRemainingEstimate($auth, $ticket, $worklog);
                fwrite($fp, date("Y-m-d H:i:s") . "\t" . $output);
            }
            catch (Exception $e){
                $error = $output . $e->getMessage() . PHP_EOL;
                echo $error;
                fwrite($fp, $error);
            }
        } else {
            echo $output;
        }
    }
    fclose($fp);
} catch (Exception $e) {
    echo "\nCaught exception: ",  $e->getMessage(), "\n";
}
?>