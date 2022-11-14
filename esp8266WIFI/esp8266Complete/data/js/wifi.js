https://datatables.net/examples/ajax/null_data_source.html
$(document).ready(function () {
    wifi = getData("wifi")
    $("#name_ssid").html(wifi.name_ssid)
    $("#signal_strength").html(wifi.name_ssid)
    reload_table_wifi();
    reload_table_wifi_list();
});


function reload_table_wifi_list(){
    var table = $('#table_wifi_list').DataTable({
        processing: true,
        serverSide: true,
        ajax: {
            url: '/api/wifi/list',
            type: 'POST',
        },
        columns: [
            { data: 'name_ssid' },
            { data: 'signal_strength' },
            { data: '' }
        ],
        columnDefs: [
            {
                targets: -1,
                data: null,
                defaultContent: '<button>Click!</button>',
            },
        ],
    });
}
function reload_table_wifi(){
    var table = $('#table_wifi_profile').DataTable({
        processing: true,
        serverSide: true,
        ajax: {
            url: '/api/wifi/profile',
            type: 'POST',
        },
        columns: [
            { data: 'name_ssid' },
            { data: 'signal_strength' },
            { data: 'device' },
            { data: '' }
        ],
        columnDefs: [
            {
                targets: -1,
                data: null,
                defaultContent: '<button>Click!</button>',
            },
        ],
    });
}