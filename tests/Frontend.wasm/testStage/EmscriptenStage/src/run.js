var Module = {
	postRun: [() => {
		Module['Stuff'] = {
			Inner: {
				giveBack: Module['giveBack'],
			},
		};
	}],
};

