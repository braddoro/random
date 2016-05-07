//**************************************************************************
//
// File        :  AllCustomers.js
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
isc.defineClass("AllCustomers", "CEDialog");

isc.applicationWindow.registeredObjects.add({
    className: "AllCustomers",
//    isProtected: true,
    singleton: true,
    module: "AllCustomers",
    resource: "Window",
    operation_type: "Show"
});

isc.AllCustomers.addProperties({
    title: "All Customers",
    name: "AllCustomers",
    width: "100%",
    height: "100%",

    initWidget: function(){
        this.Super("initWidget", arguments);

        this.AllCustomers = isc.PCLJSONDataSource.create({
            dataURL: isc.appData.controllerStub + "?om=AllCustomers.CustomerData",
            fields: [
                {name: "ACCOUNT_NAME",      type: "text"},
                {name: "ACCOUNT_NUMBER",    type: "text"},
                {name: "ADDRESS1",          type: "text"},
                {name: "ADDRESSEE",         type: "text"},
                {name: "CAS_ORG_ID",        type: "text"},
                {name: "CA_STATUS",         type: "text"},
                {name: "CITY",              type: "text"},
                {name: "COUNTY",            type: "text"},
                {name: "CREATION_DATE",     type: "text"},
                {name: "CUST_ACCOUNT_ID",   type: "text", primaryKey: true},
                {name: "ORG_NAME",          type: "text"},
                {name: "PARTY_NAME",        type: "text"},
                {name: "PARTY_TYPE",        type: "text"},
                {name: "PA_STATUS",         type: "text"},
                {name: "POSTAL_CODE",       type: "text"},
                {name: "PS_STATUS",         type: "text"}
            ]
        });

        this.customerListGrid = isc.ListGrid.create({
            parent: this,
            name: "customerListGrid",
            alternateRecordStyles: true,
            dataSource: this.AllCustomers,
            autoFetchData: false,
            selectionType: "single",
            canEdit: false,
            modalEditing: true,
            showFilterEditor: true,
            groupByField: ["ACCOUNT_NAME", "ORG_NAME"],
            groupStartOpen: "none",
            fields: [
                {name: "ACCOUNT_NAME"},
                {name: "ORG_NAME"},
                {name: "ADDRESSEE"},
                {name: "PARTY_NAME", detail: true},
                {name: "ACCOUNT_NUMBER", width: 100},
                {name: "CREATION_DATE"},
                {name: "ADDRESS1"},
                {name: "CITY"},
                {name: "COUNTY", detail: true},
                {name: "POSTAL_CODE", width: 100},
                {name: "CAS_ORG_ID", width: 100, detail: true},
                {name: "CUST_ACCOUNT_ID", width: 100},
                {name: "PARTY_TYPE", width: 100},
                {name: "PA_STATUS", width: 50},
                {name: "CA_STATUS", width: 50},
                {name: "PS_STATUS", width: 50}
            ],
            recordDoubleClick: "this.parent.selectCustomer(viewer, record)",
//            recordClick: function(viewer, record, recordNum, field, fieldNum, value, rawValue){
//                console.log(record);
//            },
            cellContextClick: "this.parent.selectedRow = rowNum;this.parent.myContextMenu.showContextMenu(); return false;"
//            cellContextClick: function(record, rowNum, colNum){
//                this.parent.selectedRow = rowNum;
//                this.parent.myContextMenu.showContextMenu();
//                return true;
//            }
        });

        this.myContextMenu = isc.Menu.create({
            parent: this,
            shadowDepth: 10,
            defaultWidth: "100",
            cellHeight:16,
            data: [
            {title: "Open Customer", click: "menu.parent.selectCustomer(menu.parent.customerListGrid.getSelectedRecord(), {'SITE_OPEN': 'TRUE'})"},
            {title: "Create New Bill to Record",
                click: function(menu){
                    console.log(menu.parent.customerListGrid.getSelectedRecord());
                    menu.parent.createBillTo(menu.parent.customerListGrid.getSelectedRecord());
                }
            }
            ]
        });

        this.mainLayout = isc.VLayout.create({
            members: [this.customerListGrid]
        });

        this.addItem(this.mainLayout);
    },

    createBillTo: function(record){
        console.log("createBillTo:");
        RPCManager.sendEASRPCRequest({
            params: {CUST_ACCOUNT_ID: record.CUST_ACCOUNT_ID},
            actionURL: isc.appData.controllerStub + "?om=Customers.createBillTo",
            callback: {target: this, methodName: "createBillToCallback"},
            prompt: "Creating Staged Bill to Record in Oracle.",
            showPrompt: true
        });
    },

    createBillToCallback: function(rpcResponse){
        console.log("createBillTo_callback:");
        console.log(rpcResponse);
        if(rpcResponse.data.status != isc.RPCResponse.STATUS_SUCCESS) {
            console.log(rpcResponse.data.errorMessage);
        }
        handleDialogBoxStatusCodes(rpcResponse.data);
    },

    selectCustomer: function(viewer, record){
        // check which tab is selected
        var dataToPass = {
            dataSource: "LIVE",
            dataKey: record.CUST_ACCOUNT_ID,
            dataNum: record.ACCOUNT_NUMBER
        };

        isc.applicationWindow.showWindow("Customer", dataToPass);
    },

    windowInitialize: function(initData){
        this.initData = initData;
    }
});
