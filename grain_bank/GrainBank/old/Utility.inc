<?php
//**************************************************************************
//
// File        :  Utility.inc
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
class Utility extends ServerObject {

    public function pm_log_data_change(
            $class,
            $class_key,
            $form_name,
            $entity,
            $entity_key,
            $old_values,
            $new_values,
            $source,
            $do_not_log_list_in=array()) {

        $do_not_log_list_default = array("RN", "SORT_BY", "SESSION_ID");
        $do_not_log_list_full = array_merge($do_not_log_list_default, $do_not_log_list_in);

        if( strlen($class) == 0 ||
            strlen($class_key) == 0 ||
            strlen($entity) == 0 ||
            strlen($entity_key) == 0 ||
            strlen($form_name) == 0 ) {

            eventLog(
                "PM",
                "Warn",
                "DataLogError",
                $this->controller->session["username"],
                "Missing required attribute to update change log in pm_log_data_change()",
                "class: '$class'\n" .
                "class_key: '$class_key'\n" .
                "form_name: '$form_name'\n" .
                "field_name: '$field_name'\n" .
                "entity: '$entity'\n" .
                "entity_key: '$entity_key'\n" .
                "old_values:\n" . print_r($old_values, TRUE) . "\n" .
                "new_values:\n" . print_r($new_values, TRUE) . "\n" .
                "source: '$source'\n" .
                "POST:\n" . print_r($_POST, TRUE)
            );
        }

        // If this is an addition type record, create a blank template for $old_values
        //
        if(strlen($old_values[0]) == 0){
            $old_values[0] = $new_values;

            foreach($old_values[0] as &$value){
                $value = "";
            }
        }

        foreach($old_values[0] as $key => $value){

            if((in_array($key, $do_not_log_list_full) && $source != "remove") || stripos($key, "_selection_") !== FALSE){
                continue;
            }

            // The remove case is added here becase in the case of
            // a removal we want to log everything no matter what.
            //
            if($new_values[$key] != $old_values[$key] || $source == "remove"){

                $sql = "
                INSERT INTO
                APPS.XXATDEAS_PM_CHANGE_LINES(
                ID,
                CLASS_NAME,
                CLASS_KEY,
                ENTITY_NAME,
                ENTITY_KEY,
                FORM_NAME,
                FIELD_NAME,
                OLD_VALUE,
                NEW_VALUE,
                CHANGED_BY,
                CHANGED_ON,
                SOURCE
                ) VALUES (
                APPS.XXATDEAS_PM_CHANGE_LINES_S.NEXTVAL,
                :class,
                :class_key,
                :entity,
                :entity_key,
                :form_name,
                :key,
                :value,
                :new_value,
                :changed_by,
                SYSDATE,
                :source
                )";

                $bindVars = array(
                    array("PH_NAME" => ":class", "VARIABLE" =>      $class),
                    array("PH_NAME" => ":class_key", "VARIABLE" =>  $class_key),
                    array("PH_NAME" => ":entity", "VARIABLE" =>     $entity),
                    array("PH_NAME" => ":entity_key", "VARIABLE" => $entity_key),
                    array("PH_NAME" => ":form_name", "VARIABLE" =>  $form_name),
                    array("PH_NAME" => ":key", "VARIABLE" =>        $key),
                    array("PH_NAME" => ":value", "VARIABLE" =>      substr($old_values[$key], 0, 2000)),
                    array("PH_NAME" => ":new_value", "VARIABLE" =>  substr($new_values[$key], 0, 2000)),
                    array("PH_NAME" => ":changed_by", "VARIABLE" => $this->controller->session["username"]),
                    array("PH_NAME" => ":source", "VARIABLE" =>     $source)
                );

                $result = oracle_db_exec($sql, $bindVars);

                if(!$result){
                    eventLog(
                        "PM",
                        "Error",
                        "DataLogError",
                        $this->controller->session["username"],
                        "Unable to save an history record.",
                        "Field: " . $key . " \n" .
                        "New Value: " . $new_values[$key] . " \n " .
                        "Old Value: " . $old_values[$key] . "\n"
                    );
                }
            }
        }
    }

    public function pm_history_log($arguments, $config) {
        $rval           = array();
        $bindVars       = array();
        $criteria       = array();
        $id             = $arguments["ID"];
        $entity_name    = $arguments["ENTITY_NAME"];
        $entity_key     = $arguments["ENTITY_KEY"];
        $form_name      = $arguments["FORM_NAME"];
        $field_name     = $arguments["FIELD_NAME"];
        $changed_by     = $arguments["CHANGED_BY"];
        $old_value      = $arguments["OLD_VALUE"];
        $new_value      = $arguments["NEW_VALUE"];
        $changed_on     = $arguments["CHANGED_ON"];
        $source         = $arguments["SOURCE"];

        $criteria = array(
            "ENTITY_KEY" =>     array($entity_key, "EQUALS"),
            "ENTITY_NAME" =>    array($entity_name, "MATCHES"),
            "FORM_NAME" =>      array($form_name, "MATCHES"),
            "FIELD_NAME" =>     array($field_name, "CONTAINS"),
            "CHANGED_BY" =>     array($changed_by, "CONTAINS"),
            "SOURCE" =>         array($source, "MATCHES"),
            "old_value" =>      array($old_value, "CONTAINS"),
            "new_value" =>      array($new_value, "CONTAINS"),
            "TO_CHAR(CHANGED_ON, 'MM/DD/RRRR')" => array($changed_on, "CONTAINS")
        );

        $where_clause = build_where_clause($criteria, $bindVars);

        $sql = "
        SELECT
            ID,
            CLASS_NAME,
            CLASS_KEY,
            ENTITY_NAME,
            ENTITY_KEY,
            FORM_NAME,
            FIELD_NAME,
            OLD_VALUE,
            NEW_VALUE,
            CHANGED_BY,
            TO_CHAR(CHANGED_ON, 'RRRR.MM.DD HH:MI:SS AM') AS CHANGED_ON,
            SOURCE
        FROM
            APPS.XXATDEAS_PM_CHANGE_LINES
        $where_clause
        ";

        $rval = oracle_paged_results(
            $sql,
            $bindVars,
            $arguments["START"] + 1,
            $arguments["END"] + 1,
            $arguments["SORT_BY"]
        );

        if(empty($rval)) {
            $rval["data"] = array();
            $rval["DSResponse"] = array();
        }

        return $rval;
    }
}
?>
