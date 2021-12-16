/*
 * This JavaScript doesn't do anything. The file exists just to demonstrate
 * including static assets from the HTML in themes.
 */

hljs.highlightAll();

$(".nav-group").click(function() {
	let nestedNav = $(this).parent().find(".nested-nav-el");
	if (nestedNav.hasClass("is-hidden")) {
		// Arrow pointing down
		$(this).find(".nav-arrow").text("\u25BE");
		nestedNav.removeClass("is-hidden");
	} else {
		// Arrow pointing to the right
		$(this).find(".nav-arrow").text("\u25B8");
		nestedNav.addClass("is-hidden");
	}
});

// Expand navburger on small screens
document.addEventListener('DOMContentLoaded', () => {
	// Get all "navbar-burger" elements
	const $navbarBurgers = Array.prototype.slice.call(document.querySelectorAll('.navbar-burger'), 0);

	// Check if there are any navbar burgers
	if ($navbarBurgers.length > 0) {

		// Add a click event on each of them
		$navbarBurgers.forEach( el => {
			el.addEventListener('click', () => {

				// Get the target from the "data-target" attribute
				const target = el.dataset.target;
				const $target = document.getElementById(target);

				// Toggle the "is-active" class on both the "navbar-burger" and the "navbar-menu"
				el.classList.toggle('is-active');
				$target.classList.toggle('is-active');
				$target.classList.toggle('bottom-border');
			});
		});
	}
});
