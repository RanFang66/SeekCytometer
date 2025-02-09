\c "SeekCytometer"

INSERT INTO CytometerSettings (setting_name, parent_type, experiment_id, threshold_op) 
VALUES ('TestSetting1', 'Experiment', 1, 'AND');

SELECT * FROM BrowserData WHERE node_type = 'Settings';