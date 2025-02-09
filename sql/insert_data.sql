\c  "SeekCytometer";

-- 添加测试数据到 Users 表
INSERT INTO Users (user_name, user_admin, department, email)
VALUES
    ('Admin', TRUE, 'SeekGene', 'alice@domain.com'),
    ('RanFang', FALSE, 'SeekoneDD', 'bob@domain.com'),
    ('Test', FALSE, 'SeekMate', 'abc@domain.com');

-- 添加测试数据到 Experiments 表
INSERT INTO Experiments (user_id, experiment_name, experiment_path)
VALUES
    (1, 'Cell Study', '/experiments/cell_study'),
    (2, 'Quantum Analysis', '/experiments/quantum_analysis'),
    (3, 'Chemical Reactions', '/experiments/chemical_reactions');

-- 添加测试数据到 Specimens 表
INSERT INTO Specimens (experiment_id, specimen_name)
VALUES
    (1, 'K562'),
    (1, 'HeLa'),
    (2, 'YAC-1'),
    (3, 'Specimen D');

-- 添加测试数据到 Tubes 表
INSERT INTO Tubes (specimen_id, tube_name)
VALUES
    (1, 'Tube 1A'),
    (1, 'Tube 1B'),
    (2, 'Tube 2A'),
    (3, 'Tube 3A'),
    (3, 'Tube 3B');

-- 添加测试数据到 Channels 表
INSERT INTO Detectors (detector_name, detector_type, filter_peak, filter_bandwidth)
VALUES
    ('Channel 1', 'PMT', 488, 100),
    ('Channel 2', 'PMT', 563, 100),
    ('Channel 3', 'PMT', 615, 100),
    ('Channel 4', 'PMT', 488, 100),
    ('Channel 5', 'PMT', 615, 100),
    ('Channel 6', 'PMT', 488, 100),
    ('Channel 7', 'PMT', 398, 100),
    ('Channel 8', 'PMT', 563, 100);

-- 添加测试数据到 Settings 表
INSERT INTO CytometerSettings (parent_type, experiment_id, threshold_op)
VALUES ('Experiment', 1, 'OR');

INSERT INTO CytometerSettings (parent_type, specimen_id, threshold_op)
VALUES  ('Specimen', 1, 'AND');

INSERT INTO CytometerSettings (parent_type, tube_id, threshold_op)
VALUES ('Tube', 1, 'OR');

-- 添加测试数据到 ChannelSettings 表
INSERT INTO DetectorSettings (setting_id, detector_id, parameter_name, detector_gain, detector_offset, enable_height, enable_width, enable_area)
VALUES
    (1, 1, 'FSC', 100, 0, TRUE, FALSE, FALSE),
    (1, 2, 'SSC', 100, 0, TRUE, FALSE, FALSE),
    (1, 3, 'FITC', 100, 0, TRUE, FALSE, FALSE),
    (2, 1, 'AO', 100, 0, TRUE, FALSE, FALSE),
    (2, 2, 'PI', 100, 0, TRUE, FALSE, FALSE),
    (2, 3, 'GFP', 100, 0, TRUE, TRUE, FALSE),
    (3, 1, 'FSC', 100, 0, TRUE, FALSE, FALSE),
    (3, 2, 'SSC', 100, 0, TRUE, FALSE, FALSE),
    (3, 3, 'FITC', 100, 0, TRUE, FALSE, FALSE);

-- -- 添加测试数据到 WorkSheets 表
-- INSERT INTO WorkSheets (worksheet_parent, parent_id, worksheet_name)
-- VALUES
--     ('experiment', 1, 'Worksheet 1'),
--     ('experiment', 2, 'Worksheet 2'),
--     ('tube', 1, 'Worksheet 1');
--     ('tube', 2, 'Worksheet 2');

-- -- 添加测试数据到 Plots 表
-- INSERT INTO Plots (worksheet_id, plot_type, plot_name, channel_id_x, channel_id_y, plot_data)
-- VALUES
--     (1, 'scatter', 'Plot 1', 1, 2, '{"x": [1, 2, 3], "y": [4, 5, 6]}'),
--     (2, 'histogram', 'Plot 2', 1, NULL, '{"bins": [1, 2, 3], "values": [10, 20, 30]}');

-- -- 添加测试数据到 Gates 表
-- INSERT INTO Gates (worksheet_id, gate_name, gate_type, channel_id_x, channel_id_y, gate_data)
-- VALUES
--     (1, 'Gate 1', 'rectangle', 1, 2, '{"x_range": [1, 2], "y_range": [3, 4]}'),
--     (2, 'Gate 2', 'ellipse', 1, 2, '{"center": [5, 5], "radius": [2, 3]}');
