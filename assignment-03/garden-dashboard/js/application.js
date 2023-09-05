$(document).ready(function() {

    /* this parameter is used to check that the
    update page has been accessed for the right
    reasons and not from an end user.
    */
    var update = true;

    /* Updates the page from the element with 
    "hw-wrapper" as ID. */
    $.ajax({
        url: 'update.php',
        type: 'post',
        data: {'update': update},
        success: function(result) {
            $("#hw-wrapper").html(result);
        }
    });

    /* Creates a function that updates the page every 
    X seconds. */
    function refresh() {
        $.ajax({
            url: 'update.php',
            type: 'post',
            data: {'update': update},
            success: function(result) {
                $("#hw-wrapper").html(result);
            }
        });
    }

    setInterval(function() {
        refresh()
    }, 5000); //5 seconds
})