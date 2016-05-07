//**************************************************************************
//
// File        :  Foo.js
//
// Copyright   :  Copyright 2007 American Tire Distributors, Inc.
//
// Author(s)   :  Rob Helgeson - rhelgeson@atd-us.com
//
//                American Tire Distributors
//                12200 Herbert Wayne Ct.
//                Huntersville, NC 28078
//
//**************************************************************************
isc.defineClass("Foo", "CEWindow");

isc.applicationWindow.registeredObjects.add({
    className: "Foo",
    isProtected: false,
    singleton: true,
    module: "Customer",
    resource: "Window",
    operation_type: "Show"
});

isc.Foo.addProperties({
    title: "Foo",
    name: "Foo",
    width: "90%",
    height: "90%",
    canDragReposition: true,
    canDragResize: true,
    initWidget: function(){
        this.Super("initWidget", arguments);
        this.baz = isc.Baz.create();
        this.addItem(this.baz);
        this.baz.draw();
    },
    windowInitialize: function(initData){
        this.initData = initData;
    },
    closeClick: function(){
        isc.applicationWindow.closeWindow(this.name);
    }
});
