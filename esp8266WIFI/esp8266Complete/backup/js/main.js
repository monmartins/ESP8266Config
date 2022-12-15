
$(document).ready(function () {

    // $("#body_switch").html("test")

});


function getData(key) {
    data = localStorage.getItem(key);
    if (data === null) {
      return false;
    } else {
      return JSON.parse(data);
    }
}

function setData(key, value) {

    if (key === undefined || value === undefined) {
        return false;
    } else {
        localStorage.setItem(key, JSON.stringify(value));
        return true;
    }
}