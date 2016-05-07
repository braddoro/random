<?php
//**************************************************************************
//
// File        :  CMR_Utility.inc
//
// Copyright   :  Copyright 2007 American Tire Distributors, Inc.
//
// Author(s)   :  Brad Hughes - bhughes@atd-us.com
//
//                American Tire Distributors
//                12200 Herbert Wayne Ct.
//                Huntersville, NC 28078
//
//**************************************************************************
class CMR_Utility extends ServerObject {

    const ERROR                     = 'Error';
    const ERROR_EXEC_QUERY          = 'Error executing query.';
    const RECORD_ALREADY_EXISTS     = 'A record like this already exists.';
    const SUCCESS                   = 'Success.';
    const UNABLE_TO_ADD             = 'Unable to add record.';
    const UNABLE_TO_REMOVE          = 'Unable to remove record.';
    const UNABLE_TO_UPDATE          = 'Unable to update record.';

    public function validateContactForApproval($arguments, $config) {
        $rval = array();
        $rval["VALIDATED"] = 1;
        $rval["MESSAGE"] = self::SUCCESS;
        $rval["ARGS"] = $arguments;
        $rval["STATUS"] = 1;

        $entity_source = $arguments["ENTITY_SOURCE"];   // LIVE/STAGED
        $entity_type = $arguments["ENTITY_TYPE"];       // CUSTOMER/SITE
        $entity_id = intval($arguments["ENTITY_ID"]);   // CUSTOMER_ID/SITE_ID

        $phone_numbers = 0;
        $caller_text = strtolower($entity_type);
        $message = "";
        $sql = "";
        $bindVars = array();

        // Case structure to validate input data.
        //
        if($entity_id < 1){
            $params = array('caller_text' => $caller_text, 'entity_id' => $entity_id);
            $default = "Invalid $caller_text ID ($entity_id) passed.";
            $rval["STATUS"] = $this->STATUS_VALIDATION_ERROR;
            $rval["VALIDATED"] = 0;
            $rval["MESSAGE"] = CMR_Utility::getL18NString('invalid.id.passed', $default, $params);
            return $rval;
        }

        // Skip check because we don't have valid data.
        //
        if($message == ""){

            // Case structure to build the sql to check for a phone number based on
            // the customer source, and entity type, and entity id.
            //
            switch($entity_source){

                case "LIVE":

                    if($entity_type == "CUSTOMER"){
                        $sql = "
                        SELECT
                            COUNT(*) AS COUNT
                        FROM
                            APPS.HZ_CONTACT_POINTS CP
                        WHERE
                            CP.CONTACT_POINT_TYPE = 'PHONE'
                            AND CP.STATUS = 'A'
                            AND CP.OWNER_TABLE_NAME = :tablename
                            AND CP.OWNER_TABLE_ID IN (SELECT PARTY_ID FROM APPS.HZ_CUST_ACCOUNTS WHERE CUST_ACCOUNT_ID = :entity_id)
                        ";

                    }else{
                        $sql = "
                        SELECT
                            COUNT(*) AS COUNT
                        FROM
                            APPS.HZ_CONTACT_POINTS CP
                        WHERE
                            CP.CONTACT_POINT_TYPE = 'PHONE'
                            AND CP.STATUS = 'A'
                            AND CP.OWNER_TABLE_NAME = :tablename
                            AND CP.OWNER_TABLE_ID = :entity_id
                        ";
                    }
                    $bindVars = array(
                        array("PH_NAME" => ":tablename", "VARIABLE" => (($entity_type == "CUSTOMER") ? "HZ_PARTIES" : "HZ_PARTY_SITES")),
                        array("PH_NAME" => ":entity_id", "VARIABLE" => $entity_id)
                    );

                break;

                case "STAGED":
                    $sql = "
                    SELECT
                        COUNT(*) AS COUNT
                    FROM
                        APPS.XXATDEAS_CONTACT_POINTS CP
                    WHERE
                        CP.CONTACT_POINT_TYPE = 'PHONE'
                        AND CP.ENTITY_TYPE = :entity_type
                        AND CP.ENTITY_ID = :entity_id
                    ";
                    $bindVars = array(
                        array("PH_NAME" => ":entity_type", "VARIABLE" => $entity_type),
                        array("PH_NAME" => ":entity_id", "VARIABLE" => $entity_id)
                    );

                break;

                default:
                    $message = CMR_Utility::getL18NString('invalid.parameters.passed', "Invalid parameters passed to validation.");
                    $rval["STATUS"] = $this->STATUS_VALIDATION_ERROR;
                    $rval["VALIDATED"] = 0;
                    $rval["MESSAGE"] = $message;
                    return $rval;

                break;

            }

            if($sql != ""){
                $stid = oracle_db_exec($sql, $bindVars);
                $results = oci_fetch_assoc($stid);
                $phone_numbers = $results["COUNT"];
            }

            if($phone_numbers == 0){
                $params = array('caller_text' => $caller_text);
                $default = "At least one phone number must exist before this $caller_text can be approved.";
                $rval["STATUS"] = $this->STATUS_VALIDATION_ERROR;
                $rval["VALIDATED"] = 0;
                $rval["MESSAGE"] = CMR_Utility::getL18NString('phone.number.must.exist', $default, $params);;
                return $rval;
            }

        }

        if($message > ""){
            eventLog(
                "CMR",
                "ERROR",
                "validateContacts",
                $this->controller->session["username"],
                $message,
                $arguments
            );
            $rval["VALIDATED"] = 0;
            $rval["MESSAGE"] = $message;
        }

        return $rval;
    }

    public function getUserEmailAddress($user_id) {

        $this->PCLOracle = new PCLOracle($GLOBALS["oracle_conn"]);

        if($this->PCLOracle->hasErrors()){
            return;
        }

        // Look up the email address that goes with to_user_id
        //
        $sql = "SELECT EMAIL_ADDRESS FROM APPLSYS.FND_USER WHERE USER_ID = :user_id";

        $bindVariables = array(
            array("PH_NAME" => ":user_id", "VARIABLE" => $user_id)
            );

        $executeResults = $this->PCLOracle->execute($sql, $bindVariables);

        if(!$executeResults["STATUS"] || $this->PCLOracle->hasErrors()){
            return;
        }

        $fetchAllRecordsResults = $this->PCLOracle->fetchAllRecords();

        if(!$fetchAllRecordsResults["STATUS"]){
            return;
        }

        return $fetchAllRecordsResults['RESULT'][0]['EMAIL_ADDRESS'];
    }

    public function getDCNumberFromSiteID($site_id) {
        $result = array();

        if(is_numeric($site_id)){
            $sql = "
            SELECT
                SUBSTR(OU.NAME, 1, 3) AS DCID
            FROM
                APPS.HZ_CUST_SITE_USES CSU,
                APPS.HZ_CUST_ACCT_SITES CAS,
                APPS.HR_ORGANIZATION_UNITS OU
            WHERE
                CAS.PARTY_SITE_ID = :site_id
                AND CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID
                AND CSU.WAREHOUSE_ID = OU.ORGANIZATION_ID
                AND CAS.STATUS = 'A'
                AND CSU.STATUS = 'A'
                AND CSU.SITE_USE_CODE = 'SHIP_TO'
            ";
            $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));
            $stid = oracle_db_exec($sql, $bindVars);
            $result = oci_fetch_assoc($stid);
            $dcid = $result['DCID'];
        }
        return $dcid;
    }

    public function get_dba_name_from_site_id($party_site_id) {
        $result = array();
        $dba_name = '';

        if(is_numeric($party_site_id)){
            $sql = "SELECT
                ADDRESSEE
            FROM
                APPS.HZ_PARTY_SITES
            WHERE
                PARTY_SITE_ID = :party_site_id
            ";
            $bindVars = array(array("PH_NAME" => ":party_site_id", "VARIABLE" => $party_site_id));
            $stid = oracle_db_exec($sql, $bindVars);
            $result = oci_fetch_assoc($stid);
            $dba_name = $result['ADDRESSEE'];
        }

        return $dba_name;
    }

    // This funtion is similar to oracle_db_exec().  The difference is that this function does
    // not do direct writes to the output buffer.  It is also different in that instead of
    // returning null it returns a status package.
    //
    public function oracle_db_exec_cmr($sql, &$bindVars = NULL, $flag = OCI_COMMIT_ON_SUCCESS, $conn = 0) {

        $reponse = array('status' => TRUE, 'result' => NULL);
        $app = 'CMR';

        // Caller may pass a specific link, or otherwise we'll use the normal, global one.
        //
        if ($conn == 0) {
            $conn = $GLOBALS['oracle_conn'];
        }

        $stid = oci_parse($conn, $sql);

        if (!$stid) {
            $e = oci_error($conn);
            $reponse['status'] = FALSE;
            $reponse['result'] = $e;
            return $reponse;
        }

        if(is_array($bindVars)) {
            foreach($bindVars as &$bind) {

                // Set some defaults if they were not passed in.
                //
                $bind["MAXLENGTH"] = isset($bind["MAXLENGTH"]) ? $bind["MAXLENGTH"] : -1;
                $bind["TYPE"] = isset($bind["TYPE"]) ? $bind["TYPE"] : SQLT_CHR;

                $r = oci_bind_by_name($stid, $bind["PH_NAME"], $bind["VARIABLE"], $bind["MAXLENGTH"], $bind["TYPE"]);

                if (!$r) {
                    $e = oci_error($stid);
                    $reponse['status'] = FALSE;
                    $reponse['result'] = $e;
                    return $reponse;
                }
            }
        }

        $r = oci_execute($stid, $flag);

        if (!$r) {
            $e = oci_error($stid);
            $reponse['status'] = FALSE;
            $reponse['result'] = $e;
            return $reponse;
        }

        $reponse['result'] = $stid;

        return $reponse;
    }

    public static function parse_vars($str, $vars=array(), $char='$'){
        if (count($vars) > 0){
            foreach ($vars as $k => $v){
                $str = str_replace($char . $k, $v, $str);
            }
        }
        return $str;
    }

    public function getL18NString($keyStr, $default='', $params=null) {
        $lang_code = 'en';
        if (isset($GLOBALS['language']) && $GLOBALS['language'] > '') {
            $lang_code = $GLOBALS['language'];
        }
        $output = $default;
        $lang_file = "lang/Translations_{$lang_code}_server.properties";
        if (file_exists($lang_file)){
            $lang_array = parse_ini_file($lang_file, true);
            if(is_array($lang_array) && array_key_exists($keyStr, $lang_array['locale_strings'])){
                $output = $lang_array['locale_strings'][$keyStr];
            }else{
                eventLog('CMR', 'Warn', 'Locale Server', "getL18NString", 'Missing language key.', "Unable to locate language key |$keyStr| for class |$class| in " . __METHOD__ . " in file: |$lang_file|.");
            }
        }else{
            eventLog('CMR', 'Error', 'Locale Server', "getL18NString", 'Missing language file.', "Unable to locate language key |$keyStr| for class for class |$class| in file |$lang_file| " . __METHOD__ . ".");
        }
        if(count($params) > 0){
            $output = CMR_Utility::parse_vars($output, $params);
        }

        return $output;
    }
}
?>
