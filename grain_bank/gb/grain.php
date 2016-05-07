<?php
require_once('php/data_library.php');
class Grain {

    // __construct
    //
    //function __construct() {}

    // balance
    //
    public function addTran($args = NULL) {
        $rows = array();
        $data = New DataLibrary();

        if(!is_numeric($args['GRAIN_ID'])) {
            unset($args['GRAIN_ID']);
        }

        if(!is_numeric($args['USER_ID'])) {
            unset($args['USER_ID']);
        }
        if(!is_numeric($args['AMOUNT'])) {
            unset($args['AMOUNT']);
        }

        $validInsert = false;
        if(isset($args['GRAIN_ID']) && isset($args['USER_ID']) && isset($args['AMOUNT'])){
            $validInsert = true;
            $grainID = $args['GRAIN_ID'];
            $userID = $args['USER_ID'];
            $transAmt = $args['AMOUNT'];
            $orderID = (isset($args['ORDER_ID']) ? $args['ORDER_ID'] : 0);
        }

        if(!$validInsert){
            return json_encode($rows);
        }

        $sql = "INSERT INTO grain_inventory (grainID_fk, userID_fk, orderID, transactionAmount) VALUES ($grainID, $userID, $orderID, $transAmt)";
echo("/* $sql */ \n");
        $response = $data->getData($sql);
        if(!$response['status']) {
            $rows['status'] = -1;
            $rows['errorMessage'] = $data->parseErrors($response['message']);
            return json_encode($rows);
        }

        $rows[] = array('USER_ID' => $userID);

        return json_encode($rows);
    }

    // balance
    //
    public function balance($args = NULL) {
        $rows = array();
        $data = New DataLibrary();

        if($args['userID'] == 0) {
            $args['userID'] = 'i.userID_fk';
        }
        $sql = "
        SELECT
            t.grain_type,
            sum(i.transactionAmount) as 'balance',
            max(transactionDate) as 'lastChange'
        FROM
            grain_bank.grain_type t
        LEFT JOIN
            grain_bank.grain_inventory i ON t.grainID = i.grainID_fk
        WHERE
            i.userID_fk = " . $args['userID'] . "
        GROUP BY
            t.grain_type
        ORDER BY
            t.grain_type";

        $response = $data->getData($sql);
        if(!$response['status']) {
            $rows['status'] = -1;
            $rows['errorMessage'] = $data->parseErrors($response['message']);
            return json_encode($rows);
        }
        $result = $response['result'];
        while ($row = $result->fetch()) {
            $rows[] = array(
                'grain_type'    => $row['grain_type'],
                'balance'        => $row['balance'],
                'lastChange'    => $row['lastChange']
                );
        }
        $result->closeCursor();
        unset($response);
        return json_encode($rows);
    }

    // transaction
    //
    public function transaction($args = NULL) {
        $rows = array();
        $data = New DataLibrary();

		if($args['userID'] == 0) {
            $args['userID'] = 'i.userID_fk';
        }
        $sql = "
            SELECT
                t.grain_type,
                i.transactionAmount,
                i.orderID,
                i.transactionDate
            FROM
                grain_bank.grain_type t
            LEFT JOIN
                grain_bank.grain_inventory i ON t.grainID = i.grainID_fk
            WHERE
                i.userID_fk = " . $args['userID'] . "
            ORDER BY
                i.transactionDate,
                i.transactionAmount desc,
                t.grain_type;";

        $response = $data->getData($sql);
        if(!$response['status']) {
            $rows['status'] = -1;
            $rows['errorMessage'] = $data->parseErrors($response['message']);
            return json_encode($rows);
        }
        $result = $response['result'];
        while ($row = $result->fetch()) {
            $rows[] = array(
                'grain_type'        => $row['grain_type'],
                'transactionAmount'	=> $row['transactionAmount'],
                'orderID'           => $row['orderID'],
                'transactionDate'   => $row['transactionDate']
                );
        }
        $result->closeCursor();
        unset($response);
        return json_encode($rows);
    }

    // user
    //
    public function user($args = NULL) {
        $rows = array();
        $data = New DataLibrary();

        $sql = "SELECT userID, userUUID, userName, emailAddress, active, addedDate FROM grain_user order by userName;";
        $response = $data->getData($sql);
        if(!$response['status']) {
            $rows['status'] = -1;
            $rows['errorMessage'] = $data->parseErrors($response['message']);
            return json_encode($rows);
        }
        $result = $response['result'];
        while ($row = $result->fetch()) {
            $rows[] = array(
                'userID'        => $row['userID'],
                'userUUID'        => $row['userUUID'],
                'userName'        => $row['userName'],
                'emailAddress'  => $row['emailAddress'],
                'active'        => $row['active'],
                'addedDate'        => $row['addedDate']
                );
        }
        $result->closeCursor();
        unset($response);
        return json_encode($rows);
    }

	// user
    //
    public function grain($args = NULL) {
        $rows = array();
        $data = New DataLibrary();

        $sql = "SELECT grainID, grainUUID, grain_type, addedDate FROM grain_type order by grain_type;";
        $response = $data->getData($sql);
        if(!$response['status']) {
            $rows['status'] = -1;
            $rows['errorMessage'] = $data->parseErrors($response['message']);
            return json_encode($rows);
        }
        $result = $response['result'];
        while ($row = $result->fetch()) {
            $rows[] = array(
                'grainID'       => $row['grainID'],
                'grainUUID'     => $row['grainUUID'],
                'grain_type'    => $row['grain_type'],
                'emailAddress'	=> $row['emailAddress'],
                'addedDate'     => $row['addedDate']
                );
        }
        $result->closeCursor();
        unset($response);
        return json_encode($rows);
    }

}
$argsIN = array();
$Foo = New Grain();

$luserID = 0;
if(isset($_POST['USER_ID'])){
    $luserID = intval($_POST['USER_ID']);
}
$argsIN['userID'] = $luserID;

$luserID = 0;
if(isset($_POST['GRAIN_TYPE'])){
    $luserID = intval($_POST['GRAIN_TYPE']);
}
$argsIN['GRAIN_TYPE'] = $luserID;

$luserID = 0;
if(isset($_POST['AMOUNT'])){
    $luserID = intval($_POST['AMOUNT']);
}
$argsIN['AMOUNT'] = $luserID;

$luserID = 0;
if(isset($_POST['ORDER_ID'])){
    $luserID = intval($_POST['ORDER_ID']);
}
$argsIN['ORDER_ID'] = $luserID;

echo $Foo->$_GET['om']($argsIN);
?>
