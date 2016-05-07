#! /usr/bin/php
<?php
require_once 'shared/classes/PCLOracleTable.inc';
require_once 'shared/classes/PCLMySQLTable.inc';
require_once 'shared/classes/PCLSmartClientTableController.inc';
try {
    function printMatch($aParams) {
        echo('ModID' . "\t" . 'EBS' . "\t" . 'PRX' . "\t" . 'SOA' . PHP_EOL);
        foreach($aParams as $row){
            if( ($row['ebs'] != $row['prx']) || ($row['ebs'] != $row['soa'])  || ($row['prx'] != $row['soa'])) {
                echo(
                $row['mod'] . "\t" .
                $row['ebs'] . "\t" .
                $row['prx'] . "\t" .
                $row['soa'] . "\t" .
                $row['PRODUCT_GROUP_ID'] . "\t" .
                $row['PRODUCT_PRECEDENCE'] . "\t" .
                $row['OPERATOR'] . "\t" .
                $row['MODIFIER_NAME'] . PHP_EOL
                );
            }
         }
         echo('ModID' . "\t" . 'EBS' . "\t" . 'PRX' . "\t" . 'SOA' . PHP_EOL);

        return true;
    }
    function getAllModifiersEBS($aParams){
        $rval = array('STATUS' => TRUE, 'RESULT' => NULL);
        $sql = "
        SELECT
          MO.ID,
          MO.ORG_ID,
          MN.MODIFIER_NAME,
          MO.PRODUCT_GROUP_ID,
          MO.OPERATOR,
          MO.EXCLUSIVE_FLAG,
          MO.PRODUCT_PRECEDENCE
        FROM
          XXATDEAS.PRX_MODIFIERS MO,
          XXATDEAS.PRX_MODIFIER_NAMES MN
        WHERE
            MO.MODIFIER_NAME_ID = MN.ID
            AND MO.ORG_ID = :org_id
        ORDER BY
          MO.ID
        ";
        $bindVars = array(
            array('PH_NAME' => ':org_id', 'VARIABLE' => $aParams['ORG_ID'])
        );
        $results = $aParams['CONN']->execute($sql, $bindVars);
        if(!$results['STATUS'] || $aParams['CONN']->hasErrors()) {
            $rval['STATUS'] = $results['STATUS'];
            $rval['RESULT'] = implode(' ', $aParams['CONN']->getErrors());
            return $rval;
        }
        $fetchResults = $aParams['CONN']->fetchAllRecords();
        if(!$fetchResults['STATUS'] || $aParams['CONN']->hasErrors()) {
            $rval['STATUS'] = $results['STATUS'];
            $rval['RESULT'] = implode(' ', $aParams['CONN']->getErrors());
            return $rval;
        }
        return $fetchResults;
    }
    function getAllModifiersPRX($aParams){
        $rval = array('STATUS' => TRUE, 'RESULT' => NULL);
        $sql = "
        SELECT DISTINCT
        	PRX.ID,
            PRX.MODIFIER_NAME,
            PRX.OPERATOR,
            PRX.EXCLUSIVE_FLAG,
            PRX.PRODUCT_PRECEDENCE
        FROM
            ORACLE_EXPORTS.PRX_MODIFIERS PRX
        ORDER BY
            PRX.ID
        ";
        $bindVars = array();
        $results = $aParams['CONN']->execute($sql, $bindVars);
        if(!$results['STATUS'] || $aParams['CONN']->hasErrors()) {
            $rval['STATUS'] = $results['STATUS'];
            $rval['RESULT'] = implode(' ', $aParams['CONN']->getErrors());
            return $rval;
        }
        $fetchResults = $aParams['CONN']->fetchAllRecords();
        if(!$fetchResults['STATUS'] || $aParams['CONN']->hasErrors()) {
            $rval['STATUS'] = $results['STATUS'];
            $rval['RESULT'] = implode(' ', $aParams['CONN']->getErrors());
            return $rval;
        }
        return $fetchResults;
    }
    function getAllModifiersSOA($aParams){
        $rval = array('STATUS' => TRUE, 'RESULT' => NULL);
        $client = new SoapClient("http://qbsoa4.atd-us.icd:9121/ATDWebServices-CacheQueryUtil-context-root/CacheSizeImpl?WSDL",array('trace'=>1));
        $response = $client->getCacheObjIds(array('strCacheName' => 'ModifierInfo'));
        if(!$response){
            $rval = array('STATUS' => FALSE, 'RESULT' => 'SOA is NOT happy.');
            return $rval;
        }
        $nodes = array();
        foreach($response as $key){
            foreach($key as $node){
                $nodes[] = $node;
            }
        }
        unset($response);
        $rval['RESULT'] = $nodes;
        return $rval;
   }
    $arrEBS = array();
    $arrPRX = array();
    $arrSOA = array();
    $arrMATCH = array();
    $credentials_file = "/home/utils/crd/credentials.ini";
    $ConnEBS = new PCLOracle(array('CREDENTIALS_INI' => $credentials_file,'SERVER' => "PRD_QA_BHUGHES_TESTING_STUFF",'USER' => "XXATDONL"));
    $ConnPRX = new PCLMySQL(array('CREDENTIALS_INI' => $credentials_file,'SERVER' => "CA_PRICING_BHUGHES_TESTING_STUFF_MYSQL",'USER' => "roger"));
    //$ConnSOA = new PCLOracle(array('CREDENTIALS_INI' => $credentials_file,'SERVER' => "SOA_BHUGHES_TESTING_STUFF",'USER' => "SOA"));

    // Return all modifers from EBS.
    //
    $aParams = array();
    $aParams['CONN'] = $ConnEBS;
    $aParams['ORG_ID'] = 1796;
    $mod_result = getAllModifiersEBS($aParams);
    if(!$mod_result['STATUS']) {
        echo("EBS Modifier query failed... Bailing". PHP_EOL);
        exit(1);
    }
    foreach($mod_result['RESULT'] as $rows => $items){

        // Populate the matching array with items from the EBS table.
        //
        $arrMATCH[$items['ID']] = array(
            'mod' => $items['ID'],
            'ebs' => 1,
            'prx' => 0,
            'soa' => 0,
            'PRODUCT_GROUP_ID'  => $items['PRODUCT_GROUP_ID'],
            'PRODUCT_PRECEDENCE'=> $items['PRODUCT_PRECEDENCE'],
            'OPERATOR'          => $items['OPERATOR'],
            'MODIFIER_NAME'     => $items['MODIFIER_NAME']
            );
    }
    // Return all modifers from PRX.
    //
    $aParams = array();
    $aParams['CONN'] = $ConnPRX;
    $mod_result = getAllModifiersPRX($aParams);
    if(!$mod_result['STATUS']) {
        echo("PRX Modifier query failed... Bailing". PHP_EOL);
        exit(1);
    }
    foreach($mod_result['RESULT'] as $rows => $items){

        // Check to see if the PRX result has all of the modifiers that EBS has.
        //
        if(array_key_exists ($items['ID'], $arrMATCH)){
            // Found.
            //
            $arrMATCH[$items['ID']]['prx'] = 1;
        }else{
            // Not found so add it.
            ///
            $arrMATCH[$items['ID']] = array(
                'ebs' => 0,
                'prx' => 1,
                'soa' => 0
            );
        }
    }

    // Return all modifers from SOA.
    //
    $aParams = array();
    $mod_result = getAllModifiersSOA($aParams);
    if(!$mod_result['STATUS']) {
        echo("SOA Modifier query failed... Bailing". PHP_EOL);
        exit(1);
    }
    foreach($mod_result['RESULT'] as $rows => $items){

        // Check to see if the PRX result has all of the modifiers that EBS has.
        //
        if(array_key_exists($items, $arrMATCH)){
            // Found.
            //
            $arrMATCH[$items]['soa'] = 1;
        }else{
            // Not found so add it.
            //
            $arrMATCH[$items] = array(
                'ebs' => 0,
                'prx' => 0,
                'soa' => 1
            );
        }
    }

    // Output.
    //
    printMatch($arrMATCH);

} catch (Exception $e) {
    echo $e->getMessage();
}
?>