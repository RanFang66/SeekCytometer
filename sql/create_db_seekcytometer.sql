DROP DATABASE IF EXISTS "SeekCytometer";

CREATE DATABASE "SeekCytometer";

\c  "SeekCytometer";

\i trigger_functions.sql

CREATE TABLE Users (
    user_id          SERIAL PRIMARY KEY NOT NULL,
    user_name        VARCHAR(32) NOT NULL UNIQUE,
    user_admin       BOOLEAN NOT NULL,
    department       VARCHAR(32),
    email            VARCHAR(64)
);


CREATE TABLE Experiments (
    experiment_id   SERIAL PRIMARY KEY NOT NULL,
    user_id         INT NOT NULL, 
    experiment_name VARCHAR(64) NOT NULL,
    experiment_path TEXT,
    FOREIGN KEY (user_id) REFERENCES Users(user_id) ON DELETE CASCADE,
    UNIQUE (user_id, experiment_name)
);

CREATE TABLE Specimens (
    specimen_id         SERIAL PRIMARY KEY NOT NULL,
    experiment_id       INT NOT NULL,
    specimen_name       VARCHAR(64) NOT NULL,
    FOREIGN KEY (experiment_id) REFERENCES Experiments(experiment_id) ON DELETE CASCADE,
    UNIQUE (experiment_id, specimen_name)
);

CREATE TABLE Tubes (
    tube_id             SERIAL PRIMARY KEY NOT NULL,
    specimen_id         INT NOT NULL,
    tube_name           VARCHAR(64) NOT NULL,

    FOREIGN KEY (specimen_id) REFERENCES Specimens(specimen_id) ON DELETE CASCADE,
    UNIQUE (specimen_id, tube_name)
);

CREATE INDEX idx_experiments_user_id ON Experiments(user_id);
CREATE INDEX idx_specimens_experiment_id ON Specimens(experiment_id);
CREATE INDEX idx_tubes_specimen_id ON Tubes(specimen_id);


CREATE TYPE ParentType AS ENUM ('Experiment', 'Specimen', 'Tube');
CREATE TYPE ThresholdOperator AS ENUM ('OR', 'AND');

CREATE TABLE CytometerSettings (
    setting_id       SERIAL PRIMARY KEY NOT NULL,
    setting_name     VARCHAR(64) NOT NULL DEFAULT 'CytometerSettings',
    parent_type     ParentType NOT NULL,
    parent_id       INT NOT NULL,
    threshold_op    ThresholdOperator NOT NULL DEFAULT 'OR',

    CONSTRAINT fk_parent_experiment FOREIGN KEY (parent_id) REFERENCES Experiments(experiment_id) ON DELETE CASCADE,
    CONSTRAINT fk_parent_specimen FOREIGN KEY (parent_id) REFERENCES Specimens(specimen_id) ON DELETE CASCADE,
    CONSTRAINT fk_parent_tube FOREIGN KEY (parent_id) REFERENCES Tubes(tube_id) ON DELETE CASCADE,
    UNIQUE (parent_type, parent_id)
);

CREATE INDEX idx_settings_parent ON CytometerSettings(parent_type, parent_id);

CREATE TYPE NodeType AS ENUM ('User', 'Experiment', 'Specimen', 'Tube', 'Setting');
CREATE TABLE BrowserData (
    id SERIAL PRIMARY KEY NOT NULL,
    parent_id INT,
    node_name VARCHAR(64) NOT NULL,
    node_type NodeType NOT NULL,
    node_id INT NOT NULL,
    depth INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at  TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TRIGGER AfterUserInsert
AFTER INSERT ON Users
FOR EACH ROW
EXECUTE FUNCTION after_user_insert_function();

CREATE TRIGGER AfterUsersUpdate
AFTER UPDATE ON Users
FOR EACH ROW
EXECUTE FUNCTION after_user_update_function();

CREATE TRIGGER AfterUsersDelete
AFTER DELETE ON Users
FOR EACH ROW
EXECUTE FUNCTION after_user_delete_function();

CREATE TRIGGER AfterExperimentInsert
AFTER INSERT ON Experiments
FOR EACH ROW
EXECUTE FUNCTION after_experiment_insert_function();

CREATE TRIGGER AfterExperimentsUpdate
AFTER UPDATE ON Experiments
FOR EACH ROW
EXECUTE FUNCTION after_experiment_update_function();

CREATE TRIGGER AfterExperimentsDelete
AFTER DELETE ON Experiments
FOR EACH ROW
EXECUTE FUNCTION after_experiment_delete_function();


CREATE TRIGGER AfterSpecimensInsert
AFTER INSERT ON Specimens
FOR EACH ROW
EXECUTE FUNCTION after_specimen_insert_function();

CREATE TRIGGER AfterSpecimensUpdate
AFTER UPDATE ON Specimens
FOR EACH ROW
EXECUTE FUNCTION after_specimen_update_function();

CREATE TRIGGER AfterSpecimensDelete
AFTER DELETE ON Specimens
FOR EACH ROW
EXECUTE FUNCTION after_specimen_delete_function();

CREATE TRIGGER AfterTubesInsert
AFTER INSERT ON Tubes
FOR EACH ROW
EXECUTE FUNCTION after_tube_insert_function();

CREATE TRIGGER AfterTubesUpdate
AFTER UPDATE ON Tubes
FOR EACH ROW
EXECUTE FUNCTION after_tube_update_function();

CREATE TRIGGER AfterTubesDelete
AFTER DELETE ON Tubes
FOR EACH ROW
EXECUTE FUNCTION after_Tube_delete_function();


CREATE TRIGGER AfterCytometerSettingsInsert
AFTER INSERT ON CytometerSettings
FOR EACH ROW
EXECUTE FUNCTION after_settings_insert_function();

CREATE TRIGGER AfterCytometerSettingsUpdate
AFTER UPDATE ON CytometerSettings
FOR EACH ROW
EXECUTE FUNCTION after_settings_update_function();

CREATE TRIGGER AfterCytometerSettingsDelete
AFTER DELETE ON CytometerSettings
FOR EACH ROW
EXECUTE FUNCTION after_settings_delete_function();


CREATE TABLE Detectors (
    detector_id         SERIAL PRIMARY KEY NOT NULL,
    detector_name       VARCHAR(64) NOT NULL,
    detector_type       VARCHAR(64) NOT NULL,
    filter_peak         INT NOT NULL,
    filter_bandwidth    INT NOT NULL,
    default_gain        INT NOT NULL DEFAULT 100,
    default_offset      INT NOT NULL DEFAULT 0
);

CREATE TABLE DetectorSettings (
    setting_id        INT NOT NULL,
    detector_id      INT NOT NULL,
    parameter_name   VARCHAR(64) NOT NULL,
    detector_gain    INT NOT NULL DEFAULT 100,
    detector_offset  INT NOT NULL DEFAULT 0,
    enable_threshold BOOLEAN NOT NULL DEFAULT FALSE,
    threshold_value  INT NOT NULL DEFAULT 1000,
    enable_height    BOOLEAN NOT NULL DEFAULT FALSE,
    enable_width     BOOLEAN NOT NULL DEFAULT FALSE,
    enable_area      BOOLEAN NOT NULL DEFAULT FALSE,
    FOREIGN KEY (setting_id) REFERENCES CytometerSettings(setting_id) ON DELETE CASCADE,
    FOREIGN KEY (detector_id) REFERENCES Detectors(detector_id) ON DELETE CASCADE,
    PRIMARY KEY (setting_id, detector_id)
);



-- CREATE TYPE WorksheetParent AS ENUM('experiment', 'tube');
-- CREATE TABLE WorkSheets (
--     worksheet_id        SERIAL PRIMARY KEY NOT NULL,
--     worksheet_parent    WorksheetParent NOT NULL,
--     parent_id           INT NOT NULL,
--     worksheet_name      VARCHAR(64) NOT NULL,
--     CONSTRAINT fk_parent_worksheet FOREIGN KEY (parent_id) REFERENCES WorkSheets(worksheet_id) ON DELETE CASCADE,
--     CONSTRAINT fk_parent_tube FOREIGN KEY (parent_id) REFERENCES Tubes(tube_id) ON DELETE CASCADE,
--     UNIQUE (parent_id, worksheet_name)
-- );


-- CREATE TYPE PlotType AS ENUM('histogram', 'scatter', 'density', 'contour');
-- CREATE TABLE Plots (
--     plot_id         SERIAL PRIMARY KEY NOT NULL,
--     worksheet_id    INT NOT NULL,
--     plot_type       PlotType NOT NULL,
--     plot_name       VARCHAR(64) NOT NULL,
--     channel_id_x    INT NOT NULL,
--     channel_id_y    INT,
--     plot_data       JSON NOT NULL,
--     FOREIGN KEY (worksheet_id) REFERENCES WorkSheets(worksheet_id) ON DELETE CASCADE,
--     FOREIGN KEY (channel_id_x) REFERENCES ChannelSettings(setting_channel_id) ON DELETE CASCADE,
--     FOREIGN KEY (channel_id_y) REFERENCES ChannelSettings(setting_channel_id) ON DELETE CASCADE,
--     UNIQUE (worksheet_id, plot_name)
-- );

-- CREATE TYPE GateType AS ENUM('rectangle', 'polygon', 'interval', 'quadrant', 'ellipse');
-- CREATE TABLE GATES (
--     gate_id         SERIAL PRIMARY KEY NOT NULL,
--     worksheet_id    INT NOT NULL,
--     gate_name       VARCHAR(64) NOT NULL,
--     gate_type       GateType NOT NULL,
--     channel_id_x    INT NOT NULL,
--     channel_id_y    INT,
--     gate_data       JSON NOT NULL,
--     FOREIGN KEY (worksheet_id) REFERENCES WorkSheets(worksheet_id) ON DELETE CASCADE,
--     FOREIGN KEY (channel_id_x) REFERENCES ChannelSettings(setting_channel_id) ON DELETE CASCADE,
--     FOREIGN KEY (channel_id_y) REFERENCES ChannelSettings(setting_channel_id) ON DELETE CASCADE,
--     UNIQUE (worksheet_id, gate_name)
-- );





