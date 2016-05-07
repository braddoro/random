<?php
//**************************************************************************
//
// File        :  Site.inc
//
// Copyright   :  Copyright 2007 American Tire Distributors, Inc.
//
// Author(s)   :  Matt Steninger - msteninger@atd-us.com
//
//                American Tire Distributors
//                12200 Herbert Wayne Ct.
//                Huntersville, NC 28078
//
//**************************************************************************
require_once "shared/classes/PCLOracleTable.inc";
require_once "shared/classes/PCLMySQLTable.inc";
require_once "shared/classes/PCLSmartClientTableController.inc";
require_once "shared/src/QPXPricingEngine.inc";

class Site extends CMR_Utility {

    public function __construct(&$controller) {
        parent::__construct($controller);
        oracle_apps_initialize($this->controller->session["user_id"], $this->controller->session["responsibility_id"], $this->controller->session["application_id"], $this->controller->session["profile_option_value"]);
    }

    function siteCategories($arguments, $config) {
        return get_lookup_values_LOV("ADDRESS_CATEGORY", $arguments["SITE_CATEGORY_VAL"], "SITE_CATEGORY_VAL", "SITE_CATEGORY_LBL");
    }

    static public function push_site_data_to_server($mysql_conn, $site_data) {
        $rval = array(
            "STATUS" => TRUE,
            "MESSAGE" => ""
        );

        if(isset($site_data["BILL_TO"])) {
            $status = Site::push_bill_to_data_to_server($mysql_conn, $site_data["BILL_TO"]);

            if(!$status["STATUS"]) {
                $rval["STATUS"] = FALSE;
                $message = $status['MESSAGE'];
                $params = array('message' => $message);
                $default = "The call to push_bill_to_data_to_server failed with error: {$message}. ";
                $rval["MESSAGE"] .= CMR_Utility::getL18NString('push_site_data_to_server.message.1', $default, $params);
            }
        }

        if(isset($site_data["BILL_TO_ATTRIBUTES"])) {
            $status = Site::push_site_attribute_data_to_server($mysql_conn, $site_data["BILL_TO_ATTRIBUTES"]);

            if(!$status["STATUS"]) {
                $rval["STATUS"] = FALSE;
                $message = $status['MESSAGE'];
                $params = array('message' => $message);
                $default = "The (bill to) call to push_site_attribute_data_to_server failed with error: {$message}. ";
                $rval["MESSAGE"] .= CMR_Utility::getL18NString('push_site_data_to_server.message.2', $default, $params);
            }
        }

        if(isset($site_data["SHIP_TO"])) {
            $status = Site::push_ship_to_data_to_server($mysql_conn, $site_data["SHIP_TO"]);

            if(!$status["STATUS"]) {
                $rval["STATUS"] = FALSE;
                $message = $status['MESSAGE'];
                $params = array('message' => $message);
                $default = "The call to push_ship_to_data_to_server failed with error: {$message}. ";
                $rval["MESSAGE"] .= CMR_Utility::getL18NString('push_site_data_to_server.message.3', $default, $params);
            }
        }

        if(isset($site_data["SHIP_TO_ATTRIBUTES"])) {
            $status = Site::push_site_attribute_data_to_server($mysql_conn, $site_data["SHIP_TO_ATTRIBUTES"]);

            if(!$status["STATUS"]) {
                $rval["STATUS"] = FALSE;
                $message = $status['MESSAGE'];
                $params = array('message' => $message);
                $default = "The (ship to) call to push_site_attribute_data_to_server failed with error: {$message}. ";
                $rval["MESSAGE"] .= CMR_Utility::getL18NString('push_site_data_to_server.message.4', $default, $params);
            }
        }

        if(isset($site_data["SHIP_TO_SITE_USE_ID"])) {
            if(is_array($site_data["BTX_SITE_PROGRAMS"])) {
                foreach($site_data["BTX_SITE_PROGRAMS"] as $program) {
                    $status = Site::push_BTX_program_data_to_server($mysql_conn, $program);

                    $program_bill_tos[] = $program["BILL_TO_SITE_USE_ID"];

                    if(!$status["STATUS"]) {
                        $rval["STATUS"] = FALSE;
                        $message = $status['MESSAGE'];
                        $params = array('message' => $message);
                        $default = "The (ship to) call to push_site_attribute_data_to_server failed with error: {$message}. ";
                        $rval["MESSAGE"] .= CMR_Utility::getL18NString('push_site_data_to_server.message.4', $default, $params);
                    }
                }

                $sql = "DELETE ";
                $sql .= "FROM ";
                $sql .= "ORACLE_EXPORTS.BTX_SITE_PROGRAMS ";
                $sql .= "WHERE ";
                $sql .= "SHIP_TO_SITE_USE_ID = {$site_data['SHIP_TO_SITE_USE_ID']} ";
                $sql .= "AND BILL_TO_SITE_USE_ID NOT IN (" . implode($program_bill_tos, ", ") . ") ";

                list($result, $numrows) = mysql_db_exec($sql, $mysql_conn);

                if(!$result) {
                    $rval["STATUS"] = FALSE;
                    $params = array('sql' => $sql);
                    $default = "Cannot remove the unused bill to programs. $sql";
                    $rval["MESSAGE"] = CMR_Utility::getL18NString('push_site_data_to_server.message.5', $default, $params);
                }
            } else {
                $sql = "DELETE ";
                $sql .= "FROM ";
                $sql .= "ORACLE_EXPORTS.BTX_SITE_PROGRAMS ";
                $sql .= "WHERE ";
                $sql .= "SHIP_TO_SITE_USE_ID = {$site_data['SHIP_TO_SITE_USE_ID']} ";

                list($result, $numrows) = mysql_db_exec($sql, $mysql_conn);

                if(!$result) {
                    $rval["STATUS"] = FALSE;
                    $params = array('sql' => $sql);
                    $default = "Cannot remove the unused bill to programs. $sql";
                    $rval["MESSAGE"] = CMR_Utility::getL18NString('push_site_data_to_server.message.5', $default, $params);
                }
            }
        }

        return $rval;
    }

    static private function push_bill_to_data_to_server($mysql_conn, $bill_to_v) {
        $rval = array(
            "STATUS" => TRUE,
            "MESSAGE" => ""
        );

        foreach($bill_to_v as &$bill_to_attribute) {
            $bill_to_attribute = mysql_real_escape_string($bill_to_attribute, $mysql_conn);
        }

        $sql = "INSERT INTO ";
        $sql .= "ORACLE_EXPORTS.BILL_TO ";
        $sql .= "SET ";
        $sql .= "BILL_TO_SITE_USE_ID = '{$bill_to_v['SITE_USE_ID']}', ";
        $sql .= "SITE_USE_CODE = '{$bill_to_v['SITE_USE_CODE']}', ";
        $sql .= "WAREHOUSE_ID = '{$bill_to_v['WAREHOUSE_ID']}', ";
        $sql .= "CUSTOMER_NUMBER = '{$bill_to_v['CUSTOMER_NUMBER']}', ";
        $sql .= "CUSTOMER_ID = '{$bill_to_v['CUSTOMER_ID']}', ";
        $sql .= "DBA_NAME = '{$bill_to_v['DBA_NAME']}', ";
        $sql .= "ADDRESS_ID = '{$bill_to_v['ADDRESS_ID']}', ";
        $sql .= "ADDRESS1 = '{$bill_to_v['ADDRESS1']}', ";
        $sql .= "ADDRESS2 = '{$bill_to_v['ADDRESS2']}', ";
        $sql .= "CITY = '{$bill_to_v['CITY']}', ";
        $sql .= "COUNTY = '{$bill_to_v['COUNTY']}', ";
        $sql .= "STATE = '{$bill_to_v['STATE']}', ";
        $sql .= "POSTAL_CODE = '{$bill_to_v['POSTAL_CODE']}', ";
        $sql .= "CONCATENATED_ADDRESS = '{$bill_to_v['CONCATENATED_ADDRESS']}', ";
        $sql .= "CUSTOMER_CATEGORY = '{$bill_to_v['CUSTOMER_CATEGORY']}', ";
        $sql .= "SALES_TAX_GEOCODE = '{$bill_to_v['SALES_TAX_GEOCODE']}', ";
        $sql .= "POSTAL_PLUS4_CODE = '{$bill_to_v['POSTAL_PLUS4_CODE']}', ";
        $sql .= "DATE_DEACTIVATED = '{$bill_to_v['DATE_DEACTIVATED']}', ";
        $sql .= "CONSUMER_SIZE = '{$bill_to_v['CONSUMER_SIZE']}', ";
        $sql .= "COMMERCIAL_SIZE = '{$bill_to_v['COMMERCIAL_SIZE']}', ";
        $sql .= "WHEEL_SIZE = '{$bill_to_v['WHEEL_SIZE']}', ";
        $sql .= "OPEN_SIZE = '{$bill_to_v['OPEN_SIZE']}', ";
        $sql .= "CUSTOMER_GROUP = '{$bill_to_v['CUSTOMER_GROUP']}', ";
        $sql .= "PRICE_LEVEL = '{$bill_to_v['PRICE_LEVEL']}', ";
        $sql .= "PRICE_LEVEL_START_DATE = '{$bill_to_v['PRICE_LEVEL_START_DATE']}', ";
        $sql .= "SERVICE_BAY = '{$bill_to_v['SERVICE_BAY']}', ";
        $sql .= "WAIVE_RESTOCK_FEE = '{$bill_to_v['WAIVE_RESTOCK_FEE']}', ";
        $sql .= "WARRANTY_FLAG = '{$bill_to_v['WARRANTY_FLAG']}', ";
        $sql .= "ACCEPT_RETURN_FLAG = '{$bill_to_v['ACCEPT_RETURN_FLAG']}' ";
        $sql .= "ON DUPLICATE KEY ";
        $sql .= "UPDATE ";
        $sql .= "SITE_USE_CODE = '{$bill_to_v['SITE_USE_CODE']}', ";
        $sql .= "WAREHOUSE_ID = '{$bill_to_v['WAREHOUSE_ID']}', ";
        $sql .= "CUSTOMER_NUMBER = '{$bill_to_v['CUSTOMER_NUMBER']}', ";
        $sql .= "CUSTOMER_ID = '{$bill_to_v['CUSTOMER_ID']}', ";
        $sql .= "DBA_NAME = '{$bill_to_v['DBA_NAME']}', ";
        $sql .= "ADDRESS_ID = '{$bill_to_v['ADDRESS_ID']}', ";
        $sql .= "ADDRESS1 = '{$bill_to_v['ADDRESS1']}', ";
        $sql .= "ADDRESS2 = '{$bill_to_v['ADDRESS2']}', ";
        $sql .= "CITY = '{$bill_to_v['CITY']}', ";
        $sql .= "COUNTY = '{$bill_to_v['COUNTY']}', ";
        $sql .= "STATE = '{$bill_to_v['STATE']}', ";
        $sql .= "POSTAL_CODE = '{$bill_to_v['POSTAL_CODE']}', ";
        $sql .= "CONCATENATED_ADDRESS = '{$bill_to_v['CONCATENATED_ADDRESS']}', ";
        $sql .= "CUSTOMER_CATEGORY = '{$bill_to_v['CUSTOMER_CATEGORY']}', ";
        $sql .= "SALES_TAX_GEOCODE = '{$bill_to_v['SALES_TAX_GEOCODE']}', ";
        $sql .= "POSTAL_PLUS4_CODE = '{$bill_to_v['POSTAL_PLUS4_CODE']}', ";
        $sql .= "DATE_DEACTIVATED = '{$bill_to_v['DATE_DEACTIVATED']}', ";
        $sql .= "CONSUMER_SIZE = '{$bill_to_v['CONSUMER_SIZE']}', ";
        $sql .= "COMMERCIAL_SIZE = '{$bill_to_v['COMMERCIAL_SIZE']}', ";
        $sql .= "WHEEL_SIZE = '{$bill_to_v['WHEEL_SIZE']}', ";
        $sql .= "OPEN_SIZE = '{$bill_to_v['OPEN_SIZE']}', ";
        $sql .= "CUSTOMER_GROUP = '{$bill_to_v['CUSTOMER_GROUP']}', ";
        $sql .= "PRICE_LEVEL = '{$bill_to_v['PRICE_LEVEL']}', ";
        $sql .= "PRICE_LEVEL_START_DATE = '{$bill_to_v['PRICE_LEVEL_START_DATE']}', ";
        $sql .= "SERVICE_BAY = '{$bill_to_v['SERVICE_BAY']}', ";
        $sql .= "WAIVE_RESTOCK_FEE = '{$bill_to_v['WAIVE_RESTOCK_FEE']}', ";
        $sql .= "WARRANTY_FLAG = '{$bill_to_v['WARRANTY_FLAG']}', ";
        $sql .= "ACCEPT_RETURN_FLAG = '{$bill_to_v['ACCEPT_RETURN_FLAG']}' ";

        list($result, $numrows) = mysql_db_exec($sql, $mysql_conn);

        if(!$result) {
            $rval["STATUS"] = FALSE;
            $siteuseid = $bill_to_v['SITE_USE_ID'];
            $params = array('siteuseid' => $siteuseid);
            $default = "Cannot merge into ORACLE_EXPORTS.BILL_TO using BILL_TO_SITE_USE_ID {$siteuseid}.";
            $rval["MESSAGE"] = CMR_Utility::getL18NString('push_bill_to_data_to_server', $default, $params);
        }

        return $rval;
    }

    static private function push_site_attribute_data_to_server($mysql_conn, $site_attributes) {
        $rval = array(
            "STATUS" => TRUE,
            "MESSAGE" => ""
        );

        foreach($site_attributes as &$site_attribute) {
            $site_attribute = mysql_real_escape_string($site_attribute, $mysql_conn);
        }

        $sql = "INSERT INTO ";
        $sql .= "ORACLE_EXPORTS.SITE_ATTRIBUTE ";
        $sql .= "SET ";
        $sql .= "CUSTOMER_ID = {$site_attributes['CUSTOMER_ID']}, ";
        $sql .= "ADDRESS_ID = {$site_attributes['ADDRESS_ID']}, ";
        $sql .= "SITE_USE_ID = {$site_attributes['SITE_USE_ID']}, ";
        $sql .= "SITE_USE_CODE = '{$site_attributes['SITE_USE_CODE']}', ";
        $sql .= "ACCEPT_RETURN_FLAG = '{$site_attributes['ACCEPT_RETURN_FLAG']}', ";
        $sql .= "COMMERCIAL_SIZE = '{$site_attributes['COMMERCIAL_SIZE']}', ";
        $sql .= "COMMISSION_PERCENT = '{$site_attributes['COMMISSION_PERCENT']}', ";
        $sql .= "CONSUMER_SIZE = '{$site_attributes['CONSUMER_SIZE']}', ";
        $sql .= "MOUNT_BALANCE = '{$site_attributes['MOUNT_BALANCE']}', ";
        $sql .= "OPEN_SIZE = '{$site_attributes['OPEN_SIZE']}', ";
        $sql .= "PRICE_LEVEL = '{$site_attributes['PRICE_LEVEL']}', ";
        $sql .= "PRICE_LEVEL_START_DATE = '{$site_attributes['PRICE_LEVEL_START_DATE']}', ";
        $sql .= "SERVICE_BAY = '{$site_attributes['SERVICE_BAY']}', ";
        $sql .= "WAIVE_RESTOCK_FEE = '{$site_attributes['WAIVE_RESTOCK_FEE']}', ";
        $sql .= "WARRANTY_FLAG = '{$site_attributes['WARRANTY_FLAG']}', ";
        $sql .= "WHEEL_SIZE = '{$site_attributes['WHEEL_SIZE']}', ";
        $sql .= "AAN_NUMBER = '{$site_attributes['AAN_NUMBER']}', ";
        $sql .= "ALLIANCE = '{$site_attributes['ALLIANCE']}', ";
        $sql .= "AMI_NUMBER = '{$site_attributes['AMI_NUMBER']}', ";
        $sql .= "BFS_OE_NUMBER = '{$site_attributes['BFS_OE_NUMBER']}', ";
        $sql .= "BS6_DIGIT_NUMBER = '{$site_attributes['BS6_DIGIT_NUMBER']}', ";
        $sql .= "COMMISSION_CODE = '{$site_attributes['COMMISSION_CODE']}', ";
        $sql .= "CONTI_OE_NUMBER = '{$site_attributes['CONTI_OE_NUMBER']}', ";
        $sql .= "COOPER_OE_NUMBER = '{$site_attributes['COOPER_OE_NUMBER']}', ";
        $sql .= "G3X_NUMBER = '{$site_attributes['G3X_NUMBER']}', ";
        $sql .= "GOLD_NUMBER = '{$site_attributes['GOLD_NUMBER']}', ";
        $sql .= "GY_ASSOC_NUMBER = '{$site_attributes['GY_ASSOC_NUMBER']}', ";
        $sql .= "GY_NON_SIG_NUMBER = '{$site_attributes['GY_NON_SIG_NUMBER']}', ";
        $sql .= "GY_OE_NUMBER = '{$site_attributes['GY_OE_NUMBER']}', ";
        $sql .= "HANKOOK_OE_NUMBER = '{$site_attributes['HANKOOK_OE_NUMBER']}', ";
        $sql .= "KUMHO_FUEL_NUMBER = '{$site_attributes['KUMHO_FUEL_NUMBER']}', ";
        $sql .= "MAST_OE_NUMBER = '{$site_attributes['MAST_OE_NUMBER']}', ";
        $sql .= "PIRELLI_OE_NUMBER = '{$site_attributes['PIRELLI_OE_NUMBER']}', ";
        $sql .= "TIRESTARZ = '{$site_attributes['TIRESTARZ']}' ";
        $sql .= "ON DUPLICATE KEY ";
        $sql .= "UPDATE ";
        $sql .= "CUSTOMER_ID = {$site_attributes['CUSTOMER_ID']}, ";
        $sql .= "ADDRESS_ID = {$site_attributes['ADDRESS_ID']}, ";
        $sql .= "SITE_USE_CODE = '{$site_attributes['SITE_USE_CODE']}', ";
        $sql .= "ACCEPT_RETURN_FLAG = '{$site_attributes['ACCEPT_RETURN_FLAG']}', ";
        $sql .= "COMMERCIAL_SIZE = '{$site_attributes['COMMERCIAL_SIZE']}', ";
        $sql .= "COMMISSION_PERCENT = '{$site_attributes['COMMISSION_PERCENT']}', ";
        $sql .= "CONSUMER_SIZE = '{$site_attributes['CONSUMER_SIZE']}', ";
        $sql .= "MOUNT_BALANCE = '{$site_attributes['MOUNT_BALANCE']}', ";
        $sql .= "OPEN_SIZE = '{$site_attributes['OPEN_SIZE']}', ";
        $sql .= "PRICE_LEVEL = '{$site_attributes['PRICE_LEVEL']}', ";
        $sql .= "PRICE_LEVEL_START_DATE = '{$site_attributes['PRICE_LEVEL_START_DATE']}', ";
        $sql .= "SERVICE_BAY = '{$site_attributes['SERVICE_BAY']}', ";
        $sql .= "WAIVE_RESTOCK_FEE = '{$site_attributes['WAIVE_RESTOCK_FEE']}', ";
        $sql .= "WARRANTY_FLAG = '{$site_attributes['WARRANTY_FLAG']}', ";
        $sql .= "WHEEL_SIZE = '{$site_attributes['WHEEL_SIZE']}', ";
        $sql .= "AAN_NUMBER = '{$site_attributes['AAN_NUMBER']}', ";
        $sql .= "ALLIANCE = '{$site_attributes['ALLIANCE']}', ";
        $sql .= "AMI_NUMBER = '{$site_attributes['AMI_NUMBER']}', ";
        $sql .= "BFS_OE_NUMBER = '{$site_attributes['BFS_OE_NUMBER']}', ";
        $sql .= "BS6_DIGIT_NUMBER = '{$site_attributes['BS6_DIGIT_NUMBER']}', ";
        $sql .= "COMMISSION_CODE = '{$site_attributes['COMMISSION_CODE']}', ";
        $sql .= "CONTI_OE_NUMBER = '{$site_attributes['CONTI_OE_NUMBER']}', ";
        $sql .= "COOPER_OE_NUMBER = '{$site_attributes['COOPER_OE_NUMBER']}', ";
        $sql .= "G3X_NUMBER = '{$site_attributes['G3X_NUMBER']}', ";
        $sql .= "GOLD_NUMBER = '{$site_attributes['GOLD_NUMBER']}', ";
        $sql .= "GY_ASSOC_NUMBER = '{$site_attributes['GY_ASSOC_NUMBER']}', ";
        $sql .= "GY_NON_SIG_NUMBER = '{$site_attributes['GY_NON_SIG_NUMBER']}', ";
        $sql .= "GY_OE_NUMBER = '{$site_attributes['GY_OE_NUMBER']}', ";
        $sql .= "HANKOOK_OE_NUMBER = '{$site_attributes['HANKOOK_OE_NUMBER']}', ";
        $sql .= "KUMHO_FUEL_NUMBER = '{$site_attributes['KUMHO_FUEL_NUMBER']}', ";
        $sql .= "MAST_OE_NUMBER = '{$site_attributes['MAST_OE_NUMBER']}', ";
        $sql .= "PIRELLI_OE_NUMBER = '{$site_attributes['PIRELLI_OE_NUMBER']}', ";
        $sql .= "TIRESTARZ = '{$site_attributes['TIRESTARZ']}' ";

        list($result, $numrows) = mysql_db_exec($sql, $mysql_conn);

        if(!$result) {
            $rval["STATUS"] = FALSE;
            $default = "Cannot merge into ORACLE_EXPORTS.SITE_ATTRIBUTE.";
            $rval["MESSAGE"] = CMR_Utility::getL18NString('push_site_attribute_data_to_server', $default);
        }

        return $rval;
    }

    static private function push_ship_to_data_to_server($mysql_conn, $ship_to_v) {
        $rval = array(
            "STATUS" => TRUE,
            "MESSAGE" => ""
        );

        foreach($ship_to_v as &$ship_to_attribute) {
            $ship_to_attribute = mysql_real_escape_string($ship_to_attribute, $mysql_conn);
        }

        $sql = "INSERT INTO ";
        $sql .= "ORACLE_EXPORTS.SHIP_TO ";
        $sql .= "SET ";
        $sql .= "CUSTOMER_ID = '{$ship_to_v['CUSTOMER_ID']}', ";
        $sql .= "CUSTOMER_NUMBER = '{$ship_to_v['CUSTOMER_NUMBER']}', ";
        $sql .= "CUSTOMER_NAME = '{$ship_to_v['CUSTOMER_NAME']}', ";
        $sql .= "SHIP_TO_NAME = '{$ship_to_v['DBA_NAME']}', ";
        $sql .= "CUSTOMER_CLASS_CODE = '{$ship_to_v['CUSTOMER_CLASS_CODE']}', ";
        $sql .= "CUSTOMER_CLASS = '{$ship_to_v['CUSTOMER_CLASS']}', ";
        $sql .= "BILL_TO_SITE_USE_ID = '{$ship_to_v['BILL_TO_SITE_USE_ID']}', ";
        $sql .= "SHIP_TO_SITE_USE_ID = '{$ship_to_v['SHIP_TO_SITE_USE_ID']}', ";
        $sql .= "LOCATION_NUMBER = '{$ship_to_v['LOCATION_NUMBER']}', ";
        $sql .= "STORE_NUMBER = '" . str_pad($ship_to_v['STORE_NUMBER'], 6, "0", STR_PAD_LEFT) . "', ";
        $sql .= "ADDRESS = '{$ship_to_v['ADDRESS1']}', ";
        $sql .= "CITY = '{$ship_to_v['CITY']}', ";
        $sql .= "COUNTY = '{$ship_to_v['COUNTY']}', ";
        $sql .= "COUNTY_FIPS = '{$ship_to_v['COUNTY_FIPS']}', ";
        $sql .= "STATE = '{$ship_to_v['STATE']}', ";
        $sql .= "STATE_FIPS = '{$ship_to_v['STATE_FIPS']}', ";
        $sql .= "POSTAL_CODE = '{$ship_to_v['POSTAL_CODE']}', ";
        $sql .= "ORGANIZATION_ID = '{$ship_to_v['ORGANIZATION_ID']}', ";
        $sql .= "PRIMARY_DC = '{$ship_to_v['PRIMARY_DC']}', ";
        $sql .= "PRIMARY_DC_CODE = '{$ship_to_v['PRIMARY_DC_CODE']}', ";
        $sql .= "PRIMARY_DC_ORGANIZATION_ID = '{$ship_to_v['PRIMARY_ORGANIZATION_ID']}', ";
        $sql .= "PRIMARY_SALESREP_ID = '{$ship_to_v['PRIMARY_SALESREP_ID']}', ";
        $sql .= "Z_SHIP_TO_CUSTOMER = '{$ship_to_v['Z_CUST_ACCT']}', ";
        $sql .= "Z_SHIP_TO_STORE = '" . str_pad($ship_to_v['STORE_NUMBER'], 6, "0", STR_PAD_LEFT) . "', ";
        $sql .= "TYPE = '" . ($ship_to_v['CUSTOMER_CLASS_CODE'] == 90 ? "D" : "S") . "', ";
        $sql .= "PARTY_SITE_ID = '{$ship_to_v['PARTY_SITE_ID']}' ";
        $sql .= "ON DUPLICATE KEY ";
        $sql .= "UPDATE ";
        $sql .= "CUSTOMER_ID = '{$ship_to_v['CUSTOMER_ID']}', ";
        $sql .= "CUSTOMER_NUMBER = '{$ship_to_v['CUSTOMER_NUMBER']}', ";
        $sql .= "CUSTOMER_NAME = '{$ship_to_v['CUSTOMER_NAME']}', ";
        $sql .= "SHIP_TO_NAME = '{$ship_to_v['DBA_NAME']}', ";
        $sql .= "CUSTOMER_CLASS_CODE = '{$ship_to_v['CUSTOMER_CLASS_CODE']}', ";
        $sql .= "CUSTOMER_CLASS = '{$ship_to_v['CUSTOMER_CLASS']}', ";
        $sql .= "BILL_TO_SITE_USE_ID = '{$ship_to_v['BILL_TO_SITE_USE_ID']}', ";
        $sql .= "LOCATION_NUMBER = '{$ship_to_v['LOCATION_NUMBER']}', ";
        $sql .= "STORE_NUMBER = '" . str_pad($ship_to_v['STORE_NUMBER'], 6, "0", STR_PAD_LEFT) . "', ";
        $sql .= "ADDRESS = '{$ship_to_v['ADDRESS1']}', ";
        $sql .= "CITY = '{$ship_to_v['CITY']}', ";
        $sql .= "COUNTY = '{$ship_to_v['COUNTY']}', ";
        $sql .= "COUNTY_FIPS = '{$ship_to_v['COUNTY_FIPS']}', ";
        $sql .= "STATE = '{$ship_to_v['STATE']}', ";
        $sql .= "STATE_FIPS = '{$ship_to_v['STATE_FIPS']}', ";
        $sql .= "POSTAL_CODE = '{$ship_to_v['POSTAL_CODE']}', ";
        $sql .= "ORGANIZATION_ID = '{$ship_to_v['ORGANIZATION_ID']}', ";
        $sql .= "PRIMARY_DC = '{$ship_to_v['PRIMARY_DC']}', ";
        $sql .= "PRIMARY_DC_CODE = '{$ship_to_v['PRIMARY_DC_CODE']}', ";
        $sql .= "PRIMARY_DC_ORGANIZATION_ID = '{$ship_to_v['PRIMARY_ORGANIZATION_ID']}', ";
        $sql .= "PRIMARY_SALESREP_ID = '{$ship_to_v['PRIMARY_SALESREP_ID']}', ";
        $sql .= "Z_SHIP_TO_CUSTOMER = '{$ship_to_v['Z_CUST_ACCT']}', ";
        $sql .= "Z_SHIP_TO_STORE = '" . str_pad($ship_to_v['STORE_NUMBER'], 6, "0", STR_PAD_LEFT) . "', ";
        $sql .= "TYPE = '" . ($ship_to_v['CUSTOMER_CLASS_CODE'] == 90 ? "D" : "S") . "', ";
        $sql .= "PARTY_SITE_ID = '{$ship_to_v['PARTY_SITE_ID']}' ";

        list($result, $numrows) = mysql_db_exec($sql, $mysql_conn);

        if(!$result) {
            $rval["STATUS"] = FALSE;
            $default = "Cannot merge into ORACLE_EXPORTS.SHIP_TO.";
            $rval["MESSAGE"] = CMR_Utility::getL18NString('push_ship_to_data_to_server', $default);
        }

        return $rval;
    }

    static private function push_BTX_program_data_to_server($mysql_conn, $program_data) {
        $rval = array(
            "STATUS" => TRUE,
            "MESSAGE" => ""
        );

        foreach($program_data as &$program_datum) {
            $program_datum = mysql_real_escape_string($program_datum, $mysql_conn);
        }

        $sql = "INSERT INTO ";
        $sql .= "ORACLE_EXPORTS.BTX_SITE_PROGRAMS ";
        $sql .= "SET ";
        $sql .= "BILL_TO_SITE_USE_ID = {$program_data['BILL_TO_SITE_USE_ID']}, ";
        $sql .= "SHIP_TO_SITE_USE_ID = {$program_data['SHIP_TO_SITE_USE_ID']} ";
        $sql .= "ON DUPLICATE KEY ";
        $sql .= "UPDATE ";
        $sql .= "BILL_TO_SITE_USE_ID = {$program_data['BILL_TO_SITE_USE_ID']}, ";
        $sql .= "SHIP_TO_SITE_USE_ID = {$program_data['SHIP_TO_SITE_USE_ID']}";

        list($result, $numrows) = mysql_db_exec($sql, $mysql_conn);

        if(!$result) {
            $rval["STATUS"] = FALSE;
            $default = "Cannot merge into ORACLE_EXPORTS.BTX_SITE_PROGRAMS.";
            $rval["MESSAGE"] = CMR_Utility::getL18NString('push_BTX_program_data_to_server.message', $default);
        }

        return $rval;
    }

    static public function get_site_data_for_push($site_id) {
        $rval = array(
            "STATUS" => TRUE,
            "MESSAGE" => ""
        );

        $sql = "SELECT ";
        $sql .= "(SELECT ";
        $sql .= "SITE_USE_ID ";
        $sql .= "FROM ";
        $sql .= "APPS.HZ_CUST_SITE_USES CSU ";
        $sql .= "WHERE ";
        $sql .= "CSU.CUST_ACCT_SITE_ID = CAS.CUST_ACCT_SITE_ID ";
        $sql .= "AND CSU.SITE_USE_CODE = 'BILL_TO' ";
        $sql .= "AND CSU.STATUS = 'A') BILL_TO_SITE_USE_ID, ";
        $sql .= "(SELECT ";
        $sql .= "SITE_USE_ID ";
        $sql .= "FROM ";
        $sql .= "APPS.HZ_CUST_SITE_USES CSU ";
        $sql .= "WHERE ";
        $sql .= "CSU.CUST_ACCT_SITE_ID = CAS.CUST_ACCT_SITE_ID ";
        $sql .= "AND CSU.SITE_USE_CODE = 'SHIP_TO' ";
        $sql .= "AND CSU.STATUS = 'A') SHIP_TO_SITE_USE_ID ";
        $sql .= "FROM ";
        $sql .= "APPS.HZ_CUST_ACCT_SITES CAS ";
        $sql .= "WHERE ";
        $sql .= "CAS.PARTY_SITE_ID = :site_id ";

        $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

        $stid = oracle_db_exec($sql, $bindVars);

        $results = oci_fetch_assoc($stid);

        if($results !== FALSE) {
            $rval = array_merge($rval, $results);
        } else {
            $rval["STATUS"] = FALSE;
            $params = array('site_id' => $site_id);
            $default = "Unable to fetch site information to export to the external servers for party_site_id $site_id.";
            $rval["MESSAGE"] = CMR_Utility::getL18NString('get_site_data_for_push.message.1', $default, $params);

            return $rval;
        }

        if(is_null($rval["BILL_TO_SITE_USE_ID"]) && is_null($rval["SHIP_TO_SITE_USE_ID"])) {
            $rval["STATUS"] = FALSE;
            $params = array('site_id' => $site_id);
            $default = "Invalid site information fetched for party_site_id $site_id.";
            $rval["MESSAGE"] = CMR_Utility::getL18NString('get_site_data_for_push.message.2', $default, $params);

            return $rval;
        }

        if(!is_null($rval["BILL_TO_SITE_USE_ID"])) {
            $sql = "SELECT ";
            $sql .= "SITE_USE_ID, ";
            $sql .= "SITE_USE_CODE, ";
            $sql .= "WAREHOUSE_ID, ";
            $sql .= "CUSTOMER_NUMBER, ";
            $sql .= "CUSTOMER_GROUP, ";
            $sql .= "CUSTOMER_ID, ";
            $sql .= "DBA_NAME, ";
            $sql .= "ADDRESS_ID, ";
            $sql .= "ADDRESS1, ";
            $sql .= "ADDRESS2, ";
            $sql .= "CITY, ";
            $sql .= "COUNTY, ";
            $sql .= "STATE, ";
            $sql .= "POSTAL_CODE, ";
            $sql .= "CONCATENATED_ADDRESS, ";
            $sql .= "CUSTOMER_CATEGORY, ";
            $sql .= "SALES_TAX_GEOCODE, ";
            $sql .= "POSTAL_PLUS4_CODE, ";
            $sql .= "DATE_DEACTIVATED, ";
            $sql .= "CONSUMER_SIZE, ";
            $sql .= "COMMERCIAL_SIZE, ";
            $sql .= "WHEEL_SIZE, ";
            $sql .= "OPEN_SIZE, ";
            $sql .= "PRICE_LEVEL, ";
            $sql .= "PRICE_LEVEL_START_DATE, ";
            $sql .= "SERVICE_BAY, ";
            $sql .= "WAIVE_RESTOCK_FEE, ";
            $sql .= "WARRANTY_FLAG, ";
            $sql .= "ACCEPT_RETURN_FLAG ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDAR_CUST_BILLTO_V ";
            $sql .= "WHERE SITE_USE_ID = :site_use_id ";

            $bindVars = array(array("PH_NAME" => ":site_use_id", "VARIABLE" => $rval["BILL_TO_SITE_USE_ID"]));

            $stid = oracle_db_exec($sql, $bindVars);

            $results = oci_fetch_assoc($stid);

            if($results !== FALSE) {
                $rval["BILL_TO"] = $results;
            } else {
                $rval["STATUS"] = FALSE;
                $params = array('site_id' => $site_id);
                $default = "Unable to fetch bill to information to export to the external servers for party_site_id $site_id.";
                $rval["MESSAGE"] = CMR_Utility::getL18NString('get_site_data_for_push.message.3', $default, $params);

                return $rval;
            }

            $sql = "SELECT ";
            $sql .= "CUSTOMER_ID, ";
            $sql .= "ADDRESS_ID, ";
            $sql .= "SITE_USE_ID, ";
            $sql .= "SITE_USE_CODE, ";
            $sql .= "ACCEPT_RETURN_FLAG, ";
            $sql .= "COMMERCIAL_SIZE, ";
            $sql .= "COMMISSION_PERCENT, ";
            $sql .= "CONSUMER_SIZE, ";
            $sql .= "MOUNT_BALANCE, ";
            $sql .= "OPEN_SIZE, ";
            $sql .= "PRICE_LEVEL, ";
            $sql .= "TO_CHAR(PRICE_LEVEL_START_DATE, 'RRRR-MM-DD') PRICE_LEVEL_START_DATE, ";
            $sql .= "SERVICE_BAY, ";
            $sql .= "WAIVE_RESTOCK_FEE, ";
            $sql .= "WARRANTY_FLAG, ";
            $sql .= "WHEEL_SIZE, ";
            $sql .= "AAN_NUMBER, ";
            $sql .= "ALLIANCE, ";
            $sql .= "AMI_NUMBER, ";
            $sql .= "BFS_OE_NUMBER, ";
            $sql .= "BS6_DIGIT_NUMBER, ";
            $sql .= "COMMISSION_CODE, ";
            $sql .= "CONTI_OE_NUMBER, ";
            $sql .= "COOPER_OE_NUMBER, ";
            $sql .= "G3X_NUMBER, ";
            $sql .= "GOLD_NUMBER, ";
            $sql .= "GY_ASSOC_NUMBER, ";
            $sql .= "GY_NON_SIG_NUMBER, ";
            $sql .= "GY_OE_NUMBER, ";
            $sql .= "HANKOOK_OE_NUMBER, ";
            $sql .= "KUMHO_FUEL_NUMBER, ";
            $sql .= "MAST_OE_NUMBER, ";
            $sql .= "PIRELLI_OE_NUMBER, ";
            $sql .= "TIRESTARZ ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDAR_CUST_SITE_USES_ATTR_V ";
            $sql .= "WHERE ";
            $sql .= "SITE_USE_ID = :site_use_id ";

            $bindVars = array(array("PH_NAME" => ":site_use_id", "VARIABLE" => $rval["BILL_TO_SITE_USE_ID"]));

            $stid = oracle_db_exec($sql, $bindVars);

            $results = oci_fetch_assoc($stid);

            if($results !== FALSE) {
                $rval["BILL_TO_ATTRIBUTES"] = $results;
            } else {
                $rval["STATUS"] = FALSE;
                $params = array('site_id' => $site_id);
                $default = "Unable to fetch bill to site attribute information to export to the external servers for party_site_id $site_id.";
                $rval["MESSAGE"] = CMR_Utility::getL18NString('get_site_data_for_push.message.4', $default, $params);

                return $rval;
            }
        }

        if(!is_null($rval["SHIP_TO_SITE_USE_ID"])) {
            $sql = "SELECT ";
            $sql .= "TO_CHAR(CUSTOMER_ID) AS CUSTOMER_ID, ";
            $sql .= "CUSTOMER_NUMBER, ";
            $sql .= "CUSTOMER_NAME, ";
            $sql .= "DBA_NAME, ";
            $sql .= "CUSTOMER_CLASS_CODE, ";
            $sql .= "CUSTOMER_CLASS, ";
            $sql .= "TO_CHAR(BILL_TO_SITE_USE_ID) AS BILL_TO_SITE_USE_ID, ";
            $sql .= "TO_CHAR(SHIP_TO_SITE_USE_ID) AS SHIP_TO_SITE_USE_ID, ";
            $sql .= "LOCATION_NUMBER, ";
            $sql .= "STORE_NUMBER, ";
            $sql .= "ADDRESS1, ";
            $sql .= "CITY, ";
            $sql .= "COUNTY, ";
            $sql .= "COUNTY_FIPS, ";
            $sql .= "STATE, ";
            $sql .= "STATE_FIPS, ";
            $sql .= "POSTAL_CODE, ";
            $sql .= "ORGANIZATION_ID, ";
            $sql .= "PRIMARY_DC, ";
            $sql .= "PRIMARY_DC_CODE, ";
            $sql .= "NVL(PRIMARY_ORGANIZATION_ID, ORGANIZATION_ID) AS PRIMARY_ORGANIZATION_ID, ";
            $sql .= "PRIMARY_SALESREP_ID, ";
            $sql .= "Z_CUST_ACCT, ";
            $sql .= "TO_CHAR(PARTY_SITE_ID) AS PARTY_SITE_ID ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDONL_SHIPTO_V ";
            $sql .= "WHERE SHIP_TO_SITE_USE_ID = :site_use_id ";

            $bindVars = array(array("PH_NAME" => ":site_use_id", "VARIABLE" => $rval["SHIP_TO_SITE_USE_ID"]));
            $stid = oracle_db_exec($sql, $bindVars);

            $results = oci_fetch_assoc($stid);

            if($results !== FALSE) {
                $rval["SHIP_TO"] = $results;
            } else {
                $rval["STATUS"] = FALSE;
                $params = array('site_id' => $site_id);
                $default = "Unable to fetch ship to information to export to the external servers for party_site_id $site_id.";
                $rval["MESSAGE"] = CMR_Utility::getL18NString('get_site_data_for_push.message.5', $default, $params);

                return $rval;
            }

            if(is_null($results['PRIMARY_ORGANIZATION_ID'])){
                $rval["STATUS"] = FALSE;
                $params = array('site_id' => $site_id);
                $default = "Unable to fetch the primary org id for export to the external servers for party_site_id $site_id.";
                $rval["MESSAGE"] = CMR_Utility::getL18NString('get_site_data_for_push.message.6', $default, $params);

                return $rval;
            }

            $sql = "SELECT ";
            $sql .= "CUSTOMER_ID, ";
            $sql .= "ADDRESS_ID, ";
            $sql .= "SITE_USE_ID, ";
            $sql .= "SITE_USE_CODE, ";
            $sql .= "ACCEPT_RETURN_FLAG, ";
            $sql .= "COMMERCIAL_SIZE, ";
            $sql .= "COMMISSION_PERCENT, ";
            $sql .= "CONSUMER_SIZE, ";
            $sql .= "MOUNT_BALANCE, ";
            $sql .= "OPEN_SIZE, ";
            $sql .= "PRICE_LEVEL, ";
            $sql .= "TO_CHAR(PRICE_LEVEL_START_DATE, 'RRRR-MM-DD') PRICE_LEVEL_START_DATE, ";
            $sql .= "SERVICE_BAY, ";
            $sql .= "WAIVE_RESTOCK_FEE, ";
            $sql .= "WARRANTY_FLAG, ";
            $sql .= "WHEEL_SIZE, ";
            $sql .= "AAN_NUMBER, ";
            $sql .= "ALLIANCE, ";
            $sql .= "AMI_NUMBER, ";
            $sql .= "BFS_OE_NUMBER, ";
            $sql .= "BS6_DIGIT_NUMBER, ";
            $sql .= "COMMISSION_CODE, ";
            $sql .= "CONTI_OE_NUMBER, ";
            $sql .= "COOPER_OE_NUMBER, ";
            $sql .= "G3X_NUMBER, ";
            $sql .= "GOLD_NUMBER, ";
            $sql .= "GY_ASSOC_NUMBER, ";
            $sql .= "GY_NON_SIG_NUMBER, ";
            $sql .= "GY_OE_NUMBER, ";
            $sql .= "HANKOOK_OE_NUMBER, ";
            $sql .= "KUMHO_FUEL_NUMBER, ";
            $sql .= "MAST_OE_NUMBER, ";
            $sql .= "PIRELLI_OE_NUMBER, ";
            $sql .= "TIRESTARZ ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDAR_CUST_SITE_USES_ATTR_V ";
            $sql .= "WHERE ";
            $sql .= "SITE_USE_ID = :site_use_id ";

            $bindVars = array(array("PH_NAME" => ":site_use_id", "VARIABLE" => $rval["SHIP_TO_SITE_USE_ID"]));

            $stid = oracle_db_exec($sql, $bindVars);

            $results = oci_fetch_assoc($stid);

            if($results !== FALSE) {
                $rval["SHIP_TO_ATTRIBUTES"] = $results;
            } else {
                $rval["STATUS"] = FALSE;
                $params = array('site_id' => $site_id);
                $default = "Unable to fetch ship to site attribute information to export to the external servers for party_site_id $site_id.";
                $rval["MESSAGE"] = CMR_Utility::getL18NString('get_site_data_for_push.message.7', $default, $params);

                return $rval;
            }

            $sql = "SELECT ";
            $sql .= "CSUH.SITE_USE_ID AS SHIP_TO_SITE_USE_ID, ";
            $sql .= "CSUNA.SITE_USE_ID AS BILL_TO_SITE_USE_ID ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDAR_CUST_SITE_USES_ATTR CSUA, ";
            $sql .= "APPS.HZ_CUST_SITE_USES CSUH, ";
            $sql .= "APPS.FND_FLEX_VALUES_VL FFVL, ";
            $sql .= "APPS.FND_FLEX_VALUE_SETS FFVS, ";
            $sql .= "APPS.HZ_CUST_ACCOUNTS CANA, ";
            $sql .= "APPS.HZ_CUST_ACCT_SITES CASNA, ";
            $sql .= "APPS.HZ_CUST_SITE_USES CSUNA, ";
            $sql .= "APPS.HZ_CUST_ACCT_RELATE CAR ";
            $sql .= "WHERE ";
            $sql .= "CSUA.SITE_USE_ID = CSUH.SITE_USE_ID ";
            $sql .= "AND CSUA.ELEMENT_NAME = SUBSTR(FFVL.DESCRIPTION, INSTR(FFVL.DESCRIPTION, '.') + 1) || '_MEMBER' ";
            $sql .= "AND FFVL.FLEX_VALUE_SET_ID = FFVS.FLEX_VALUE_SET_ID ";
            $sql .= "AND FFVL.FLEX_VALUE = CANA.ACCOUNT_NUMBER ";
            $sql .= "AND CANA.CUST_ACCOUNT_ID = CASNA.CUST_ACCOUNT_ID ";
            $sql .= "AND CASNA.CUST_ACCT_SITE_ID = CSUNA.CUST_ACCT_SITE_ID ";
            $sql .= "AND CAR.CUST_ACCOUNT_ID = CSUA.CUSTOMER_ID ";
            $sql .= "AND CAR.RELATED_CUST_ACCOUNT_ID = CANA.CUST_ACCOUNT_ID ";
            $sql .= "AND FFVS.FLEX_VALUE_SET_NAME  = 'XXATDI_NATIONAL_ACCOUNT_MATRIX' ";
            $sql .= "AND FFVL.ENABLED_FLAG = 'Y' ";
            $sql .= "AND NVL(FFVL.START_DATE_ACTIVE, SYSDATE) <= SYSDATE ";
            $sql .= "AND NVL(FFVL.END_DATE_ACTIVE, SYSDATE) >= SYSDATE ";
            $sql .= "AND ELEMENT_VALUE = 'Y' ";
            $sql .= "AND CSUNA.SITE_USE_CODE = 'BILL_TO' ";
            $sql .= "AND CAR.STATUS = 'A' ";
            $sql .= "AND CANA.STATUS = 'A' ";
            $sql .= "AND CASNA.STATUS = 'A' ";
            $sql .= "AND CSUNA.STATUS = 'A' ";
            $sql .= "AND CSUH.STATUS = 'A' ";
            $sql .= "AND CSUH.SITE_USE_ID = :site_use_id ";
            $sql .= "UNION ";
            $sql .= "SELECT ";
            $sql .= "CSUH.SITE_USE_ID AS SHIP_TO_SITE_USE_ID, ";
            $sql .= "CSUNA.SITE_USE_ID AS BILL_TO_SITE_USE_ID ";
            $sql .= "FROM ";
            $sql .= "APPS.HZ_CUST_SITE_USES CSUH, ";
            $sql .= "APPS.HZ_CUST_ACCT_SITES CASH, ";
            $sql .= "APPS.FND_FLEX_VALUES_VL FFVL, ";
            $sql .= "APPS.FND_FLEX_VALUE_SETS FFVS, ";
            $sql .= "APPS.HZ_CUST_ACCOUNTS CANA, ";
            $sql .= "APPS.HZ_CUST_ACCT_SITES CASNA, ";
            $sql .= "APPS.HZ_CUST_SITE_USES CSUNA, ";
            $sql .= "APPS.HZ_CUST_ACCT_RELATE CAR ";
            $sql .= "WHERE ";
            $sql .= "FFVL.FLEX_VALUE_SET_ID = FFVS.FLEX_VALUE_SET_ID ";
            $sql .= "AND FFVL.FLEX_VALUE = CANA.ACCOUNT_NUMBER ";
            $sql .= "AND CANA.CUST_ACCOUNT_ID = CASNA.CUST_ACCOUNT_ID ";
            $sql .= "AND CASNA.CUST_ACCT_SITE_ID = CSUNA.CUST_ACCT_SITE_ID ";
            $sql .= "AND CAR.RELATED_CUST_ACCOUNT_ID = CANA.CUST_ACCOUNT_ID ";
            $sql .= "AND CAR.CUST_ACCOUNT_ID = CASH.CUST_ACCOUNT_ID ";
            $sql .= "AND CASH.CUST_ACCT_SITE_ID = CSUH.CUST_ACCT_SITE_ID ";
            $sql .= "AND FFVS.FLEX_VALUE_SET_NAME  = 'XXATDI_LEASING_COMPANY_MATRIX' ";
            $sql .= "AND FFVL.ENABLED_FLAG = 'Y' ";
            $sql .= "AND NVL(FFVL.START_DATE_ACTIVE, SYSDATE) <= SYSDATE ";
            $sql .= "AND NVL(FFVL.END_DATE_ACTIVE, SYSDATE) >= SYSDATE ";
            $sql .= "AND CSUNA.SITE_USE_CODE = 'BILL_TO' ";
            $sql .= "AND CAR.STATUS = 'A' ";
            $sql .= "AND CANA.STATUS = 'A' ";
            $sql .= "AND CASNA.STATUS = 'A' ";
            $sql .= "AND CSUNA.STATUS = 'A' ";
            $sql .= "AND CSUH.STATUS = 'A' ";
            $sql .= "AND CSUH.SITE_USE_ID = :site_use_id ";

            $bindVars = array(array("PH_NAME" => ":site_use_id", "VARIABLE" => $rval["SHIP_TO_SITE_USE_ID"]));

            $stid = oracle_db_exec($sql, $bindVars);

            if(!$stid) {
                $rval["STATUS"] = FALSE;
                $params = array('site_id' => $site_id);
                $default = "Unable to fetch BTX program information to export to the external servers for party_site_id $site_id.";
                $rval["MESSAGE"] = CMR_Utility::getL18NString('get_site_data_for_push.message.8', $default, $params);
            }

            while($results = oci_fetch_assoc($stid)) {
                $rval["BTX_SITE_PROGRAMS"][] = $results;
            }
        }

        return $rval;
    }

    function liveLocations($arguments, $config) {
        $rval = array();

        $bindVars = array();

        if(!is_numeric($arguments["SITE_ID"])) {
            unset($arguments["SITE_ID"]);
        }

        if(!is_numeric($arguments["CUSTOMER_ID"])) {
            unset($arguments["CUSTOMER_ID"]);
        }

        if(!is_numeric($arguments["WAREHOUSE_ID"])) {
            unset($arguments["WAREHOUSE_ID"]);
        }

        if(!is_numeric($arguments["LOCATION"])) {
            unset($arguments["LOCATION"]);
        }

        if(!is_numeric($arguments["STORE_NUMBER"])) {
            unset($arguments["STORE_NUMBER"]);
        }

        if(!is_numeric($arguments["PRIMARY_SALESREP_ID"])) {
            unset($arguments["PRIMARY_SALESREP_ID"]);
        }

        // Allow for listgrid filtered columns
        //
        $criteria = array(
            "PS.PARTY_SITE_ID" => array($arguments["SITE_ID"], "EQUALS"),
            "CA.CUST_ACCOUNT_ID" => array($arguments["CUSTOMER_ID"], "EQUALS"),
            "CA.ACCOUNT_NUMBER" => array($arguments["CUSTOMER_NUMBER"], "MATCHES"),
            "CA.ACCOUNT_NAME" => array($arguments["CUSTOMER_NAME"], "CONTAINS"),
            "CAS.ATTRIBUTE15" => array($arguments["Z_CUSTOMER"], "CONTAINS"),
            "(SELECT ELEMENT_VALUE FROM XXATDAR.XXATDAR_CUST_SITE_USES_ATTR CSUA WHERE CSUA.SITE_USE_ID = CSU.SITE_USE_ID AND CSUA.ELEMENT_NAME = 'AMI_NUMBER')" => array($arguments["AMI_NUMBER"], "CONTAINS"),
            "L.ADDRESS1" => array($arguments["ADDRESS1"], "CONTAINS"),
            "L.ADDRESS2" => array($arguments["ADDRESS2"], "CONTAINS"),
            "L.CITY" => array($arguments["CITY"], "CONTAINS"),
            "DECODE(NVL(L.COUNTRY, 'US'), 'US', L.STATE, L.PROVINCE)" => array($arguments["STATE"], "CONTAINS"),
            "L.COUNTRY" => array($arguments["COUNTRY"], "CONTAINS"),
            "L.POSTAL_CODE" => array($arguments["POSTAL_CODE"], "CONTAINS"),
            "L.POSTAL_PLUS4_CODE" => array($arguments["POSTAL_PLUS4_CODE"], "CONTAINS"),
            "DECODE(NVL(CAS.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D'))" => array($arguments["SITE_TYPE"], "MATCHES"),
            "DECODE(CAS.BILL_TO_FLAG, 'P', 'Y', NULL, 'N', CAS.BILL_TO_FLAG)" => array($arguments["BILL_TO_FLAG"], "MATCHES"),
            "DECODE(CAS.SHIP_TO_FLAG, 'P', 'Y', NULL, 'N', CAS.SHIP_TO_FLAG)" => array($arguments["SHIP_TO_FLAG"], "MATCHES"),
            "CSU.WAREHOUSE_ID" => array($arguments["WAREHOUSE_ID"], "EQUALS"),
            "CA.CUSTOMER_TYPE" => array($arguments["CUSTOMER_TYPE"], "MATCHES"),
            "CSU.LOCATION" => array($arguments["LOCATION"], "EQUALS"),
            "TO_NUMBER(CSU.ATTRIBUTE4)" => array($arguments["STORE_NUMBER"], "EQUALS"),
            "CSU.ATTRIBUTE6" => array($arguments["CUSTOMER_STORE_NUMBER"], "MATCHES"),
            "CSU.ATTRIBUTE7" => array($arguments["GLOBAL_LOCATION_NO"], "MATCHES"),
            "CSU.ATTRIBUTE8" => array($arguments["DUNS_NUMBER"], "MATCHES"),
            "PS.ADDRESSEE" => array($arguments["DBA_NAME"], "CONTAINS"),
            "CSU.PRIMARY_SALESREP_ID" => array($arguments["PRIMARY_SALESREP_ID"], "EQUALS"),
            "CAS.CUSTOMER_CATEGORY_CODE" => array($arguments["CUSTOMER_CATEGORY_CODE"], "MATCHES"),
            "CA.CUSTOMER_CLASS_CODE" => array($arguments["CUSTOMER_CLASS_CODE"], "MATCHES"),
            "PS.STATUS" => array($arguments["SITE_STATUS"], "MATCHES"),
            "TO_CHAR(PS.CREATION_DATE, 'MM/DD/RRRR HH:MI:SS AM')" => array($arguments["CREATION_DATE"], "CONTAINS")
        );

        $where_clause = build_where_clause($criteria, $bindVars);

        $sql = "SELECT ";
        $sql .= "PS.PARTY_SITE_ID AS SITE_ID, ";
        $sql .= "CA.CUST_ACCOUNT_ID AS CUSTOMER_ID, ";
        $sql .= "CA.ACCOUNT_NUMBER AS CUSTOMER_NUMBER, ";
        $sql .= "CA.ACCOUNT_NAME AS CUSTOMER_NAME, ";
        $sql .= "CAS.ATTRIBUTE15 AS Z_CUSTOMER, ";
        $sql .= "(SELECT ELEMENT_VALUE FROM XXATDAR.XXATDAR_CUST_SITE_USES_ATTR CSUA WHERE CSUA.SITE_USE_ID = CSU.SITE_USE_ID AND CSUA.ELEMENT_NAME = 'AMI_NUMBER') AS AMI_NUMBER, ";
        $sql .= "L.ADDRESS1, ";
        $sql .= "L.ADDRESS2, ";
        $sql .= "L.CITY, ";
        $sql .= "DECODE(NVL(L.COUNTRY, 'US'), 'US', L.STATE, L.PROVINCE) AS STATE, ";
        $sql .= "L.COUNTRY, ";
        $sql .= "L.POSTAL_CODE, ";
        $sql .= "L.POSTAL_PLUS4_CODE, ";
        $sql .= "DECODE(NVL(CAS.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D')) AS SITE_TYPE, ";
        $sql .= "DECODE(CAS.BILL_TO_FLAG, 'P', 'Y', NULL, 'N', CAS.BILL_TO_FLAG) AS BILL_TO_FLAG, ";
        $sql .= "DECODE(CAS.SHIP_TO_FLAG, 'P', 'Y', NULL, 'N', CAS.SHIP_TO_FLAG) AS SHIP_TO_FLAG, ";
        $sql .= "CSU.WAREHOUSE_ID AS WAREHOUSE_ID, ";
        $sql .= "CA.CUSTOMER_TYPE AS CUSTOMER_TYPE, ";
        $sql .= "CSU.LOCATION AS LOCATION, ";
        $sql .= "TO_NUMBER(CSU.ATTRIBUTE4) AS STORE_NUMBER, ";
        $sql .= "CSU.ATTRIBUTE6 AS CUSTOMER_STORE_NUMBER, ";
        $sql .= "CSU.ATTRIBUTE7 AS GLOBAL_LOCATION_NO, ";
        $sql .= "CSU.ATTRIBUTE8 AS DUNS_NUMBER, ";
        $sql .= "PS.ADDRESSEE AS DBA_NAME, ";
        $sql .= "PS.ADDRESSEE AS SITE_NAME, ";
        $sql .= "CSU.PRIMARY_SALESREP_ID AS PRIMARY_SALESREP_ID, ";
        $sql .= "CAS.CUSTOMER_CATEGORY_CODE, ";
        $sql .= "CA.CUSTOMER_CLASS_CODE, ";
        $sql .= "PS.STATUS AS SITE_STATUS, ";
        $sql .= "TO_CHAR(PS.CREATION_DATE, 'MM/DD/RRRR  HH:MI:SS AM') AS CREATION_DATE ";
        $sql .= "FROM ";
        $sql .= "APPS.HZ_CUST_ACCOUNTS CA, ";
        $sql .= "APPS.HZ_PARTY_SITES PS, ";
        $sql .= "APPS.HZ_LOCATIONS L, ";
        $sql .= "APPS.HZ_CUST_ACCT_SITES CAS, ";
        $sql .= "APPS.HZ_CUST_SITE_USES CSU ";
        $sql .= $where_clause;
        $sql .= ($where_clause) ? " AND " : "WHERE ";
        $sql .= "CA.CUST_ACCOUNT_ID = CAS.CUST_ACCOUNT_ID ";
        $sql .= "AND PS.LOCATION_ID = L.LOCATION_ID ";
        $sql .= "AND CAS.PARTY_SITE_ID = PS.PARTY_SITE_ID ";
        $sql .= "AND CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID ";
        $sql .= "AND CA.STATUS = 'A' ";
        $sql .= "AND CAS.STATUS = 'A' ";
        $sql .= "AND CSU.STATUS = 'A' ";
        $sql .= "AND CSU.SITE_USE_CODE = DECODE(DECODE(NVL(CAS.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D')), 'B', 'BILL_TO', 'SHIP_TO') ";

        $rval = oracle_paged_results($sql, $bindVars, $arguments["START"] + 1, $arguments["END"] + 1, $arguments["SORT_BY"], array("SITE_ID", "CUSTOMER_NUMBER", "CUSTOMER_ID", "LOCATION", "STORE_NUMBER"));

        // If the return value is empty, fake out the caller since this is a paged datasource.
        //
        if(empty($rval)) {
            $rval["data"] = array();
            $rval["DSResponse"] = array();
        }

        return $rval;
    }

    function stagedLocations($arguments, $config) {
        $rval = array();

        $bindVars = array();

        if(!is_numeric($arguments["SITE_ID"])) {
            unset($arguments["SITE_ID"]);
        }

        // Allow for listgrid filtered columns
        $criteria = array(
            "S.ID" => array($arguments["SITE_ID"], "EQUALS"),
            "DECODE(C.LIVE_CUSTOMER_ID,NULL,C.ID,HCA.ACCOUNT_NUMBER)" => array($arguments["CUSTOMER_NUMBER"], "CONTAINS"),
            "DECODE(C.LIVE_CUSTOMER_ID,NULL,C.ID,C.LIVE_CUSTOMER_ID)" => array($arguments["CUSTOMER_ID"], "MATCHES"),
            "CAS.LOCATION" => array($arguments["LOCATION"], "EQUALS"),
            "CAS.Z_CUSTOMER_NUMBER" => array($arguments["Z_CUSTOMER"], "CONTAINS"),
            "(SELECT CSUA.ELEMENT_VALUE FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'AMI_NUMBER')" => array($arguments["AMI_NUMBER"], "CONTAINS"),
            "DECODE(NVL(CAS.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D'))" => array($arguments["SITE_TYPE"], "MATCHES"),
            "CAS.WAREHOUSE_ID" => array($arguments["WAREHOUSE_ID"], "EQUALS"),
            "DECODE(C.LIVE_CUSTOMER_ID,NULL,NVL(P.PARTY_NAME, NVL(P.PERSON_FIRST_NAME, 'Not Identified') || ' ' || NVL(P.PERSON_LAST_NAME, 'Not Identified')),
                HCA.ACCOUNT_NAME)" => array($arguments["CUSTOMER_NAME"], "CONTAINS"),
            "CAS.DBA_NAME" => array($arguments["DBA_NAME"], "CONTAINS"),
            "L.ADDRESS1" => array($arguments["ADDRESS1"], "CONTAINS"),
            "L.ADDRESS2" => array($arguments["ADDRESS2"], "CONTAINS"),
            "L.CITY" => array($arguments["CITY"], "CONTAINS"),
            "UPPER(FVS.FLEX_VALUE)" => array($arguments["STATE"], "CONTAINS"),
            "L.COUNTRY" => array($arguments["COUNTRY"], "CONTAINS"),
            "L.POSTAL_CODE" => array($arguments["POSTAL_CODE"], "CONTAINS"),
            "L.POSTAL_PLUS4_CODE" => array($arguments["POSTAL_PLUS4_CODE"], "CONTAINS"),
            "CAS.STORE_NUMBER" => array($arguments["CUSTOMER_STORE_NUMBER"], "MATCHES"),
            "TO_CHAR(S.CREATION_DATE, 'MM/DD/RRRR HH:MI:SS AM')" => array($arguments["CREATION_DATE"], "CONTAINS"),
            "NVL(STATUS,'I')" => array($arguments["SITE_STATUS"], "MATCHES")
        );

        $where_clause = build_where_clause($criteria, $bindVars);

        $sql = "SELECT ";
        $sql .= "S.ID AS SITE_ID, ";
        $sql .= "DECODE(C.LIVE_CUSTOMER_ID,NULL,C.ID,HCA.ACCOUNT_NUMBER) AS CUSTOMER_NUMBER, ";
        $sql .= "DECODE(C.LIVE_CUSTOMER_ID,NULL,C.ID,C.LIVE_CUSTOMER_ID) AS CUSTOMER_ID, ";
        $sql .= "CAS.LOCATION AS LOCATION, ";
        $sql .= "CAS.Z_CUSTOMER_NUMBER AS Z_CUSTOMER, ";
        $sql .= "(SELECT CSUA.ELEMENT_VALUE FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'AMI_NUMBER') AS AMI_NUMBER, ";
        $sql .= "DECODE(NVL(CAS.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D')) AS SITE_TYPE, ";
        $sql .= "CAS.WAREHOUSE_ID AS WAREHOUSE_ID, ";
        $sql .= "DECODE(C.LIVE_CUSTOMER_ID,NULL,NVL(P.PARTY_NAME, NVL(P.PERSON_FIRST_NAME, 'Not Identified') || ' ' || NVL(P.PERSON_LAST_NAME, 'Not Identified')), HCA.ACCOUNT_NAME) AS CUSTOMER_NAME, ";
        $sql .= "CAS.DBA_NAME AS DBA_NAME, ";
        $sql .= "L.ADDRESS1, ";
        $sql .= "L.ADDRESS2, ";
        $sql .= "L.CITY, ";
        $sql .= "UPPER(FVS.FLEX_VALUE) AS STATE, ";
        $sql .= "L.COUNTRY, ";
        $sql .= "L.POSTAL_CODE, ";
        $sql .= "L.POSTAL_PLUS4_CODE, ";
        $sql .= "CAS.STORE_NUMBER AS STORE_NUMBER, ";
        $sql .= "TO_CHAR(S.CREATION_DATE, 'MM/DD/RRRR  HH:MI:SS AM') AS CREATION_DATE, ";
        $sql .= "'A' AS SITE_STATUS, ";
        $sql .= "SUBSTR(OU.NAME, 1, 3) AS BRANCH_ID, ";
        $sql .= "S.INFO_APPROVED, ";
        $sql .= "S.CONTACTS_APPROVED, ";
        $sql .= "S.BRANCH_APPROVED, ";
        $sql .= "S.AR_APPROVED, ";
        $sql .= "S.MA_APPROVED, ";
        $sql .= "S.OPS_APPROVED, ";
        $sql .= "S.B2B_APPROVED, ";
        $sql .= "'Y' AS STAGED_FLAG, ";
        $sql .= "DECODE(C.LIVE_CUSTOMER_ID,NULL,'Y','N') AS CUST_STAGED_FLAG ";
        $sql .= "FROM APPS.XXATDEAS_SITES S, ";
        $sql .= "APPS.XXATDEAS_LOCATIONS L, ";
        $sql .= "APPS.XXATDEAS_CUSTOMERS C, ";
        $sql .= "APPS.XXATDEAS_CUST_ACCOUNT_SITES CAS, ";
        $sql .= "APPS.HR_ORGANIZATION_UNITS OU, ";
        $sql .= "APPS.FND_FLEX_VALUES_VL FVS, ";
        $sql .= "APPS.FND_FLEX_VALUE_SETS FSS, ";
        $sql .= "APPS.XXATDEAS_PARTIES P, ";
        $sql .= "APPS.HZ_CUST_ACCOUNTS HCA ";
        $sql .= $where_clause;
        $sql .= ($where_clause) ? " AND " : "WHERE ";
        $sql .= "S.ID = L.SITE_ID ";
        $sql .= "AND S.ID = P.SITE_ID(+) ";
        $sql .= "AND 'P' = P.BILL_TO_FLAG(+) ";
        $sql .= "AND S.ID = CAS.SITE_ID ";
        $sql .= "AND S.CUSTOMER_ID = C.ID ";
        $sql .= "AND C.LIVE_CUSTOMER_ID = HCA.CUST_ACCOUNT_ID(+) ";
//        $sql .= "AND EXISTS ";
//        $sql .= "(SELECT 1 FROM APPS.HZ_CUST_ACCT_SITES HCAS, APPS.HZ_CUST_SITE_USES HCSU WHERE HCAS.CUST_ACCT_SITE_ID = HCSU.CUST_ACCT_SITE_ID ";
//        $sql .= "AND HCAS.CUST_ACCOUNT_ID = HCA.CUST_ACCOUNT_ID AND HCAS.STATUS = 'A' AND HCSU.STATUS = 'A') ";
        $sql .= "AND CAS.WAREHOUSE_ID = OU.ORGANIZATION_ID(+) ";
        $sql .= "AND L.STATE_FIPS = FVS.FLEX_VALUE_MEANING(+) ";
        $sql .= "AND 'Y' = FVS.ENABLED_FLAG(+) ";
        $sql .= "AND FSS.FLEX_VALUE_SET_NAME(+) = 'XXATD_STATE_FIPS' ";
        $sql .= "AND (FVS.FLEX_VALUE_SET_ID IS NULL  OR FVS.FLEX_VALUE_SET_ID = FSS.FLEX_VALUE_SET_ID) ";
        $sql .= "AND NVL (FVS.START_DATE_ACTIVE, SYSDATE) <= SYSDATE ";
        $sql .= "AND NVL (FVS.END_DATE_ACTIVE, SYSDATE) >= SYSDATE ";
        $sql .= ($show_type == "BATCH") ? "AND C.BATCH_NUMBER IS NOT NULL " : "AND C.BATCH_NUMBER IS NULL ";

        $rval = oracle_paged_results($sql, $bindVars, $arguments["START"] + 1, $arguments["END"] + 1, $arguments["SORT_BY"], array("SITE_ID", "CUSTOMER_NUMBER", "CUSTOMER_ID", "LOCATION", "STORE_NUMBER"));

        // If the return value is empty, fake out the caller since this is a paged datasource.
        //
        if(empty($rval)) {
            $rval["data"] = array();
            $rval["DSResponse"] = array();
        }

        return $rval;
    }

    function siteList($arguments, $config) {
        $rval = array();
        $oldValues = array();

        $source = $arguments["CUSTOMER_SOURCE"];

        switch($source) {

            case "STAGED":

                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteList_Staged($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval["data"][0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Make sure that the data is no longer approved
                    // and log the changed data
                    //
                    set_review_status("SITE", $arguments["SITE_ID"], "INFO", 0);
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToLocationInfoDForm", "SITE", $arguments["SITE_ID"], $oldValues, $arguments, $source, $this->controller->session["username"]);

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteList_Staged($arguments, $config);
                }

            break;

            case "LIVE":
                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteList_Live($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval["data"][0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Log the changed data
                    //
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToLocationInfoDForm", "SITE", $arguments["SITE_ID"], $oldValues, $arguments, $source, $this->controller->session["username"]);

                    $dropLocationATDOnlineSessions = FALSE;

                    // When updating the primary Org (DC) for a LIVE location, need to drop any/all active ATDOnline sessions for that location
                    if ($arguments['OPERATION_TYPE'] == 'update' && $oldValues['WAREHOUSE_ID'] != $arguments['WAREHOUSE_ID']){
                        $dropLocationATDOnlineSessions = TRUE;
                    }

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteList_Live($arguments, $config, $dropLocationATDOnlineSessions);
                }

            break;

            default:

                eventLog("CMR", "Warn", "siteList", $this->controller->session["username"], "Invalid Source", $source);

            break;

        }

        return $rval;
    }

    private function siteList_Staged($arguments, $config) {
        $rval = array();

        $customer_id = $arguments["CUSTOMER_ID"];
        $site_id = $arguments["SITE_ID"];

        if(!is_numeric($customer_id)) {
            unset($customer_id);
        }

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id) || isset($customer_id));
        $validUpdate = (isset($site_id));
        $validDelete = (isset($site_id));

        switch($arguments["OPERATION_TYPE"]) {
            case "update":
                if($validUpdate) {

                    $updateArgs = array();

                    $updateArgs["Z_CUSTOMER_NUMBER"] = array($arguments["Z_CUSTOMER"], "");
                    $updateArgs["CUSTOMER_CATEGORY_CODE"] = array($arguments["CUSTOMER_CATEGORY_CODE"], "");
                    $updateArgs["WAREHOUSE_ID"] = array($arguments["WAREHOUSE_ID"], "");
                    $updateArgs["LOCATION"] = array($arguments["LOCATION"], "");
                    $updateArgs["STORE_NUMBER"] = array($arguments["STORE_NUMBER"], "");
                    $updateArgs["DBA_NAME"] = array($arguments["DBA_NAME"], "");
                    $updateArgs["PRIMARY_SALESREP_ID"] = array($arguments["PRIMARY_SALESREP_ID"], "");
                    $updateArgs["RETURN_TO_CUSTOMER"] = array($arguments["RETURN_TO_CUSTOMER"], "");
                    $updateArgs["CUSTOMER_STATUS_CODE"] = array($arguments["CUSTOMER_STATUS_CODE"], "");
                    $updateArgs["CUSTOMER_STORE_NUMBER"] = array($arguments["CUSTOMER_STORE_NUMBER"], "");
                    $updateArgs["GLOBAL_LOCATION_NO"] = array($arguments["GLOBAL_LOCATION_NO"], "");
                    $updateArgs["DUNS_NUMBER"] = array($arguments["DUNS_NUMBER"], "");

                    $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

                    $sql = build_update_sql("APPS.XXATDEAS_CUST_ACCOUNT_SITES", $updateArgs, $bindVars, "SITE_ID = :site_id", TRUE);

                    if($sql) {
                        $result = oracle_db_exec($sql, $bindVars);
                    }

                    $updateArgs = array();
                    $updateArgs[] = array("NAME" => "PO_TEMPLATE", "TYPE" => "VARCHAR2");
                    $updateArgs[] = array("NAME" => "FUTURE_SALESREP_NUMBER", "TYPE" => "VARCHAR2");
                    $updateArgs[] = array("NAME" => "FUTURE_SALESREP_START_DATE", "TYPE" => "DATE");
                    $updateArgs[] = array("NAME" => "DRIVER_INSTR", "TYPE" => "VARCHAR2");
                    $updateArgs[] = array("NAME" => "ALLOW_TRANSFERS", "TYPE" => "VARCHAR2");
                    $updateArgs[] = array("NAME" => "ALLOW_BACKORDERS", "TYPE" => "VARCHAR2");
                    $updateArgs[] = array("NAME" => "LANGUAGE_PREFERENCE", "TYPE" => "VARCHAR2");

                    foreach($updateArgs as $field) {
                        $status = set_cust_site_use_attr($site_id, $field['NAME'], $arguments[$field['NAME']], $field['TYPE']);
                        if($status["X_RETURN_CODE"] != "SUCCESS") {
                            $rval["status"] = $this->STATUS_FAILURE;
                            $rval["errorMessage"] = CMR_Utility::getL18NString('siteList_Staged.update', "Failure within set_cust_site_use_attr: ") . $procArgs["X_RETURN_CODE"];
                            $rval["errors"] = $status;
                            $rval["data"] = array();
                            return $rval;
                        }
                    }
                }

            break;

            case "remove":

                if($validDelete) {

                    $sql = "SELECT COUNT(*) AS COUNT ";
                    $sql .= "FROM APPS.XXATDEAS_CUST_ACCOUNT_SITES ";
                    $sql .= "WHERE SITE_ID = :site_id AND BILL_TO_FLAG IN ('P', 'Y')";

                    $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

                    $stid = oracle_db_exec($sql, $bindVars);
                    $result = oci_fetch_assoc($stid);

                    if($result["COUNT"] == 0) {

                        $procArgs = array();
                        $procArgs["P_SITE_ID"] = $site_id;
                        $procArgs["X_RETURN_CODE"] = "";

                        oracle_proc_exec("APPS.XXATDEAS_CUST_MAINT_REG_PKG.REMOVE_STAGED_SITE", $procArgs);

                        if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                            $rval["status"] = $this->STATUS_FAILURE;
                            $rval["errorMessage"] = CMR_Utility::getL18NString('siteList_Staged.remove', "Failure within REMOVE_STAGED_SITE: ") . $procArgs["X_RETURN_CODE"];
                            $rval["errors"] = $procArgs;
                            $rval["data"] = array();
                            return $rval;
                        }
                    }
                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteList_Staged", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if( ($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "remove" && $validDelete) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $bindVars = array();

            $criteria_fields = array(
                "'Y'" => array($arguments['STAGED_FLAG'], "MATCHES")
            );

            $where_clause = build_where_clause($criteria_fields, $bindVars);

            $sql = "SELECT ";
            $sql .= "S.CUSTOMER_ID AS CUST_ACCOUNT_ID, ";
            $sql .= "S.ID AS SITE_ID, ";
            $sql .= "CAS.Z_CUSTOMER_NUMBER AS Z_CUSTOMER, ";
            $sql .= "L.ADDRESS1, ";
            $sql .= "L.ADDRESS2, ";
            $sql .= "L.CITY, ";
            $sql .= "UPPER(FVS.FLEX_VALUE) AS STATE, ";
            $sql .= "L.COUNTRY, ";
            $sql .= "L.POSTAL_CODE, ";
            $sql .= "L.POSTAL_PLUS4_CODE, ";
            $sql .= "NULL AS PS_OBJECT_VERSION_NUMBER, ";
            $sql .= "DECODE(NVL(CAS.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D')) AS SITE_TYPE, ";
            $sql .= "'Y' AS STAGED_FLAG, ";
            $sql .= "NULL AS CAS_OBJECT_VERSION_NUMBER, ";
            $sql .= "CAS.WAREHOUSE_ID AS WAREHOUSE_ID, ";
            $sql .= "SUBSTR(OU.NAME, 1, 3) AS BRANCH_ID, ";
            $sql .= "NVL(CA.CUSTOMER_TYPE, HCA.CUSTOMER_TYPE) AS CUSTOMER_TYPE, ";
            $sql .= "DECODE( CA.CUSTOMER_TYPE, NULL, DECODE(HCA.CUSTOMER_TYPE, NULL, 'ERROR', 'LIVE'), 'STAGED') AS CUSTOMER_SOURCE, ";
            $sql .= "'STAGED' AS SITE_SOURCE, ";
            $sql .= "CAS.LOCATION AS LOCATION, ";
            $sql .= "TO_NUMBER(CAS.STORE_NUMBER) AS STORE_NUMBER, ";
            $sql .= "CAS.DBA_NAME AS DBA_NAME, ";
            $sql .= "CAS.DBA_NAME AS SITE_NAME, ";
            $sql .= "CAS.PRIMARY_SALESREP_ID AS PRIMARY_SALESREP_ID, ";
            $sql .= "NULL AS CSU_OBJECT_VERSION_NUMBER, ";
            $sql .= "NULL AS L_OBJECT_VERSION_NUMBER, ";
            $sql .= "CAS.RETURN_TO_CUSTOMER, ";
            $sql .= "CAS.CUSTOMER_CATEGORY_CODE, ";
            $sql .= "NVL(CA.CUSTOMER_CLASS_CODE, HCA.CUSTOMER_CLASS_CODE) AS CUSTOMER_CLASS_CODE, ";
            $sql .= "CAS.CUSTOMER_STATUS_CODE AS CUSTOMER_STATUS_CODE, ";
            $sql .= "'A' as SITE_STATUS, ";
            $sql .= "TO_CHAR(S.CREATION_DATE, 'MM/DD/RRRR  HH:MI:SS AM') AS CREATION_DATE, ";
            $sql .= "S.INFO_APPROVED, ";
            $sql .= "S.CONTACTS_APPROVED, ";
            $sql .= "S.BRANCH_APPROVED, ";
            $sql .= "S.AR_APPROVED, ";
            $sql .= "S.MA_APPROVED, ";
            $sql .= "S.OPS_APPROVED, ";
            $sql .= "S.B2B_APPROVED, ";
            $sql .= "CAS.CUSTOMER_STORE_NUMBER, ";
            $sql .= "CAS.GLOBAL_LOCATION_NO, ";
            $sql .= "CAS.DUNS_NUMBER, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'PO_TEMPLATE') AS PO_TEMPLATE, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'DRIVER_INSTR') AS DRIVER_INSTR, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'ALLOW_TRANSFERS') AS ALLOW_TRANSFERS, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'ALLOW_BACKORDERS') AS ALLOW_BACKORDERS, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'FUTURE_SALESREP_NUMBER') AS FUTURE_SALESREP_NUMBER, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'FUTURE_SALESREP_START_DATE') AS FUTURE_SALESREP_START_DATE, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'LANGUAGE_PREFERENCE') AS LANGUAGE_PREFERENCE ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDEAS_SITES S, ";
            $sql .= "APPS.XXATDEAS_LOCATIONS L, ";
            $sql .= "APPS.HZ_CUST_ACCOUNTS HCA, ";
            $sql .= "APPS.XXATDEAS_CUSTOMERS C, ";
            $sql .= "APPS.XXATDEAS_CUST_ACCOUNTS CA, ";
            $sql .= "APPS.XXATDEAS_CUST_ACCOUNT_SITES CAS, ";
            $sql .= "APPS.HR_ORGANIZATION_UNITS OU, ";
            $sql .= "APPS.FND_FLEX_VALUES_VL FVS, ";
            $sql .= "APPS.FND_FLEX_VALUE_SETS FSS ";
            $sql .= $where_clause;
            $sql .= ($where_clause) ? " AND " : "WHERE ";
            $sql .= "S.ID = L.SITE_ID ";
            $sql .= "AND S.ID = CAS.SITE_ID ";
            $sql .= "AND S.CUSTOMER_ID = C.ID ";
            $sql .= "AND C.LIVE_CUSTOMER_ID = HCA.CUST_ACCOUNT_ID(+) ";
            $sql .= "AND S.CUSTOMER_ID = CA.CUSTOMER_ID(+) ";
            $sql .= "AND CAS.WAREHOUSE_ID = OU.ORGANIZATION_ID(+) ";
            $sql .= "AND L.STATE_FIPS = FVS.FLEX_VALUE_MEANING(+) ";
            $sql .= "AND FVS.ENABLED_FLAG(+) = 'Y' ";
            $sql .= "AND FSS.FLEX_VALUE_SET_NAME(+) = 'XXATD_STATE_FIPS' ";
            $sql .= "AND (FVS.FLEX_VALUE_SET_ID IS NULL OR FVS.FLEX_VALUE_SET_ID = FSS.FLEX_VALUE_SET_ID) ";
            $sql .= "AND NVL(FVS.START_DATE_ACTIVE, SYSDATE) <= SYSDATE ";
            $sql .= "AND NVL(FVS.END_DATE_ACTIVE, SYSDATE) >= SYSDATE ";

            if(isset($site_id)) {
                $sql .= "AND S.ID = :site_id ";

                $bindVars = array_merge($bindVars, array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id)));
            } elseif(isset($customer_id)) {
                $sql .= "AND S.CUSTOMER_ID = :customer_id ";

                $bindVars = array_merge($bindVars, array(array("PH_NAME" => ":customer_id", "VARIABLE" => $customer_id)));
            }

            $rval = oracle_paged_results($sql, $bindVars, $arguments["START"] + 1, $arguments["END"] + 1, $arguments["SORT_BY"]);

            if(empty($rval)) {
                $rval["data"] = array();
                $rval["DSResponse"] = array();
            }
        }

        return $rval;
    }

    private function siteList_Live($arguments, $config, $dropLocationATDOnlineSessions = FALSE) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];
        $customer_id = $arguments["CUSTOMER_ID"];
        $staged_flag = $arguments["STAGED_FLAG"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        if(!is_numeric($customer_id)) {
            unset($customer_id);
        }

        $validSelect = (isset($site_id) || isset($customer_id));
        $validUpdate = (isset($site_id));
        $validDelete = (isset($site_id));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":
                if($validUpdate) {

                    // Reference: EAS-651
                    // This is here to disable existing cross reference records for this store/z number combination.
                    //
                    // If either Z_CUSTOMER or STORE_NUMBER has changed call procedure DISABLE_CUST_XREF_SITE to
                    // disable existing xref records so that below the new record will be created as needed.
                    //
                    $sql = "
                    SELECT
                        CAS.PARTY_SITE_ID AS SITE_ID,
                        CAS.ATTRIBUTE15   AS OLD_Z_CUSTOMER,
                        CSU.ATTRIBUTE4    AS OLD_STORE_NUMBER
                    FROM
                        APPS.HZ_CUST_ACCT_SITES CAS,
                        APPS.HZ_CUST_SITE_USES CSU
                    WHERE
                        CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID
                        AND CAS.STATUS = 'A'
                        AND CSU.STATUS = 'A'
                        AND CSU.SITE_USE_CODE = 'SHIP_TO'
                        AND CAS.PARTY_SITE_ID = :site_id
                    ";
                    $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));
                    $stid = oracle_db_exec($sql, $bindVars);
                    $results = oci_fetch_assoc($stid);
                    if($results === FALSE) {
                        $rval["status"] = $this->STATUS_FAILURE;
                        $params = array('site_id' => $site_id);
                        $default = "Unable to fetch site information to update the XREF table for party_site_id $site_id.";
                        $rval["errorMessage"] = CMR_Utility::getL18NString('siteList_Live.update', $default, $params);
                        $rval["errors"] = var_export($results, TRUE);
                        $rval["data"] = array();
                        return $rval;
                    }

                    $old_z_customer = $results["OLD_Z_CUSTOMER"];
                    $old_store_number = $results["OLD_STORE_NUMBER"];

                    $padded_old_z_customer = str_pad($old_z_customer, 9, '0', STR_PAD_LEFT);
                    $padded_old_store_number = str_pad($old_store_number, 6, '0', STR_PAD_LEFT);

                    $padded_new_z_customer = str_pad($arguments["Z_CUSTOMER"], 9, '0', STR_PAD_LEFT);
                    $padded_new_store_number = str_pad($arguments["STORE_NUMBER"], 6, '0', STR_PAD_LEFT);

                    if( ($padded_new_store_number != $padded_old_store_number) ||
                        ($padded_new_z_customer != $padded_old_z_customer) ){

                        $disArgs = array();
                        $disArgs["P_PARTY_SITE_ID"] = $site_id;
                        $disArgs["X_RETURN_CODE"] = "";

                        oracle_proc_exec("APPS.XXATDEAS_CUST_MAINT_REG_PKG.DISABLE_CUST_XREF_SITE", $disArgs);

                        if($disArgs["X_RETURN_CODE"] != "SUCCESS") {
                            $rval["status"] = $this->STATUS_FAILURE;
                            $ret_code = $procArgs["X_RETURN_CODE"];
                            $params = array('ret_code' => $ret_code);
                            $default = "Failure within DISABLE_CUST_XREF_SITE: $ret_code";
                            $rval["errorMessage"] = CMR_Utility::getL18NString('siteList_Live.update.2', $default, $params);
                            $rval["errors"] = $disArgs;
                            $rval["data"] = array();
                            return $rval;
                        }

                        // This is here to make the crossd reference table look the way it should for the update
                        // below to work the way it should.
                        //
                        // This process updates the columns CONV_SHIP_TO_CUST and CONV_SHIPTO_STORE to have the
                        // new values which the use has entered.  This is needed so that when UPDATE_SITE_INFO
                        // runs the merge into process the compare causes an update to happen instead of an insert.
                        //
                        $sql = "
                        UPDATE APPS.XXATDAR_CUST_XREF SET
                            CONV_SHIPTO_CUSTOMER = :new_conv_shipto_cust,
                            CONV_SHIPTO_STORE = :new_conv_shipto_store
                        WHERE
                            CONV_SHIPTO_CUSTOMER = :old_conv_shipto_cust
                            AND CONV_SHIPTO_STORE = :old_conv_shipto_store
                        ";
                        $bindVars = array(
                            array('PH_NAME' => ":new_conv_shipto_cust", 'VARIABLE'  => $padded_new_z_customer),
                            array('PH_NAME' => ":new_conv_shipto_store", 'VARIABLE' => $padded_new_store_number),
                            array('PH_NAME' => ":old_conv_shipto_cust", 'VARIABLE'  => $padded_old_z_customer),
                            array('PH_NAME' => ":old_conv_shipto_store", 'VARIABLE' => $padded_old_store_number)
                        );
                        $stid = oracle_db_exec($sql, $bindVars);

                        if(!$stid) {
                            $rval["status"] = $this->STATUS_FAILURE;
                            $params = array('site_id' => $site_id);
                            $default = "Unable to update the XREF table for party_site_id $site_id.";
                            $rval["errorMessage"] = CMR_Utility::getL18NString('siteList_Live.update.3', $default, $params);
                            $rval["errors"] = $stid;
                            $rval["data"] = array();
                            return $rval;
                        }
                    }

                    $procArgs["P_PARTY_SITE_ID"]             = $site_id;
                    $procArgs["P_WAREHOUSE_ID"]              = $arguments["WAREHOUSE_ID"];
                    $procArgs["P_LOCATION"]                  = $arguments["LOCATION"];
                    $procArgs["P_STORE_NUMBER"]              = $arguments["STORE_NUMBER"];
                    $procArgs["P_PRIMARY_SALESREP_ID"]       = $arguments["PRIMARY_SALESREP_ID"];
                    $procArgs["P_CSU_OBJECT_VERSION_NUMBER"] = $arguments["CSU_OBJECT_VERSION_NUMBER"];
                    $procArgs["P_DBA_NAME"]                  = $arguments["DBA_NAME"];
                    $procArgs["P_PS_OBJECT_VERSION_NUMBER"]  = $arguments["PS_OBJECT_VERSION_NUMBER"];
                    $procArgs["P_RETURN_TO_CUSTOMER"]        = $arguments["RETURN_TO_CUSTOMER"];
                    $procArgs["P_Z_CUSTOMER"]                = $arguments["Z_CUSTOMER"];
                    $procArgs["P_CUSTOMER_CATEGORY_CODE"]    = $arguments["CUSTOMER_CATEGORY_CODE"];
                    $procArgs["P_CUSTOMER_STATUS_CODE"]      = $arguments["CUSTOMER_STATUS_CODE"];
                    $procArgs["P_CAS_OBJECT_VERSION_NUMBER"] = $arguments["CAS_OBJECT_VERSION_NUMBER"];
                    $procArgs["P_CUSTOMER_STORE_NUMBER"]     = $arguments["CUSTOMER_STORE_NUMBER"];
                    $procArgs["P_GLOBAL_LOCATION_NO"]        = $arguments["GLOBAL_LOCATION_NO"];
                    $procArgs["P_DUNS_NUMBER"]               = $arguments["DUNS_NUMBER"];
                    $procArgs["P_LANGUAGE_PREFERENCE"]       = $arguments["LANGUAGE_PREFERENCE"];
                    $procArgs["X_RETURN_CODE"]               = "";

                    oracle_proc_exec("APPS.XXATDEAS_CUST_MAINT_REG_PKG.UPDATE_SITE_INFO", $procArgs);

                    if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                        $rval["status"] = $this->STATUS_FAILURE;
                        $ret_code = $procArgs["X_RETURN_CODE"];
                        $params = array('ret_code' => $ret_code);
                        $default = "Failure within UPDATE_SITE_INFO: $ret_code";
                        $rval["errorMessage"] = CMR_Utility::getL18NString('siteList_Live.update.4', $default, $params);
                        $rval["errors"] = $procArgs;
                        $rval["data"] = array();
                        return $rval;
                    }

                    $procArgs = update_cross_reference_by_site($site_id);

                    if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                        $rval["status"] = $this->STATUS_FAILURE;
                        $ret_code = $procArgs["X_RETURN_CODE"];
                        $params = array('ret_code' => $ret_code);
                        $default = "Failure within UPDATE_SITE_INFO: $ret_code";
                        $rval["errorMessage"] = CMR_Utility::getL18NString('siteList_Live.update.4', $default, $params);
                        $rval["errors"] = $procArgs;
                        $rval["data"] = array();
                        return $rval;
                    }

                    $fields = array(
                        array("NAME" => "PO_TEMPLATE", "TYPE" => "VARCHAR2"),
                        array("NAME" => "FUTURE_SALESREP_NUMBER", "TYPE" => "VARCHAR2"),
                        array("NAME" => "FUTURE_SALESREP_START_DATE", "TYPE" => "DATE"),
                        array("NAME" => "DRIVER_INSTR", "TYPE" => "VARCHAR2"),
                        array("NAME" => "ALLOW_TRANSFERS", "TYPE" => "VARCHAR2"),
                        array("NAME" => "ALLOW_BACKORDERS", "TYPE" => "VARCHAR2")
                    );

                    foreach($fields as $field) {
                        $procArgs = set_cust_site_use_attr_live($site_id, "SHIP_TO", $field["NAME"], $arguments[$field["NAME"]], $field["TYPE"]);

                        if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                            $rval["status"] = $this->STATUS_FAILURE;
                            $ret_code = $procArgs["X_RETURN_CODE"];
                            $params = array('ret_code' => $ret_code);
                            $default = "Failure within set_cust_site_use_attr_live: $ret_code";
                            $rval["errorMessage"] = CMR_Utility::getL18NString('siteList_Live.update.5', $default, $params);
                            $rval["errors"] = $procArgs;
                            $rval["data"] = array();
                            return $rval;
                        }
                    }

                    //When updating the primary Org (DC) for a LIVE location, need to drop any/all active ATDOnline sessions for that location
                    if($dropLocationATDOnlineSessions == TRUE) {
                        $live_isa_user_account_server = $this->controller->servers_ini_file[$this->controller->server]["live_isa_user_account_server"];
                        $live_isa_user_account_user = $this->controller->servers_ini_file[$this->controller->server]["live_isa_user_account_user"];

                        $atdonline_db_conn = $this->controller->ConnectToMySQL($this->controller->credentials_file, $live_isa_user_account_server, $live_isa_user_account_user);

                        $sql = "DELETE FROM SITE_DATA.SESSIONS WHERE LOCATION_NUMBER = '{$arguments["LOCATION"]}' AND LOCATION_NUMBER IS NOT NULL";

                        list($result, $numrows) = mysql_db_exec($sql, $atdonline_db_conn);
                        // NOTE: No error handling here is intentional - should the session drop not occur, fail silently
                    }

                }

            break;

            case "remove":

                if($validDelete) {
                    if($staged_flag == "N") {
                        $procArgs["P_PARTY_SITE_ID"] = $site_id;
                        $procArgs["P_STATUS"] = 'I';
                        $procArgs["P_OBJECT_VERSION_NUMBER"] = $arguments["PS_OBJECT_VERSION_NUMBER"];
                        $procArgs["X_RETURN_CODE"] = "";

                        oracle_proc_exec("APPS.XXATDEAS_CUST_MAINT_REG_PKG.SET_SITE_STATUS", $procArgs);

                        if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                            $rval["status"] = $this->STATUS_FAILURE;
                            $ret_code = $procArgs["X_RETURN_CODE"];
                            $params = array('ret_code' => $ret_code);
                            $default = "Failure within SET_SITE_STATUS: $ret_code";
                            $rval["errorMessage"] = CMR_Utility::getL18NString('siteList_Live.remove.1', $default, $params);
                            $rval["errors"] = $procArgs;
                            $rval["data"] = array();
                            return $rval;
                        }
                    } else {
                        $sql = "SELECT COUNT(*) AS COUNT ";
                        $sql .= "FROM APPS.XXATDEAS_CUST_ACCOUNT_SITES ";
                        $sql .= "WHERE SITE_ID = :site_id AND BILL_TO_FLAG IN ('P', 'Y')";

                        $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

                        $stid = oracle_db_exec($sql, $bindVars);
                        $result = oci_fetch_assoc($stid);

                        if($result["COUNT"] == 0) {

                            $procArgs = array();

                            $procArgs["P_SITE_ID"] = $site_id;
                            $procArgs["X_RETURN_CODE"] = "";

                            oracle_proc_exec("APPS.XXATDEAS_CUST_MAINT_REG_PKG.REMOVE_STAGED_SITE", $procArgs);

                            if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                                $rval["status"] = $this->STATUS_FAILURE;
                                $ret_code = $procArgs["X_RETURN_CODE"];
                                $params = array('ret_code' => $ret_code);
                                $default = "Failure within REMOVE_STAGED_SITE: $ret_code";
                                $rval["errorMessage"] = CMR_Utility::getL18NString('siteList_Live.remove.2', $default, $params);
                                $rval["errors"] = $procArgs;
                                $rval["data"] = array();
                                return $rval;
                            }
                        }
                    }
                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteList_Live", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if( ($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "remove" && $validDelete) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $bindVars = array();

            $live_criteria_fields = array("'N'" => array($arguments['STAGED_FLAG'], "MATCHES"));

            $staged_criteria_fields = array("'Y'" => array($arguments['STAGED_FLAG'], "MATCHES"));

            $live_where_clause = build_where_clause($live_criteria_fields, $bindVars, "live_where_clause_bind");

            if(!isset($site_id) && isset($customer_id)) {
                $staged_where_clause = build_where_clause($staged_criteria_fields, $bindVars, "staged_where_clause_bind");
            }

            $sql = "SELECT ";
            $sql .= "CA.CUST_ACCOUNT_ID, ";
            $sql .= "PS.PARTY_SITE_ID AS SITE_ID, ";
            $sql .= "CAS.ATTRIBUTE15 AS Z_CUSTOMER, ";
            $sql .= "L.ADDRESS1, ";
            $sql .= "L.ADDRESS2, ";
            $sql .= "L.CITY, ";
            $sql .= "DECODE(NVL(L.COUNTRY, 'US'), 'US', L.STATE, L.PROVINCE) AS STATE, ";
            $sql .= "L.COUNTRY, ";
            $sql .= "L.POSTAL_CODE, ";
            $sql .= "L.POSTAL_PLUS4_CODE, ";
            $sql .= "PS.OBJECT_VERSION_NUMBER AS PS_OBJECT_VERSION_NUMBER, ";
            $sql .= "DECODE(NVL(CAS.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D')) AS SITE_TYPE, ";
            $sql .= "'N' AS STAGED_FLAG, ";
            $sql .= "CAS.OBJECT_VERSION_NUMBER AS CAS_OBJECT_VERSION_NUMBER, ";
            $sql .= "CSU.WAREHOUSE_ID AS WAREHOUSE_ID, ";
            $sql .= "SUBSTR(OU.NAME, 1, 3) AS BRANCH_ID, ";
            $sql .= "CA.CUSTOMER_TYPE AS CUSTOMER_TYPE, ";
            $sql .= "'LIVE' AS CUSTOMER_SOURCE, ";
            $sql .= "'LIVE' AS SITE_SOURCE, ";
            $sql .= "CSU.LOCATION AS LOCATION, ";
            $sql .= "TO_NUMBER(CSU.ATTRIBUTE4) AS STORE_NUMBER, ";
            $sql .= "CSU.ATTRIBUTE6 AS CUSTOMER_STORE_NUMBER, ";
            $sql .= "CSU.ATTRIBUTE7 AS GLOBAL_LOCATION_NO, ";
            $sql .= "CSU.ATTRIBUTE8 AS DUNS_NUMBER, ";
            $sql .= "PS.ADDRESSEE AS DBA_NAME, ";
            $sql .= "CSU.PRIMARY_SALESREP_ID AS PRIMARY_SALESREP_ID, ";
            $sql .= "CSU.OBJECT_VERSION_NUMBER AS CSU_OBJECT_VERSION_NUMBER, ";
            $sql .= "L.OBJECT_VERSION_NUMBER AS L_OBJECT_VERSION_NUMBER, ";
            $sql .= "CSU.ATTRIBUTE5 AS RETURN_TO_CUSTOMER, ";
            $sql .= "CAS.CUSTOMER_CATEGORY_CODE, ";
            $sql .= "CA.CUSTOMER_CLASS_CODE, ";
            $sql .= "CAS.ATTRIBUTE1 AS CUSTOMER_STATUS_CODE, ";
            $sql .= "PS.STATUS AS SITE_STATUS, ";
            $sql .= "TO_CHAR(PS.CREATION_DATE, 'MM/DD/RRRR  HH:MI:SS AM') AS CREATION_DATE, ";
            $sql .= "-1 AS INFO_APPROVED, ";
            $sql .= "-1 AS CONTACTS_APPROVED, ";
            $sql .= "-1 AS BRANCH_APPROVED, ";
            $sql .= "-1 AS AR_APPROVED, ";
            $sql .= "-1 AS MA_APPROVED, ";
            $sql .= "-1 AS OPS_APPROVED, ";
            $sql .= "-1 AS B2B_APPROVED, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDAR_CUST_SITE_USES_ATTR CSUA ";
            $sql .= "WHERE CSUA.SITE_USE_ID = CSU.SITE_USE_ID AND CSUA.ELEMENT_NAME = 'PO_TEMPLATE') AS PO_TEMPLATE, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDAR_CUST_SITE_USES_ATTR CSUA ";
            $sql .= "WHERE CSUA.SITE_USE_ID = CSU.SITE_USE_ID AND CSUA.ELEMENT_NAME = 'DRIVER_INSTR') AS DRIVER_INSTR, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDAR_CUST_SITE_USES_ATTR CSUA ";
            $sql .= "WHERE CSUA.SITE_USE_ID = CSU.SITE_USE_ID AND CSUA.ELEMENT_NAME = 'ALLOW_TRANSFERS') AS ALLOW_TRANSFERS, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDAR_CUST_SITE_USES_ATTR CSUA ";
            $sql .= "WHERE CSUA.SITE_USE_ID = CSU.SITE_USE_ID AND CSUA.ELEMENT_NAME = 'ALLOW_BACKORDERS') AS ALLOW_BACKORDERS, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDAR_CUST_SITE_USES_ATTR CSUA ";
            $sql .= "WHERE CSUA.SITE_USE_ID = CSU.SITE_USE_ID AND CSUA.ELEMENT_NAME = 'FUTURE_SALESREP_NUMBER') AS FUTURE_SALESREP_NUMBER, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDAR_CUST_SITE_USES_ATTR CSUA ";
            $sql .= "WHERE CSUA.SITE_USE_ID = CSU.SITE_USE_ID AND CSUA.ELEMENT_NAME = 'FUTURE_SALESREP_START_DATE') AS FUTURE_SALESREP_START_DATE, ";
            $sql .= "L.LANGUAGE AS LANGUAGE_PREFERENCE ";
            $sql .= "FROM ";
            $sql .= "APPS.HZ_CUST_ACCOUNTS CA, ";
            $sql .= "APPS.HZ_PARTY_SITES PS, ";
            $sql .= "APPS.HZ_LOCATIONS L, ";
            $sql .= "APPS.HZ_CUST_ACCT_SITES CAS, ";
            $sql .= "APPS.HZ_CUST_SITE_USES CSU, ";
            $sql .= "APPS.HR_ORGANIZATION_UNITS OU ";
            $sql .= $live_where_clause;
            $sql .= ($live_where_clause) ? " AND " : "WHERE ";
            $sql .= "CA.CUST_ACCOUNT_ID = CAS.CUST_ACCOUNT_ID ";
            $sql .= "AND CAS.PARTY_SITE_ID = PS.PARTY_SITE_ID ";
            $sql .= "AND PS.LOCATION_ID = L.LOCATION_ID ";
            $sql .= "AND CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID ";
            $sql .= "AND CSU.WAREHOUSE_ID = OU.ORGANIZATION_ID(+) ";
            $sql .= "AND CAS.STATUS = 'A' ";
            $sql .= "AND CSU.STATUS = 'A' ";
            $sql .= "AND CSU.SITE_USE_CODE = DECODE(DECODE(NVL(CAS.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D')), 'B', 'BILL_TO', 'SHIP_TO')";

            if(isset($site_id)) {
                $sql .= " AND PS.PARTY_SITE_ID = :site_id ";

                $bindVars = array_merge($bindVars, array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id)));
            } elseif(isset($customer_id)) {
                // load all of the staged sites too!
                $sql .= "AND CA.CUST_ACCOUNT_ID = :customer_id ";
                $sql .= "UNION ALL ";
                $sql .= "SELECT ";

                $sql .= "S.CUSTOMER_ID AS CUST_ACCOUNT_ID, ";
                $sql .= "S.ID AS SITE_ID, ";
                $sql .= "CAS.Z_CUSTOMER_NUMBER AS Z_CUSTOMER, ";
                $sql .= "L.ADDRESS1, ";
                $sql .= "L.ADDRESS2, ";
                $sql .= "L.CITY, ";
                $sql .= "UPPER(FVS.FLEX_VALUE) AS STATE, ";
                $sql .= "L.COUNTRY, ";
                $sql .= "L.POSTAL_CODE, ";
                $sql .= "L.POSTAL_PLUS4_CODE, ";
                $sql .= "NULL AS PS_OBJECT_VERSION_NUMBER, ";
                $sql .= "DECODE(NVL(CAS.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D')) AS SITE_TYPE, ";
                $sql .= "'Y' AS STAGED_FLAG, ";
                $sql .= "NULL AS CAS_OBJECT_VERSION_NUMBER, ";
                $sql .= "CAS.WAREHOUSE_ID AS WAREHOUSE_ID, ";
                $sql .= "SUBSTR(OU.NAME, 1, 3) AS BRANCH_ID, ";
                $sql .= "CA.CUSTOMER_TYPE AS CUSTOMER_TYPE, ";
                $sql .= "DECODE(C.LIVE_CUSTOMER_ID,NULL,'LIVE','STAGED') AS CUSTOMER_SOURCE, ";
                $sql .= "'STAGED' AS SITE_SOURCE, ";
                $sql .= "CAS.LOCATION AS LOCATION, ";
                $sql .= "TO_NUMBER(CAS.STORE_NUMBER) AS STORE_NUMBER, ";
                $sql .= "CAS.CUSTOMER_STORE_NUMBER, ";
                $sql .= "CAS.GLOBAL_LOCATION_NO, ";
                $sql .= "CAS.DUNS_NUMBER, ";
                $sql .= "CAS.DBA_NAME AS DBA_NAME, ";
                $sql .= "CAS.PRIMARY_SALESREP_ID AS PRIMARY_SALESREP_ID, ";
                $sql .= "NULL AS CSU_OBJECT_VERSION_NUMBER, ";
                $sql .= "NULL AS L_OBJECT_VERSION_NUMBER, ";
                $sql .= "CAS.RETURN_TO_CUSTOMER, ";
                $sql .= "CAS.CUSTOMER_CATEGORY_CODE, ";
                $sql .= "CA.CUSTOMER_CLASS_CODE, ";
                $sql .= "CAS.CUSTOMER_STATUS_CODE AS CUSTOMER_STATUS_CODE, ";
                $sql .= "'A' as SITE_STATUS, ";
                $sql .= "TO_CHAR(S.CREATION_DATE, 'MM/DD/RRRR  HH:MI:SS AM') AS CREATION_DATE, ";
                $sql .= "S.INFO_APPROVED, ";
                $sql .= "S.CONTACTS_APPROVED, ";
                $sql .= "S.BRANCH_APPROVED, ";
                $sql .= "S.AR_APPROVED, ";
                $sql .= "S.MA_APPROVED, ";
                $sql .= "S.OPS_APPROVED, ";
                $sql .= "S.B2B_APPROVED, ";
                $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
                $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
                $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'PO_TEMPLATE') AS PO_TEMPLATE, ";
                $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
                $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
                $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'DRIVER_INSTR') AS DRIVER_INSTR, ";
                $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
                $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
                $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'ALLOW_TRANSFERS') AS ALLOW_TRANSFERS, ";
                $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
                $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
                $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'ALLOW_BACKORDERS') AS ALLOW_BACKORDERS, ";
                $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
                $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
                $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'FUTURE_SALESREP_NUMBER') AS FUTURE_SALESREP_NUMBER, ";
                $sql .= "(SELECT TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
                $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
                $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'FUTURE_SALESREP_START_DATE') AS FUTURE_SALESREP_START_DATE, ";
                $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
                $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
                $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'LANGUAGE_PREFERENCE') AS LANGUAGE_PREFERENCE ";
				$sql .= "FROM ";
                $sql .= "APPS.XXATDEAS_SITES S, ";
                $sql .= "APPS.XXATDEAS_LOCATIONS L, ";
                $sql .= "APPS.XXATDEAS_CUSTOMERS C, ";
                $sql .= "APPS.XXATDEAS_CUST_ACCOUNTS CA, ";
                $sql .= "APPS.XXATDEAS_CUST_ACCOUNT_SITES CAS, ";
                $sql .= "APPS.HR_ORGANIZATION_UNITS OU, ";
                $sql .= "APPS.FND_FLEX_VALUES_VL FVS, ";
                $sql .= "APPS.FND_FLEX_VALUE_SETS FSS ";
                $sql .= $staged_where_clause;
                $sql .= ($staged_where_clause) ? " AND " : "WHERE ";
                $sql .= "S.ID = L.SITE_ID ";
                $sql .= "AND S.ID = CAS.SITE_ID ";
                $sql .= "AND S.CUSTOMER_ID = CA.CUSTOMER_ID(+) ";
                $sql .= "AND S.CUSTOMER_ID = C.ID ";
                $sql .= "AND CAS.WAREHOUSE_ID = OU.ORGANIZATION_ID(+) ";
                $sql .= "AND C.LIVE_CUSTOMER_ID = :customer_id ";
                $sql .= "AND L.STATE_FIPS = FVS.FLEX_VALUE_MEANING(+) ";
                $sql .= "AND FVS.ENABLED_FLAG(+) = 'Y' ";
                $sql .= "AND FSS.FLEX_VALUE_SET_NAME(+) = 'XXATD_STATE_FIPS' ";
                $sql .= "AND (FVS.FLEX_VALUE_SET_ID IS NULL OR FVS.FLEX_VALUE_SET_ID = FSS.FLEX_VALUE_SET_ID) ";
                $sql .= "AND NVL(FVS.START_DATE_ACTIVE, SYSDATE) <= SYSDATE ";
                $sql .= "AND NVL(FVS.END_DATE_ACTIVE, SYSDATE) >= SYSDATE ";

                $bindVars = array_merge($bindVars, array(array("PH_NAME" => ":customer_id", "VARIABLE" => $customer_id)));
            }

            $rval = oracle_paged_results($sql, $bindVars, $arguments["START"] + 1, $arguments["END"] + 1, $arguments["SORT_BY"]);
        }

        if(empty($rval)) {
            $rval["data"] = array();
            $rval["DSResponse"] = array();
        }

        return $rval;
    }

    public function marketingLocations($arguments, $config) {
        $rval = array();

        $bindVars = array();

        if(!is_numeric($arguments["SITE_ID"])) {
            unset($arguments["SITE_ID"]);
        }

        if(!is_numeric($arguments["WAREHOUSE_ID"])) {
            unset($arguments["WAREHOUSE_ID"]);
        }

        if(!is_numeric($arguments["LOCATION"])) {
            unset($arguments["LOCATION"]);
        }

        if(!is_numeric($arguments["STORE_NUMBER"])) {
            unset($arguments["STORE_NUMBER"]);
        }

        // Build filters.
        //
        $live_criteria_fields = array(
            "CAS.PARTY_SITE_ID" => array($arguments["SITE_ID"], "EQUALS"),
            "CA.CUST_ACCOUNT_ID" => array($arguments["CUSTOMER_ID"], "MATCHES"),
            "SMP.MEMBERSHIP_TYPE" => array($arguments["MEMBERSHIP_TYPE"], "MATCHES"),
            "SMP.MEMBERSHIP_NUMBER" => array($arguments["MEMBERSHIP_NUMBER"], "MATCHES"),
            "CA.ACCOUNT_NUMBER" => array($arguments["CUSTOMER_NUMBER"], "MATCHES"),
            "CSU.LOCATION" => array($arguments["LOCATION"], "EQUALS"),
            "CAS.ATTRIBUTE15" => array($arguments["Z_CUSTOMER"], "CONTAINS"),
            "DECODE(NVL(CAS.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D'))" => array($arguments["SITE_TYPE"], "MATCHES"),
            "CSU.WAREHOUSE_ID" => array($arguments["WAREHOUSE_ID"], "EQUALS"),
            "CA.ACCOUNT_NAME" => array($arguments["CUSTOMER_NAME"], "CONTAINS"),
            "PS.ADDRESSEE" => array($arguments["DBA_NAME"], "CONTAINS"),
            "L.ADDRESS1" => array($arguments["ADDRESS1"], "CONTAINS"),
            "L.ADDRESS2" => array($arguments["ADDRESS2"], "CONTAINS"),
            "L.CITY" => array($arguments["CITY"], "CONTAINS"),
            "DECODE(NVL(L.COUNTRY,'US'), 'US', L.STATE, L.PROVINCE)" => array($arguments["STATE"], "CONTAINS"),
            "L.COUNTRY" => array($arguments["COUNTRY"], "CONTAINS"),
            "L.POSTAL_CODE" => array($arguments["POSTAL_CODE"], "CONTAINS"),
            "L.POSTAL_PLUS4_CODE" => array($arguments["POSTAL_PLUS4_CODE"], "CONTAINS"),
            "TO_NUMBER(CSU.ATTRIBUTE4)" => array($arguments["STORE_NUMBER"], "EQUALS"),

        );
        $live_where_clause = build_where_clause($live_criteria_fields, $bindVars, "live_where_clause_bind");
        $live_where_clause = str_replace('WHERE ', 'AND ', $live_where_clause);

        $stage_criteria_fields = array(
            "S.ID" => array($arguments["SITE_ID"], "EQUALS"),
            "CA.CUSTOMER_ID" => array($arguments["CUSTOMER_ID"], "MATCHES"),
            "SMP.MEMBERSHIP_TYPE" => array($arguments["MEMBERSHIP_TYPE"], "CONTAINS"),
            "SMP.MEMBERSHIP_NUMBER" => array($arguments["MEMBERSHIP_NUMBER"], "CONTAINS"),
            "CA.CUSTOMER_ID" => array($arguments["CUSTOMER_NUMBER"], "MATCHES"),
            "CAS.LOCATION" => array($arguments["LOCATION"], "EQUALS"),
            "CAS.Z_CUSTOMER_NUMBER" => array($arguments["Z_CUSTOMER"], "CONTAINS"),
            "(SELECT ELEMENT_VALUE FROM XXATDAR.XXATDAR_CUST_SITE_USES_ATTR CSUA WHERE CSUA.SITE_USE_ID = CSU.SITE_USE_ID AND CSUA.ELEMENT_NAME = 'AMI_NUMBER')" => array($arguments["AMI_NUMBER"], "CONTAINS"),
            "DECODE(NVL(CAS.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D'))" => array($arguments["SITE_TYPE"], "MATCHES"),
            "CAS.WAREHOUSE_ID" => array($arguments["WAREHOUSE_ID"], "EQUALS"),
            "UPPER(NVL(P.PARTY_NAME, NVL(P.PERSON_FIRST_NAME, 'Not Identified') || ' ' || NVL(P.PERSON_LAST_NAME, 'Not Identified')))" => array($arguments["CUSTOMER_NAME"], "CONTAINS"),
            "CAS.DBA_NAME" => array($arguments["DBA_NAME"], "CONTAINS"),
            "L.ADDRESS1" => array($arguments["ADDRESS1"], "CONTAINS"),
            "L.ADDRESS2" => array($arguments["ADDRESS2"], "CONTAINS"),
            "L.CITY" => array($arguments["CITY"], "CONTAINS"),
            "UPPER(FVS.FLEX_VALUE)" => array($arguments["STATE"], "CONTAINS"),
            "L.COUNTRY" => array($arguments["COUNTRY"], "CONTAINS"),
            "L.POSTAL_CODE" => array($arguments["POSTAL_CODE"], "CONTAINS"),
            "L.POSTAL_PLUS4_CODE" => array($arguments["POSTAL_PLUS4_CODE"], "CONTAINS"),
            "TO_NUMBER(CAS.STORE_NUMBER)" => array($arguments["STORE_NUMBER"], "EQUALS")
        );
        $stage_where_clause = build_where_clause($stage_criteria_fields, $bindVars, "stage_where_clause_bind");
        $stage_where_clause = str_replace('WHERE ', 'AND ', $stage_where_clause);

        $sql = "
        SELECT
            CAS.PARTY_SITE_ID AS SITE_ID,
            CA.CUST_ACCOUNT_ID AS CUSTOMER_ID,
            'LIVE' AS STAGELIVE,
            SMP.MEMBERSHIP_TYPE AS MEMBERSHIP_TYPE,
            SMP.MEMBERSHIP_NUMBER AS MEMBERSHIP_NUMBER,
            CA.ACCOUNT_NUMBER AS CUSTOMER_NUMBER,
            CSU.LOCATION AS LOCATION,
            CAS.ATTRIBUTE15 AS Z_CUSTOMER,
            (SELECT ELEMENT_VALUE FROM XXATDAR.XXATDAR_CUST_SITE_USES_ATTR CSUA WHERE CSUA.SITE_USE_ID = CSU.SITE_USE_ID AND CSUA.ELEMENT_NAME = 'AMI_NUMBER') AS AMI_NUMBER,
            DECODE (NVL (CAS.BILL_TO_FLAG, 'N'), 'N', DECODE (NVL (CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE (NVL (CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D')) AS SITE_TYPE,
            CSU.WAREHOUSE_ID AS WAREHOUSE_ID,
            CA.ACCOUNT_NAME AS CUSTOMER_NAME,
            PS.ADDRESSEE AS DBA_NAME,
            L.ADDRESS1,
            L.ADDRESS2,
            L.CITY,
            DECODE(NVL(L.COUNTRY,'US'), 'US', L.STATE, L.PROVINCE) AS STATE,
            L.COUNTRY,
            L.POSTAL_CODE,
            L.POSTAL_PLUS4_CODE,
            TO_NUMBER (CSU.ATTRIBUTE4) AS STORE_NUMBER,
            PS.STATUS AS SITE_STATUS,
            TO_CHAR (PS.CREATION_DATE, 'MM/DD/RRRR  HH:MI:SS AM') AS CREATION_DATE,
            'N' AS CUST_STAGED_FLAG,
            'N' AS STAGED_FLAG
        FROM
            APPS.HZ_CUST_ACCOUNTS CA,
            APPS.HZ_PARTY_SITES PS,
            APPS.HZ_LOCATIONS L,
            APPS.HZ_CUST_ACCT_SITES CAS,
            APPS.HZ_CUST_SITE_USES CSU,
            APPS.XXATDAR_SITE_MKT_PROGRAM SMP
        WHERE
            CA.CUST_ACCOUNT_ID = CAS.CUST_ACCOUNT_ID
            AND CAS.PARTY_SITE_ID = PS.PARTY_SITE_ID
            AND PS.LOCATION_ID = L.LOCATION_ID
            AND CSU.SITE_USE_ID = SMP.SITE_USE_ID(+)
            AND NVL(SMP.DELETED_FLAG, 'N') = 'N'
            AND CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID
            AND CA.STATUS = 'A'
            AND CAS.STATUS = 'A'
            AND CSU.STATUS = 'A'
            AND CSU.SITE_USE_CODE = 'SHIP_TO'
            AND (CAS.SHIP_TO_FLAG = 'Y' OR CAS.SHIP_TO_FLAG = 'P')
            $live_where_clause

        UNION

        SELECT
            S.ID AS SITE_ID,
            CA.CUSTOMER_ID AS CUSTOMER_ID,
            'STAGE' AS STAGELIVE,
            SMP.MEMBERSHIP_TYPE AS MEMBERSHIP_TYPE,
            SMP.MEMBERSHIP_NUMBER AS MEMBERSHIP_NUMBER,
            TO_CHAR(C.ID) AS CUSTOMER_NUMBER,
            CAS.LOCATION AS LOCATION,
            CAS.Z_CUSTOMER_NUMBER AS Z_CUSTOMER,
            (SELECT CSUA.ELEMENT_VALUE FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'AMI_NUMBER') AS AMI_NUMBER,
            DECODE(NVL(CAS.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D')) AS SITE_TYPE,
            CAS.WAREHOUSE_ID AS WAREHOUSE_ID,
            UPPER(NVL(P.PARTY_NAME, NVL(P.PERSON_FIRST_NAME, 'Not Identified') || ' ' || NVL(P.PERSON_LAST_NAME, 'Not Identified'))) AS CUSTOMER_NAME,
            CAS.DBA_NAME AS DBA_NAME,
            L.ADDRESS1,
            L.ADDRESS2,
            L.CITY,
            UPPER(FVS.FLEX_VALUE) AS STATE,
            L.COUNTRY,
            L.POSTAL_CODE,
            L.POSTAL_PLUS4_CODE,
            TO_NUMBER(CAS.STORE_NUMBER) AS STORE_NUMBER,
            'A' as SITE_STATUS,
            TO_CHAR(S.CREATION_DATE, 'MM/DD/RRRR  HH:MI:SS AM') AS CREATION_DATE,
            DECODE(C.LIVE_CUSTOMER_ID,NULL,'Y','N') AS CUST_STAGED_FLAG,
            'Y' AS STAGED_FLAG
        FROM
            APPS.XXATDEAS_SITES S,
            APPS.XXATDEAS_LOCATIONS L,
            APPS.XXATDEAS_PARTIES P,
            APPS.XXATDEAS_CUST_ACCOUNT_SITES CAS,
            APPS.XXATDEAS_CUST_ACCOUNTS CA,
            APPS.XXATDEAS_CUSTOMERS C,
            APPS.HR_ORGANIZATION_UNITS OU,
            APPS.FND_FLEX_VALUES_VL FVS,
            APPS.FND_FLEX_VALUE_SETS FSS,
            APPS.XXATDEAS_SITE_MKT_PROGRAM SMP
        WHERE
            S.ID = L.SITE_ID
            AND S.ID = CAS.SITE_ID
            AND S.CUSTOMER_ID = CA.CUSTOMER_ID(+)
            AND S.ID = SMP.SITE_ID(+)
            AND S.ID = P.SITE_ID(+)
            AND CA.CUSTOMER_ID = C.ID
            AND NVL(SMP.DELETED_FLAG, 'N') = 'N'
            AND (CAS.SHIP_TO_FLAG = 'Y' OR CAS.SHIP_TO_FLAG = 'P')
            AND CAS.WAREHOUSE_ID = OU.ORGANIZATION_ID(+)
            AND L.STATE_FIPS = FVS.FLEX_VALUE_MEANING(+)
            AND FVS.ENABLED_FLAG(+) = 'Y'
            AND FSS.FLEX_VALUE_SET_NAME(+) = 'XXATD_STATE_FIPS'
            AND (FVS.FLEX_VALUE_SET_ID IS NULL OR FVS.FLEX_VALUE_SET_ID = FSS.FLEX_VALUE_SET_ID)
            AND NVL(FVS.START_DATE_ACTIVE, SYSDATE) <= SYSDATE
            AND NVL(FVS.END_DATE_ACTIVE, SYSDATE) >= SYSDATE
            $stage_where_clause
        ";

        $rval = oracle_paged_results($sql, $bindVars, $arguments["START"] + 1, $arguments["END"] + 1, $arguments["SORT_BY"], array("SITE_ID", "CUSTOMER_NUMBER", "LOCATION", "STORE_NUMBER"));

        if ($arguments["OUTPUT_FORMAT"] == "CSV") {
            outputCSV($rval['data'], 'marketing_participants.csv');
        }

        // If the return value is empty, fake out the caller since this is a paged datasource.
        //
        if(empty($rval)) {
            $rval["data"] = array();
            $rval["DSResponse"] = array();
        }

        return $rval;
    }

    function siteComments($arguments, $config) {
        $rval = array();
        $oldValues = array();

        $source = $arguments["CUSTOMER_SOURCE"];

        switch($source) {

            case "STAGED":

                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteComments_Staged($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval[0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Make sure that the data is no longer approved
                    // and log the changed data
                    //
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToCommentDForm", "SITE", $arguments["SITE_ID"], $oldValues, $arguments, $source, $this->controller->session["username"]);

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteComments_Staged($arguments, $config);
                }

            break;

            default:
                // There is no LIVE handler for comments, since the comment textarea is not shown in the client interface for
                // live customers.  So if we fall through to here, it's not an error.
                //
                if ($source != "LIVE") {
                    eventLog("CMR", "Warn", "siteComments", $this->controller->session["username"], "Invalid Source", $source);
                }

            break;

        }

        return $rval;
    }

    private function siteComments_Staged($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id));
        $validUpdate = (isset($site_id));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":
                if($validUpdate) {
                    $updateArgs = array();
                    $updateArgs["SITE_COMMENTS"] = array($arguments["SITE_COMMENTS"], "");
                    $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));
                    $sql = build_update_sql("APPS.XXATDEAS_SITES", $updateArgs, $bindVars, "ID = :site_id", TRUE);
                    if($sql) {
                        $result = oracle_db_exec($sql, $bindVars);
                    }
                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteComments_Staged", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $sql = "SELECT ";
            $sql .= "ID AS SITE_ID, ";
            $sql .= "SITE_COMMENTS ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDEAS_SITES ";
            $sql .= "WHERE ";
            $sql .= "ID = :site_id";

            $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));
            $stid = oracle_db_exec($sql, $bindVars);
            while($results = oci_fetch_assoc($stid)) {
                $rval[] = $results;
            }
        }

        return $rval;
    }


    function hoursOfOperation($arguments, $config) {
        $rval = array();

        $source = $arguments["CUSTOMER_SOURCE"];

        switch($source) {

            case "STAGED":
                $rval = $this->hoursOfOperation_Staged($arguments, $config);

            break;

            case "LIVE":
                $rval = $this->hoursOfOperation_Live($arguments, $config);

            break;

            default:
                eventLog("CMR", "Warn", "hoursOfOperation", $this->controller->session["username"], "Invalid Source", $source);

            break;

        }

        return $rval;
    }

    private function hoursOfOperation_Staged($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];
        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id));
        $validUpdate = (isset($site_id));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":
                if($validUpdate) {
                    $status = set_cust_site_use_attr($site_id, 'HOURS_OF_OPERATION', $arguments['HOURS_OF_OPERATION']);
                    if($status["X_RETURN_CODE"] != "SUCCESS") {
                        $rval["status"] = $this->STATUS_FAILURE;
                        $ret_code = $procArgs["X_RETURN_CODE"];
                        $params = array('ret_code' => $ret_code);
                        $default = "Failure within set_cust_site_use_attr: $ret_code";
                        $rval["errorMessage"] = CMR_Utility::getL18NString('hoursOfOperation_Staged.message.1', $default, $params);
                        $rval["errors"] = $status;
                        $rval["data"] = array();
                        return $rval;
                    }
                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteComments_Staged", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $sql = "
            SELECT
                MAX(DECODE(CSUA.ELEMENT_NAME, 'HOURS_OF_OPERATION', CSUA.ELEMENT_VALUE, '')) AS HOURS_OF_OPERATION
            FROM
                APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA
            WHERE
                CSUA.SITE_ID = :site_id
            ";
            $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));
            $stid = oracle_db_exec($sql, $bindVars);
            while($results = oci_fetch_assoc($stid)) {
                $rval[] = $results;
            }
        }

        return $rval;
    }

    private function hoursOfOperation_Live($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id));
        $validUpdate = (isset($site_id));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":
                if($validUpdate) {

                    $procArgs = set_cust_site_use_attr_live($site_id, "SHIP_TO", 'HOURS_OF_OPERATION', $arguments['HOURS_OF_OPERATION']);
                    if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                        $rval["status"] = $this->STATUS_FAILURE;
                        $ret_code = $procArgs["X_RETURN_CODE"];
                        $params = array('ret_code' => $ret_code);
                        $default = "Failure within set_cust_site_use_attr_live: $ret_code";
                        $rval["errorMessage"] = CMR_Utility::getL18NString('hoursOfOperation_Live.message.1', $default, $params);
                        $rval["errors"] = $procArgs;
                        $rval["data"] = array();
                        return $rval;
                    }
                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteComments_Staged", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $sql = "
            SELECT
                MAX(DECODE(CSUA.ELEMENT_NAME, 'HOURS_OF_OPERATION', CSUA.ELEMENT_VALUE, '')) AS HOURS_OF_OPERATION
            FROM
                APPS.HZ_CUST_ACCT_SITES CAS,
                APPS.HZ_CUST_SITE_USES CSU,
                APPS.XXATDAR_CUST_SITE_USES_ATTR CSUA
            WHERE
                CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID
                AND CSUA.SITE_USE_ID = CSU.SITE_USE_ID
                AND CSU.SITE_USE_CODE = 'SHIP_TO'
                AND CAS.SHIP_TO_FLAG IN ('P', 'Y')
                AND CAS.PARTY_SITE_ID = :site_id
            ";
            $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));
            $stid = oracle_db_exec($sql, $bindVars);
            while($results = oci_fetch_assoc($stid)) {
                $rval[] = $results;
            }
        }

        return $rval;
    }

    function siteMarketing($arguments, $config) {
        $rval = array();
        $oldValues = array();

        $source = $arguments["CUSTOMER_SOURCE"];

        switch($source) {
            case "STAGED":

                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteMarketing_Staged($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval[0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Make sure that the data is no longer approved
                    // and log the changed data
                    //
                    set_review_status("SITE", $arguments["SITE_ID"], "MA", 0);
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToMATeamDForm", "SITE", $arguments["SITE_ID"], $oldValues, $arguments, $source, $this->controller->session["username"]);

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteMarketing_Staged($arguments, $config);
                }

            break;

            case "LIVE":
                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteMarketing_Live($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval[0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Log the changed data
                    //
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToMATeamDForm", "SITE", $arguments["SITE_ID"], $oldValues, $arguments, $source, $this->controller->session["username"]);

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteMarketing_Live($arguments, $config);
                }

            break;

            default:

                eventLog("CMR", "Warn", "siteMarketing", $this->controller->session["username"], "Invalid Source", $source);

            break;

        }

        return $rval;
    }

    public function siteMarketing_Staged($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id));
        $validUpdate = (isset($site_id));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":
                if($validUpdate) {
                    $updateArgs = array();


                    $updateArgs[] = "AMI_NUMBER";
                    $updateArgs[] = "LOCATION_AAN";
                    $updateArgs[] = "LOCATION_6_DIGIT";
                    $updateArgs[] = "LOCATION_G3X";
                    $updateArgs[] = "TIREBUYER";
                    $updateArgs[] = "WAIVE_RESTOCK_FEE";
                    $updateArgs[] = "WAIVE_FUEL_SURCHARGE";
                    $updateArgs[] = "UNCOLLECTED_ALLOWED";
                    $updateArgs[] = "CREDIT_CARD_REQUIRED";
                    $updateArgs[] = "LOCATION_PROFORMA";
                    $updateArgs[] = "COOPER_DC_ASSIGNMENT";

                    foreach($updateArgs as $colName) {
                        $status = set_cust_site_use_attr($site_id, $colName, $arguments[$colName]);
                        if($status["X_RETURN_CODE"] != "SUCCESS") {
                            return $status;
                        }
                    }

                    $updateArgs = array();

                    $updateArgs[] = "RESTOCK_FEE_START_DATE";
                    $updateArgs[] = "RESTOCK_FEE_END_DATE";
                    $updateArgs[] = "FUEL_SURCHARGE_START_DATE";
                    $updateArgs[] = "FUEL_SURCHARGE_END_DATE";
                    $updateArgs[] = "AMI_NUMBER_START_DATE";
                    $updateArgs[] = "AMI_NUMBER_END_DATE";
                    $updateArgs[] = "LOCATION_AAN_START_DATE";
                    $updateArgs[] = "LOCATION_AAN_END_DATE";
                    $updateArgs[] = "LOCATION_6_DIGIT_START_DATE";
                    $updateArgs[] = "LOCATION_6_DIGIT_END_DATE";
                    $updateArgs[] = "LOCATION_G3X_START_DATE";
                    $updateArgs[] = "LOCATION_G3X_END_DATE";
                    $updateArgs[] = "TIREBUYER_START_DATE";
                    $updateArgs[] = "TIREBUYER_END_DATE";

                    foreach($updateArgs as $colName) {
                        $status = set_cust_site_use_attr($site_id, $colName, $arguments[$colName], "DATE");
                        if($status["X_RETURN_CODE"] != "SUCCESS") {
                            return $status;
                        }
                    }
                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteMarketing_Staged", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $sql = "SELECT ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'AMI_NUMBER') AS AMI_NUMBER, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'AMI_NUMBER_START_DATE') AS AMI_NUMBER_START_DATE, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'AMI_NUMBER_END_DATE') AS AMI_NUMBER_END_DATE, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'LOCATION_AAN') AS LOCATION_AAN, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'LOCATION_AAN_START_DATE') AS LOCATION_AAN_START_DATE, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'LOCATION_AAN_END_DATE') AS LOCATION_AAN_END_DATE, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'LOCATION_6_DIGIT') AS LOCATION_6_DIGIT, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'LOCATION_6_DIGIT_START_DATE') AS LOCATION_6_DIGIT_START_DATE, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'LOCATION_6_DIGIT_END_DATE') AS LOCATION_6_DIGIT_END_DATE, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'LOCATION_G3X') AS LOCATION_G3X, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'LOCATION_G3X_START_DATE') AS LOCATION_G3X_START_DATE, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'LOCATION_G3X_END_DATE') AS LOCATION_G3X_END_DATE, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'TIREBUYER') AS TIREBUYER, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'TIREBUYER_START_DATE') AS TIREBUYER_START_DATE, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'TIREBUYER_END_DATE') AS TIREBUYER_END_DATE, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'WAIVE_RESTOCK_FEE') AS WAIVE_RESTOCK_FEE, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'RESTOCK_FEE_START_DATE') AS RESTOCK_FEE_START_DATE, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'RESTOCK_FEE_END_DATE') AS RESTOCK_FEE_END_DATE, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'FUEL_SURCHARGE_START_DATE') AS FUEL_SURCHARGE_START_DATE, ";
            $sql .= "(SELECT TO_CHAR(TO_DATE(CSUA.ELEMENT_VALUE), 'MM/DD/RRRR') ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'FUEL_SURCHARGE_END_DATE') AS FUEL_SURCHARGE_END_DATE, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'UNCOLLECTED_ALLOWED') AS UNCOLLECTED_ALLOWED, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'CREDIT_CARD_REQUIRED') AS CREDIT_CARD_REQUIRED, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'LOCATION_PROFORMA') AS LOCATION_PROFORMA, ";
            $sql .= "(SELECT CSUA.ELEMENT_VALUE ";
            $sql .= "FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE CSUA.SITE_ID = S.ID AND CSUA.ELEMENT_NAME = 'COOPER_DC_ASSIGNMENT') AS COOPER_DC_ASSIGNMENT ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDEAS_SITES S ";
            $sql .= "WHERE ";
            $sql .= "S.ID = :site_id";

            $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

	        $stid = $this->oracle_db_exec_cmr($sql, $bindVars);

            if(!$stid['status']){
                $ret_code = $stid['result']['message'];
                $params = array('ret_code' => $ret_code);
                $default = "There has been a problem fetching data for Staged Site Marketing. <br/>$ret_code";
                $error_string = CMR_Utility::getL18NString('siteMarketing_Staged.message.1', $default, $params);
                eventLog('CMR', 'Error', __METHOD__, $this->controller->session["username"], 'Error fetching query' , $error_string);
                $rval["X_RETURN_CODE"] = $error_string;
                return $rval;
            }else{
                while($results = oci_fetch_assoc($stid['result'])) {
                    $rval[] = $results;
                }
            }

        }

        return $rval;
    }

    public function siteMarketing_Live($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id));
        $validUpdate = (isset($site_id));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":

                if($validUpdate) {

                    $fields = array(
                        array("NAME" => "AMI_NUMBER", "TYPE" => "VARCHAR2"),
                        array("NAME" => "AMI_NUMBER_START_DATE", "TYPE" => "DATE"),
                        array("NAME" => "AMI_NUMBER_END_DATE", "TYPE" => "DATE"),
                        array("NAME" => "LOCATION_AAN", "TYPE" => "VARCHAR2"),
                        array("NAME" => "LOCATION_AAN_START_DATE", "TYPE" => "DATE"),
                        array("NAME" => "LOCATION_AAN_END_DATE", "TYPE" => "DATE"),
                        array("NAME" => "LOCATION_6_DIGIT", "TYPE" => "VARCHAR2"),
                        array("NAME" => "LOCATION_6_DIGIT_START_DATE", "TYPE" => "DATE"),
                        array("NAME" => "LOCATION_6_DIGIT_END_DATE", "TYPE" => "DATE"),
                        array("NAME" => "LOCATION_G3X", "TYPE" => "VARCHAR2"),
                        array("NAME" => "LOCATION_G3X_START_DATE", "TYPE" => "DATE"),
                        array("NAME" => "LOCATION_G3X_END_DATE", "TYPE" => "DATE"),
                        array("NAME" => "TIREBUYER", "TYPE" => "VARCHAR2"),
                        array("NAME" => "TIREBUYER_START_DATE", "TYPE" => "DATE"),
                        array("NAME" => "TIREBUYER_END_DATE", "TYPE" => "DATE"),
                        array("NAME" => "WAIVE_RESTOCK_FEE", "TYPE" => "VARCHAR2"),
                        array("NAME" => "RESTOCK_FEE_START_DATE", "TYPE" => "DATE"),
                        array("NAME" => "RESTOCK_FEE_END_DATE", "TYPE" => "DATE"),
                        array("NAME" => "WAIVE_FUEL_SURCHARGE", "TYPE" => "VARCHAR2"),
                        array("NAME" => "FUEL_SURCHARGE_START_DATE", "TYPE" => "DATE"),
                        array("NAME" => "FUEL_SURCHARGE_END_DATE", "TYPE" => "DATE"),
                        array("NAME" => "UNCOLLECTED_ALLOWED", "TYPE" => "VARCHAR2"),
                        array("NAME" => "CREDIT_CARD_REQUIRED", "TYPE" => "VARCHAR2"),
                        array("NAME" => "LOCATION_PROFORMA", "TYPE" => "VARCHAR2"),
                        array("NAME" => "COOPER_DC_ASSIGNMENT", "TYPE" => "VARCHAR2")
                    );

                    foreach($fields as $field) {
                        $procArgs = set_cust_site_use_attr_live($site_id, "SHIP_TO", $field["NAME"], $arguments[$field["NAME"]], $field["TYPE"]);

                        if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                            return $procArgs;
                        }
                    }
                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteMarketing_Live", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $sql = "SELECT    ";
            $sql .= " MAX (DECODE (CSUA.ELEMENT_NAME, 'AMI_NUMBER', CSUA.ELEMENT_VALUE, '')) ";
            $sql .= "    AS AMI_NUMBER, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'AMI_NUMBER_START_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS AMI_NUMBER_START_DATE, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'AMI_NUMBER_END_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS AMI_NUMBER_END_DATE, ";
            $sql .= " MAX (DECODE (CSUA.ELEMENT_NAME, 'LOCATION_AAN', CSUA.ELEMENT_VALUE, '')) ";
            $sql .= "    AS LOCATION_AAN, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'LOCATION_AAN_START_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS LOCATION_AAN_START_DATE, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'LOCATION_AAN_END_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS LOCATION_AAN_END_DATE, ";
            $sql .= " MAX (DECODE (CSUA.ELEMENT_NAME, 'LOCATION_6_DIGIT', CSUA.ELEMENT_VALUE, '')) ";
            $sql .= "    AS LOCATION_6_DIGIT, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'LOCATION_6_DIGIT_START_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS LOCATION_6_DIGIT_START_DATE, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'LOCATION_6_DIGIT_END_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS LOCATION_6_DIGIT_END_DATE, ";
            $sql .= " MAX (DECODE (CSUA.ELEMENT_NAME, 'LOCATION_G3X', CSUA.ELEMENT_VALUE, '')) ";
            $sql .= "    AS LOCATION_G3X, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'LOCATION_G3X_START_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS LOCATION_G3X_START_DATE, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'LOCATION_G3X_END_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS LOCATION_G3X_END_DATE, ";
            $sql .= " MAX (DECODE (CSUA.ELEMENT_NAME, 'TIREBUYER', CSUA.ELEMENT_VALUE, '')) ";
            $sql .= "    AS TIREBUYER, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'TIREBUYER_START_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS TIREBUYER_START_DATE, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'TIREBUYER_END_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS TIREBUYER_END_DATE, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'WAIVE_RESTOCK_FEE', CSUA.ELEMENT_VALUE, ";
            $sql .= "             '')) ";
            $sql .= "    AS WAIVE_RESTOCK_FEE, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'RESTOCK_FEE_START_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS RESTOCK_FEE_START_DATE, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'RESTOCK_FEE_END_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS RESTOCK_FEE_END_DATE, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'WAIVE_FUEL_SURCHARGE', CSUA.ELEMENT_VALUE, ";
            $sql .= "             '')) ";
            $sql .= "    AS WAIVE_FUEL_SURCHARGE, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'FUEL_SURCHARGE_START_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS FUEL_SURCHARGE_START_DATE, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'FUEL_SURCHARGE_END_DATE', ";
            $sql .= "             TO_CHAR (TO_DATE (CSUA.ELEMENT_VALUE), 'MM/DD/RRRR'), ";
            $sql .= "             '')) ";
            $sql .= "    AS FUEL_SURCHARGE_END_DATE, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'UNCOLLECTED_ALLOWED', CSUA.ELEMENT_VALUE, ";
            $sql .= "             '')) ";
            $sql .= "    AS UNCOLLECTED_ALLOWED, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'CREDIT_CARD_REQUIRED', CSUA.ELEMENT_VALUE, ";
            $sql .= "             '')) ";
            $sql .= "    AS CREDIT_CARD_REQUIRED, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'LOCATION_PROFORMA', CSUA.ELEMENT_VALUE, ";
            $sql .= "             '')) ";
            $sql .= "    AS LOCATION_PROFORMA, ";
            $sql .= " MAX(DECODE (CSUA.ELEMENT_NAME, ";
            $sql .= "             'COOPER_DC_ASSIGNMENT', CSUA.ELEMENT_VALUE, ";
            $sql .= "             '')) ";
            $sql .= "    AS COOPER_DC_ASSIGNMENT ";
            $sql .= "  FROM   APPS.HZ_CUST_ACCT_SITES CAS, ";
            $sql .= "         APPS.HZ_CUST_SITE_USES CSU, ";
            $sql .= "         APPS.XXATDAR_CUST_SITE_USES_ATTR CSUA ";
            $sql .= " WHERE       CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID ";
            $sql .= "         AND CSUA.SITE_USE_ID = CSU.SITE_USE_ID ";
            $sql .= "         AND CSU.SITE_USE_CODE = 'SHIP_TO' ";
            $sql .= "         AND CAS.SHIP_TO_FLAG IN ('P', 'Y') ";
            $sql .= "AND CAS.PARTY_SITE_ID = :site_id ";

            $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

	        $stid = $this->oracle_db_exec_cmr($sql, $bindVars);

            if(!$stid['status']){
                $ret_code = $stid['result']['message'];
                $params = array('ret_code' => $ret_code);
                $default = "There has been a problem fetching data for Live Site Marketing. <br/>$ret_code";
                $error_string = CMR_Utility::getL18NString('siteMarketing_Live.message.1', $default, $params);
                eventLog('CMR', 'Error', __METHOD__, $this->controller->session["username"], 'Error fetching query' , $error_string);
                $rval["X_RETURN_CODE"] = $error_string;
                return $rval;
            }else{
                while($results = oci_fetch_assoc($stid['result'])) {
                    $rval[] = $results;
                }
            }
        }

        return $rval;
    }

    function siteAR($arguments, $config) {
        $rval = array();
        $oldValues = array();

        $source = $arguments["CUSTOMER_SOURCE"];

        switch($source) {

            case "STAGED":

                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteAR_Staged($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval[0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Make sure that the data is no longer approved
                    // and log the changed data
                    //
                    set_review_status("SITE", $arguments["SITE_ID"], "AR", 0);
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToARTeamDForm", "SITE", $arguments["SITE_ID"], $oldValues, $arguments, $source, $this->controller->session["username"]);

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteAR_Staged($arguments, $config);
                }

            break;

            case "LIVE":
                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteAR_Live($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval[0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Log the changed data
                    //
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToARTeamDForm", "SITE", $arguments["SITE_ID"], $oldValues, $arguments, $source, $this->controller->session["username"]);

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteAR_Live($arguments, $config);
                }

            break;

            default:

                eventLog("CMR", "Warn", "siteAR", $this->controller->session["username"], "Invalid Source", $source);

            break;

        }

        return $rval;
    }

    private function siteAR_Staged($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id));
        $validUpdate = (isset($site_id));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":

                if($validUpdate) {

                    $updateArgs = array();

                    $updateArgs["TAX_CLASS"] = array($arguments["TAX_CLASS"], "");
                    $updateArgs["DISPOSAL_FEE_FLAG"] = array($arguments["DISPOSAL_FEE_FLAG"], "");

                    $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

                    $sql = build_update_sql("APPS.XXATDEAS_CUST_ACCOUNT_SITES", $updateArgs, $bindVars, "SITE_ID = :site_id", TRUE);

                    if($sql) {
                        $result = oracle_db_exec($sql, $bindVars);
                    }

                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteAR_Staged", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $sql = "SELECT ";
            $sql .= "CAS.TAX_CLASS, ";
            $sql .= "CAS.DISPOSAL_FEE_FLAG ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDEAS_CUST_ACCOUNT_SITES CAS ";
            $sql .= "WHERE ";
            $sql .= "CAS.SITE_ID = :site_id ";

            $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

            $stid = oracle_db_exec($sql, $bindVars);

            while($results = oci_fetch_assoc($stid)) {
                $rval[] = $results;
            }
        }

        return $rval;
    }

    private function siteAR_Live($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id));
        $validUpdate = (isset($site_id));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":

                if($validUpdate) {

                    $procArgs["P_PARTY_SITE_ID"] = $site_id;
                    $procArgs["P_TAX_CLASS"] = $arguments["TAX_CLASS"];
                    $procArgs["P_DISPOSAL_FEE_FLAG"] = $arguments["DISPOSAL_FEE_FLAG"];
                    $procArgs["P_OBJECT_VERSION_NUMBER"] = $arguments["CSU_OBJECT_VERSION_NUMBER"];

                    $procArgs["X_RETURN_CODE"] = "";

                    oracle_proc_exec("APPS.XXATDEAS_CUST_MAINT_REG_PKG.UPDATE_SITE_AR_INFO", $procArgs);

                    if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                        return $procArgs;
                    }

                    $procArgs = update_cross_reference_by_site($site_id);

                    if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                        return $procArgs;
                    }

                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteAR_Live", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;
        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $sql = "SELECT ";
            $sql .= "CSU.ATTRIBUTE2 AS TAX_CLASS, ";
            $sql .= "CSU.ATTRIBUTE3 AS DISPOSAL_FEE_FLAG, ";
            $sql .= "CSU.OBJECT_VERSION_NUMBER AS CSU_OBJECT_VERSION_NUMBER ";
            $sql .= "FROM ";
            $sql .= "APPS.HZ_CUST_ACCT_SITES CAS, ";
            $sql .= "APPS.HZ_CUST_SITE_USES CSU ";
            $sql .= "WHERE ";
            $sql .= "CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID ";
            $sql .= "AND CSU.SITE_USE_CODE = 'SHIP_TO' ";
            $sql .= "AND CAS.SHIP_TO_FLAG IN ('P', 'Y') ";
            $sql .= "AND CAS.STATUS = 'A' ";
            $sql .= "AND CSU.STATUS = 'A' ";
            $sql .= "AND CAS.PARTY_SITE_ID = :site_id ";

            $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

            $stid = oracle_db_exec($sql, $bindVars);

            while($results = oci_fetch_assoc($stid)) {
                $rval[] = $results;
            }
        }

        return $rval;
    }

    function siteReview($arguments, $config) {
        $rval = array();
        $oldValues = array();

        switch($arguments["CUSTOMER_SOURCE"]) {
            case "STAGED":
                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteReview_Staged($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval[0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Log the changed data
                    //
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToReviewDForm", "SITE", $arguments["SITE_ID"], $oldValues, $arguments, "STAGED", $this->controller->session["username"]);

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteReview_Staged($arguments, $config);
                }
            break;

            case "LIVE":
                $rval = $this->siteReview_Live($arguments, $config);
            break;

            default:
                if($source != "STAGED") {
                    eventLog("CMR", "Warn", "siteReview", $this->controller->session["username"], "Invalid Source", $source);
                }
            break;
        }

        return $rval;
    }

    private function siteReview_Staged($arguments, $config) {
        $rval = array();

        $column_tab_lookup = array(
            "INFO_APPROVED" => "Location",
            "CONTACTS_APPROVED" => "Contacts",
            "BRANCH_APPROVED" => "National Accounts",
            "AR_APPROVED" => "Tax",
            "MA_APPROVED" => "Marketing",
            "OPS_APPROVED" => "Operations",
            "B2B_APPROVED" => "Online Sites"
        );

        $site_id = $arguments["SITE_ID"];
        $column_name = $arguments["COLUMN_NAME"];
        $status = $arguments["STATUS"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id));
        $validUpdate = (isset($site_id) && isset($column_name) && isset($status));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":
                if($validUpdate) {
                    $updateArgs = array();

                    $updateArgs[$column_name] = array($arguments["STATUS"], "");

                    $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

                    $sql = build_update_sql("APPS.XXATDEAS_SITES", $updateArgs, $bindVars, "ID = :site_id");

                    if($sql) {
                        $result = oracle_db_exec($sql, $bindVars);
                    }

                    // Need to lookup the customer_id for this site.
                    //
                    $sql = "SELECT CUSTOMER_ID FROM XXATDEAS.CMR_SITES WHERE ID = $site_id";
                    $stid = oracle_db_exec($sql);
                    $row = oci_fetch_assoc($stid);
                    $customer_id = $row['CUSTOMER_ID'];

                    // Check if the Customer is now fully approved, if so send an email to the GEN MGR of assigned branches
                    //
                    $server = $_SERVER['SERVER_NAME'];
                    if(isset($this->controller->servers_ini_file[$server]['automated_email_override'])) {
                        email_branch_managers_for_staged_customer_if_fully_approved($customer_id, $this->controller->session["username"], get_users_email_address($this->controller->session["username"]));
                    } else {
                        email_branch_managers_for_staged_customer_if_fully_approved($customer_id, $this->controller->session["username"]);
                    }
                }
            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteReview_Staged", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;
        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $sql = "SELECT ";
            $sql .= "NVL(S.INFO_APPROVED, 0) AS INFO_APPROVED, ";
            $sql .= "NVL(S.CONTACTS_APPROVED, 0) AS CONTACTS_APPROVED, ";
            $sql .= "NVL(S.BRANCH_APPROVED, 0) AS BRANCH_APPROVED, ";
            $sql .= "NVL(S.AR_APPROVED, 0) AS AR_APPROVED, ";
            $sql .= "NVL(S.MA_APPROVED, 0) AS MA_APPROVED ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDEAS_SITES S ";
            $sql .= "WHERE ";
            $sql .= "S.ID = :site_id ";

            $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

            $stid = oracle_db_exec($sql, $bindVars);

            while($results = oci_fetch_assoc($stid)) {
                foreach($results as $key => $value) {
                    if(!isset($column_name) || $column_name == $key) {

                        $rval[] = array(
                            "TAB_NAME" => $column_tab_lookup[$key],
                            "COLUMN_NAME" => $key,
                            "STATUS" => $value
                        );
                    }
                }
            }
        }

        return $rval;
    }

    private function siteReview_Live($arguments, $config) {
        $rval = array(
            array("TAB_NAME" => "Location", "COLUMN_NAME" => "INFO_APPROVED", "STATUS" => "3"),
            array("TAB_NAME" => "Contacts", "COLUMN_NAME" => "CONTACTS_APPROVED", "STATUS" => "3"),
            array("TAB_NAME" => "National Accounts", "COLUMN_NAME" => "BRANCH_APPROVED", "STATUS" => "3"),
            array("TAB_NAME" => "Tax", "COLUMN_NAME" => "AR_APPROVED", "STATUS" => "3"),
            array("TAB_NAME" => "Marketing", "COLUMN_NAME" => "MA_APPROVED", "STATUS" => "3")
        );

        return $rval;
    }

    function siteTaxExemptions($arguments, $config) {
        $rval = array();
        $oldValues = array();

        $source = $arguments["CUSTOMER_SOURCE"];

        $id = $arguments["ID"];

        if(!is_numeric($id)) {
            unset($id);
        }

        switch($source) {

            case "STAGED":

                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteTaxExemptions_Staged($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval[0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Make sure that the data is no longer approved
                    // and log the changed data
                    //
                    set_review_status("SITE", $arguments["SITE_ID"], "AR", 0);
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToTaxExemptionsListGrid", "TAX_EXEMPTIONS", (isset($id) ? $id : "NEW"), $oldValues, $arguments, $source, $this->controller->session["username"]);

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteTaxExemptions_Staged($arguments, $config);
                }

            break;

            case "LIVE":
                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteTaxExemptions_Live($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval[0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Log the changed data
                    //
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToTaxExemptionsListGrid", "TAX_EXEMPTIONS", (isset($id) ? $id : "NEW"), $oldValues, $arguments, $source, $this->controller->session["username"]);

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteTaxExemptions_Live($arguments, $config);
                }

            break;

            default:

                eventLog("CMR", "Warn", "siteTaxExemptions", $this->controller->session["username"], "Invalid Source", $source);

            break;

        }

        return $rval;
    }

    private function siteTaxExemptions_Staged($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];
        $id = $arguments["ID"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        if(!is_numeric($id)) {
            unset($id);
        }

        $exemption_status = $arguments["EXEMPTION_STATUS"];
        $tax_code = $arguments["TAX_CODE"];
        $customer_exemption_number = $arguments["CUSTOMER_EXEMPTION_NUMBER"];
        $reason_code = $arguments["EXEMPTION_REASON_CODE"];
        $start_date = $arguments["START_DATE"];
        $end_date = isset($arguments["END_DATE"]) ? $arguments["END_DATE"] : NULL;

        $validSelect = (isset($site_id) || isset($id));
        $validDelete = (isset($id));
        $validUpdate = (isset($id));
        $validInsert = (isset($site_id) && isset($customer_exemption_number) &&
            isset($reason_code) && isset($start_date));

        switch($arguments["OPERATION_TYPE"]) {

            case "add":
                if($validInsert) {
                    $id = oracle_get_nextval("APPS.XXATDEAS_TAX_EXEMPTIONS_S");

                    $sql = "INSERT INTO ";
                    $sql .= "APPS.XXATDEAS_TAX_EXEMPTIONS ";
                    $sql .= "(ID, ";
                    $sql .= "SITE_ID, ";
                    $sql .= "EXEMPTION_TYPE, ";
                    $sql .= "PERCENT_EXEMPT, ";
                    $sql .= "TAX_CODE, ";
                    $sql .= "CUSTOMER_EXEMPTION_NUMBER, ";
                    $sql .= "REASON_CODE, ";
                    $sql .= "START_DATE, ";
                    $sql .= "END_DATE, ";
                    $sql .= "STATUS, ";
                    $sql .= "IN_USE_FLAG) ";
                    $sql .= "VALUES (";
                    $sql .= ":id, ";
                    $sql .= ":site_id, ";
                    $sql .= ":exemption_type, ";
                    $sql .= ":percent_exempt, ";
                    $sql .= ":tax_code, ";
                    $sql .= ":customer_exemption_number, ";
                    $sql .= ":reason_code, ";
                    $sql .= "TO_DATE(:start_date, 'MM/DD/RRRR'), ";
                    $sql .= "TO_DATE(:end_date, 'MM/DD/RRRR'), ";
                    $sql .= ":status, ";
                    $sql .= ":in_use_flag)";

                    $bindVars = array(
                        array("PH_NAME" => ":id",                           "VARIABLE" => $id),
                        array("PH_NAME" => ":site_id",                      "VARIABLE" => $site_id),
                        array("PH_NAME" => ":exemption_type",               "VARIABLE" => "DISCOUNT"),
                        array("PH_NAME" => ":percent_exempt",               "VARIABLE" => 100),
                        array("PH_NAME" => ":tax_code",                     "VARIABLE" => $tax_code),
                        array("PH_NAME" => ":customer_exemption_number",    "VARIABLE" => $customer_exemption_number),
                        array("PH_NAME" => ":reason_code",                  "VARIABLE" => $reason_code),
                        array("PH_NAME" => ":start_date",                   "VARIABLE" => $start_date),
                        array("PH_NAME" => ":end_date",                     "VARIABLE" => $end_date),
                        array("PH_NAME" => ":status",                       "VARIABLE" => $exemption_status),
                        array("PH_NAME" => ":in_use_flag",                  "VARIABLE" => "Y")
                    );

                    $result = oracle_db_exec($sql, $bindVars);
                }

            break;

            case "update":

                if($validUpdate) {

                    $updateArgs = array();
                    $updateArgs["TAX_CODE"] = array($arguments["TAX_CODE"], "");
                    $updateArgs["CUSTOMER_EXEMPTION_NUMBER"] = array($arguments["CUSTOMER_EXEMPTION_NUMBER"], "");
                    $updateArgs["REASON_CODE"] = array($arguments["EXEMPTION_REASON_CODE"], "");
                    $updateArgs["STATUS"] = array($arguments["EXEMPTION_STATUS"], "");
                    $updateArgs["START_DATE"] = array($arguments["START_DATE"], "TO_DATE");
                    $updateArgs["END_DATE"] = array($arguments["END_DATE"], "TO_DATE");
                    $bindVars = array(array("PH_NAME" => ":id", "VARIABLE" => $id));
                    $sql = build_update_sql("APPS.XXATDEAS_TAX_EXEMPTIONS", $updateArgs, $bindVars, "ID = :id", TRUE);
                    if($sql) {
                        $result = oracle_db_exec($sql, $bindVars);
                    }
                }

            break;

            case "remove":
                if($validDelete) {

                    $sql = "DELETE FROM APPS.XXATDEAS_TAX_EXEMPTIONS WHERE ID = :id";

                    $bindVars = array(array("PH_NAME" => ":id", "VARIABLE" => $id));

                    $result = oracle_db_exec($sql, $bindVars);

                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteTaxExemptions_Staged", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate) ||
            ($arguments["OPERATION_TYPE"] == "add" && $validInsert)) {

            $bindVars = NULL;

            $sql = "SELECT ";
            $sql .= "ID, ";
            $sql .= "'STAGED' AS CUSTOMER_SOURCE, ";
            $sql .= "SITE_ID, ";
            $sql .= "EXEMPTION_TYPE, ";
            $sql .= "PERCENT_EXEMPT, ";
            $sql .= "STATUS AS EXEMPTION_STATUS, ";
            $sql .= "TAX_CODE, ";
            $sql .= "CUSTOMER_EXEMPTION_NUMBER, ";
            $sql .= "REASON_CODE AS EXEMPTION_REASON_CODE, ";
            $sql .= "TO_CHAR(START_DATE, 'MM/DD/RRRR') AS START_DATE, ";
            $sql .= "TO_CHAR(END_DATE, 'MM/DD/RRRR') AS END_DATE, ";
            $sql .= "TAX_CODE AS TAX_FULL_NAME ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDEAS_TAX_EXEMPTIONS ";
            $sql .= "WHERE ";

            if(isset($id)) {
                $sql .= "ID = :id";
                $bindVars = array(array("PH_NAME" => ":id", "VARIABLE" => $id));
            } elseif(isset($site_id)) {
                $sql .= "SITE_ID = :site_id";
                $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));
            }
            $stid = oracle_db_exec($sql, $bindVars);
            while($results = oci_fetch_assoc($stid)) {
                $rval[] = $results;
            }
        }

        return $rval;
    }

    private function siteTaxExemptions_Live($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];
        $id = $arguments["ID"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        if(!is_numeric($id)) {
            unset($id);
        }

        $customer_exemption_number = $arguments["CUSTOMER_EXEMPTION_NUMBER"];
        $tax_code = $arguments["TAX_CODE"];
        $status_code = $arguments["EXEMPTION_STATUS"];
        $reason_code = $arguments["EXEMPTION_REASON_CODE"];
        $start_date = $arguments["START_DATE"];
        $end_date = $arguments["END_DATE"];
        $validSelect = (isset($site_id) || isset($id));
        $validUpdate = (isset($id) && isset($end_date));
        $validInsert = (isset($site_id) && isset($tax_code) && isset($start_date) && isset($reason_code) && isset($status_code) && isset($customer_exemption_number));

        switch($arguments["OPERATION_TYPE"]) {

            case "add":

                if($validInsert) {
                    $procArgs["P_TAX_EXEMPTION_ID"] = "";
                    $procArgs["P_PARTY_SITE_ID"] = $arguments["SITE_ID"];
                    $procArgs["P_EXEMPTION_TYPE"] = 'DISCOUNT';
                    $procArgs["P_PERCENT_EXEMPT"] = 100;
                    $procArgs["P_TAX_CODE"] = $arguments["TAX_CODE"];
                    $procArgs["P_CUSTOMER_EXEMPTION_NUMBER"] = $arguments["CUSTOMER_EXEMPTION_NUMBER"];
                    $procArgs["P_REASON_CODE"] = $arguments["EXEMPTION_REASON_CODE"];
                    if(isset($arguments["START_DATE"])) {
                        $procArgs["P_START_DATE"] = date("d-M-Y", strtotime($arguments["START_DATE"]));
                    } else {
                        $procArgs["P_START_DATE"] = "";
                    }
                    if(isset($arguments["END_DATE"])) {
                        $procArgs["P_END_DATE"] = date("d-M-Y", strtotime($arguments["END_DATE"]));
                    } else {
                        $procArgs["P_END_DATE"] = "";
                    }
                    $procArgs["P_STATUS"] = $arguments["EXEMPTION_STATUS"];
                    $procArgs["X_RETURN_CODE"] = "";
                    oracle_proc_exec("APPS.XXATDEAS_CUST_MAINT_REG_PKG.CREATE_SITE_TAX_EXEMPTION_INFO", $procArgs);
                    $id = $procArgs["P_TAX_EXEMPTION_ID"];
                    if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                        return $procArgs;
                    }
                    $procArgs = update_cross_reference_by_site($site_id);
                    if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                        return $procArgs;
                    }
                }

            break;

            case "update":

                if($validUpdate) {
                    $procArgs["P_TAX_EXEMPTION_ID"] = $id;
                    $procArgs["P_END_DATE"] = date("d-M-Y", strtotime($arguments["END_DATE"]));
                    $procArgs["X_RETURN_CODE"] = "";
                    oracle_proc_exec("APPS.XXATDEAS_CUST_MAINT_REG_PKG.UPDATE_SITE_TAX_EXEMPTION_INFO", $procArgs);
                    if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                        return $procArgs;
                    }
                    $procArgs = update_cross_reference_by_site($site_id);
                    if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                        return $procArgs;
                    }
                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteTaxExemptions_Live", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "add" && $validInsert) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $bindVars = NULL;

            $sql = "
            SELECT
                TAX_EXEMPTION_ID                            AS ID,
                'LIVE'                                      AS CUSTOMER_SOURCE,
                PRF.PARTY_ID                                AS SITE_ID,
                EX.RATE_MODIFIER                            AS PERCENT_EXEMPT,
                STAT.MEANING                                AS EXEMPTION_STATUS,
                TAX.TAX                                     AS TAX_CODE,
                EX.EXEMPT_CERTIFICATE_NUMBER                AS CUSTOMER_EXEMPTION_NUMBER,
                RSN.MEANING                                 AS EXEMPTION_REASON_CODE,
                TO_CHAR(EX.EFFECTIVE_FROM, 'MM/DD/RRRR')    AS START_DATE,
                TO_CHAR(EX.EFFECTIVE_TO, 'MM/DD/RRRR')      AS END_DATE,
                TAX.TAX_FULL_NAME
            FROM
                APPS.ZX_EXEMPTIONS          EX,
                APPS.FND_LOOKUP_VALUES_VL   STAT,
                APPS.ZX_TAXES_VL            TAX,
                APPS.FND_LOOKUP_VALUES_VL   RSN,
                APPS.ZX_PARTY_TAX_PROFILE   PRF,
                APPS.ZX_REGIMES_B           REG
            WHERE
                PRF.PARTY_TYPE_CODE     = 'THIRD_PARTY_SITE' AND
                EX.PARTY_TAX_PROFILE_ID = PRF.PARTY_TAX_PROFILE_ID AND
                STAT.LOOKUP_TYPE        = 'ZX_EXEMPTION_STATUS' AND
                STAT.ENABLED_FLAG       = 'Y' AND
                TRUNC(SYSDATE)          BETWEEN NVL(STAT.START_DATE_ACTIVE, SYSDATE-1)
                    AND NVL(STAT.END_DATE_ACTIVE, SYSDATE+1) AND
                STAT.LOOKUP_CODE        = EX.EXEMPTION_STATUS_CODE AND
                TAX.TAX                 = EX.TAX AND
                TAX.TAX_REGIME_CODE     = EX.TAX_REGIME_CODE AND
                RSN.LOOKUP_TYPE         = 'ZX_EXEMPTION_REASON_CODE' AND
                RSN.LOOKUP_CODE         = EX.EXEMPT_REASON_CODE AND
                REG.TAX_REGIME_CODE      = EX.TAX_REGIME_CODE
            ";
            if(isset($id)) {
                $sql .= " AND TAX_EXEMPTION_ID = :id";
                $bindVars = array(array("PH_NAME" => ":id", "VARIABLE" => $id));
            } elseif(isset($site_id)) {
                $sql .= " AND PRF.PARTY_ID = :site_id";
                $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));
            }
            $stid = oracle_db_exec($sql, $bindVars);
            while($results = oci_fetch_assoc($stid)) {
                $rval[] = $results;
            }
        }

        return $rval;
    }

    function siteNationalAccounts($arguments, $config) {
        $rval = array();
        $oldValues = array();

        $source = $arguments["CUSTOMER_SOURCE"];

        switch($source) {

            case "STAGED":

                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteNationalAccounts_Staged($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval[0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Make sure that the data is no longer approved
                    // and log the changed data
                    //
                    set_review_status("SITE", $arguments["SITE_ID"], "BRANCH", 0);
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToDCTeamListGrid", "NATIONAL_ACCOUNTS", $arguments["ELEMENT_NAME"], $oldValues, $arguments, $source, $this->controller->session["username"]);

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteNationalAccounts_Staged($arguments, $config);
                }

            break;

            case "LIVE":

                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteNationalAccounts_Live($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval[0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Log the changed data
                    //
                    log_data_change(
                        "SITE",
                        $arguments["SITE_ID"],
                        "shipToDCTeamListGrid",
                        "NATIONAL_ACCOUNTS",
                        (isset($arguments["ELEMENT_NAME"]) ? $arguments["ELEMENT_NAME"] : "NEW"),
                        $oldValues,
                        $arguments,
                        $source,
                        $this->controller->session["username"]
                        );

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteNationalAccounts_Live($arguments, $config);
                }

            break;

            default:

                eventLog("CMR", "Warn", "siteNationalAccounts", $this->controller->session["username"], "Invalid Source", $source);

            break;

        }

        return $rval;
    }

    private function siteNationalAccounts_Staged($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];
        $nationalAccountName = $arguments["NATIONAL_ACCOUNT_NAME"];
        $rownum = $arguments["ROWNUM"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id));
        $validUpdate = (isset($site_id) && isset($nationalAccountName));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":
                if($validUpdate) {

                    $status = set_cust_site_use_attr($site_id, $nationalAccountName . "_MEMBER", $arguments["MEMBER"]);
                    if($status["X_RETURN_CODE"] != "SUCCESS") {
                        return $status;
                    }

                    $status = set_cust_site_use_attr($site_id, $nationalAccountName . "_NUMBER", $arguments["NUMBER"]);
                    if($status["X_RETURN_CODE"] != "SUCCESS") {
                        return $status;
                    }

                    $status = set_cust_site_use_attr($site_id, $nationalAccountName . "_ALT", $arguments["ALT"]);
                    if($status["X_RETURN_CODE"] != "SUCCESS") {
                        return $status;
                    }

                    $status = set_cust_site_use_attr($site_id, $nationalAccountName . "_COMMENT", $arguments["COMMENT"]);
                    if($status["X_RETURN_CODE"] != "SUCCESS") {
                        return $status;
                    }
                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteNationalAccounts_Staged", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $bindVars = array();

            $sql = "SELECT ";

            if(isset($rownum)) {
                $sql .= ":rownumber AS \"ROWNUM\", ";
                $bindVars = array(array("PH_NAME" => ":rownumber", "VARIABLE" => $rownum));
            } else {
                $sql .= "ROWNUM, ";
            }

            $sql .= "A.* FROM (SELECT ACCS.NATIONAL_ACCOUNT_CUST_NO, ";
            $sql .= "       ACCS.NATIONAL_ACCOUNT_CUST_ID, ";
            $sql .= "       DESCRIPTION, ";
            $sql .= "       NATIONAL_ACCOUNT_NAME, ";
            $sql .= "       NATIONAL_ACCOUNT_GROUP, ";
            $sql .= "       ACCS.ID PARTY_SITE_ID, ";
            $sql .= "       ACCS.CUST_ACCOUNT_ID, ";
            $sql .= "       MAX (DECODE (MEM.ELEMENT_TYPE, 'MEMBER', MEM.ELEMENT_VALUE, 'N')) ";
            $sql .= "           AS \"MEMBER\", ";
            $sql .= "       MAX (DECODE (MEM.ELEMENT_TYPE, 'NUMBER', MEM.ELEMENT_VALUE, NULL)) ";
            $sql .= "           AS \"NUMBER\", ";
            $sql .= "       MAX (DECODE (MEM.ELEMENT_TYPE, 'ALT', MEM.ELEMENT_VALUE, NULL)) AS \"ALT\", ";
            $sql .= "       MAX (DECODE (MEM.ELEMENT_TYPE, 'COMMENT', MEM.ELEMENT_VALUE, NULL)) AS \"COMMENT\" ";
            $sql .= "  FROM     (SELECT FFVL.FLEX_VALUE NATIONAL_ACCOUNT_CUST_NO, ";
            $sql .= "                   FFVS.FLEX_VALUE_SET_NAME, ";
            $sql .= "                   FFVL.DESCRIPTION, ";
            $sql .= "                   SUBSTR (FFVL.DESCRIPTION, ";
            $sql .= "                           INSTR (FFVL.DESCRIPTION, '.') + 1) ";
            $sql .= "                       NATIONAL_ACCOUNT_NAME, ";
            $sql .= "                   FFVL.FLEX_VALUE_MEANING NATIONAL_ACCOUNT_GROUP, ";
            $sql .= "                   CA.CUST_ACCOUNT_ID AS NATIONAL_ACCOUNT_CUST_ID, ";
            $sql .= "                   S.ID, ";
            $sql .= "                   CA.CUST_ACCOUNT_ID ";
            $sql .= "              FROM APPS.FND_FLEX_VALUE_SETS FFVS, ";
            $sql .= "                   APPS.FND_FLEX_VALUES_VL FFVL, ";
            $sql .= "                   APPS.XXATDEAS_NAT_ACCT_RELATIONS NAR, ";
            $sql .= "                   APPS.HZ_CUST_ACCOUNTS CA, ";
            $sql .= "                   APPS.XXATDEAS_SITES S ";
            $sql .= "             WHERE     S.CUSTOMER_ID = NAR.CUSTOMER_ID ";
            $sql .= "                   AND NAR.NATIONAL_ACCOUNT_CUST_ID = CA.CUST_ACCOUNT_ID ";
            $sql .= "                   AND CA.ACCOUNT_NUMBER = FFVL.FLEX_VALUE ";
            $sql .= "                   AND FFVL.FLEX_VALUE_SET_ID = FFVS.FLEX_VALUE_SET_ID ";
            $sql .= "                   AND FFVS.FLEX_VALUE_SET_NAME = ";
            $sql .= "                           'XXATDI_NATIONAL_ACCOUNT_MATRIX' ";
            $sql .= "                   AND NVL (FFVL.START_DATE_ACTIVE, SYSDATE) <= SYSDATE ";
            $sql .= "                   AND NVL (FFVL.END_DATE_ACTIVE, SYSDATE) >= SYSDATE ";
            $sql .= "                   AND FFVL.ENABLED_FLAG = 'Y' ";
            $sql .= "                   AND S.ID = :SITE_ID) ACCS ";
            $sql .= "       LEFT OUTER JOIN ";
            $sql .= "           (SELECT CSUA.ELEMENT_NAME, ";
            $sql .= "                   CSUA.ELEMENT_VALUE, ";
            $sql .= "                   SUBSTR (CSUA.ELEMENT_NAME, ";
            $sql .= "                           INSTR (CSUA.ELEMENT_NAME, '_') + 1) ";
            $sql .= "                       AS ELEMENT_TYPE ";
            $sql .= "              FROM APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "             WHERE CSUA.SITE_ID = :SITE_ID ";
            $sql .= "                   AND SUBSTR (CSUA.ELEMENT_NAME, ";
            $sql .= "                               INSTR (CSUA.ELEMENT_NAME, '_') + 1) IN ";
            $sql .= "                           ('NUMBER', 'MEMBER', 'ALT', 'COMMENT')) MEM ";
            $sql .= "       ON ACCS.NATIONAL_ACCOUNT_NAME = ";
            $sql .= "              SUBSTR (MEM.ELEMENT_NAME, 1, INSTR (MEM.ELEMENT_NAME, '_') - 1) ";
            $sql .= "GROUP BY ACCS.NATIONAL_ACCOUNT_CUST_NO, ";
            $sql .= "         DESCRIPTION, ";
            $sql .= "         NATIONAL_ACCOUNT_NAME, ";
            $sql .= "         NATIONAL_ACCOUNT_GROUP, ";
            $sql .= "       ACCS.NATIONAL_ACCOUNT_CUST_ID, ";
            $sql .= "       ACCS.ID , ";
            $sql .= "       ACCS.CUST_ACCOUNT_ID ";
            $sql .= "       ORDER BY ACCS.NATIONAL_ACCOUNT_GROUP, ACCS.NATIONAL_ACCOUNT_NAME) A ";

            $bindVars = array_merge($bindVars, array(
                    array("PH_NAME" => ":site_id", "VARIABLE" => $site_id)
                    ));

            if(isset($nationalAccountName)) {
                $sql .= "WHERE NATIONAL_ACCOUNT_NAME = :account_name ";
                $bindVars = array_merge($bindVars, array(array("PH_NAME" => ":account_name", "VARIABLE" => $nationalAccountName)));
            }

            $stid = oracle_db_exec($sql, $bindVars);

            while($results = oci_fetch_assoc($stid)) {
                $rval[] = $results;
            }
        }
        return $rval;
    }

    private function siteNationalAccounts_Live($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];
        $element_name = $arguments["ELEMENT_NAME"];
        $rownum = $arguments["ROWNUM"];
        $nationalAccountName = $arguments["NATIONAL_ACCOUNT_NAME"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id));
        $validUpdate = (isset($site_id) && isset($nationalAccountName));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":
                if($validUpdate) {

                    $status = set_cust_site_use_attr_live($site_id, 'SHIP_TO', $nationalAccountName . "_MEMBER", $arguments["MEMBER"]);
                    if($status["X_RETURN_CODE"] != "SUCCESS") {
                        return $status;
                    }

                    $status = set_cust_site_use_attr_live($site_id, 'SHIP_TO', $nationalAccountName . "_NUMBER", $arguments["NUMBER"]);
                    if($status["X_RETURN_CODE"] != "SUCCESS") {
                        return $status;
                    }

                    $status = set_cust_site_use_attr_live($site_id, 'SHIP_TO', $nationalAccountName . "_ALT", $arguments["ALT"]);
                    if($status["X_RETURN_CODE"] != "SUCCESS") {
                        return $status;
                    }

                    $status = set_cust_site_use_attr_live($site_id, 'SHIP_TO', $nationalAccountName . "_COMMENT", $arguments["COMMENT"]);
                    if($status["X_RETURN_CODE"] != "SUCCESS") {
                        return $status;
                    }
                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteNationalAccounts_Live", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $bindVars = array();

            $sql = "SELECT ";

            if(isset($rownum)) {
                $sql .= ":rownumber AS \"ROWNUM\", ";
                $bindVars = array(array("PH_NAME" => ":rownumber", "VARIABLE" => $rownum));
            } else {
                $sql .= "ROWNUM, ";
            }

            $sql .= "NATIONAL_ACCOUNT_NAME, NATIONAL_ACCOUNT_CUST_NO, NATIONAL_ACCOUNT_CUST_ID, DESCRIPTION, ";
            $sql .= " NATIONAL_ACCOUNT_GROUP, ";
            $sql .= "PARTY_SITE_ID, CUST_ACCOUNT_ID, ";
            $sql .= " \"MEMBER\" , \"NUMBER\", \"ALT\", \"COMMENT\" ";
            $sql .= "FROM (SELECT ACCS.NATIONAL_ACCOUNT_CUST_NO, ";
            $sql .= "ACCS.NATIONAL_ACCOUNT_CUST_ID, ";
            $sql .= "DESCRIPTION, ";
            $sql .= "NATIONAL_ACCOUNT_NAME, ";
            $sql .= "NATIONAL_ACCOUNT_GROUP, ";
            $sql .= "ACCS.PARTY_SITE_ID, ";
            $sql .= "ACCS.CUST_ACCOUNT_ID, ";
            $sql .= "MAX ( ";
            $sql .= "DECODE (MEM.ELEMENT_TYPE, ";
            $sql .= "'MEMBER', MEM.ELEMENT_VALUE, ";
            $sql .= "'N')) ";
            $sql .= "AS \"MEMBER\", ";
            $sql .= "MAX ( ";
            $sql .= "DECODE (MEM.ELEMENT_TYPE, ";
            $sql .= "'NUMBER', MEM.ELEMENT_VALUE, ";
            $sql .= "NULL)) ";
            $sql .= "AS \"NUMBER\", ";
            $sql .= "MAX ( ";
            $sql .= "DECODE (MEM.ELEMENT_TYPE, 'ALT', MEM.ELEMENT_VALUE, NULL)) ";
            $sql .= "AS \"ALT\", ";
            $sql .= "MAX ( ";
            $sql .= "DECODE (MEM.ELEMENT_TYPE, ";
            $sql .= "'COMMENT', MEM.ELEMENT_VALUE, ";
            $sql .= "NULL)) ";
            $sql .= "AS \"COMMENT\" ";
            $sql .= "FROM     (SELECT FFVL.FLEX_VALUE NATIONAL_ACCOUNT_CUST_NO, ";
            $sql .= "FFVS.FLEX_VALUE_SET_NAME, ";
            $sql .= "FFVL.DESCRIPTION, ";
            $sql .= "SUBSTR (FFVL.DESCRIPTION, ";
            $sql .= "INSTR (FFVL.DESCRIPTION, '.') + 1) ";
            $sql .= "NATIONAL_ACCOUNT_NAME, ";
            $sql .= "FFVL.FLEX_VALUE_MEANING NATIONAL_ACCOUNT_GROUP, ";
            $sql .= "CA.CUST_ACCOUNT_ID AS NATIONAL_ACCOUNT_CUST_ID, ";
            $sql .= "S.PARTY_SITE_ID, ";
            $sql .= "CA.CUST_ACCOUNT_ID ";
            $sql .= "FROM APPS.FND_FLEX_VALUE_SETS FFVS, ";
            $sql .= "APPS.FND_FLEX_VALUES_VL FFVL, ";
            $sql .= "APPS.HZ_CUST_ACCT_RELATE NAR, ";
            $sql .= "APPS.HZ_CUST_ACCOUNTS CA, ";
            $sql .= "APPS.HZ_CUST_ACCT_SITES S ";
            $sql .= "WHERE S.CUST_ACCOUNT_ID = NAR.CUST_ACCOUNT_ID ";
            $sql .= "AND NAR.RELATED_CUST_ACCOUNT_ID = ";
            $sql .= "CA.CUST_ACCOUNT_ID ";
            $sql .= "AND NAR.STATUS = 'A' ";
            $sql .= "AND CA.ACCOUNT_NUMBER = FFVL.FLEX_VALUE ";
            $sql .= "AND FFVL.FLEX_VALUE_SET_ID = ";
            $sql .= "FFVS.FLEX_VALUE_SET_ID ";
            $sql .= "AND FFVS.FLEX_VALUE_SET_NAME = ";
            $sql .= "'XXATDI_NATIONAL_ACCOUNT_MATRIX' ";
            $sql .= "AND NVL (FFVL.START_DATE_ACTIVE, SYSDATE) <= ";
            $sql .= "SYSDATE ";
            $sql .= "AND NVL (FFVL.END_DATE_ACTIVE, SYSDATE) >= SYSDATE ";
            $sql .= "AND FFVL.ENABLED_FLAG = 'Y' ";
            $sql .= "AND S.PARTY_SITE_ID = :site_id) ACCS ";
            $sql .= "LEFT OUTER JOIN ";
            $sql .= "(SELECT  ";
            $sql .= "CSUA.ELEMENT_NAME,  ";
            $sql .= "CSUA.ELEMENT_VALUE, ";
            $sql .= "SUBSTR (CSUA.ELEMENT_NAME, ";
            $sql .= "INSTR (CSUA.ELEMENT_NAME, '_') + 1) ";
            $sql .= "AS ELEMENT_TYPE  ";
            $sql .= "FROM  ";
            $sql .= "APPS.HZ_CUST_ACCT_SITES CAS,  ";
            $sql .= "APPS.HZ_CUST_SITE_USES CSU,  ";
            $sql .= "APPS.XXATDAR_CUST_SITE_USES_ATTR CSUA  ";
            $sql .= "WHERE  ";
            $sql .= "CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID  ";
            $sql .= "AND CSU.SITE_USE_ID = CSUA.SITE_USE_ID  ";
            $sql .= "AND CAS.PARTY_SITE_ID = :site_id ";
            $sql .= "AND SUBSTR (CSUA.ELEMENT_NAME, ";
            $sql .= "INSTR (CSUA.ELEMENT_NAME, '_') + 1) IN ";
            $sql .= "('NUMBER', 'MEMBER', 'ALT', 'COMMENT')) MEM ";
            $sql .= "ON ACCS.NATIONAL_ACCOUNT_NAME = ";
            $sql .= "SUBSTR (MEM.ELEMENT_NAME, ";
            $sql .= "1, ";
            $sql .= "INSTR (MEM.ELEMENT_NAME, '_') - 1) ";
            $sql .= "GROUP BY ACCS.NATIONAL_ACCOUNT_CUST_NO, ";
            $sql .= "DESCRIPTION, ";
            $sql .= "NATIONAL_ACCOUNT_NAME, ";
            $sql .= "NATIONAL_ACCOUNT_GROUP, ";
            $sql .= "ACCS.NATIONAL_ACCOUNT_CUST_ID, ";
            $sql .= "ACCS.PARTY_SITE_ID, ";
            $sql .= "ACCS.CUST_ACCOUNT_ID ";
            $sql .= "ORDER BY ACCS.NATIONAL_ACCOUNT_GROUP, ACCS.NATIONAL_ACCOUNT_NAME) A ";

            $bindVars = array_merge($bindVars, array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id)));

            if(isset($nationalAccountName)) {
                $sql .= "where NATIONAL_ACCOUNT_NAME = :account_name ";
                $bindVars = array_merge($bindVars, array(array("PH_NAME" => ":account_name", "VARIABLE" => $nationalAccountName)));
            }

            $stid = oracle_db_exec($sql, $bindVars);

            while($results = oci_fetch_assoc($stid)) {
                $rval[] = $results;
            }
        }

        return $rval;
    }

    function siteProgramParticipation($arguments, $config) {
        $rval = array();
        $oldValues = array();

        $source = $arguments["CUSTOMER_SOURCE"];

        switch($source) {

            case "STAGED":

                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteProgramParticipation_Staged($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval[0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Make sure that the data is no longer approved
                    // and log the changed data
                    //
                    set_review_status("SITE", $arguments["SITE_ID"], "BRANCH", 0);
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToParticipationListGrid", "PROGRAM PARTICIPATION", $arguments["ELEMENT_NAME"], $oldValues, $arguments, $source, $this->controller->session["username"]);

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteProgramParticipation_Staged($arguments, $config);
                }

            break;

            case "LIVE":

                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteProgramParticipation_Live($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval[0];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Log the changed data
                    //
                    log_data_change("SITE", $arguments["SITE_ID"], "shipToParticipationListGrid", "PROGRAM PARTICIPATION", $arguments["ELEMENT_NAME"], $oldValues, $arguments, $source, $this->controller->session["username"]);

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteProgramParticipation_Live($arguments, $config);
                }

            break;

            default:

                eventLog("CMR", "Warn", "siteProgramParticipation", $this->controller->session["username"], "Invalid Source", $source);

            break;

        }

        return $rval;
    }

    private function siteProgramParticipation_Staged($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];
        $element_name = $arguments["ELEMENT_NAME"];
        $element_value = $arguments["ELEMENT_VALUE"];
        $rownum = $arguments["ROWNUM"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id));
        $validUpdate = (isset($site_id) && isset($element_name));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":
                if($validUpdate) {
                    $status = set_cust_site_use_attr($site_id, $element_name, $element_value);
                    if($status["X_RETURN_CODE"] != "SUCCESS") {
                        return $status;
                    }
                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteProgramParticipation_Staged", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $bindVars = array();

            $sql = "SELECT ";

            if(isset($rownum)) {
                $sql .= ":rownumber AS \"ROWNUM\", ";
                $bindVars = array(array("PH_NAME" => ":rownumber", "VARIABLE" => $rownum));
            } else {
                $sql .= "ROWNUM, ";
            }

            $sql .= "CA.ACCOUNT_NAME AS NATIONAL_ACCOUNT_NAME, ";
            $sql .= "CA.CUST_ACCOUNT_ID AS NATIONAL_ACCOUNT_CUST_ID, ";
            $sql .= "FFVL.FLEX_VALUE NATIONAL_ACCOUNT_CUST_NO, ";
            $sql .= "FFVL.FLEX_VALUE_MEANING AS NATIONAL_ACCOUNT_GROUP, ";
            $sql .= "FFVL.DESCRIPTION, ";
            $sql .= "SUBSTR(FFVL.DESCRIPTION, INSTR(FFVL.DESCRIPTION, '.') + 1) || '_MEMBER' AS ELEMENT_MEANING, ";
            $sql .= "SUBSTR(FFVL.DESCRIPTION, INSTR(FFVL.DESCRIPTION, '.') + 1) || '_MEMBER' AS ELEMENT_NAME, ";
            $sql .= "SNAM.ELEMENT_VALUE ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDEAS_SITES S, ";
            $sql .= "APPS.XXATDEAS_NAT_ACCT_RELATIONS NAR, ";
            $sql .= "APPS.HZ_CUST_ACCOUNTS CA, ";
            $sql .= "APPS.HZ_CUST_ACCT_SITES CAS, ";
            $sql .= "APPS.HZ_CUST_SITE_USES CSU, ";
            $sql .= "APPS.FND_FLEX_VALUES_VL FFVL, ";
            $sql .= "APPS.FND_FLEX_VALUE_SETS FFVS, ";
            $sql .= "(SELECT ";
            $sql .= "CSUA.ELEMENT_NAME, ";
            $sql .= "CSUA.ELEMENT_VALUE ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDEAS_CUST_SITE_USE_ATTRS CSUA ";
            $sql .= "WHERE ";
            $sql .= "CSUA.SITE_ID = :site_id) SNAM ";
            $sql .= "WHERE ";
            $sql .= "S.CUSTOMER_ID = NAR.CUSTOMER_ID ";
            $sql .= "AND NAR.NATIONAL_ACCOUNT_CUST_ID = CA.CUST_ACCOUNT_ID ";
            $sql .= "AND CA.CUST_ACCOUNT_ID = CAS.CUST_ACCOUNT_ID ";
            $sql .= "AND CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID ";
            $sql .= "AND CA.ACCOUNT_NUMBER = FFVL.FLEX_VALUE ";
            $sql .= "AND FFVL.FLEX_VALUE_SET_ID = FFVS.FLEX_VALUE_SET_ID ";
            $sql .= "AND SUBSTR(FFVL.DESCRIPTION, INSTR(FFVL.DESCRIPTION, '.') + 1) || '_MEMBER' = SNAM.ELEMENT_NAME (+) ";
            $sql .= "AND CSU.SITE_USE_CODE = 'BILL_TO' ";
            $sql .= "AND FFVS.FLEX_VALUE_SET_NAME = 'XXATDI_NATIONAL_ACCOUNT_MATRIX' ";
            $sql .= "AND FFVL.ENABLED_FLAG = 'Y' ";
            $sql .= "AND CA.STATUS = 'A' ";
            $sql .= "AND CAS.STATUS = 'A' ";
            $sql .= "AND CSU.STATUS = 'A' ";
            $sql .= "AND NVL(FFVL.START_DATE_ACTIVE, SYSDATE) <= SYSDATE ";
            $sql .= "AND NVL(FFVL.END_DATE_ACTIVE, SYSDATE) >= SYSDATE ";
            $sql .= "AND S.ID = :site_id ";

            $bindVars = array_merge($bindVars, array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id)));

            if(isset($element_name)) {
                $sql .= "AND SUBSTR(FFVL.DESCRIPTION, INSTR(FFVL.DESCRIPTION, '.') + 1) || '_MEMBER' = :element_name ";

                $bindVars = array_merge($bindVars, array(
                    array("PH_NAME" => ":element_name", "VARIABLE" => $element_name)
                ));
            }

            $sql .= "ORDER BY FFVL.FLEX_VALUE_MEANING, FFVL.DESCRIPTION";

            $stid = oracle_db_exec($sql, $bindVars);

            while($results = oci_fetch_assoc($stid)) {
                $rval[] = $results;
            }
        }

        return $rval;
    }

    private function siteProgramParticipation_Live($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];
        $element_name = $arguments["ELEMENT_NAME"];
        $rownum = $arguments["ROWNUM"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $validSelect = (isset($site_id));
        $validUpdate = (isset($site_id) && isset($element_name));

        switch($arguments["OPERATION_TYPE"]) {

            case "update":
                if($validUpdate) {
                    $procArgs = set_cust_site_use_attr_live($site_id, "SHIP_TO", $element_name, $arguments["ELEMENT_VALUE"]);

                    if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                        return $procArgs;
                    }

                    $procArgs = update_cross_reference_by_site($site_id);

                    if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                        return $procArgs;
                    }
                }

            break;

            default:

                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteProgramParticipation_Live", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }

            break;

        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $bindVars = array();

            $sql = "SELECT ";

            if(isset($rownum)) {
                $sql .= ":rownumber AS \"ROWNUM\", ";
                $bindVars = array(array("PH_NAME" => ":rownumber", "VARIABLE" => $rownum));
            } else {
                $sql .= "ROWNUM, ";
            }

            $sql .= "CA.CUST_ACCOUNT_ID AS NATIONAL_ACCOUNT_CUST_ID, ";
            $sql .= "FFVL.FLEX_VALUE NATIONAL_ACCOUNT_CUST_NO, ";
            $sql .= "FFVL.FLEX_VALUE_MEANING AS NATIONAL_ACCOUNT_GROUP, ";
            $sql .= "FFVL.DESCRIPTION, ";
            $sql .= "SUBSTR(FFVL.DESCRIPTION, INSTR(FFVL.DESCRIPTION, '.') + 1) || '_MEMBER' AS ELEMENT_MEANING, ";
            $sql .= "SUBSTR(FFVL.DESCRIPTION, INSTR(FFVL.DESCRIPTION, '.') + 1) || '_MEMBER' AS ELEMENT_NAME, ";
            $sql .= "SNAM.ELEMENT_VALUE ";
            $sql .= "FROM ";
            $sql .= "APPS.HZ_CUST_ACCT_SITES CAS, ";
            $sql .= "APPS.HZ_CUST_ACCT_RELATE CAR, ";
            $sql .= "APPS.HZ_CUST_ACCOUNTS CA, ";
            $sql .= "APPS.FND_FLEX_VALUES_VL FFVL, ";
            $sql .= "APPS.FND_FLEX_VALUE_SETS FFVS, ";
            $sql .= "(SELECT ";
            $sql .= "CSUA.ELEMENT_NAME, ";
            $sql .= "CSUA.ELEMENT_VALUE ";
            $sql .= "FROM ";
            $sql .= "APPS.HZ_CUST_ACCT_SITES CAS, ";
            $sql .= "APPS.HZ_CUST_SITE_USES CSU, ";
            $sql .= "APPS.XXATDAR_CUST_SITE_USES_ATTR CSUA ";
            $sql .= "WHERE ";
            $sql .= "CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID ";
            $sql .= "AND CSU.SITE_USE_ID = CSUA.SITE_USE_ID ";
            $sql .= "AND CAS.PARTY_SITE_ID = :site_id) SNAM ";
            $sql .= "WHERE ";
            $sql .= "CAS.CUST_ACCOUNT_ID = CAR.CUST_ACCOUNT_ID ";
            $sql .= "AND CAR.RELATED_CUST_ACCOUNT_ID = CA.CUST_ACCOUNT_ID ";
            $sql .= "AND CA.ACCOUNT_NUMBER = FFVL.FLEX_VALUE ";
            $sql .= "AND FFVL.FLEX_VALUE_SET_ID = FFVS.FLEX_VALUE_SET_ID ";
            $sql .= "AND SUBSTR(FFVL.DESCRIPTION, INSTR(FFVL.DESCRIPTION, '.') + 1) || '_MEMBER' = SNAM.ELEMENT_NAME (+) ";
            $sql .= "AND CAR.STATUS = 'A' ";
            $sql .= "AND FFVS.FLEX_VALUE_SET_NAME  = 'XXATDI_NATIONAL_ACCOUNT_MATRIX' ";
            $sql .= "AND FFVL.ENABLED_FLAG = 'Y' ";
            $sql .= "AND NVL(FFVL.START_DATE_ACTIVE, SYSDATE) <= SYSDATE ";
            $sql .= "AND NVL(FFVL.END_DATE_ACTIVE, SYSDATE) >= SYSDATE ";
            $sql .= "AND CAS.PARTY_SITE_ID = :site_id ";

            $bindVars = array_merge($bindVars, array(
                array("PH_NAME" => ":site_id", "VARIABLE" => $site_id)
            ));

            if(isset($element_name)) {
                $sql .= "AND SUBSTR(FFVL.DESCRIPTION, INSTR(FFVL.DESCRIPTION, '.') + 1) || '_MEMBER' = :element_name ";

                $bindVars = array_merge($bindVars, array(
                    array("PH_NAME" => ":element_name", "VARIABLE" => $element_name)
                ));
            }

            $sql .= "ORDER BY FFVL.FLEX_VALUE_MEANING, FFVL.DESCRIPTION";

            $stid = oracle_db_exec($sql, $bindVars);

            while($results = oci_fetch_assoc($stid)) {
                $rval[] = $results;
            }
        }

        return $rval;
    }

    function siteOnlineProduct($arguments, $config) {
        $rval = array();
        $oldValues = array();

        $id = $arguments["ID"];

        if(!is_numeric($id)) {
            unset($id);
        }

        // Online access setup is for live customers only, so if it's not supplied default to LIVE
        //
        $source = isset($arguments["SITE_SOURCE"]) ? $arguments["SITE_SOURCE"] : "LIVE";

        switch($source) {
            case "LIVE":
                // First fetch the existing data if it's not an insert
                //
                if($arguments["OPERATION_TYPE"] != "add") {
                    $fetchArgs = $arguments;
                    $fetchArgs["OPERATION_TYPE"] = "fetch";
                    $rval = $this->siteOnlineProduct_Live($fetchArgs, $config);
                    if(!empty($rval)) {
                        $oldValues = $rval["data"];
                    }
                }

                // If we're going to be doing more than fetching this call
                // (if we're not doing anything but fetching, we can keep our old $rval)
                //
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    // Log the changed data
                    //
                    log_data_change(
                        "SITE",
                        $arguments["SITE_ID"],
                        "shipToOnlineListGrid",
                        "ONLINE_PRODUCT",
                        (isset($id) ? $id : "NEW"),
                        $oldValues,
                        $arguments,
                        $source,
                        $this->controller->session["username"]
                    );

                    // Since we know that we're not just fetching,
                    // we must call into the CRUD function with the proper operation
                    //
                    $rval = $this->siteOnlineProduct_Live($arguments, $config);

                    $isa_push_status = $this->pushSiteProductsToISA($arguments["SITE_ID"], $arguments["PRODUCT_CODE"], $arguments["PRODUCT_VARIANT_ID"]);

                    if(!$isa_push_status["STATUS"]) {
                        $rval["X_RETURN_CODE"] = $isa_push_status["MESSAGE"];

                        eventLog(
                            "CMR",
                            "Warn",
                            "siteOnlineProduct",
                            $this->controller->session["username"],
                            "Unable to push site product data to ISA.",
                            print_r($rval, TRUE) . "\n" . print_r($isa_push_status, TRUE)
                        );
                    }
                }

            break;

            default:
                if($source != "STAGED") {
                    eventLog("CMR", "Warn", "siteOnlineProduct", $this->controller->session["username"], "Invalid Source", $source);
                }
            break;

        }

        return $rval;
    }

    private function siteOnlineProduct_Live($arguments, $config) {
        $rval = array();
        $rval["data"] = array();
        $rval["DSResponse"] = array();

        $id = $arguments["ID"];

        if(!is_numeric($id)) {
            unset($id);
        }

        $site_id = $arguments["SITE_ID"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $product_variant_id = $arguments["PRODUCT_VARIANT_ID"];

        if(!is_numeric($product_variant_id)) {
            unset($product_variant_id);
        }

        $status = $arguments["STATUS"];

        $validSelect = (isset($site_id) || isset($id));
        $validInsert = (isset($site_id) && isset($product_variant_id) && isset($status));
        $validUpdate = (isset($id));
        $validDelete = (isset($id));

        // Check the unique keys and other things that would cause the query to fail
        //
        if(($arguments["OPERATION_TYPE"] == "add" && $validInsert) || ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {
            $bindVars = array(
                array("PH_NAME" => ":party_site_id", "VARIABLE" => $site_id),
                array("PH_NAME" => ":product_variant_id", "VARIABLE" => $product_variant_id)
            );

            $sql = "SELECT ";
            $sql .= "COUNT(*) AS COUNT ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDEAS_SHIP_TO_PRODUCTS ";
            $sql .= "WHERE ";
            $sql .= "PARTY_SITE_ID = :party_site_id ";
            $sql .= "AND PRODUCT_VARIANT_ID = :product_variant_id ";

            // If we're updating we also need to exclude ourselves from the duplicate check.
            //
            if($arguments["OPERATION_TYPE"] == "update") {
                $sql .= "AND ID <> :id";

                $bindVars = array_merge($bindVars, array(array("PH_NAME" => ":id", "VARIABLE" => $id)));
            }

            $stid = oracle_db_exec($sql, $bindVars);
            $result = oci_fetch_assoc($stid);
            if($result["COUNT"] != 0) {
                $rval["status"] = $this->STATUS_VALIDATION_ERROR;
                $rval["errorMessage"] = CMR_Utility::getL18NString('siteOnlineProduct_Live.message.2', "A Customer Product record like this already exists.");
                return $rval;
            }
        }

        switch($arguments["OPERATION_TYPE"]) {
            case "add":
                if($validInsert) {
                    $id = oracle_get_nextval("APPS.XXATDEAS_SHIP_TO_PRODUCTS_S");

                    $sql = "INSERT INTO ";
                    $sql .= "APPS.XXATDEAS_SHIP_TO_PRODUCTS ";
                    $sql .= "(ID, ";
                    $sql .= "PARTY_SITE_ID, ";
                    $sql .= "PRODUCT_VARIANT_ID, ";
                    $sql .= "STATUS, ";
                    $sql .= "CREATION_DATE, ";
                    $sql .= "CREATED_BY, ";
                    $sql .= "LAST_UPDATE_DATE, ";
                    $sql .= "LAST_UPDATED_BY) ";
                    $sql .= "VALUES (";
                    $sql .= ":id, ";
                    $sql .= ":party_site_id, ";
                    $sql .= ":product_variant_id, ";
                    $sql .= ":status, ";
                    $sql .= "SYSDATE, ";
                    $sql .= ":user_id, ";
                    $sql .= "SYSDATE, ";
                    $sql .= ":user_id)";

                    $bindVars = array(
                        array("PH_NAME" => ":id", "VARIABLE" => $id),
                        array("PH_NAME" => ":party_site_id", "VARIABLE" => $site_id),
                        array("PH_NAME" => ":product_variant_id", "VARIABLE" => $product_variant_id),
                        array("PH_NAME" => ":status", "VARIABLE" => $status),
                        array("PH_NAME" => ":user_id", "VARIABLE" => $this->controller->session["user_id"])
                    );

                    $stid = oracle_db_exec($sql, $bindVars);
                }
            break;

            case "update":
                if($validUpdate) {
                    $sql = "UPDATE ";
                    $sql .= "APPS.XXATDEAS_SHIP_TO_PRODUCTS ";
                    $sql .= "SET ";
                    $sql .= "STATUS = :status, ";
                    $sql .= "LAST_UPDATE_DATE = SYSDATE, ";
                    $sql .= "LAST_UPDATED_BY = :user_id ";
                    $sql .= "WHERE ";
                    $sql .= "ID = :id";

                    $bindVars = array(
                        array("PH_NAME" => ":status", "VARIABLE" => $status),
                        array("PH_NAME" => ":id", "VARIABLE" => $id),
                        array("PH_NAME" => ":user_id", "VARIABLE" => $this->controller->session["user_id"])
                    );

                    $stid = oracle_db_exec($sql, $bindVars);
                }
            break;

            case "remove":
                if($validDelete) {
                    $sql = "DELETE ";
                    $sql .= "APPS.XXATDEAS_SHIP_TO_PRODUCTS ";
                    $sql .= "WHERE ";
                    $sql .= "ID = :id";

                    $bindVars = array(array("PH_NAME" => ":id", "VARIABLE" => $id));

                    $stid = oracle_db_exec($sql, $bindVars);

                    $rval["data"][] = $arguments;
                }
            break;

            default:
                if($arguments["OPERATION_TYPE"] != "fetch") {
                    eventLog("CMR", "Warn", "siteOnlineProduct_Live", $this->controller->session["username"], "Invalid Operation Type", $arguments["OPERATION_TYPE"]);
                }
            break;
        }

        if(($arguments["OPERATION_TYPE"] == "fetch" && $validSelect) ||
            ($arguments["OPERATION_TYPE"] == "add" && $validInsert) ||
            ($arguments["OPERATION_TYPE"] == "update" && $validUpdate)) {

            $bindVars = array();
            $criteria_fields = array();

            if($arguments["OPERATION_TYPE"] == "fetch") {
                $criteria_fields = array(
                    "STP.ID" => array($id, "EQUALS"),
                    "STP.PARTY_SITE_ID" => array($site_id, "EQUALS"),
                    "STP.PRODUCT_VARIANT_ID" => array($arguments["PRODUCT_VARIANT_ID"], "EQUALS"),
                    "OP.PRODUCT_CODE" => array($arguments["PRODUCT_CODE"], "CONTAINS"),
                    "STP.STATUS" => array($arguments["STATUS"], "MATCHES"),
                    "TO_CHAR(STP.CREATION_DATE, 'MM/DD/RRRR')" => array($arguments["CREATION_DATE"], "CONTAINS"),
                    "STP.CREATED_BY" => array($arguments["CREATED_BY"], "EQUALS"),
                    "TO_CHAR(STP.LAST_UPDATE_DATE, 'MM/DD/RRRR')" => array($arguments["LAST_UPDATE_DATE"], "CONTAINS"),
                    "STP.LAST_UPDATED_BY" => array($arguments["LAST_UPDATED_BY"], "EQUALS")
                );
            } else {
                $criteria_fields = array("STP.ID" => array($id, "EQUALS"));
            }
            $where_clause = build_where_clause($criteria_fields, $bindVars);

            $sql = "SELECT ";
            $sql .= "STP.ID, ";
            $sql .= "STP.PARTY_SITE_ID, ";
            $sql .= "STP.PRODUCT_VARIANT_ID, ";
            $sql .= "OP.PRODUCT_CODE, ";
            $sql .= "STP.STATUS, ";
            $sql .= "TO_CHAR(STP.CREATION_DATE, 'MM/DD/RRRR') AS CREATION_DATE, ";
            $sql .= "STP.CREATED_BY, ";
            $sql .= "TO_CHAR(STP.LAST_UPDATE_DATE, 'MM/DD/RRRR') AS LAST_UPDATE_DATE, ";
            $sql .= "STP.LAST_UPDATED_BY ";
            $sql .= "FROM ";
            $sql .= "APPS.XXATDEAS_SHIP_TO_PRODUCTS STP, ";
            $sql .= "APPS.XXATDEAS_ONLINE_PROD_VARS OPV, ";
            $sql .= "APPS.XXATDEAS_ONLINE_PRODUCTS OP ";
            $sql .= $where_clause;
            $sql .= ($where_clause) ? " AND " : "WHERE ";
            $sql .= "STP.PRODUCT_VARIANT_ID = OPV.ID ";
            $sql .= "AND OPV.PRODUCT_ID = OP.ID ";

            $rval = oracle_paged_results($sql, $bindVars, $arguments["START"] + 1, $arguments["END"] + 1, $arguments["SORT_BY"]);
            if(empty($rval)) {
                $rval["data"] = array();
                $rval["DSResponse"] = array();
            }
        }

        return $rval;
    }

    function pushSiteProductsToISA($site_id, $product_code, $product_variant_id) {
        $rval = array(
            "STATUS" => TRUE,
            "MESSAGE" => ""
        );

        $site_product_data = array();

        $user = $this->controller->servers_ini_file[$this->controller->server]["isa_data_push_user"];
        $servers = explode(",", $this->controller->servers_ini_file[$this->controller->server]["isa_data_push_servers"]);

        if (!isset($user) || !isset($servers) || empty($servers)) {
            $rval["STATUS"] = FALSE;
            $default = "Configuration error, please check that isa_data_push_user and isa_data_push_servers are both set in servers.ini";
            $rval["MESSAGE"] = CMR_Utility::getL18NString('pushSiteProductsToISA.message.1', $default);

            return $rval;
        }

        $sql = "SELECT ";
        $sql .= "STP.PARTY_SITE_ID, ";
        $sql .= "OP.PRODUCT_CODE AS PRODUCT, ";
        $sql .= "DECODE(STP.STATUS, 'A', 'Y', 'N') AS IS_ACTIVE ";
        $sql .= "FROM ";
        $sql .= "APPS.XXATDEAS_SHIP_TO_PRODUCTS STP, ";
        $sql .= "APPS.XXATDEAS_ONLINE_PROD_VARS OPV, ";
        $sql .= "APPS.XXATDEAS_ONLINE_PRODUCTS OP ";
        $sql .= "WHERE ";
        $sql .= "STP.PRODUCT_VARIANT_ID = OPV.ID ";
        $sql .= "AND OPV.PRODUCT_ID = OP.ID ";
        $sql .= "AND STP.PARTY_SITE_ID = TO_CHAR(:site_id) ";
        $sql .= "AND STP.PRODUCT_VARIANT_ID = :product_variant_id";

        $bindVars = array(
            array("PH_NAME" => ":site_id", "VARIABLE" => $site_id),
            array("PH_NAME" => ":product_variant_id", "VARIABLE" => $product_variant_id)
        );

        $stid = oracle_db_exec($sql, $bindVars);

        while($results = oci_fetch_assoc($stid)) {
            $site_product_data[] = $results;
        }

        foreach($servers as $server) {
            $mysql_conn = mysql_db_connect($this->controller->credentials_file, $server, $user);

            if(!$mysql_conn) {
                $rval["STATUS"] = FALSE;
                $params = array('server' => $server, 'user' => $user, 'site_id' => $site_id);
                $default = "Unable to connect to $server as $user to push new site data for party_site_id $site_id.";
                $rval["MESSAGE"] .= CMR_Utility::getL18NString('pushSiteProductsToISA.message.2', $default, $params);

                continue;
            }

            if(!empty($site_product_data)) {
                foreach($site_product_data as $data) {
                    foreach($data as &$column) {
                        $column = mysql_real_escape_string($column, $mysql_conn);
                    }

                    $sql = "INSERT INTO ";
                    $sql .= "ORACLE_EXPORTS.SHIP_TO_PRODUCT ";
                    $sql .= "SET ";
                    $sql .= "PARTY_SITE_ID = '{$data['PARTY_SITE_ID']}', ";
                    $sql .= "PRODUCT = '{$data['PRODUCT']}', ";
                    $sql .= "IS_ACTIVE = '{$data['IS_ACTIVE']}' ";
                    $sql .= "ON DUPLICATE KEY ";
                    $sql .= "UPDATE ";
                    $sql .= "IS_ACTIVE = '{$data['IS_ACTIVE']}'";

                    list($result, $numrows) = mysql_db_exec($sql, $mysql_conn);

                    if(!$result) {
                        $rval["STATUS"] = FALSE;
                        $rval["MESSAGE"] .= CMR_Utility::getL18NString('pushSiteProductsToISA.message.3', "Cannot merge into ORACLE_EXPORTS.SHIP_TO_PRODUCT.");

                        continue;
                    }
                }
            } else {
                $sql = "DELETE ";
                $sql .= "FROM ";
                $sql .= "ORACLE_EXPORTS.SHIP_TO_PRODUCT ";
                $sql .= "WHERE ";
                $sql .= "PARTY_SITE_ID = '" . mysql_real_escape_string($site_id, $mysql_conn) . "' ";
                $sql .= "AND PRODUCT = '" . mysql_real_escape_string($product_code, $mysql_conn) . "'";

                list($result, $numrows) = mysql_db_exec($sql, $mysql_conn);

                if(!$result) {
                    $rval["STATUS"] = FALSE;
                    $rval["MESSAGE"] .= CMR_Utility::getL18NString('pushSiteProductsToISA.message.4', "Cannot delete from ORACLE_EXPORTS.SHIP_TO_PRODUCT.");
                }
            }

            mysql_close($mysql_conn);
        }

        return $rval;
    }

    /*------------------------------------------------------------*\
    |              Server-side Validation Functions                |
    \*------------------------------------------------------------*/

    function validateSiteInfo($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];
        $site_source = $arguments["SITE_SOURCE"];
        $customer_source = $arguments["CUSTOMER_SOURCE"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        if(!isset($site_id)) {
            $rval["VALIDATED"] = 0;
            $params = array('site_id' => $site_id);
            $default = "Invalid Site ID passed to the validation routine: $site_id.";
            $rval["MESSAGE"] = CMR_Utility::getL18NString('validateSiteInfo.message.1', $default, $params);
        } else {
            if($customer_source == "LIVE") {
                if($site_source == "LIVE") {
                    $sql = "SELECT ";
                    $sql .= "SUM(COUNT) AS COUNT ";
                    $sql .= "FROM (";
                    $sql .= "SELECT ";
                    $sql .= "COUNT(*) AS COUNT ";
                    $sql .= "FROM ";
                    $sql .= "APPS.HZ_PARTY_SITES PS1, ";
                    $sql .= "APPS.HZ_PARTY_SITES PS2, ";
                    $sql .= "APPS.HZ_CUST_ACCT_SITES CAS1, ";
                    $sql .= "APPS.HZ_CUST_ACCT_SITES CAS2, ";
                    $sql .= "APPS.HZ_CUST_SITE_USES CSU1, ";
                    $sql .= "APPS.HZ_CUST_SITE_USES CSU2 ";
                    $sql .= "WHERE ";
                    $sql .= "PS1.PARTY_ID = PS2.PARTY_ID ";
                    $sql .= "AND PS1.PARTY_SITE_ID = CAS1.PARTY_SITE_ID ";
                    $sql .= "AND CAS1.CUST_ACCT_SITE_ID = CSU1.CUST_ACCT_SITE_ID ";
                    $sql .= "AND PS2.PARTY_SITE_ID = CAS2.PARTY_SITE_ID ";
                    $sql .= "AND CAS2.CUST_ACCT_SITE_ID = CSU2.CUST_ACCT_SITE_ID ";
                    $sql .= "AND CSU1.SITE_USE_ID <> CSU2.SITE_USE_ID ";
                    $sql .= "AND CSU1.ATTRIBUTE4 = CSU2.ATTRIBUTE4 ";
                    $sql .= "AND CAS2.STATUS = 'A' ";
                    $sql .= "AND CSU2.STATUS = 'A' ";
                    $sql .= "AND CSU1.SITE_USE_CODE = 'SHIP_TO' ";
                    $sql .= "AND CSU2.SITE_USE_CODE = 'SHIP_TO' ";
                    $sql .= "AND PS1.PARTY_SITE_ID = :site_id ";
                    $sql .= "UNION ALL ";
                    $sql .= "SELECT ";
                    $sql .= "COUNT(*) AS COUNT ";
                    $sql .= "FROM ";
                    $sql .= "APPS.HZ_CUST_ACCOUNTS CA, ";
                    $sql .= "APPS.HZ_PARTY_SITES PS, ";
                    $sql .= "APPS.HZ_CUST_ACCT_SITES CAS, ";
                    $sql .= "APPS.HZ_CUST_SITE_USES CSU, ";
                    $sql .= "APPS.XXATDEAS_CUSTOMERS C, ";
                    $sql .= "APPS.XXATDEAS_SITES S, ";
                    $sql .= "APPS.XXATDEAS_CUST_ACCOUNT_SITES XCAS ";
                    $sql .= "WHERE ";
                    $sql .= "CA.CUST_ACCOUNT_ID = CAS.CUST_ACCOUNT_ID ";
                    $sql .= "AND CAS.PARTY_SITE_ID = PS.PARTY_SITE_ID ";
                    $sql .= "AND CA.CUST_ACCOUNT_ID = C.LIVE_CUSTOMER_ID ";
                    $sql .= "AND C.ID = S.CUSTOMER_ID ";
                    $sql .= "AND S.ID = XCAS.SITE_ID ";
                    $sql .= "AND XCAS.STORE_NUMBER = CSU.ATTRIBUTE4 ";
                    $sql .= "AND CSU.SITE_USE_CODE = 'SHIP_TO' ";
                    $sql .= "AND PS.PARTY_SITE_ID = :site_id ";
                    $sql .= ")";
                } else {
                    $sql = "SELECT ";
                    $sql .= "SUM(COUNT) AS COUNT ";
                    $sql .= "FROM( ";
                    $sql .= "SELECT ";
                    $sql .= "COUNT(*) AS COUNT ";
                    $sql .= "FROM ";
                    $sql .= "APPS.HZ_CUST_ACCOUNTS CA, ";
                    $sql .= "APPS.HZ_CUST_ACCT_SITES CAS, ";
                    $sql .= "APPS.HZ_CUST_SITE_USES CSU, ";
                    $sql .= "APPS.XXATDEAS_CUSTOMERS C, ";
                    $sql .= "APPS.XXATDEAS_SITES S, ";
                    $sql .= "APPS.XXATDEAS_CUST_ACCOUNT_SITES XCAS ";
                    $sql .= "WHERE ";
                    $sql .= "S.CUSTOMER_ID = C.ID ";
                    $sql .= "AND S.ID = XCAS.SITE_ID ";
                    $sql .= "AND C.LIVE_CUSTOMER_ID = CA.CUST_ACCOUNT_ID ";
                    $sql .= "AND CA.CUST_ACCOUNT_ID = CAS.CUST_ACCOUNT_ID ";
                    $sql .= "AND CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID ";
                    $sql .= "AND CAS.STATUS = 'A' ";
                    $sql .= "AND CSU.STATUS = 'A' ";
                    $sql .= "AND CSU.SITE_USE_CODE = 'SHIP_TO' ";
                    $sql .= "AND CSU.ATTRIBUTE4 = XCAS.STORE_NUMBER ";
                    $sql .= "AND S.ID = :site_id ";
                    $sql .= "UNION ALL ";
                    $sql .= "SELECT ";
                    $sql .= "COUNT(*) AS COUNT ";
                    $sql .= "FROM ";
                    $sql .= "APPS.XXATDEAS_SITES S1, ";
                    $sql .= "APPS.XXATDEAS_SITES S2, ";
                    $sql .= "APPS.XXATDEAS_CUST_ACCOUNT_SITES CAS1, ";
                    $sql .= "APPS.XXATDEAS_CUST_ACCOUNT_SITES CAS2 ";
                    $sql .= "WHERE ";
                    $sql .= "S1.CUSTOMER_ID = S2.CUSTOMER_ID ";
                    $sql .= "AND S1.ID = CAS1.SITE_ID ";
                    $sql .= "AND S2.ID = CAS2.SITE_ID ";
                    $sql .= "AND S1.ID <> S2.ID ";
                    $sql .= "AND CAS1.STORE_NUMBER = CAS2.STORE_NUMBER ";
                    $sql .= "AND S1.ID = :site_id ";
                    $sql .= ") ";
                }
            } else {
                $sql = "SELECT ";
                $sql .= "COUNT(*) AS COUNT ";
                $sql .= "FROM ";
                $sql .= "APPS.XXATDEAS_SITES S1, ";
                $sql .= "APPS.XXATDEAS_SITES S2, ";
                $sql .= "APPS.XXATDEAS_CUST_ACCOUNT_SITES CAS1, ";
                $sql .= "APPS.XXATDEAS_CUST_ACCOUNT_SITES CAS2 ";
                $sql .= "WHERE ";
                $sql .= "S1.CUSTOMER_ID = S2.CUSTOMER_ID ";
                $sql .= "AND S1.ID = CAS1.SITE_ID ";
                $sql .= "AND S2.ID = CAS2.SITE_ID ";
                $sql .= "AND S1.ID <> S2.ID ";
                $sql .= "AND CAS1.STORE_NUMBER = CAS2.STORE_NUMBER ";
                $sql .= "AND S1.ID = :site_id ";
            }

            $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));
            $stid = oracle_db_exec($sql, $bindVars);
            $results = oci_fetch_assoc($stid);

            if($results["COUNT"] > 0) {
                $rval["VALIDATED"] = 0;
                $default = "Store number conflicts with another store number for this customer.";
                $rval["MESSAGE"] = CMR_Utility::getL18NString('validateSiteInfo.message.2', $default);
            } else {
                $rval["VALIDATED"] = 1;
                $rval["MESSAGE"] = CMR_Utility::getL18NString('validateSiteInfo.success', CMR_Utility::SUCCESS);
            }
        }

        return $rval;
    }

    function validateSiteContacts($arguments, $config) {
        $rval = array();

        $site_id = $arguments["SITE_ID"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        if(!isset($site_id)) {
            $rval["VALIDATED"] = 0;
            $params = array('site_id' => $site_id);
            $default = "Invalid Site ID passed to the validation routine: $site_id.";
            $rval["MESSAGE"] = CMR_Utility::getL18NString('validateSiteContacts.message.1', $default, $params);
        } else {
            $sql = "SELECT COUNT(*) AS COUNT FROM APPS.XXATDEAS_PARTIES WHERE SITE_ID = :site_id";

            $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

            $stid = oracle_db_exec($sql, $bindVars);
            $results = oci_fetch_assoc($stid);

            if($results["COUNT"] > 0) {
                $sql = "SELECT COUNT(*) AS COUNT ";
                $sql .= "FROM APPS.XXATDEAS_CONTACT_POINTS ";
                $sql .= "WHERE ENTITY_TYPE = 'PARTY' ";
                $sql .= "AND ENTITY_ID IN (SELECT ID FROM APPS.XXATDEAS_PARTIES WHERE SITE_ID = :site_id)";

                $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $site_id));

                $stid = oracle_db_exec($sql, $bindVars);
                $results = oci_fetch_assoc($stid);

                if($results["COUNT"] > 0) {
                    $rval["VALIDATED"] = 1;
                    $rval["MESSAGE"] = CMR_Utility::getL18NString('validateSiteInfo.success', CMR_Utility::SUCCESS);
                } else {
                    $rval["VALIDATED"] = 0;
                    $default = "At least one contact must have at least one phone number before this site can be approved.";
                    $rval["MESSAGE"] = CMR_Utility::getL18NString('validateSiteContacts.message.2', $default);
                }
            } else {
                $rval["VALIDATED"] = 0;
                $default = "At least one contact is required before this site can be approved.";
                $rval["MESSAGE"] = CMR_Utility::getL18NString('validateSiteContacts.message.3', $default);
            }
        }

        return $rval;
    }

    public function push_site_data($site_id) {
        $rval = array(
            "STATUS" => TRUE,
            "MESSAGE" => ""
        );

        $user = $this->controller->servers_ini_file[$this->controller->server]["pricing_data_push_user"];
        $servers = explode(",", $this->controller->servers_ini_file[$this->controller->server]["pricing_data_push_servers"]);

        if (!isset($user) || !isset($servers) || empty($servers)) {
            $rval["STATUS"] = FALSE;
            $default = "Configuration error, please check that pricing_data_push_user and pricing_data_push_servers are both set in servers.ini";
            $rval["MESSAGE"] = CMR_Utility::getL18NString('push_site_data.message.1', $default);

            return $rval;
        }

        $site = Site::get_site_data_for_push($site_id);

        if(!$site["STATUS"]) {
            $rval["STATUS"] = FALSE;
            $message = $site['MESSAGE'];
            $params = array('site_id' => $site_id, 'message' => $message);
            $default = "The call to get_site_data_for_push failed for party_site_id $site_id with error: {$message}.";
            $rval["MESSAGE"] = CMR_Utility::getL18NString('push_site_data.message.2', $default, $params);

            return $rval;
        }

        foreach($servers as $server) {
            $mysql_conn = mysql_db_connect($this->controller->credentials_file, $server, $user);

            if(!$mysql_conn) {
                $rval["STATUS"] = FALSE;
                $params = array('server' => $server, 'user' => $user, 'site_id' => $site_id);
                $default = "Unable to connect to $server as $user to push new site data for party_site_id $site_id.";
                $rval["MESSAGE"] .= CMR_Utility::getL18NString('push_site_data.message.3', $default, $params);

                continue;
            }

            if(isset($price_engine)) {
                $price_engine->set_connection($mysql_conn);
            } else {
                $price_engine = new QPXPricingEngine($mysql_conn);
            }

            $status = Site::push_site_data_to_server($mysql_conn, $site);

            if(!$status["STATUS"]) {
                $rval["STATUS"] = FALSE;
                $params = array('server' => $server, 'user' => $user, 'site_id' => $site_id);
                $default = "Unable to update server $server as $user with new site data for party_site_id $site_id.";
                $rval["MESSAGE"] .= CMR_Utility::getL18NString('push_site_data.message.4', $default, $params);
            }

            if(isset($site["SHIP_TO_SITE_USE_ID"]) && !is_null($site["SHIP_TO_SITE_USE_ID"])) {
                $price_engine->clear_cache(NULL, $site["SHIP_TO_SITE_USE_ID"]);
            }

            mysql_close($mysql_conn);
        }

        return $rval;
    }

    function fixSiteCrossReference($site_id) {
        $rval = array(
            "STATUS" => FALSE,
            "MESSAGE" => "SUCCESS"
        );

        if(is_array($site_id)) {
            $site_id = $site_id["SITE_ID"];
        }

        if(!is_numeric($site_id)) {
            $params = array('site_id' => $site_id);
            $default = "Non numeric site ID detected ($site_id) while attetmpting to repair the cross reference.";
            $rval["MESSAGE"] = CMR_Utility::getL18NString('fixSiteCrossReference.message.1', $default, $params);
            return $rval;
        }

        $update_result = update_cross_reference_by_site($site_id);

        if($update_result["X_RETURN_CODE"] != "SUCCESS") {
            $rval["MESSAGE"] = $update_result["X_RETURN_CODE"];
            return $rval;
        }

        $rval["STATUS"] = TRUE;

        return $rval;
    }

    /*------------------------------------------------------------*\
    |              Server-side Validation Functions                |
    \*------------------------------------------------------------*/

    function pushSiteData($site_id) {
        if(is_array($site_id)) {
            $site_id = $site_id["SITE_ID"];
        }

        if(!is_numeric($site_id)) {
            $params = array('site_id' => $site_id);
            $default = "Non numeric site ID detected ($site_id) while sending data to pricing servers";

            return array(
                "STATUS" => FALSE,
                "MESSAGE" => CMR_Utility::getL18NString('pushSiteData.message.1', $default, $params)
            );
        }

        eventLog("CMR", "Info", "pushSiteData", $this->controller->session["username"],
            "User requested that site_id $site_id have its attributes refreshed on the pricing servers.",
            "Customers.pushSiteData");

        $rval = Site::push_site_data($site_id);

        return $rval;
    }

    public function commitStagedSite($arguments, $config) {
        $site_id = $arguments["SITE_ID"];

        if(!is_numeric($site_id)) {
            unset($site_id);
        }

        $valid = (isset($site_id));

        if($valid) {
            $procArgs["P_STAGED_SITE_ID"] = $site_id;
            $procArgs["X_LIVE_SITE_ID"] = "";

            $procArgs["X_RETURN_CODE"] = "";

            oracle_proc_exec("APPS.XXATDEAS_CUST_MAINT_REG_PKG.COMMIT_STAGED_SITE", $procArgs);

            if($procArgs["X_RETURN_CODE"] != "SUCCESS") {
                return $procArgs;
            }

            $procArgs["SITE_ID"] = $procArgs["X_LIVE_SITE_ID"];

            $sql = "SELECT ";
            $sql .= "CSU.LOCATION, ";
            $sql .= "PS.ADDRESSEE, ";
            $sql .= "CSU.SITE_USE_CODE ";
            $sql .= "FROM ";
            $sql .= "APPS.HZ_CUST_ACCT_SITES CAS, ";
            $sql .= "APPS.HZ_PARTY_SITES PS, ";
            $sql .= "APPS.HZ_CUST_SITE_USES CSU ";
            $sql .= "WHERE ";
            $sql .= "CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID ";
            $sql .= "AND CAS.PARTY_SITE_ID = PS.PARTY_SITE_ID ";
            $sql .= "AND CAS.PARTY_SITE_ID = :site_id";

            $bindVars = array(array("PH_NAME" => ":site_id", "VARIABLE" => $procArgs["SITE_ID"]));

            $stid = oracle_db_exec($sql, $bindVars);
            $result = oci_fetch_assoc($stid);

            $location = $result["LOCATION"];
            $dba_name = $result["ADDRESSEE"];
            $site_use_code = $result["SITE_USE_CODE"];

            log_data_change(
                "SITE",
                $procArgs["SITE_ID"],
                "Site Creation",
                "SITE",
                $procArgs["SITE_ID"],
                array(array("SITE_ID" => "")),
                array("SITE_ID" => $procArgs["SITE_ID"]),
                "LIVE",
                $this->controller->session["username"]
            );

            $data_push_results = $this->pushSiteData($procArgs["SITE_ID"]);

            if(!$data_push_results["STATUS"]) {
                eventLog("CMR", "Warn", "NewLiveCustomer", $this->controller->session["username"],
                    "Location $location (DBA Name $dba_name) could not export site data to all external databases.",
                    "Customers.commitStagedCustomer. {$data_push_results['MESSAGE']}");
            }

            eventLog("CMR", "Info", "NewLiveSite", $this->controller->session["username"],
                "Location $location (DBA Name $dba_name) has been successfully created in Oracle.",
                "Customers.commitStagedCustomer");

            return $procArgs;
        }

    }

    function siteAcquisitions($arguments, $config) {
        $QBERecord = NULL;

        $rval = array(
            'data' => array(),
            'status' => $this->STATUS_VALIDATION_ERROR,
            'totalRows' => 0,
            'startRow' => 0,
            'endRow' => 0
        );

        $arguments['SITE_ID'] = ($arguments['SITE_ID']) ? $arguments['SITE_ID'] : '';

        $PCLOracleConnection = new PCLOracle($GLOBALS['oracle_conn']);

        switch ($arguments['CUSTOMER_SOURCE']) {
            case 'LIVE':
                if ($arguments['SITE_ID'] && !$arguments['SITE_USE_ID'] ) {
                    // Convert SITE_ID to SITE_USE_ID
                    $sql = "SELECT DISTINCT
                                CSU.SITE_USE_ID
                            FROM
                                APPS.HZ_CUST_ACCT_SITES CAS,
                                APPS.HZ_CUST_SITE_USES CSU
                            WHERE
                                CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID
                                AND CSU.SITE_USE_CODE = 'SHIP_TO'
                                AND CAS.SHIP_TO_FLAG IN ('P', 'Y')
                                AND CAS.STATUS = 'A'
                                AND CSU.STATUS = 'A'
                                AND CAS.PARTY_SITE_ID = :SITE_ID
                            ";

                    $bindVariables = array(array('PH_NAME' => ':SITE_ID', 'VARIABLE' => $arguments['SITE_ID']));

                    $executeResults = $PCLOracleConnection->execute($sql, $bindVariables);

                    if(!$executeResults['STATUS'] || $PCLOracleConnection->hasErrors()) {
                        $default = "Error executing SITE_USE_ID lookup.";
                        $rval['errorMessage'] = CMR_Utility::getL18NString('siteAcquisitions.message.1', $default);
                        $rval['errorMessage'] .= implode('<br>', $PCLOracleConnection->getErrors());
                        return $rval;
                    }

                    $fetchResults = $PCLOracleConnection->fetchAllRecords();

                    if(!$fetchResults['STATUS'] || $PCLOracleConnection->hasErrors()) {
                        $default = "Error fetching SITE_USE_ID lookup.";
                        $rval['errorMessage'] = CMR_Utility::getL18NString('siteAcquisitions.message.2', $default);
                        $rval['errorMessage'] .= implode('<br>', $PCLOracleConnection->getErrors());
                        return $rval;
                    }

                    $arguments['SITE_USE_ID'] = $fetchResults['RESULT'][0]['SITE_USE_ID'];
                } else {
                    $arguments['SITE_USE_ID'] = ($arguments['SITE_USE_ID']) ? $arguments['SITE_USE_ID'] : '';
                }

                $acquisitionTableObject = new PCLOracleTable('Acquisitions_LIVE', $this->GetTableMetadata('XXATDAR_SITE_ACQUISITIONS'), $PCLOracleConnection);

                if ($arguments['OPERATION_TYPE'] == 'add' || $arguments['OPERATION_TYPE'] == 'update'){
                    // need to check that current combination of ACQ_SOURCE/ACQ_CUSTOMER/ACQ_STORE is not already used -
                    // and if used return a message that indicates where used.

                    $sql = "SELECT
                                CSU.LOCATION AS LOCATION
                            FROM
                                APPS.XXATDAR_SITE_ACQUISITIONS ACQ,
                                APPS.HZ_CUST_SITE_USES CSU
                            WHERE
                                ACQ.ACQ_SOURCE = :ACQ_SOURCE
                                AND ACQ.ACQ_CUSTOMER = UPPER(:ACQ_CUSTOMER)
                                AND NVL(ACQ.ACQ_STORE,'~') = UPPER(NVL(:ACQ_STORE,'~'))
                                AND ACQ.ID != NVL(:ID,-1)
                                AND CSU.SITE_USE_ID = ACQ.SITE_USE_ID
                            ";

                    $bindVariables = array(
                        array('PH_NAME' => ':ACQ_SOURCE', 'VARIABLE' => $arguments['ACQ_SOURCE']),
                        array('PH_NAME' => ':ACQ_CUSTOMER', 'VARIABLE' => $arguments['ACQ_CUSTOMER']),
                        array('PH_NAME' => ':ACQ_STORE', 'VARIABLE' => $arguments['ACQ_STORE']),
                        array('PH_NAME' => ':ID', 'VARIABLE' => $arguments['ID'])
                        );

                    $executeResults = $PCLOracleConnection->execute($sql, $bindVariables);

                    if(!$executeResults['STATUS'] || $PCLOracleConnection->hasErrors()) {
                        $default = "Error executing alternate location lookup.";
                        $rval['errorMessage'] = CMR_Utility::getL18NString('siteAcquisitions.message.3', $default);
                        $rval['errorMessage'] .= implode('<br>', $PCLOracleConnection->getErrors());
                        return $rval;
                    }

                    $fetchResults = $PCLOracleConnection->fetchAllRecords();

                    if(!$fetchResults['STATUS'] || $PCLOracleConnection->hasErrors()) {
                        $default = "Error fetching alternate location lookup.";
                        $rval['errorMessage'] = CMR_Utility::getL18NString('siteAcquisitions.message.4', $default);
                        $rval['errorMessage'] .= implode('<br>', $PCLOracleConnection->getErrors());
                        return $rval;
                    }

                    $location = $fetchResults['RESULT'][0]['LOCATION'];
                    if($location){
                        $params = array('location' => $location);
                        $default = "Acquisition source/customer/store is already in use on LIVE Location Number: $location.";
                        $rval['errorMessage'] = CMR_Utility::getL18NString('siteAcquisitions.message.4', $default, $params);
                        return $rval;
                    }
                }
                // log data change
                if($arguments['OPERATION_TYPE'] != 'fetch' ){
                    $old_Args['ID'] = $arguments['ID'];
                    $old_Data = $acquisitionTableObject->fetchByExample($old_Args);

                    log_data_change(
                        'SITE',
                        $arguments['SITE_ID'],
                        'siteAcquisitionsListGrid',
                        'SITE_ACQUISITIONS',
                        (isset($arguments['ID']) ? $arguments['ID'] : 'NEW'),
                        $old_Data['RESULT'][0],
                        $arguments,
                        $arguments['CUSTOMER_SOURCE'],
                        $this->controller->session['username']
                    );
                }
            break;

            case 'STAGED':
                $acquisitionTableObject = new PCLOracleTable('Acquisitions_STAGED', $this->GetTableMetadata('XXATDEAS_SITE_ACQUISITIONS'), $PCLOracleConnection);
            break;

            default:
                eventLog("CMR", "Warn", "siteAcquisitions", $this->controller->session["username"], "Invalid Source", $source);
            break;
        }

        $acquisitionTableController = new PCLSmartClientTableController($acquisitionTableObject, $this->controller);

        switch($arguments['OPERATION_TYPE']) {
            case "add":
                $acquisitionTableController->setCreation($arguments);
                $acquisitionTableController->setLastUpdate($arguments);

                $acquisitionTableController->setIfMissing($arguments, 'ACQ_SOURCE');
                $acquisitionTableController->setIfMissing($arguments, 'ACQ_CUSTOMER');
                $acquisitionTableController->setIfMissing($arguments, 'ACQ_STORE');

                // Get the org_id from Oracle because we need it for R12.
                //
                $sql = "SELECT APPS.FND_GLOBAL.ORG_ID AS ORG_ID FROM DUAL";
                $stid = oracle_db_exec($sql);
                $result = oci_fetch_assoc($stid);
                $arguments['ORG_ID'] = $result["ORG_ID"];

                $arguments['ACQ_SOURCE'] = strtoupper($arguments['ACQ_SOURCE']);
                $arguments['ACQ_CUSTOMER'] = strtoupper($arguments['ACQ_CUSTOMER']);
                $arguments['ACQ_STORE'] = strtoupper($arguments['ACQ_STORE']);
            break;

            case "update":
                $QBERecord["ID"] = isset($arguments["ID"]) ? $arguments["ID"] : "";

                $acquisitionTableController->unsetIfPresent($arguments, "CREATION_DATE");
                $acquisitionTableController->unsetIfPresent($arguments, "CREATED_BY_NAME");
                $acquisitionTableController->setLastUpdate($arguments);

                $acquisitionTableController->setIfMissing($arguments, 'ACQ_SOURCE');
                $acquisitionTableController->setIfMissing($arguments, 'ACQ_CUSTOMER');
                $acquisitionTableController->setIfMissing($arguments, 'ACQ_STORE');
                $arguments['ACQ_SOURCE'] = strtoupper($arguments['ACQ_SOURCE']);
                $arguments['ACQ_CUSTOMER'] = strtoupper($arguments['ACQ_CUSTOMER']);
                $arguments['ACQ_STORE'] = strtoupper($arguments['ACQ_STORE']);
            break;

            case "remove":
                $acquisitionTableController->setIfMissing($arguments, "ID");
                $acquisitionTableController->unsetIfPresent($arguments, "CREATION_DATE");
                $acquisitionTableController->unsetIfPresent($arguments, "CREATED_BY_NAME");
                $acquisitionTableController->unsetIfPresent($arguments, "LAST_UPDATE_DATE");
                $acquisitionTableController->unsetIfPresent($arguments, "LAST_UPDATED_BY_NAME");
                $acquisitionTableController->unsetIfPresent($arguments, 'ACQ_SOURCE');
                $acquisitionTableController->unsetIfPresent($arguments, 'ACQ_CUSTOMER');
                $acquisitionTableController->unsetIfPresent($arguments, 'ACQ_STORE');
            break;
        }

        $rval = $acquisitionTableController->doTableOperation($arguments, $QBERecord);
        return $rval;
    }

    function GetTableMetadata ($tablename) {
        $metadata = array();

        switch($tablename) {

            case "XXATDAR_SITE_ACQUISITIONS":
                $metadata = array(
                    "SCHEMA" => "APPS",
                    "TABLE_NAME" => "XXATDAR_SITE_ACQUISITIONS",
                    "TABLE_ALIAS" => "XXATDAR_SITE_ACQUISITIONS",
                    "COLUMNS" => array(
                        "ID" => array(
                            "BIND_VARIABLE_NAME" => ":ID",
                            "SET_BIND_VARIABLE_NAME" => ":ID_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_ACQUISITIONS.ID",
                            "SELECT_ALIAS" => "ID",
                            "SET_TEXT" => ":ID_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "APPS.XXATDAR_SITE_ACQUISITIONS_S",
                            "IS_PK" => TRUE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "SITE_ID" => array(
                            "BIND_VARIABLE_NAME" => ":SITE_ID",
                            "SET_BIND_VARIABLE_NAME" => ":SITE_ID_set",
                            "SELECT_TEXT" => "
                                (SELECT DISTINCT
                                    CAS.PARTY_SITE_ID
                                FROM
                                    APPS.HZ_CUST_ACCT_SITES CAS,
                                    APPS.HZ_CUST_SITE_USES CSU
                                WHERE
                                    CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID
                                    AND CSU.SITE_USE_CODE = 'SHIP_TO'
                                    AND CAS.SHIP_TO_FLAG IN ('P', 'Y')
                                    AND CAS.STATUS = 'A'
                                    AND CSU.STATUS = 'A'
                                    AND CSU.SITE_USE_ID = XXATDAR_SITE_ACQUISITIONS.SITE_USE_ID)",
                            "SELECT_ALIAS" => "SITE_ID",
                            "SET_TEXT" => "",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "ORG_ID" => array(
                            "BIND_VARIABLE_NAME" => ":ORG_ID",
                            "SET_BIND_VARIABLE_NAME" => ":ORG_ID_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_ACQUISITIONS.ORG_ID",
                            "SELECT_ALIAS" => "ORG_ID",
                            "SET_TEXT" => ":ORG_ID_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "SITE_USE_ID" => array(
                            "BIND_VARIABLE_NAME" => ":SITE_USE_ID",
                            "SET_BIND_VARIABLE_NAME" => ":SITE_USE_ID_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_ACQUISITIONS.SITE_USE_ID",
                            "SELECT_ALIAS" => "SITE_USE_ID",
                            "SET_TEXT" => ":SITE_USE_ID_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "ACQ_SOURCE" => array(
                            "BIND_VARIABLE_NAME" => ":ACQ_SOURCE",
                            "SET_BIND_VARIABLE_NAME" => ":ACQ_SOURCE_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_ACQUISITIONS.ACQ_SOURCE",
                            "SELECT_ALIAS" => "ACQ_SOURCE",
                            "SET_TEXT" => ":ACQ_SOURCE_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array('XXATDAR_SITE_ACQ_UNQ'),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "ACQ_CUSTOMER" => array(
                            "BIND_VARIABLE_NAME" => ":ACQ_CUSTOMER",
                            "SET_BIND_VARIABLE_NAME" => ":ACQ_CUSTOMER_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_ACQUISITIONS.ACQ_CUSTOMER",
                            "SELECT_ALIAS" => "ACQ_CUSTOMER",
                            "SET_TEXT" => ":ACQ_CUSTOMER_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array('XXATDAR_SITE_ACQ_UNQ'),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "ACQ_STORE" => array(
                            "BIND_VARIABLE_NAME" => ":ACQ_STORE",
                            "SET_BIND_VARIABLE_NAME" => ":ACQ_STORE_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_ACQUISITIONS.ACQ_STORE",
                            "SELECT_ALIAS" => "ACQ_STORE",
                            "SET_TEXT" => ":ACQ_STORE_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array('XXATDAR_SITE_ACQ_UNQ'),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CUSTOMER_SOURCE" => array(
                            "BIND_VARIABLE_NAME" => ":CUSTOMER_SOURCE",
                            "SET_BIND_VARIABLE_NAME" => ":CUSTOMER_SOUCRE_set",
                            "SELECT_TEXT" => "'LIVE'",
                            "SELECT_ALIAS" => "CUSTOMER_SOURCE",
                            "SET_TEXT" => "",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATION_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":creation_date",
                            "SET_BIND_VARIABLE_NAME" => ":creation_date_set",
                            "SELECT_TEXT" => "TO_CHAR(XXATDAR_SITE_ACQUISITIONS.CREATION_DATE, 'MM/DD/RRRR HH24:MI:SS')",
                            "SELECT_ALIAS" => "CREATION_DATE",
                            "SET_TEXT" => "TO_DATE(:creation_date_set, 'MM/DD/RRRR HH24:MI:SS')",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_DATE" => array("MM/DD/YYYY HH24:MI:SS"),
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATED_BY_NAME" => array(
                            "BIND_VARIABLE_NAME" => ":created_by_name",
                            "SET_BIND_VARIABLE_NAME" => ":created_by_name_set",
                            "SELECT_TEXT" => "(SELECT U.USER_NAME FROM APPS.FND_USER U WHERE U.USER_ID = XXATDAR_SITE_ACQUISITIONS.CREATED_BY)",
                            "SELECT_ALIAS" => "CREATED_BY_NAME",
                            "SET_TEXT" => "(SELECT U.USER_ID FROM APPS.FND_USER U WHERE U.USER_NAME = :created_by_name_set)",
                            "SET_ALIAS" => "CREATED_BY",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATED_BY" => array(
                            "BIND_VARIABLE_NAME" => ":created_by",
                            "SET_BIND_VARIABLE_NAME" => ":created_by_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_ACQUISITIONS.CREATED_BY",
                            "SELECT_ALIAS" => "CREATED_BY",
                            "SET_TEXT" => ":created_by_set",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array("IS_NUMERIC" => array()),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATE_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":last_update_date",
                            "SET_BIND_VARIABLE_NAME" => ":last_update_date_set",
                            "SELECT_TEXT" => "TO_CHAR(XXATDAR_SITE_ACQUISITIONS.LAST_UPDATE_DATE, 'MM/DD/RRRR HH24:MI:SS')",
                            "SELECT_ALIAS" => "LAST_UPDATE_DATE",
                            "SET_TEXT" => "TO_DATE(:last_update_date_set, 'MM/DD/RRRR HH24:MI:SS')",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "IS_DATE" => array("MM/DD/YYYY HH24:MI:SS")
                                ),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATED_BY_NAME" => array(
                            "BIND_VARIABLE_NAME" => ":last_updated_by_name",
                            "SET_BIND_VARIABLE_NAME" => ":last_updated_by_name_set",
                            "SELECT_TEXT" => "(SELECT U.USER_NAME FROM APPS.FND_USER U WHERE U.USER_ID = XXATDAR_SITE_ACQUISITIONS.LAST_UPDATED_BY)",
                            "SELECT_ALIAS" => "LAST_UPDATED_BY_NAME",
                            "SET_TEXT" => "(SELECT U.USER_ID FROM APPS.FND_USER U WHERE U.USER_NAME = :last_updated_by_name_set)",
                            "SET_ALIAS" => "LAST_UPDATED_BY",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATED_BY" => array(
                            "BIND_VARIABLE_NAME" => ":last_updated_by",
                            "SET_BIND_VARIABLE_NAME" => ":last_updated_by_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_ACQUISITIONS.LAST_UPDATED_BY",
                            "SELECT_ALIAS" => "LAST_UPDATED_BY",
                            "SET_TEXT" => ":last_updated_by_set",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "IS_NUMERIC" => array()
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                    ),
                    "FUNCTIONS" => array(
                        "add" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_SEQUENCE_FETCH_METHOD" => "",
                            "POST_SEQUENCE_FETCH_METHOD" => "",
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_UNIQUE_KEY_METHOD" => "",
                            "POST_UNIQUE_KEY_METHOD" => "",
                            "PRE_INSERT_METHOD" => "",
                            "POST_INSERT_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "update" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 2),
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_UNIQUE_KEY_METHOD" => "",
                            "POST_UNIQUE_KEY_METHOD" => "",
                            "PRE_UPDATE_METHOD" => "",
                            "POST_UPDATE_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "remove" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_DELETE_METHOD" => "",
                            "POST_DELETE_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "fetch" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_SELECT_METHOD" => "",
                            "POST_SELECT_METHOD" => ""
                        )
                    )
                );
            break;

            case "XXATDEAS_SITE_ACQUISITIONS":
                $metadata = array(
                    "SCHEMA" => "APPS",
                    "TABLE_NAME" => "XXATDEAS_SITE_ACQUISITIONS",
                    "TABLE_ALIAS" => "XXATDEAS_SITE_ACQUISITIONS",
                    "COLUMNS" => array(
                        "ID" => array(
                            "BIND_VARIABLE_NAME" => ":ID",
                            "SET_BIND_VARIABLE_NAME" => ":ID_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_ACQUISITIONS.ID",
                            "SELECT_ALIAS" => "ID",
                            "SET_TEXT" => ":ID_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "APPS.XXATDEAS_SITE_ACQUISITIONS_S",
                            "IS_PK" => TRUE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "SITE_ID" => array(
                            "BIND_VARIABLE_NAME" => ":SITE_ID",
                            "SET_BIND_VARIABLE_NAME" => ":SITE_ID_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_ACQUISITIONS.SITE_ID",
                            "SELECT_ALIAS" => "SITE_ID",
                            "SET_TEXT" => ":SITE_ID_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "ACQ_SOURCE" => array(
                            "BIND_VARIABLE_NAME" => ":ACQ_SOURCE",
                            "SET_BIND_VARIABLE_NAME" => ":ACQ_SOURCE_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_ACQUISITIONS.ACQ_SOURCE",
                            "SELECT_ALIAS" => "ACQ_SOURCE",
                            "SET_TEXT" => ":ACQ_SOURCE_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array('CMR_SITE_ACQ_UNQ'),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "ACQ_CUSTOMER" => array(
                            "BIND_VARIABLE_NAME" => ":ACQ_CUSTOMER",
                            "SET_BIND_VARIABLE_NAME" => ":ACQ_CUSTOMER_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_ACQUISITIONS.ACQ_CUSTOMER",
                            "SELECT_ALIAS" => "ACQ_CUSTOMER",
                            "SET_TEXT" => ":ACQ_CUSTOMER_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array('CMR_SITE_ACQ_UNQ'),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "ACQ_STORE" => array(
                            "BIND_VARIABLE_NAME" => ":ACQ_STORE",
                            "SET_BIND_VARIABLE_NAME" => ":ACQ_STORE_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_ACQUISITIONS.ACQ_STORE",
                            "SELECT_ALIAS" => "ACQ_STORE",
                            "SET_TEXT" => ":ACQ_STORE_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array('CMR_SITE_ACQ_UNQ'),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CUSTOMER_SOURCE" => array(
                            "BIND_VARIABLE_NAME" => ":CUSTOMER_SOURCE",
                            "SET_BIND_VARIABLE_NAME" => ":CUSTOMER_SOURCE_set",
                            "SELECT_TEXT" => "'STAGED'",
                            "SELECT_ALIAS" => "CUSTOMER_SOURCE",
                            "SET_TEXT" => "",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATION_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":creation_date",
                            "SET_BIND_VARIABLE_NAME" => ":creation_date_set",
                            "SELECT_TEXT" => "TO_CHAR(XXATDEAS_SITE_ACQUISITIONS.CREATION_DATE, 'MM/DD/RRRR HH24:MI:SS')",
                            "SELECT_ALIAS" => "CREATION_DATE",
                            "SET_TEXT" => "TO_DATE(:creation_date_set, 'MM/DD/RRRR HH24:MI:SS')",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_DATE" => array("MM/DD/YYYY HH24:MI:SS"),
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATED_BY_NAME" => array(
                            "BIND_VARIABLE_NAME" => ":created_by_name",
                            "SET_BIND_VARIABLE_NAME" => ":created_by_name_set",
                            "SELECT_TEXT" => "(SELECT U.USER_NAME FROM APPS.FND_USER U WHERE U.USER_ID = XXATDEAS_SITE_ACQUISITIONS.CREATED_BY)",
                            "SELECT_ALIAS" => "CREATED_BY_NAME",
                            "SET_TEXT" => "(SELECT U.USER_ID FROM APPS.FND_USER U WHERE U.USER_NAME = :created_by_name_set)",
                            "SET_ALIAS" => "CREATED_BY",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATED_BY" => array(
                            "BIND_VARIABLE_NAME" => ":created_by",
                            "SET_BIND_VARIABLE_NAME" => ":created_by_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_ACQUISITIONS.CREATED_BY",
                            "SELECT_ALIAS" => "CREATED_BY",
                            "SET_TEXT" => ":created_by_set",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array("IS_NUMERIC" => array()),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATE_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":last_update_date",
                            "SET_BIND_VARIABLE_NAME" => ":last_update_date_set",
                            "SELECT_TEXT" => "TO_CHAR(XXATDEAS_SITE_ACQUISITIONS.LAST_UPDATE_DATE, 'MM/DD/RRRR HH24:MI:SS')",
                            "SELECT_ALIAS" => "LAST_UPDATE_DATE",
                            "SET_TEXT" => "TO_DATE(:last_update_date_set, 'MM/DD/RRRR HH24:MI:SS')",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "IS_DATE" => array("MM/DD/YYYY HH24:MI:SS")
                                ),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATED_BY_NAME" => array(
                            "BIND_VARIABLE_NAME" => ":last_updated_by_name",
                            "SET_BIND_VARIABLE_NAME" => ":last_updated_by_name_set",
                            "SELECT_TEXT" => "(SELECT U.USER_NAME FROM APPS.FND_USER U WHERE U.USER_ID = XXATDEAS_SITE_ACQUISITIONS.LAST_UPDATED_BY)",
                            "SELECT_ALIAS" => "LAST_UPDATED_BY_NAME",
                            "SET_TEXT" => "(SELECT U.USER_ID FROM APPS.FND_USER U WHERE U.USER_NAME = :last_updated_by_name_set)",
                            "SET_ALIAS" => "LAST_UPDATED_BY",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATED_BY" => array(
                            "BIND_VARIABLE_NAME" => ":last_updated_by",
                            "SET_BIND_VARIABLE_NAME" => ":last_updated_by_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_ACQUISITIONS.LAST_UPDATED_BY",
                            "SELECT_ALIAS" => "LAST_UPDATED_BY",
                            "SET_TEXT" => ":last_updated_by_set",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "IS_NUMERIC" => array()
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                    ),
                    "FUNCTIONS" => array(
                        "add" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_SEQUENCE_FETCH_METHOD" => "",
                            "POST_SEQUENCE_FETCH_METHOD" => "",
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_UNIQUE_KEY_METHOD" => "",
                            "POST_UNIQUE_KEY_METHOD" => "",
                            "PRE_INSERT_METHOD" => "",
                            "POST_INSERT_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "update" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 2),
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_UNIQUE_KEY_METHOD" => "",
                            "POST_UNIQUE_KEY_METHOD" => "",
                            "PRE_UPDATE_METHOD" => "",
                            "POST_UPDATE_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "remove" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_DELETE_METHOD" => "",
                            "POST_DELETE_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "fetch" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_SELECT_METHOD" => "",
                            "POST_SELECT_METHOD" => ""
                        )
                    )
                );
            break;

            default:
                // do nothing
            break;
        }

        return $metadata;
    }
}

class SiteMarketingTableController extends PCLSmartClientTableController {

    public function doTableOperation($arguments) {
        $operationType = isset($arguments["OPERATION_TYPE"]) ? $arguments["OPERATION_TYPE"] : "";
        $QBERecord = array();

        switch($operationType) {
            case "add":
                $this->setCreation($arguments);
                $this->setLastUpdate($arguments);
                $this->formatDateArgument($arguments['START_DATE']);
                $this->formatDateArgument($arguments['END_DATE']);
            break;

            case "update":
                $QBERecord["ID"] = isset($arguments["ID"]) ? $arguments["ID"] : "";

                // missing or blank END_DATE means set to null
                $arguments['END_DATE'] = isset($arguments["END_DATE"]) ? trim($arguments["END_DATE"]) : '';

                if ($arguments['END_DATE']) {
                    $this->formatDateArgument($arguments['END_DATE']);
                }
                $this->formatDateArgument($arguments['START_DATE']);
                $this->unsetIfPresent($arguments, "CREATION_DATE");
                $this->unsetIfPresent($arguments, "CREATED_BY_NAME");

                $this->setLastUpdate($arguments);
            break;

            case "remove":
                $this->setIfMissing($arguments, "ID");

                $this->unsetIfPresent($arguments, "START_DATE");
                $this->unsetIfPresent($arguments, "END_DATE");
                $this->unsetIfPresent($arguments, "CREATION_DATE");
                $this->unsetIfPresent($arguments, "CREATED_BY_NAME");
                $this->unsetIfPresent($arguments, "LAST_UPDATE_DATE");
                $this->unsetIfPresent($arguments, "LAST_UPDATED_BY_NAME");
            break;

            default:
                // Do not alter the record for fetch, and let the main function
                // catch invalid operation types.
            break;
        }

        return parent::doTableOperation($arguments, $QBERecord);
    }
}

final class SiteMarketingManager extends ServerObject {
    private $PCLOracleConnection;

    public function __construct(&$controller) {
        parent::__construct($controller);
        oracle_apps_initialize($this->controller->session['user_id'], $this->controller->session['responsibility_id'], $this->controller->session['application_id'], $this->controller->session["profile_option_value"]);
        $this->PCLOracleConnection = new PCLOracle($GLOBALS['oracle_conn']);
    }

    public function siteMarketingProgram(&$arguments, $config) {

        $rval = array(
            'data' => array(),
            'status' => $this->STATUS_VALIDATION_ERROR,
            'totalRows' => 0,
            'startRow' => 0,
            'endRow' => 0
        );

        switch($arguments['CUSTOMER_SOURCE']) {
            case 'LIVE':

                if (!$arguments['SITE_USE_ID']){
                    $arguments['SITE_USE_ID'] = $this->convertSITEIDtoSITEUSEID($arguments['SITE_ID']);
                }

                if(!$arguments['SITE_USE_ID']){
                    $default = "Unable to identify SITE_USE_ID for customer.";
                    $rval['errorMessage'] = CMR_Utility::getL18NString('siteMarketingProgram.message.1', $default);
                    return $rval;
                }

                if (intval($arguments['SITE_ID']) == 0){
                    $arguments['SITE_ID'] = $this->convertSITEUSEIDtoSITEID($arguments['SITE_USE_ID']);
                }

                if (intval($arguments['SITE_ID']) == 0){
                    $default = "Unable to identify SITE_ID for customer.";
                    $rval['errorMessage'] = CMR_Utility::getL18NString('siteMarketingProgram.message.2', $default);
                    return $rval;
                }

                if ($arguments['OPERATION_TYPE'] != 'fetch'){
                    $this->logChangedValues($arguments);
                }

                // from this point on SITE_ID is not needed
                unset($arguments['SITE_ID']);

                if ($arguments['OPERATION_TYPE'] == 'remove' && $arguments['DELETED_FLAG'] == 'Y') {
                    $default = "This record has already been deleted.";
                    $rval['errorMessage'] = CMR_Utility::getL18NString('siteMarketingProgram.message.3', $default);
                    return $rval;
                }

                if ($arguments['OPERATION_TYPE'] == 'remove') {
                    // Call do_delete procedure
                    $sql = "BEGIN
                            APPS.XXATDAR_SITE_MKT_PROGRAM_PKG.do_delete (
                                  p_mkt_program_id      => :ID,
                                  p_deleted_by          => :DELETED_BY,
                                  p_last_update_date    => TO_DATE(:LAST_UPDATE_DATE, 'MM/DD/RRRR HH24:MI:SS')
                                  );
                            END;";

                    $bindVariables = array();
                    $bindVariables[] = array("PH_NAME" => ":DELETED_BY",        "VARIABLE" => $this->controller->session["user_id"]);
                    $bindVariables[] = array("PH_NAME" => ":LAST_UPDATE_DATE",  "VARIABLE" => $arguments['LAST_UPDATE_DATE']);
                    $bindVariables[] = array("PH_NAME" => ":ID",                "VARIABLE" => &$arguments['ID'],    "MAXLENGTH" => 20);

                    $executeResult = $this->PCLOracleConnection->execute($sql, $bindVariables);

                    // if procedure errors return error to client
                    if (!$executeResult["STATUS"] || $this->PCLOracleConnection->hasErrors()){
                        $rval['errorMessage'] = $this->PCLOracleConnection->hasErrors() ? implode("\n", $this->PCLOracleConnection->getErrors()) : "No error details!\n";

                        return $rval;
                    }

                    $rval['status'] = $this->STATUS_SUCCESS;

                    // return delete success message
                    return $rval;

                }else if ($arguments['OPERATION_TYPE'] == 'add' || $arguments['OPERATION_TYPE'] == 'update'){

                    if(!$arguments['START_DATE']){
                        $default = "Start Date is required.";
                        $rval['errorMessage'] = CMR_Utility::getL18NString('siteMarketingProgram.message.4', $default);
                        return $rval;
                    }

                    // for add/update operation need to check if date overlaps exist before saving record

                    $checkProgramDateOverlapResult = $this->programDateOverlapExists($arguments);
                    if (!$checkProgramDateOverlapResult['STATUS']){
                        // error occurred
                        $default = "Error occurred checking program overlap dates.";
                        $message = CMR_Utility::getL18NString('siteMarketingProgram.message.5', $default);
                        $rval['errorMessage'] = $checkProgramDateOverlapResult['RESULT'] ? $checkProgramDateOverlapResult['RESULT'] : $message;
                        return $rval;
                    }else if ($checkProgramDateOverlapResult['RESULT']){
                        // overlap exists
                        $default = "Cannot save changes: Program participation overlaps with an existing program participation period.";
                        $rval['errorMessage'] = CMR_Utility::getL18NString('siteMarketingProgram.message.6', $default);
                        return $rval;
                    }

                    $bindVariables = array();
                    $arguments['START_DATE'] = ($arguments['START_DATE']) ? date("m/d/Y", strtotime($arguments['START_DATE'])) : NULL;
                    $arguments['END_DATE'] = ($arguments['END_DATE']) ? date("m/d/Y", strtotime($arguments['END_DATE'])) : NULL;

                    if ($arguments['OPERATION_TYPE'] == 'add'){
                        // do_create procedure
                        $sql = "BEGIN
                                APPS.XXATDAR_SITE_MKT_PROGRAM_PKG.do_create (
                                      p_site_use_id         => :SITE_USE_ID,
                                      p_membership_type     => :MEMBERSHIP_TYPE,
                                      p_membership_number   => :MEMBERSHIP_NUMBER,
                                      p_start_date          => TO_DATE(:START_DATE, 'MM/DD/RRRR'),
                                      p_end_date            => TO_DATE(:END_DATE, 'MM/DD/RRRR'),
                                      p_created_by          => :CREATED_BY,
                                      p_mkt_program_id      => :ID
                                      );
                                END;";

                        $bindVariables[] = array("PH_NAME" => ":SITE_USE_ID",       "VARIABLE" => $arguments['SITE_USE_ID']);
                        $bindVariables[] = array("PH_NAME" => ":MEMBERSHIP_TYPE",   "VARIABLE" => $arguments['MEMBERSHIP_TYPE']);
                        $bindVariables[] = array("PH_NAME" => ":MEMBERSHIP_NUMBER", "VARIABLE" => $arguments['MEMBERSHIP_NUMBER']);
                        $bindVariables[] = array("PH_NAME" => ":START_DATE",        "VARIABLE" => $arguments['START_DATE']);
                        $bindVariables[] = array("PH_NAME" => ":END_DATE",          "VARIABLE" => $arguments['END_DATE']);
                        $bindVariables[] = array("PH_NAME" => ":CREATED_BY",        "VARIABLE" => $this->controller->session["user_id"]);
                        $bindVariables[] = array("PH_NAME" => ":ID",                "VARIABLE" => &$arguments['ID'],    "MAXLENGTH" => 20);

                    } else if ($arguments['OPERATION_TYPE'] == 'update'){
                        // do_modify procedure
                        $sql = "BEGIN
                                APPS.XXATDAR_SITE_MKT_PROGRAM_PKG.do_modify (
                                      p_id                  => :ID,
                                      p_site_use_id         => :SITE_USE_ID,
                                      p_membership_type     => :MEMBERSHIP_TYPE,
                                      p_membership_number   => :MEMBERSHIP_NUMBER,
                                      p_start_date          => TO_DATE(:START_DATE, 'MM/DD/RRRR'),
                                      p_end_date            => TO_DATE(:END_DATE, 'MM/DD/RRRR'),
                                      p_deleted_flag        => 'N',
                                      p_last_updated_by     => :LAST_UPDATE_BY,
                                      p_last_update_date    => TO_DATE(:LAST_UPDATE_DATE, 'MM/DD/RRRR HH24:MI:SS')
                                      );
                                END;";

                        $bindVariables[] = array("PH_NAME" => ":ID",                "VARIABLE" => &$arguments['ID'],    "MAXLENGTH" => 20);
                        $bindVariables[] = array("PH_NAME" => ":SITE_USE_ID",       "VARIABLE" => $arguments['SITE_USE_ID']);
                        $bindVariables[] = array("PH_NAME" => ":MEMBERSHIP_TYPE",   "VARIABLE" => $arguments['MEMBERSHIP_TYPE']);
                        $bindVariables[] = array("PH_NAME" => ":MEMBERSHIP_NUMBER", "VARIABLE" => $arguments['MEMBERSHIP_NUMBER']);
                        $bindVariables[] = array("PH_NAME" => ":START_DATE",        "VARIABLE" => $arguments['START_DATE']);
                        $bindVariables[] = array("PH_NAME" => ":END_DATE",          "VARIABLE" => $arguments['END_DATE']);
                        $bindVariables[] = array("PH_NAME" => ":LAST_UPDATE_BY",    "VARIABLE" => $this->controller->session["user_id"]);
                        $bindVariables[] = array("PH_NAME" => ":LAST_UPDATE_DATE",  "VARIABLE" => $arguments['LAST_UPDATE_DATE']);
                    }

                    $executeResult = $this->PCLOracleConnection->execute($sql, $bindVariables);

                    // if procedure errors return error to client
                    if (!$executeResult["STATUS"] || $this->PCLOracleConnection->hasErrors()){
                        $default = "No error details!";
                        $rval['errorMessage'] = $this->PCLOracleConnection->hasErrors() ? implode("\n", $this->PCLOracleConnection->getErrors()) : CMR_Utility::getL18NString('siteMarketingProgram.message.7', $default) . "\n";

                        return $rval;
                    }

                    unset($arguments['LAST_UPDATE_BY']);
                    unset($arguments['LAST_UPDATE_DATE']);

                    // change the OPERATION_TYPE to fetch-back the new/modified record
                    $arguments['OPERATION_TYPE'] = 'fetch';
                }

                //create the LIVE site marketing program PCLOracleTable object
                $siteMarketingProgramTableObject = new PCLOracleTable('SiteMarketing_LIVE', $this->GetTableMetadata('XXATDAR_SITE_MKT_PROGRAM'), $this->PCLOracleConnection);
            break;

            case 'STAGED':

                if ($arguments['OPERATION_TYPE'] != 'fetch'){
                    $this->logChangedValues($arguments);
                    set_review_status("SITE", $arguments["SITE_ID"], "MA", 0);
                }

                if ($arguments['OPERATION_TYPE'] == 'add' || $arguments['OPERATION_TYPE'] == 'update'){
                    // for add/update operation need to check if date overlaps exist before saving record

                    $checkProgramDateOverlapResult = $this->programDateOverlapExists($arguments);
                    if (!$checkProgramDateOverlapResult['STATUS']){
                        // error occurred
                        $default = "Error occurred checking program overlap dates.";
                        $message = CMR_Utility::getL18NString('siteMarketingProgram.message.5', $default);
                        $rval['errorMessage'] = $checkProgramDateOverlapResult['RESULT'] ? $checkProgramDateOverlapResult['RESULT'] : $message;
                        return $rval;
                    }else if ($checkProgramDateOverlapResult['RESULT']){
                        // overlap exists
                        $default = "Cannot save changes: Program participation overlaps with an existing program participation period.";
                        $rval['errorMessage'] = CMR_Utility::getL18NString('siteMarketingProgram.message.6', $default);
                        return $rval;
                    }
                }

                // create the STAGED site marketing program PCLOracleTable object
                $siteMarketingProgramTableObject = new PCLOracleTable('SiteMarketing_STAGED', $this->GetTableMetadata('XXATDEAS_SITE_MKT_PROGRAM'), $this->PCLOracleConnection);
            break;

            default:
                $default = "Unknown customer source.  Must be STAGED or LIVE.";
                $rval['errorMessage'] = CMR_Utility::getL18NString('siteMarketingProgram.message.8', $default);
                return $rval;
            break;
        }

        $siteMarketingProgramTableController = new SiteMarketingTableController($siteMarketingProgramTableObject, $this->controller);
        $rval = $siteMarketingProgramTableController->doTableOperation($arguments);

        return $rval;
    }

    private function programDateOverlapExists($arguments) {
        $rval = array (
                'STATUS' => 0,
                'RESULT' => NULL
                );

        switch($arguments['CUSTOMER_SOURCE']){
            case 'LIVE':
                $tableName = 'APPS.XXATDAR_SITE_MKT_PROGRAM';
                $siteColumn = 'SITE_USE_ID';
            break;

            case 'STAGED':
                $tableName = 'APPS.XXATDEAS_SITE_MKT_PROGRAM';
                $siteColumn = 'SITE_ID';
            break;

            default:
                $default = "Unknown Customer Source.";
                $rval['RESULT'] = CMR_Utility::getL18NString('programDateOverlapExists.message.1', $default);
                return $rval;
            break;
        }

        // Query checks for overlapping participation periods - returns 0 if no overlaps, 1 if overlaps exist.
        // Empty end dates are assumed to extend for 100 years so that we can do a straight date comparison to
        // a fixed date sometime in the future which greatly simplifies the query. Start/end dates are NOT allowed to
        // overlap - one participation can end on 12/31/2011 and a new one would start on the next day 1/1/2012
        $sql = "SELECT
                    DECODE(COUNT(ID),0,0,1) AS OVERLAP_EXISTS
                FROM
                    $tableName
                WHERE
                    DELETED_FLAG = 'N'
                    AND ID != NVL(:ID,-1)
                    AND $siteColumn = :SITE_ID
                    AND MEMBERSHIP_TYPE = :MEMBERSHIP_TYPE
                    AND TO_DATE(:START_DATE, 'MM/DD/RRRR') <= NVL(END_DATE, ADD_MONTHS(SYSDATE, 1200))
                    AND START_DATE <= NVL(TO_DATE(:END_DATE, 'MM/DD/RRRR'), ADD_MONTHS(SYSDATE, 1200))
                ";

        $bindVariables = array(
            array('PH_NAME' => ':ID', 'VARIABLE' => $arguments['ID']),
            array('PH_NAME' => ':SITE_ID', 'VARIABLE' => $arguments[$siteColumn]),
            array('PH_NAME' => ':MEMBERSHIP_TYPE', 'VARIABLE' => $arguments['MEMBERSHIP_TYPE']),
            array('PH_NAME' => ':START_DATE', 'VARIABLE' => $arguments['START_DATE']),
            array('PH_NAME' => ':END_DATE', 'VARIABLE' => $arguments['END_DATE'])
            );

        $executeResults = $this->PCLOracleConnection->execute($sql, $bindVariables);

        if(!$executeResults['STATUS'] || $this->PCLOracleConnection->hasErrors()) {
            $default = "Error executing programDateOverlapExists query.";
            $rval['RESULT'] = CMR_Utility::getL18NString('programDateOverlapExists.message.2', $default);
            return $rval;
        }

        $fetchResults = $this->PCLOracleConnection->fetchAllRecords();

        if(!$fetchResults['STATUS'] || $this->PCLOracleConnection->hasErrors()) {
            $default = "Error executing programDateOverlapExists query.";
            $rval['RESULT'] = CMR_Utility::getL18NString('programDateOverlapExists.message.2', $default);
            return $rval;
        }

        $rval['STATUS'] = 1;
        $rval['RESULT'] = $fetchResults['RESULT'][0]['OVERLAP_EXISTS'];

        return $rval;
    }

    private function convertSITEIDtoSITEUSEID($site_id) {

        $sql = "SELECT DISTINCT
                    CSU.SITE_USE_ID
                FROM
                    APPS.HZ_CUST_ACCT_SITES CAS,
                    APPS.HZ_CUST_SITE_USES CSU
                WHERE
                    CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID
                    AND CSU.SITE_USE_CODE = 'SHIP_TO'
                    AND CAS.SHIP_TO_FLAG IN ('P', 'Y')
                    AND CAS.STATUS = 'A'
                    AND CSU.STATUS = 'A'
                    AND CAS.PARTY_SITE_ID = :SITE_ID
                ";

        $bindVariables = array(
            array('PH_NAME' => ':SITE_ID', 'VARIABLE' => $site_id)
            );

        $executeResults = $this->PCLOracleConnection->execute($sql, $bindVariables);

        if(!$executeResults['STATUS'] || $this->PCLOracleConnection->hasErrors()) {
            return FALSE;
        }

        $fetchResults = $this->PCLOracleConnection->fetchAllRecords();

        if(!$fetchResults['STATUS'] || $this->PCLOracleConnection->hasErrors()) {
            return FALSE;
        }

        return $fetchResults['RESULT'][0]['SITE_USE_ID'];
    }

    private function convertSITEUSEIDtoSITEID($site_use_id) {

        $sql = "
            SELECT DISTINCT
                CAS.PARTY_SITE_ID
            FROM
                APPS.HZ_CUST_ACCT_SITES CAS,
                APPS.HZ_CUST_SITE_USES CSU
            WHERE
                CAS.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID
                AND CSU.SITE_USE_CODE = 'SHIP_TO'
                AND CAS.SHIP_TO_FLAG IN ('P', 'Y')
                AND CAS.STATUS = 'A'
                AND CSU.STATUS = 'A'
                AND CSU.SITE_USE_ID = :SITE_USE_ID
            ";

        $bindVariables = array(array('PH_NAME' => ':SITE_USE_ID', 'VARIABLE' => $site_use_id));

        $executeResults = $this->PCLOracleConnection->execute($sql, $bindVariables);

        if(!$executeResults['STATUS'] || $this->PCLOracleConnection->hasErrors()) {
            return FALSE;
        }

        $fetchResults = $this->PCLOracleConnection->fetchAllRecords();

        if(!$fetchResults['STATUS'] || $this->PCLOracleConnection->hasErrors()) {
            return FALSE;
        }

        return $fetchResults['RESULT'][0]['PARTY_SITE_ID'];
    }

    private function logChangedValues($arguments) {

        $old_vals = array();
        $new_vals = array();

        switch($arguments["OPERATION_TYPE"]){
            case "add":
                $new_vals = $arguments;
            break;

            case "update":
                $new_vals = $arguments;
                $old_vals = $this->getOldValues($arguments);
            break;

            case "remove":
                $old_vals = $this->getOldValues($arguments);
            break;

            default:
                // do nothing
            break;
        }

        // log changed values
        log_data_change(
            "SITE",
            $arguments['SITE_ID'],
            "shipToMATeamDForm",
            "SITE_MKT_PROGRAM",
            isset($arguments['ID']) ? $arguments['ID'] : 'NEW',
            $old_vals,
            $new_vals,
            $arguments["CUSTOMER_SOURCE"],
            $this->controller->session["username"]
        );
    }

    private function getOldValues($arguments) {

        switch($arguments['CUSTOMER_SOURCE']){
            case 'LIVE':
                $tableName = 'APPS.XXATDAR_SITE_MKT_PROGRAM';
            break;

            case 'STAGED':
                $tableName = 'APPS.XXATDEAS_SITE_MKT_PROGRAM';
            break;

            default:
                return FALSE;
            break;
        }

        $sql = "
            SELECT
              MEMBERSHIP_TYPE,
              MEMBERSHIP_NUMBER,
              TO_CHAR(START_DATE, 'MM/DD/RRRR') AS START_DATE,
              TO_CHAR(END_DATE, 'MM/DD/RRRR') AS END_DATE
            FROM
                $tableName
            WHERE
                ID = :ID
            ";

        $bindVariables = array(
            array('PH_NAME' => ':ID', 'VARIABLE' => $arguments['ID'])
            );

        $executeResults = $this->PCLOracleConnection->execute($sql, $bindVariables);

        if(!$executeResults['STATUS'] || $this->PCLOracleConnection->hasErrors()) {
            return FALSE;
        }

        $fetchResults = $this->PCLOracleConnection->fetchAllRecords();

        if(!$fetchResults['STATUS'] || $this->PCLOracleConnection->hasErrors()) {
            return FALSE;
        }

        return $fetchResults['RESULT'][0];
    }

    private function GetTableMetadata( $tablename ) {

        $metadata = array();

        switch($tablename) {

            case "XXATDAR_SITE_MKT_PROGRAM":
                $metadata = array(
                    "SCHEMA" => "APPS",
                    "TABLE_NAME" => "XXATDAR_SITE_MKT_PROGRAM",
                    "TABLE_ALIAS" => "XXATDAR_SITE_MKT_PROGRAM",
                    "COLUMNS" => array(
                        "ID" => array(
                            "BIND_VARIABLE_NAME" => ":ID",
                            "SET_BIND_VARIABLE_NAME" => ":ID_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_MKT_PROGRAM.ID",
                            "SELECT_ALIAS" => "ID",
                            "SET_TEXT" => ":ID_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "APPS.XXATDAR_SITE_MKT_PROGRAM_S",
                            "IS_PK" => TRUE,
                            "UK_LIST" => array('PK'),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "SITE_USE_ID" => array(
                            "BIND_VARIABLE_NAME" => ":SITE_USE_ID",
                            "SET_BIND_VARIABLE_NAME" => ":SITE_USE_ID_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_MKT_PROGRAM.SITE_USE_ID",
                            "SELECT_ALIAS" => "SITE_USE_ID",
                            "SET_TEXT" => ":SITE_USE_ID_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "MEMBERSHIP_TYPE" => array(
                            "BIND_VARIABLE_NAME" => ":MEMBERSHIP_TYPE",
                            "SET_BIND_VARIABLE_NAME" => ":MEMBERSHIP_TYPE_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_MKT_PROGRAM.MEMBERSHIP_TYPE",
                            "SELECT_ALIAS" => "MEMBERSHIP_TYPE",
                            "SET_TEXT" => ":MEMBERSHIP_TYPE_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "MEMBERSHIP_NUMBER" => array(
                            "BIND_VARIABLE_NAME" => ":MEMBERSHIP_NUMBER",
                            "SET_BIND_VARIABLE_NAME" => ":MEMBERSHIP_NUMBER_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_MKT_PROGRAM.MEMBERSHIP_NUMBER",
                            "SELECT_ALIAS" => "MEMBERSHIP_NUMBER",
                            "SET_TEXT" => ":MEMBERSHIP_NUMBER_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "DELETED_FLAG" => array(
                            "BIND_VARIABLE_NAME" => ":DELETED_FLAG",
                            "SET_BIND_VARIABLE_NAME" => ":DELETED_FLAG_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_MKT_PROGRAM.DELETED_FLAG",
                            "SELECT_ALIAS" => "DELETED_FLAG",
                            "SET_TEXT" => ":DELETED_FLAG_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "START_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":START_DATE",
                            "SET_BIND_VARIABLE_NAME" => ":START_DATE_set",
                            "SELECT_TEXT" => "TO_CHAR(XXATDAR_SITE_MKT_PROGRAM.START_DATE, 'MM/DD/RRRR')",
                            "SELECT_ALIAS" => "START_DATE",
                            "SET_TEXT" => "TO_DATE(:START_DATE_set, 'MM/DD/RRRR')",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array(),
                                    "IS_DATE" => array("MM/DD/YYYY")
                                ),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "END_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":END_DATE",
                            "SET_BIND_VARIABLE_NAME" => ":END_DATE_set",
                            "SELECT_TEXT" => "TO_CHAR(XXATDAR_SITE_MKT_PROGRAM.END_DATE, 'MM/DD/RRRR')",
                            "SELECT_ALIAS" => "END_DATE",
                            "SET_TEXT" => "TO_DATE(:END_DATE_set, 'MM/DD/RRRR')",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_DATE" => array("MM/DD/YYYY")
                                ),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATION_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":creation_date",
                            "SET_BIND_VARIABLE_NAME" => ":creation_date_set",
                            "SELECT_TEXT" => "TO_CHAR(XXATDAR_SITE_MKT_PROGRAM.CREATION_DATE, 'MM/DD/RRRR HH24:MI:SS')",
                            "SELECT_ALIAS" => "CREATION_DATE",
                            "SET_TEXT" => "TO_DATE(:creation_date_set, 'MM/DD/RRRR HH24:MI:SS')",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_DATE" => array("MM/DD/YYYY HH24:MI:SS"),
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATED_BY_NAME" => array(
                            "BIND_VARIABLE_NAME" => ":created_by_name",
                            "SET_BIND_VARIABLE_NAME" => ":created_by_name_set",
                            "SELECT_TEXT" => "(SELECT U.USER_NAME FROM APPS.FND_USER U WHERE U.USER_ID = XXATDAR_SITE_MKT_PROGRAM.CREATED_BY)",
                            "SELECT_ALIAS" => "CREATED_BY_NAME",
                            "SET_TEXT" => "(SELECT U.USER_ID FROM APPS.FND_USER U WHERE U.USER_NAME = :created_by_name_set)",
                            "SET_ALIAS" => "CREATED_BY",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATED_BY" => array(
                            "BIND_VARIABLE_NAME" => ":created_by",
                            "SET_BIND_VARIABLE_NAME" => ":created_by_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_MKT_PROGRAM.CREATED_BY",
                            "SELECT_ALIAS" => "CREATED_BY",
                            "SET_TEXT" => ":created_by_set",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array("IS_NUMERIC" => array()),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATE_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":last_update_date",
                            "SET_BIND_VARIABLE_NAME" => ":last_update_date_set",
                            "SELECT_TEXT" => "TO_CHAR(XXATDAR_SITE_MKT_PROGRAM.LAST_UPDATE_DATE, 'MM/DD/RRRR HH24:MI:SS')",
                            "SELECT_ALIAS" => "LAST_UPDATE_DATE",
                            "SET_TEXT" => "TO_DATE(:last_update_date_set, 'MM/DD/RRRR HH24:MI:SS')",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "IS_DATE" => array("MM/DD/YYYY HH24:MI:SS")
                                ),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATED_BY_NAME" => array(
                            "BIND_VARIABLE_NAME" => ":last_updated_by_name",
                            "SET_BIND_VARIABLE_NAME" => ":last_updated_by_name_set",
                            "SELECT_TEXT" => "(SELECT U.USER_NAME FROM APPS.FND_USER U WHERE U.USER_ID = XXATDAR_SITE_MKT_PROGRAM.LAST_UPDATED_BY)",
                            "SELECT_ALIAS" => "LAST_UPDATED_BY_NAME",
                            "SET_TEXT" => "(SELECT U.USER_ID FROM APPS.FND_USER U WHERE U.USER_NAME = :last_updated_by_name_set)",
                            "SET_ALIAS" => "LAST_UPDATED_BY",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATED_BY" => array(
                            "BIND_VARIABLE_NAME" => ":last_updated_by",
                            "SET_BIND_VARIABLE_NAME" => ":last_updated_by_set",
                            "SELECT_TEXT" => "XXATDAR_SITE_MKT_PROGRAM.LAST_UPDATED_BY",
                            "SELECT_ALIAS" => "LAST_UPDATED_BY",
                            "SET_TEXT" => ":last_updated_by_set",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "IS_NUMERIC" => array()
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CUSTOMER_SOURCE" => array(
                            "BIND_VARIABLE_NAME" => ":CUSTOMER_SOUCRE",
                            "SET_BIND_VARIABLE_NAME" => ":CUSTOMER_SOUCRE_set",
                            "SELECT_TEXT" => "'LIVE'",
                            "SELECT_ALIAS" => "CUSTOMER_SOURCE",
                            "SET_TEXT" => "",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "PROGRAM_TYPE" => array(
                            "BIND_VARIABLE_NAME" => ":PROGRAM_TYPE",
                            "SET_BIND_VARIABLE_NAME" => ":PROGRAM_TYPE_set",
                            "SELECT_TEXT" => "(SELECT
                                                  'TIREPROS'
                                               FROM
                                                  APPS.FND_FLEX_VALUES_VL
                                               WHERE
                                                  FLEX_VALUE =  XXATDAR_SITE_MKT_PROGRAM.MEMBERSHIP_TYPE
                                                  AND PARENT_FLEX_VALUE_LOW = 'TIREPROS MARKETING PROGRAM'
                                                  AND ENABLED_FLAG = 'Y'
                                                  AND NVL(START_DATE_ACTIVE, SYSDATE) <= SYSDATE
                                                  AND NVL(END_DATE_ACTIVE, SYSDATE) >= SYSDATE
                                                )",
                            "SELECT_ALIAS" => "PROGRAM_TYPE",
                            "SET_TEXT" => "",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                    ),
                    "FUNCTIONS" => array(
                        "add" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_SEQUENCE_FETCH_METHOD" => "",
                            "POST_SEQUENCE_FETCH_METHOD" => "",
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_UNIQUE_KEY_METHOD" => "",
                            "POST_UNIQUE_KEY_METHOD" => "",
                            "PRE_INSERT_METHOD" => "",
                            "POST_INSERT_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "update" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 2),
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_UNIQUE_KEY_METHOD" => "",
                            "POST_UNIQUE_KEY_METHOD" => "",
                            "PRE_UPDATE_METHOD" => "",
                            "POST_UPDATE_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "remove" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_DELETE_METHOD" => "",
                            "POST_DELETE_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "fetch" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_SELECT_METHOD" => "",
                            "POST_SELECT_METHOD" => ""
                        )
                    )
                );
            break;

            case "XXATDEAS_SITE_MKT_PROGRAM":
                $metadata = array(
                    "SCHEMA" => "APPS",
                    "TABLE_NAME" => "XXATDEAS_SITE_MKT_PROGRAM",
                    "TABLE_ALIAS" => "XXATDEAS_SITE_MKT_PROGRAM",
                    "COLUMNS" => array(
                        "ID" => array(
                            "BIND_VARIABLE_NAME" => ":ID",
                            "SET_BIND_VARIABLE_NAME" => ":ID_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_MKT_PROGRAM.ID",
                            "SELECT_ALIAS" => "ID",
                            "SET_TEXT" => ":ID_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "APPS.XXATDEAS_SITE_MKT_PROGRAM_S",
                            "IS_PK" => TRUE,
                            "UK_LIST" => array('PRIMARY_KEY'),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "SITE_ID" => array(
                            "BIND_VARIABLE_NAME" => ":SITE_ID",
                            "SET_BIND_VARIABLE_NAME" => ":SITE_ID_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_MKT_PROGRAM.SITE_ID",
                            "SELECT_ALIAS" => "SITE_ID",
                            "SET_TEXT" => ":SITE_ID_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "MEMBERSHIP_TYPE" => array(
                            "BIND_VARIABLE_NAME" => ":MEMBERSHIP_TYPE",
                            "SET_BIND_VARIABLE_NAME" => ":MEMBERSHIP_TYPE_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_MKT_PROGRAM.MEMBERSHIP_TYPE",
                            "SELECT_ALIAS" => "MEMBERSHIP_TYPE",
                            "SET_TEXT" => ":MEMBERSHIP_TYPE_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "MEMBERSHIP_NUMBER" => array(
                            "BIND_VARIABLE_NAME" => ":MEMBERSHIP_NUMBER",
                            "SET_BIND_VARIABLE_NAME" => ":MEMBERSHIP_NUMBER_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_MKT_PROGRAM.MEMBERSHIP_NUMBER",
                            "SELECT_ALIAS" => "MEMBERSHIP_NUMBER",
                            "SET_TEXT" => ":MEMBERSHIP_NUMBER_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "DELETED_FLAG" => array(
                            "BIND_VARIABLE_NAME" => ":DELETED_FLAG",
                            "SET_BIND_VARIABLE_NAME" => ":DELETED_FLAG_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_MKT_PROGRAM.DELETED_FLAG",
                            "SELECT_ALIAS" => "DELETED_FLAG",
                            "SET_TEXT" => ":DELETED_FLAG_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "START_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":START_DATE",
                            "SET_BIND_VARIABLE_NAME" => ":START_DATE_set",
                            "SELECT_TEXT" => "TO_CHAR(XXATDEAS_SITE_MKT_PROGRAM.START_DATE, 'MM/DD/RRRR')",
                            "SELECT_ALIAS" => "START_DATE",
                            "SET_TEXT" => "TO_DATE(:START_DATE_set, 'MM/DD/RRRR')",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_REQUIRED" => array(),
                                    "IS_DATE" => array("MM/DD/YYYY")
                                ),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "END_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":END_DATE",
                            "SET_BIND_VARIABLE_NAME" => ":END_DATE_set",
                            "SELECT_TEXT" => "TO_CHAR(XXATDEAS_SITE_MKT_PROGRAM.END_DATE, 'MM/DD/RRRR')",
                            "SELECT_ALIAS" => "END_DATE",
                            "SET_TEXT" => "TO_DATE(:END_DATE_set, 'MM/DD/RRRR')",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_DATE" => array("MM/DD/YYYY")
                                ),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATION_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":creation_date",
                            "SET_BIND_VARIABLE_NAME" => ":creation_date_set",
                            "SELECT_TEXT" => "TO_CHAR(XXATDEAS_SITE_MKT_PROGRAM.CREATION_DATE, 'MM/DD/RRRR HH24:MI:SS')",
                            "SELECT_ALIAS" => "CREATION_DATE",
                            "SET_TEXT" => "TO_DATE(:creation_date_set, 'MM/DD/RRRR HH24:MI:SS')",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(
                                    "IS_DATE" => array("MM/DD/YYYY HH24:MI:SS"),
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATED_BY_NAME" => array(
                            "BIND_VARIABLE_NAME" => ":created_by_name",
                            "SET_BIND_VARIABLE_NAME" => ":created_by_name_set",
                            "SELECT_TEXT" => "(SELECT U.USER_NAME FROM APPS.FND_USER U WHERE U.USER_ID = XXATDEAS_SITE_MKT_PROGRAM.CREATED_BY)",
                            "SELECT_ALIAS" => "CREATED_BY_NAME",
                            "SET_TEXT" => "(SELECT U.USER_ID FROM APPS.FND_USER U WHERE U.USER_NAME = :created_by_name_set)",
                            "SET_ALIAS" => "CREATED_BY",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATED_BY" => array(
                            "BIND_VARIABLE_NAME" => ":created_by",
                            "SET_BIND_VARIABLE_NAME" => ":created_by_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_MKT_PROGRAM.CREATED_BY",
                            "SELECT_ALIAS" => "CREATED_BY",
                            "SET_TEXT" => ":created_by_set",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array("IS_NUMERIC" => array()),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATE_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":last_update_date",
                            "SET_BIND_VARIABLE_NAME" => ":last_update_date_set",
                            "SELECT_TEXT" => "TO_CHAR(XXATDEAS_SITE_MKT_PROGRAM.LAST_UPDATE_DATE, 'MM/DD/RRRR HH24:MI:SS')",
                            "SELECT_ALIAS" => "LAST_UPDATE_DATE",
                            "SET_TEXT" => "TO_DATE(:last_update_date_set, 'MM/DD/RRRR HH24:MI:SS')",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "IS_DATE" => array("MM/DD/YYYY HH24:MI:SS")
                                ),
                                "add" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "update" => array(
                                    "IS_REQUIRED" => array()
                                ),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATED_BY_NAME" => array(
                            "BIND_VARIABLE_NAME" => ":last_updated_by_name",
                            "SET_BIND_VARIABLE_NAME" => ":last_updated_by_name_set",
                            "SELECT_TEXT" => "(SELECT U.USER_NAME FROM APPS.FND_USER U WHERE U.USER_ID = XXATDEAS_SITE_MKT_PROGRAM.LAST_UPDATED_BY)",
                            "SELECT_ALIAS" => "LAST_UPDATED_BY_NAME",
                            "SET_TEXT" => "(SELECT U.USER_ID FROM APPS.FND_USER U WHERE U.USER_NAME = :last_updated_by_name_set)",
                            "SET_ALIAS" => "LAST_UPDATED_BY",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATED_BY" => array(
                            "BIND_VARIABLE_NAME" => ":last_updated_by",
                            "SET_BIND_VARIABLE_NAME" => ":last_updated_by_set",
                            "SELECT_TEXT" => "XXATDEAS_SITE_MKT_PROGRAM.LAST_UPDATED_BY",
                            "SELECT_ALIAS" => "LAST_UPDATED_BY",
                            "SET_TEXT" => ":last_updated_by_set",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "IS_NUMERIC" => array()
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CUSTOMER_SOURCE" => array(
                            "BIND_VARIABLE_NAME" => ":CUSTOMER_SOUCRE",
                            "SET_BIND_VARIABLE_NAME" => ":CUSTOMER_SOUCRE_set",
                            "SELECT_TEXT" => "'STAGED'",
                            "SELECT_ALIAS" => "CUSTOMER_SOURCE",
                            "SET_TEXT" => "",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "PROGRAM_TYPE" => array(
                            "BIND_VARIABLE_NAME" => ":PROGRAM_TYPE",
                            "SET_BIND_VARIABLE_NAME" => ":PROGRAM_TYPE_set",
                            "SELECT_TEXT" => "(SELECT
                                                  'TIREPROS'
                                               FROM
                                                  APPS.FND_FLEX_VALUES_VL
                                               WHERE
                                                  FLEX_VALUE =  XXATDEAS_SITE_MKT_PROGRAM.MEMBERSHIP_TYPE
                                                  AND PARENT_FLEX_VALUE_LOW = 'TIREPROS MARKETING PROGRAM'
                                                  AND ENABLED_FLAG = 'Y'
                                                  AND NVL(START_DATE_ACTIVE, SYSDATE) <= SYSDATE
                                                  AND NVL(END_DATE_ACTIVE, SYSDATE) >= SYSDATE
                                                )",
                            "SELECT_ALIAS" => "PROGRAM_TYPE",
                            "SET_TEXT" => "",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(
                                    "MAX_LENGTH" => array(100)
                                ),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                    ),
                    "FUNCTIONS" => array(
                        "add" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_SEQUENCE_FETCH_METHOD" => "",
                            "POST_SEQUENCE_FETCH_METHOD" => "",
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_UNIQUE_KEY_METHOD" => "",
                            "POST_UNIQUE_KEY_METHOD" => "",
                            "PRE_INSERT_METHOD" => "",
                            "POST_INSERT_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "update" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 2),
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_UNIQUE_KEY_METHOD" => "",
                            "POST_UNIQUE_KEY_METHOD" => "",
                            "PRE_UPDATE_METHOD" => "",
                            "POST_UPDATE_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "remove" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_DELETE_METHOD" => "",
                            "POST_DELETE_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "fetch" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_SELECT_METHOD" => "",
                            "POST_SELECT_METHOD" => ""
                        )
                    )
                );
            break;

            default:
                // do nothing
            break;
        }

        return $metadata;
    }
}

class CreditCardTableController extends PCLSmartClientTableController {
    public function setLastUpdate(&$arguments) {
        $arguments["LAST_UPDATE_DATE"] = date("Y-m-d H:i:s");
        $arguments["LAST_UPDATED_BY"] = strlen($this->controller->session["user_id"]) > 0 ? $this->controller->session["user_id"] : NULL;
    }

    public function doTableOperation($arguments) {
        $operationType = isset($arguments["OPERATION_TYPE"]) ? $arguments["OPERATION_TYPE"] : "";
        $QBERecord = array();

        switch($operationType) {

            case "update":
                // This is a normal update.
                $QBERecord["ID"] = isset($arguments["ID"]) ? $arguments["ID"] : "";
                $this->setIfMissing($arguments, "CC_NUM");
                $this->setIfMissing($arguments, "CC_TYPE");
                $this->setIfMissing($arguments, "CC_MONTH");
                $this->setIfMissing($arguments, "CC_YEAR");
                $this->setIfMissing($arguments, "PRIMARY_CARD");
                $temp_args['UPLOAD_STATUS'] = 'C';
                $this->unsetIfPresent($arguments, "CREATED_BY");
                $this->unsetIfPresent($arguments, "CREATION_DATE");
                $this->setLastUpdate($arguments);
            break;

            case "reset":
                // This updates all other PRIMARY_CARD records to N.
                $this->unsetIfPresent($arguments, "ID");
                $this->unsetIfPresent($arguments, "PARTY_SITE_ID");
                $this->unsetIfPresent($arguments, "CC_NUM");
                $this->unsetIfPresent($arguments, "CC_TYPE");
                $this->unsetIfPresent($arguments, "CC_MONTH");
                $this->unsetIfPresent($arguments, "CC_YEAR");
                $this->unsetIfPresent($arguments, "CREATED_BY");
                $this->unsetIfPresent($arguments, "CREATION_DATE");
                $QBERecord["LOCATION_NUMBER"] = $arguments["LOCATION_NUMBER"];
                $QBERecord["UPLOAD_STATUS"] = 'C';
                $arguments['OPERATION_TYPE'] = 'update';
                $this->setLastUpdate($arguments);
            break;

            default:
                // Do not alter the record for fetch, and let the main function
                // catch invalid operation types.
            break;
        }

        return parent::doTableOperation($arguments, $QBERecord);
    }
}

final class CreditCardManager extends ServerObject {
    private $Conns;

    public function __construct(&$controller) {
        parent::__construct($controller);

        $this->Conns = array(
            'CREDENTIALS_INI' => $this->controller->servers_ini_file[$this->controller->server]['credentials'],
            'SERVER' => $this->controller->servers_ini_file[$this->controller->server]['mysql_cluster_server'],
            'USER' => $this->controller->servers_ini_file[$this->controller->server]['mysql_cluster_user']
        );
    }

    public function CreditCard(&$arguments, $config) {
        // Is the current data PRIMARY_CARD value a Yes/1.
        //
        if($arguments['OPERATION_TYPE'] == 'update' && $arguments['PRIMARY_CARD'] == 1){
            // Update all cards to no for this site.
            //
            $retval = $this->updateOtherCardsToNo($arguments);
            if(!$retval['STATUS']){
                //$this->parent->addError("Problem updating other cards.");
            }
        }

        $PCLMySQLTable = new PCLMySQLTable("CreditCard", $this->GetTableMetadata('CUSTOMER_CREDIT_CARD'), $this->Conns);
        $TableController = new CreditCardTableController($PCLMySQLTable, $this->controller);
        return $TableController->doTableOperation($arguments);
    }

    private function updateOtherCardsToNo($arguments) {
        $temp_args['OPERATION_TYPE'] = 'reset';
        $temp_args['UPLOAD_STATUS'] = 'C';
        $temp_args['PRIMARY_CARD'] = '0';
        $temp_args['LOCATION_NUMBER'] = $arguments['LOCATION_NUMBER'];
        $PCLMySQLTable = new PCLMySQLTable("CreditCard", $this->GetTableMetadata('CUSTOMER_CREDIT_CARD'), $this->Conns);
        $TableController = new CreditCardTableController($PCLMySQLTable, $this->controller);
        $query = $TableController->doTableOperation($temp_args);
        return $query;
    }

    private function GetTableMetadata($itemType) {
        $metadata = array();

        switch($itemType) {

            case 'CUSTOMER_CREDIT_CARD':

                $metadata = array(
                    "SCHEMA" => "CUSTOMER_DATA",
                    "TABLE_NAME" => "CUSTOMER_CREDIT_CARD",
                    "TABLE_ALIAS" => "CCC",
                    "SELECT_SQL" => "
                    SELECT
                        C.ID,
                        S.PARTY_SITE_ID,
                        C.LOCATION_NUMBER,
                        C.CC_NUM,
                        C.CC_TYPE,
                        C.CC_MONTH,
                        C.CC_YEAR,
                        C.PRIMARY_CARD,
                        C.UPLOAD_STATUS,
                        C.CREATION_DATE,
                        C.CREATED_BY,
                        C.LAST_UPDATED_BY,
                        C.LAST_UPDATE_DATE
                    FROM CUSTOMER_DATA.CUSTOMER_CREDIT_CARD C
                    INNER JOIN ORACLE_EXPORTS.SHIP_TO S
                        ON C.LOCATION_NUMBER = S.LOCATION_NUMBER
                        AND C.UPLOAD_STATUS <> 'D'
                    ",
                    "COLUMNS" => array(
                        "ID" => array(
                            "BIND_VARIABLE_NAME" => ":ID",
                            "SET_BIND_VARIABLE_NAME" => ":ID_set",
                            "SELECT_TEXT" => "ID",
                            "SELECT_ALIAS" => "ID",
                            "SET_TEXT" => ":ID_set",
                            "SET_ALIAS" => "ID",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => TRUE,
                            "UK_LIST" => array('asdf'),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "PARTY_SITE_ID" => array(
                            "BIND_VARIABLE_NAME" => ":PARTY_SITE_ID",
                            "SET_BIND_VARIABLE_NAME" => ":PARTY_SITE_ID_set",
                            "SELECT_TEXT" => "PARTY_SITE_ID",
                            "SELECT_ALIAS" => "PARTY_SITE_ID",
                            "SET_TEXT" => "",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "LOCATION_NUMBER" => array(
                            "BIND_VARIABLE_NAME" => ":LOCATION_NUMBER",
                            "SET_BIND_VARIABLE_NAME" => ":LOCATION_NUMBER_set",
                            "SELECT_TEXT" => "LOCATION_NUMBER",
                            "SELECT_ALIAS" => "LOCATION_NUMBER",
                            "SET_TEXT" => "",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CC_NUM" => array(
                            "BIND_VARIABLE_NAME" => ":CC_NUM",
                            "SET_BIND_VARIABLE_NAME" => ":CC_NUM_set",
                            "SELECT_TEXT" => "CC_NUM",
                            "SELECT_ALIAS" => "CC_NUM",
                            "SET_TEXT" => ":CC_NUM_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CC_TYPE" => array(
                            "BIND_VARIABLE_NAME" => ":CC_TYPE",
                            "SET_BIND_VARIABLE_NAME" => ":CC_TYPE_set",
                            "SELECT_TEXT" => "CC_TYPE",
                            "SELECT_ALIAS" => "CC_TYPE",
                            "SET_TEXT" => ":CC_TYPE_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CC_MONTH" => array(
                            "BIND_VARIABLE_NAME" => ":CC_MONTH",
                            "SET_BIND_VARIABLE_NAME" => ":CC_MONTH_set",
                            "SELECT_TEXT" => "CC_MONTH",
                            "SELECT_ALIAS" => "CC_MONTH",
                            "SET_TEXT" => ":CC_MONTH_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CC_YEAR" => array(
                            "BIND_VARIABLE_NAME" => ":CC_YEAR",
                            "SET_BIND_VARIABLE_NAME" => ":CC_YEAR_set",
                            "SELECT_TEXT" => "CC_YEAR",
                            "SELECT_ALIAS" => "CC_YEAR",
                            "SET_TEXT" => ":CC_YEAR_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "PRIMARY_CARD" => array(
                            "BIND_VARIABLE_NAME" => ":PRIMARY_CARD",
                            "SET_BIND_VARIABLE_NAME" => ":PRIMARY_CARD_set",
                            "SELECT_TEXT" => "PRIMARY_CARD",
                            "SELECT_ALIAS" => "PRIMARY_CARD",
                            "SET_TEXT" => ":PRIMARY_CARD_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "UPLOAD_STATUS" => array(
                            "BIND_VARIABLE_NAME" => ":UPLOAD_STATUS",
                            "SET_BIND_VARIABLE_NAME" => ":UPLOAD_STATUS_set",
                            "SELECT_TEXT" => "UPLOAD_STATUS",
                            "SELECT_ALIAS" => "UPLOAD_STATUS",
                            "SET_TEXT" => ":UPLOAD_STATUS_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "CREATION_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":creation_date",
                            "SET_BIND_VARIABLE_NAME" => ":creation_date_set",
                            "SELECT_TEXT" => "CREATION_DATE",
                            "SELECT_ALIAS" => "CREATION_DATE",
                            "SET_TEXT" => "",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )

                        ),
                        "CREATED_BY" => array(
                            "BIND_VARIABLE_NAME" => ":created_by",
                            "SET_BIND_VARIABLE_NAME" => ":created_by_set",
                            "SELECT_TEXT" => "CREATED_BY",
                            "SELECT_ALIAS" => "CREATED_BY",
                            "SET_TEXT" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATE_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":last_update_date",
                            "SET_BIND_VARIABLE_NAME" => ":last_update_date_set",
                            "SELECT_TEXT" => "LAST_UPDATE_DATE",
                            "SELECT_ALIAS" => "LAST_UPDATE_DATE",
                            "SET_TEXT" => ":last_update_date_set",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )

                        ),
                        "LAST_UPDATED_BY" => array(
                            "BIND_VARIABLE_NAME" => ":last_updated_by",
                            "SET_BIND_VARIABLE_NAME" => ":last_updated_by_set",
                            "SELECT_TEXT" => "LAST_UPDATED_BY",
                            "SELECT_ALIAS" => "LAST_UPDATED_BY",
                            "SET_TEXT" => ":last_updated_by_set",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        )
                    ),
                    "FUNCTIONS" => array(
                        "add" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_SEQUENCE_FETCH_METHOD" => "",
                            "POST_SEQUENCE_FETCH_METHOD" => "",
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_UNIQUE_KEY_METHOD" => "",
                            "POST_UNIQUE_KEY_METHOD" => "",
                            "PRE_INSERT_METHOD" => "",
                            "POST_INSERT_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "update" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 2),
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_UNIQUE_KEY_METHOD" => "",
                            "POST_UNIQUE_KEY_METHOD" => "",
                            "PRE_UPDATE_METHOD" => "",
                            "POST_UPDATE_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "remove" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_DELETE_METHOD" => "",
                            "POST_DELETE_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "fetch" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_SELECT_METHOD" => "",
                            "POST_SELECT_METHOD" => ""
                        )
                    )
                );

            break;

            case 'OTHER_CARDS':

                $metadata = array(
                    "SCHEMA" => "CUSTOMER_DATA",
                    "TABLE_NAME" => "CUSTOMER_CREDIT_CARD",
                    "TABLE_ALIAS" => "CCC",
                    "COLUMNS" => array(
                        "LOCATION_NUMBER" => array(
                            "BIND_VARIABLE_NAME" => ":LOCATION_NUMBER",
                            "SET_BIND_VARIABLE_NAME" => ":LOCATION_NUMBER_set",
                            "SELECT_TEXT" => "LOCATION_NUMBER",
                            "SELECT_ALIAS" => "LOCATION_NUMBER",
                            "SET_TEXT" => "",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "PRIMARY_CARD" => array(
                            "BIND_VARIABLE_NAME" => ":PRIMARY_CARD",
                            "SET_BIND_VARIABLE_NAME" => ":PRIMARY_CARD_set",
                            "SELECT_TEXT" => "PRIMARY_CARD",
                            "SELECT_ALIAS" => "PRIMARY_CARD",
                            "SET_TEXT" => ":PRIMARY_CARD_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "UPLOAD_STATUS" => array(
                            "BIND_VARIABLE_NAME" => ":UPLOAD_STATUS",
                            "SET_BIND_VARIABLE_NAME" => ":UPLOAD_STATUS_set",
                            "SELECT_TEXT" => "UPLOAD_STATUS",
                            "SELECT_ALIAS" => "UPLOAD_STATUS",
                            "SET_TEXT" => ":UPLOAD_STATUS_set",
                            "SET_ALIAS" => "",
                            "MATCH_TYPE" => "MATCHES",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        ),
                        "LAST_UPDATE_DATE" => array(
                            "BIND_VARIABLE_NAME" => ":last_update_date",
                            "SET_BIND_VARIABLE_NAME" => ":last_update_date_set",
                            "SELECT_TEXT" => "LAST_UPDATE_DATE",
                            "SELECT_ALIAS" => "LAST_UPDATE_DATE",
                            "SET_TEXT" => ":last_update_date_set",
                            "MATCH_TYPE" => "CONTAINS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )

                        ),
                        "LAST_UPDATED_BY" => array(
                            "BIND_VARIABLE_NAME" => ":last_updated_by",
                            "SET_BIND_VARIABLE_NAME" => ":last_updated_by_set",
                            "SELECT_TEXT" => "LAST_UPDATED_BY",
                            "SELECT_ALIAS" => "LAST_UPDATED_BY",
                            "SET_TEXT" => ":last_updated_by_set",
                            "MATCH_TYPE" => "EQUALS",
                            "SEQUENCE" => "",
                            "IS_PK" => FALSE,
                            "UK_LIST" => array(),
                            "VALIDATORS" => array(
                                "all" => array(),
                                "add" => array(),
                                "update" => array(),
                                "remove" => array()
                            )
                        )
                    ),
                    "FUNCTIONS" => array(
                        "add" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_SEQUENCE_FETCH_METHOD" => "",
                            "POST_SEQUENCE_FETCH_METHOD" => "",
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_UNIQUE_KEY_METHOD" => "",
                            "POST_UNIQUE_KEY_METHOD" => "",
                            "PRE_INSERT_METHOD" => "",
                            "POST_INSERT_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "update" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 2),
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_UNIQUE_KEY_METHOD" => "",
                            "POST_UNIQUE_KEY_METHOD" => "",
                            "PRE_UPDATE_METHOD" => "",
                            "POST_UPDATE_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "remove" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_VALIDATION_METHOD" => "",
                            "POST_VALIDATION_METHOD" => "",
                            "PRE_DELETE_METHOD" => "",
                            "POST_DELETE_METHOD" => "",
                            "PRE_FETCHBACK_METHOD" => "",
                            "POST_FETCHBACK_METHOD" => ""
                        ),
                        "fetch" => array(
                            "NAME" => "",
                            "PARAMETER_COUNT" => array("MIN" => 1, "MAX" => 1),
                            "PRE_SELECT_METHOD" => "",
                            "POST_SELECT_METHOD" => ""
                        )
                    )
                );

            break;

            default:

                // Comment required for empty DEFAULT case.
            break;

        }

        return $metadata;
    }
}
?>
