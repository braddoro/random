<?php
class test{
    public function getData($arguments) {
        $foo = array(
            endRow          => 1,
            errorMessage    => '',
            errors          => array(),
            startRow        => 0,
            status          => 2,
            totalRows       => 3,
            data => array(
                array(
                    salutation => 'aa',
                    firstname => 'aa',
                    lastname => 'aa'
                ),
                array(
                    salutation => 'ba',
                    firstname => 'ba',
                    lastname => 'ba'
                ),
                array(
                    salutation => 'ca',
                    firstname => 'ca',
                    lastname => 'ca'
                )
            )
        );
        return json_encode($foo);
    }
}
$test = new test();
print $test->getData();
?>
