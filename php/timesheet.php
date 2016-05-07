#!/usr/bin/php
<?php
$tickets = array('EAS-1159');
$wsdl = 'http://jira.prod.icd/rpc/soap/jirasoapservice-v2?wsdl';

$jirauser = 'bhughes';
$passwd = $argv[1];

$soapClient = new SoapClient($wsdl);
$auth = $soapClient->login($jirauser, $passwd);

//8 hours =  480 minutes.
$time =  round((480 / count($tickets)));
$timeSpent = $time . 'm';
$timeInSeconds = $time * 60;

$worklog = new StdClass;
$worklog->comment = '';
$worklog->timeSpent = $timeSpent;
$worklog->timeSpentInSeconds = $timeInSeconds;
$worklog->startDate = strtotime('Today');


foreach ($tickets as $ticket) {
    $logWork = $soapClient->addWorklogAndAutoAdjustRemainingEstimate($auth, $ticket, $worklog);
    echo $ticket . '=' . $timeInSeconds . '|' . $timeSpent . PHP_EOL;
}