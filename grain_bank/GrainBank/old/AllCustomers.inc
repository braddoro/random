<?php
//**************************************************************************
//
// File        :  AllCustomers.inc
//
// Copyright   :  Copyright 2013 American Tire Distributors, Inc.
//
// Author(s)   :  Brad Hughes - bhughes@atd-us.com
//
//                American Tire Distributors
//                12200 Herbert Wayne Ct.
//                Huntersville, NC 28078
//
//**************************************************************************
require_once 'shared/classes/PCLOracleTable.inc';
require_once 'shared/classes/PCLSmartClientTableController.inc';

final class AllCustomers extends ServerObject {
    private $PCLOracleConnection;

    public function __construct(&$controller) {
        parent::__construct($controller);
        $this->PCLOracleConnection = new PCLOracle($GLOBALS['oracle_conn']);
        oracle_apps_initialize($this->controller->session["user_id"], $this->controller->session["responsibility_id"], $this->controller->session["application_id"], $this->controller->session["profile_option_value"]);
    }

    public function CustomerData(&$arguments, $config) {
        $TableObject = new PCLOracleTable(__FUNCTION__, $this->GetTableMetadata(__FUNCTION__), $this->PCLOracleConnection);
        $PCLSTC = new PCLSmartClientTableController($TableObject, $this->controller);
        return $PCLSTC->doTableOperation($arguments);
    }

    public function LocationData(&$arguments, $config) {
        $TableObject = new PCLOracleTable(__FUNCTION__, $this->GetTableMetadata(__FUNCTION__), $this->PCLOracleConnection);
        $PCLSTC = new PCLSmartClientTableController($TableObject, $this->controller);
        return $PCLSTC->doTableOperation($fetchArgs);
    }


    private function GetTableMetadata( $tablename ) {

        $metadata = array();

        switch($tablename) {
            case 'CustomerData':
                $metadata = array(
                    'SCHEMA' => 'APPS',
                    'TABLE_NAME' => 'HZ_CUST_ACCOUNTS',
                    'TABLE_ALIAS' => 'TBL',
                    'SELECT_SQL' => "
                        SELECT
                          CA.CUST_ACCOUNT_ID,
                          PA.PARTY_NUMBER,
                          PA.PARTY_NAME,
                          PA.PARTY_TYPE,
                          PA.PARTY_ID,
                          CA.ACCOUNT_NUMBER,
                          CA.ACCOUNT_NAME,
                          PS.ADDRESSEE,
                          L.ADDRESS1,
                          L.CITY,
                          L.COUNTY,
                          L.POSTAL_CODE,
                          OU.NAME AS ORG_NAME,
                          PA.STATUS AS PA_STATUS,
                          CA.STATUS AS CA_STATUS,
                          PS.STATUS AS PS_STATUS,
                          CAS.ORG_ID AS CAS_ORG_ID,
                          SC.CREATION_DATE
                        FROM
                          APPS.HZ_PARTIES PA,
                          APPS.HZ_CUST_ACCOUNTS CA,
                          APPS.HZ_PARTY_SITES PS,
                          APPS.HZ_LOCATIONS L,
                          APPS.HZ_CUST_ACCT_SITES_ALL CAS,
                          APPS.XXATDEAS_CUSTOMERS SC,
                          APPS.HR_ORGANIZATION_UNITS OU
                        WHERE
                          PA.PARTY_ID           = CA.PARTY_ID
                          AND PA.PARTY_ID       = PS.PARTY_ID
                          AND PS.LOCATION_ID    = L.LOCATION_ID
                          AND CA.CUST_ACCOUNT_ID = CAS.CUST_ACCOUNT_ID(+)
                          AND CA.CUST_ACCOUNT_ID = SC.LIVE_CUSTOMER_ID(+)
                          --AND CAS.ORG_ID        = SC.ORG_ID(+)
                          AND CAS.ORG_ID        = OU.ORGANIZATION_ID(+)
                    ",
                    'COLUMNS' => array(
                        'ACCOUNT_NAME' => array(
                            'BIND_VARIABLE_NAME' => ':ACCOUNT_NAME',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.ACCOUNT_NAME',
                            'SELECT_ALIAS' => 'ACCOUNT_NAME',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'PARTY_NAME' => array(
                            'BIND_VARIABLE_NAME' => ':PARTY_NAME',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.PARTY_NAME',
                            'SELECT_ALIAS' => 'PARTY_NAME',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'ADDRESSEE' => array(
                            'BIND_VARIABLE_NAME' => ':ADDRESSEE',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.ADDRESSEE',
                            'SELECT_ALIAS' => 'ADDRESSEE',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'CUST_ACCOUNT_ID' => array(
                            'BIND_VARIABLE_NAME' => ':CUST_ACCOUNT_ID',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.CUST_ACCOUNT_ID',
                            'SELECT_ALIAS' => 'CUST_ACCOUNT_ID',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'MATCHES',
                            'SEQUENCE' => '',
                            'IS_PK' => TRUE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'PARTY_NUMBER' => array(
                            'BIND_VARIABLE_NAME' => ':PARTY_NUMBER',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.PARTY_NUMBER',
                            'SELECT_ALIAS' => 'PARTY_NUMBER',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'MATCHES',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'PARTY_TYPE' => array(
                            'BIND_VARIABLE_NAME' => ':PARTY_TYPE',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.PARTY_TYPE',
                            'SELECT_ALIAS' => 'PARTY_TYPE',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'MATCHES',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'ACCOUNT_NUMBER' => array(
                            'BIND_VARIABLE_NAME' => ':ACCOUNT_NUMBER',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.ACCOUNT_NUMBER',
                            'SELECT_ALIAS' => 'ACCOUNT_NUMBER',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'MATCHES',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'PARTY_ID' => array(
                            'BIND_VARIABLE_NAME' => ':PARTY_ID',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.PARTY_ID',
                            'SELECT_ALIAS' => 'PARTY_ID',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'ACCOUNT_NUMBER' => array(
                            'BIND_VARIABLE_NAME' => ':ACCOUNT_NUMBER',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.ACCOUNT_NUMBER',
                            'SELECT_ALIAS' => 'ACCOUNT_NUMBER',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'ACCOUNT_NAME' => array(
                            'BIND_VARIABLE_NAME' => ':ACCOUNT_NAME',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.ACCOUNT_NAME',
                            'SELECT_ALIAS' => 'ACCOUNT_NAME',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'ADDRESS1' => array(
                            'BIND_VARIABLE_NAME' => ':ADDRESS1',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.ADDRESS1',
                            'SELECT_ALIAS' => 'ADDRESS1',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'CITY' => array(
                            'BIND_VARIABLE_NAME' => ':CITY',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.CITY',
                            'SELECT_ALIAS' => 'CITY',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'COUNTY' => array(
                            'BIND_VARIABLE_NAME' => ':COUNTY',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'TBL.COUNTY',
                            'SELECT_ALIAS' => 'COUNTY',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'POSTAL_CODE' => array(
                            'BIND_VARIABLE_NAME' => ':POSTAL_CODE',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => "TBL.POSTAL_CODE",
                            'SELECT_ALIAS' => 'POSTAL_CODE',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'ORG_NAME' => array(
                            'BIND_VARIABLE_NAME' => ':ORG_NAME',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => "TBL.ORG_NAME",
                            'SELECT_ALIAS' => 'ORG_NAME',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'PA_STATUS' => array(
                            'BIND_VARIABLE_NAME' => ':PA_STATUS',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => "TBL.PA_STATUS",
                            'SELECT_ALIAS' => 'PA_STATUS',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'CA_STATUS' => array(
                            'BIND_VARIABLE_NAME' => ':CA_STATUS',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => "TBL.CA_STATUS",
                            'SELECT_ALIAS' => 'CA_STATUS',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'PS_STATUS' => array(
                            'BIND_VARIABLE_NAME' => ':PS_STATUS',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => "TBL.PS_STATUS",
                            'SELECT_ALIAS' => 'PS_STATUS',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'CAS_ORG_ID' => array(
                            'BIND_VARIABLE_NAME' => ':CAS_ORG_ID',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => "TBL.CAS_ORG_ID",
                            'SELECT_ALIAS' => 'CAS_ORG_ID',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'CREATION_DATE' => array(
                            'BIND_VARIABLE_NAME' => ':CREATION_DATE',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => "TBL.CREATION_DATE",
                            'SELECT_ALIAS' => 'CREATION_DATE',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                    ),
                    'FUNCTIONS' => array(
                        'add' => array(
                            'NAME' => '',
                            'PARAMETER_COUNT' => array('MIN' => 1, 'MAX' => 1),
                            'PRE_SEQUENCE_FETCH_METHOD' => '',
                            'POST_SEQUENCE_FETCH_METHOD' => '',
                            'PRE_VALIDATION_METHOD' => '',
                            'POST_VALIDATION_METHOD' => '',
                            'PRE_UNIQUE_KEY_METHOD' => '',
                            'POST_UNIQUE_KEY_METHOD' => '',
                            'PRE_INSERT_METHOD' => '',
                            'POST_INSERT_METHOD' => '',
                            'PRE_FETCHBACK_METHOD' => '',
                            'POST_FETCHBACK_METHOD' => ''
                        ),                        'update' => array(
                            'NAME' => '',
                            'PARAMETER_COUNT' => array('MIN' => 1, 'MAX' => 1),
                            'PRE_VALIDATION_METHOD' => '',
                            'POST_VALIDATION_METHOD' => '',
                            'PRE_UNIQUE_KEY_METHOD' => '',
                            'POST_UNIQUE_KEY_METHOD' => '',
                            'PRE_UPDATE_METHOD' => '',
                            'POST_UPDATE_METHOD' => '',
                            'PRE_FETCHBACK_METHOD' => '',
                            'POST_FETCHBACK_METHOD' => ''
                        ),
                        'remove' => array(
                            'NAME' => '',
                            'PARAMETER_COUNT' => array('MIN' => 1, 'MAX' => 1),
                            'PRE_VALIDATION_METHOD' => '',
                            'POST_VALIDATION_METHOD' => '',
                            'PRE_DELETE_METHOD' => '',
                            'POST_DELETE_METHOD' => '',
                            'PRE_FETCHBACK_METHOD' => '',
                            'POST_FETCHBACK_METHOD' => ''
                        ),
                        'fetch' => array(
                            'NAME' => '',
                            'PARAMETER_COUNT' => array('MIN' => 1, 'MAX' => 1),
                            'PRE_SELECT_METHOD' => '',
                            'POST_SELECT_METHOD' => ''
                        )
                    )
                );
            break;

            case 'LocationData':
                $metadata = array(
                    'SCHEMA' => 'APPS',
                    'TABLE_NAME' => 'HZ_CUST_ACCT_SITES',
                    'TABLE_ALIAS' => 'CAS',
                    'SELECT_SQL' => "
                        SELECT
                            PS.PARTY_SITE_ID,
                            CA.ACCOUNT_NUMBER,
                            CA.CUST_ACCOUNT_ID,
                            PS.STATUS AS STATUS,
                            CSU.STATUS AS CSU_STATUS,
                            CAS1.STATUS AS CAS_STATUS,
                            CAS1.ATTRIBUTE15 AS Z_NUMBER,
                            L.ADDRESS1,
                            L.ADDRESS2,
                            L.CITY,
                            DECODE(NVL(L.COUNTRY, 'US'), 'US', L.STATE, L.PROVINCE) AS STATE,
                            L.COUNTRY,
                            L.POSTAL_CODE,
                            L.POSTAL_PLUS4_CODE,
                            DECODE(NVL(CAS1.BILL_TO_FLAG, 'N'), 'N', DECODE(NVL(CAS1.SHIP_TO_FLAG, 'N'), 'N', '?', 'S'), DECODE(NVL(CAS1.SHIP_TO_FLAG, 'N'), 'N', 'B', 'D')) AS SITE_TYPE,
                            CSU.SITE_USE_CODE,
                            CAS1.BILL_TO_FLAG,
                            CAS1.SHIP_TO_FLAG,
                            CSU.WAREHOUSE_ID AS WAREHOUSE_ID,
                            CSU.LOCATION AS LOCATION,
                            TO_NUMBER(CSU.ATTRIBUTE4) AS STORE_NUMBER,
                            PS.ADDRESSEE AS DBA_NAME,
                            CAS1.CUST_ACCT_SITE_ID,
                            CAS1.OBJECT_VERSION_NUMBER AS CAS_OBJECT_VERSION_NUMBER,
                            CSU.SITE_USE_ID,
                            CSU.OBJECT_VERSION_NUMBER AS CSU_OBJECT_VERSION_NUMBER,
                            (SELECT NVL(SUM(APS.AMOUNT_DUE_REMAINING),0) FROM APPS.AR_PAYMENT_SCHEDULES APS WHERE APS.CUSTOMER_SITE_USE_ID = CSU.SITE_USE_ID) AS BALANCE_DUE,
                            (SELECT COUNT(*) FROM APPS.OE_ORDER_HEADERS OH WHERE CSU.SITE_USE_ID = OH.SHIP_TO_ORG_ID AND OH.FLOW_STATUS_CODE NOT IN ('CLOSED', 'CANCELLED', 'CANCELED')) AS ORDERS
                        FROM
                            APPS.HZ_CUST_ACCOUNTS CA,
                            APPS.HZ_CUST_ACCT_SITES CAS1,
                            APPS.HZ_PARTY_SITES PS,
                            APPS.HZ_LOCATIONS L,
                            APPS.HZ_CUST_SITE_USES CSU
                        WHERE
                            CA.CUST_ACCOUNT_ID = CAS1.CUST_ACCOUNT_ID
                            AND CAS1.CUST_ACCT_SITE_ID = CSU.CUST_ACCT_SITE_ID
                            AND CAS1.PARTY_SITE_ID = PS.PARTY_SITE_ID
                            AND PS.LOCATION_ID = L.LOCATION_ID
                    ",
                    'COLUMNS' => array(
                        'PARTY_SITE_ID' => array(
                            'BIND_VARIABLE_NAME' => ':PARTY_SITE_ID',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.PARTY_SITE_ID',
                            'SELECT_ALIAS' => 'PARTY_SITE_ID',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'MATCHES',
                            'SEQUENCE' => '',
                            'IS_PK' => TRUE,
                            'UK_LIST' => array('CUST_ACCOUNT_ID'),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'ACCOUNT_NUMBER' => array(
                            'BIND_VARIABLE_NAME' => ':ACCOUNT_NUMBER',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.ACCOUNT_NUMBER',
                            'SELECT_ALIAS' => 'ACCOUNT_NUMBER',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'CUST_ACCOUNT_ID' => array(
                            'BIND_VARIABLE_NAME' => ':CUST_ACCOUNT_ID',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.CUST_ACCOUNT_ID',
                            'SELECT_ALIAS' => 'CUST_ACCOUNT_ID',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array('CUST_ACCOUNT_ID'),
                            'VALIDATORS' => array(
                                'all' => array('IS_NUMERIC' => array()),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'CSU_STATUS' => array(
                            'BIND_VARIABLE_NAME' => ':CSU_STATUS',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.CSU_STATUS',
                            'SELECT_ALIAS' => 'CSU_STATUS',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'CAS_STATUS' => array(
                            'BIND_VARIABLE_NAME' => ':CAS_STATUS',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.CAS_STATUS',
                            'SELECT_ALIAS' => 'CAS_STATUS',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'STATUS' => array(
                            'BIND_VARIABLE_NAME' => ':STATUS',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.STATUS',
                            'SELECT_ALIAS' => 'STATUS',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'DBA_NAME' => array(
                            'BIND_VARIABLE_NAME' => ':DBA_NAME',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.DBA_NAME',
                            'SELECT_ALIAS' => 'DBA_NAME',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => 'DBA_NAME',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'Z_NUMBER' => array(
                            'BIND_VARIABLE_NAME' => ':Z_NUMBER',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'Z_NUMBER',
                            'SELECT_ALIAS' => 'Z_NUMBER',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'ADDRESS1' => array(
                            'BIND_VARIABLE_NAME' => ':ADDRESS1',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.ADDRESS1',
                            'SELECT_ALIAS' => 'ADDRESS1',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'CITY' => array(
                            'BIND_VARIABLE_NAME' => ':CITY',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.CITY',
                            'SELECT_ALIAS' => 'CITY',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'STATE' => array(
                            'BIND_VARIABLE_NAME' => ':STATE',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.STATE',
                            'SELECT_ALIAS' => 'STATE',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'COUNTRY' => array(
                            'BIND_VARIABLE_NAME' => ':COUNTRY',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.COUNTRY',
                            'SELECT_ALIAS' => 'COUNTRY',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'POSTAL_CODE' => array(
                            'BIND_VARIABLE_NAME' => ':POSTAL_CODE',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.POSTAL_CODE',
                            'SELECT_ALIAS' => 'POSTAL_CODE',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'POSTAL_PLUS4_CODE' => array(
                            'BIND_VARIABLE_NAME' => ':POSTAL_PLUS4_CODE',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.POSTAL_PLUS4_CODE',
                            'SELECT_ALIAS' => 'PLUS4',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'SITE_TYPE' => array(
                            'BIND_VARIABLE_NAME' => ':SITE_TYPE',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.SITE_TYPE',
                            'SELECT_ALIAS' => 'SITE_TYPE',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'SITE_USE_CODE' => array(
                            'BIND_VARIABLE_NAME' => ':SITE_USE_CODE',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.SITE_USE_CODE',
                            'SELECT_ALIAS' => 'SITE_USE_CODE',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'BILL_TO_FLAG' => array(
                            'BIND_VARIABLE_NAME' => ':BILL_TO_FLAG',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.BILL_TO_FLAG',
                            'SELECT_ALIAS' => 'BILL_TO_FLAG',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'SHIP_TO_FLAG' => array(
                            'BIND_VARIABLE_NAME' => ':SHIP_TO_FLAG',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.SHIP_TO_FLAG',
                            'SELECT_ALIAS' => 'SHIP_TO_FLAG',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'WAREHOUSE_ID' => array(
                            'BIND_VARIABLE_NAME' => ':WAREHOUSE_ID',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.WAREHOUSE_ID',
                            'SELECT_ALIAS' => 'WAREHOUSE_ID',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'LOCATION' => array(
                            'BIND_VARIABLE_NAME' => ':LOCATION',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.LOCATION',
                            'SELECT_ALIAS' => 'LOCATION',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'STORE_NUMBER' => array(
                            'BIND_VARIABLE_NAME' => ':STORE_NUMBER',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.STORE_NUMBER',
                            'SELECT_ALIAS' => 'STORE_NUMBER',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'CONTAINS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'CUST_ACCT_SITE_ID' => array(
                            'BIND_VARIABLE_NAME' => ':CUST_ACCT_SITE_ID',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.CUST_ACCT_SITE_ID',
                            'SELECT_ALIAS' => 'CUST_ACCT_SITE_ID',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'BALANCE_DUE' => array(
                            'BIND_VARIABLE_NAME' => ':BALANCE_DUE',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'BALANCE_DUE',
                            'SELECT_ALIAS' => 'BALANCE_DUE',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'ORDERS' => array(
                            'BIND_VARIABLE_NAME' => ':ORDERS',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'ORDERS',
                            'SELECT_ALIAS' => 'ORDERS',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'SITE_USE_ID' => array(
                            'BIND_VARIABLE_NAME' => ':SITE_USE_ID',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.SITE_USE_ID',
                            'SELECT_ALIAS' => 'SITE_USE_ID',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'CSU_OBJECT_VERSION_NUMBER' => array(
                            'BIND_VARIABLE_NAME' => ':CSU_OBJECT_VERSION_NUMBER',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.CSU_OBJECT_VERSION_NUMBER',
                            'SELECT_ALIAS' => 'CSU_OBJECT_VERSION_NUMBER',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        ),
                        'CAS_OBJECT_VERSION_NUMBER' => array(
                            'BIND_VARIABLE_NAME' => ':CAS_OBJECT_VERSION_NUMBER',
                            'SET_BIND_VARIABLE_NAME' => '',
                            'SELECT_TEXT' => 'CAS.CAS_OBJECT_VERSION_NUMBER',
                            'SELECT_ALIAS' => 'CAS_OBJECT_VERSION_NUMBER',
                            'SET_TEXT' => '',
                            'SET_ALIAS' => '',
                            'MATCH_TYPE' => 'EQUALS',
                            'SEQUENCE' => '',
                            'IS_PK' => FALSE,
                            'UK_LIST' => array(),
                            'VALIDATORS' => array(
                                'all' => array(),
                                'add' => array(),
                                'update' => array(),
                                'remove' => array()
                            )
                        )
                    ),
                    'FUNCTIONS' => array(
                        'add' => array(
                            'NAME' => '',
                            'PARAMETER_COUNT' => array('MIN' => 1, 'MAX' => 1),
                            'PRE_SEQUENCE_FETCH_METHOD' => '',
                            'POST_SEQUENCE_FETCH_METHOD' => '',
                            'PRE_VALIDATION_METHOD' => '',
                            'POST_VALIDATION_METHOD' => '',
                            'PRE_UNIQUE_KEY_METHOD' => '',
                            'POST_UNIQUE_KEY_METHOD' => '',
                            'PRE_INSERT_METHOD' => '',
                            'POST_INSERT_METHOD' => '',
                            'PRE_FETCHBACK_METHOD' => '',
                            'POST_FETCHBACK_METHOD' => ''
                        ),                        'update' => array(
                            'NAME' => '',
                            'PARAMETER_COUNT' => array('MIN' => 1, 'MAX' => 1),
                            'PRE_VALIDATION_METHOD' => '',
                            'POST_VALIDATION_METHOD' => '',
                            'PRE_UNIQUE_KEY_METHOD' => '',
                            'POST_UNIQUE_KEY_METHOD' => '',
                            'PRE_UPDATE_METHOD' => '',
                            'POST_UPDATE_METHOD' => '',
                            'PRE_FETCHBACK_METHOD' => '',
                            'POST_FETCHBACK_METHOD' => ''
                        ),
                        'remove' => array(
                            'NAME' => '',
                            'PARAMETER_COUNT' => array('MIN' => 1, 'MAX' => 1),
                            'PRE_VALIDATION_METHOD' => '',
                            'POST_VALIDATION_METHOD' => '',
                            'PRE_DELETE_METHOD' => '',
                            'POST_DELETE_METHOD' => '',
                            'PRE_FETCHBACK_METHOD' => '',
                            'POST_FETCHBACK_METHOD' => ''
                        ),
                        'fetch' => array(
                            'NAME' => '',
                            'PARAMETER_COUNT' => array('MIN' => 1, 'MAX' => 1),
                            'PRE_SELECT_METHOD' => '',
                            'POST_SELECT_METHOD' => ''
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
?>
