<?php
    $foo = array(
        'endRow'          => 1,
        'errorMessage'    => '',
        'errors'          => array(),
        'startRow'        => 0,
        'status'          => 0,
        'totalRows'       => 1,
        'data' => array(
            array(
                'salutation' => 'aa',
                'firstname' => 'aa',
                'lastname' => 'aa'
            ),
            array(
                'salutation' => 'ba',
                'firstname' => 'ba',
                'lastname' => 'ba'
            ),
            array(
                'salutation' => 'ca',
                'firstname' => 'ca',
                'lastname' => 'ca'
            )
        )
    );
    $baz = json_encode($foo);
    var_dump($baz);
    //var_export($baz,false);
?>