<?php
class foo{
    public function getData($arguments) {
        $foo = array(
            endRow          => 4,
            errorMessage    => NULL,
            errors          => array(),
            startRow        => 0,
            status          => 0,
            totalRows       => 4,
            data => array(
                array(
                    salutation => 'Foo',
                    firstname => 'Foo',
                    lastname => 'Foo'
                ),
                array(
                    salutation => 'Baz',
                    firstname => 'Baz',
                    lastname => 'Baz'
                ),
                array(
                    salutation => 'Qux',
                    firstname => 'Qux',
                    lastname => 'Qux'
                ),
                array(
                    salutation => 'Zod',
                    firstname => 'Zod',
                    lastname => 'Zod'
                )
            )
        );
        return json_encode($foo);
    }
}
$foo = new foo();
print $foo->getData();
?>
