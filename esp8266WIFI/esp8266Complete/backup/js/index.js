https://datatables.net/examples/ajax/null_data_source.html
$(document).ready(function () {

    
    $('#page_list').click( function () {
        $.get("/wifi/list.html", function(data, status){
            // console.log("Data: " + data + "\nStatus: " + status);
            // console.log("oi");
            $("#body_switch").html(data);
            reload_table_wifi_list();
        });
    });
    $('#page_home').click( function () {
        $.get("/home/index.html", function(data, status){
            // console.log("Data: " + data + "\nStatus: " + status);
            // console.log("oi");
            $("#body_switch").html(data);
            reload_table_wifi_home();
        });
    });
    $('#table_index tbody').on('click', 'button', function () {
        var data = table.row($(this).parents('tr')).data();
        console.log(data)
        setData("wifi",data)
        // alert(data[0] + "'s salary is: " + data["access"]);
        $.get(data["access"].split("api/")[1]+".html", function(data, status){
            console.log("Data: " + data + "\nStatus: " + status);
            console.log("oi");
            $("#body_switch").html(data);
            reload_table_wifi();
        });
        // $.ajax({
        //     url: data["access"],
        //     type: 'post',
        //     dataType: 'json',
        //     contentType: 'application/json',
        //     success: function (data) {
        //         console.log("oi")
        //         console.log(data.msg);
        //         // $('#target').html(data.msg);
        //         $("#body_switch").html("test");

        //     },
        //     data: JSON.stringify(data)
        // });
        // window.location.href=data["access"].split("api/")[1]+".html"
    });
    $.ajax({
        url: "/api/wifi/",
        type: 'post',
        dataType: 'json',
        contentType: 'application/json',
        success: function (data) {
            console.log(data)
            $("#name_wifi").html(data.name_wifi)
            $("#ip_wifi").html(data.ip)
            reload_table_wifi_home();
        },
        data: JSON.stringify({"d":"init"})
    });

});
