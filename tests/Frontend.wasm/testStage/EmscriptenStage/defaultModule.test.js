const m = require('./defaultModule');

test('Default test', () => {
	m.onRuntimeInitialized = function() {
		expect(m.sayTen()).toBe(10);
	};
});
