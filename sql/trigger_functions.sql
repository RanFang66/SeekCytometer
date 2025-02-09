CREATE OR REPLACE FUNCTION after_user_insert_function()
RETURNS TRIGGER AS $$
BEGIN
    INSERT INTO BrowserData (parent_id, node_name, node_type, node_id, depth)
    VALUES (0, NEW.user_name, 'User', NEW.user_id, 1);
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION after_user_update_function()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE BrowserData
    SET 
        node_name = NEW.user_name,
        updated_at = NOW()
    WHERE node_type = 'User' AND node_id = NEW.user_id;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION after_user_delete_function()
RETURNS TRIGGER AS $$
BEGIN
    WITH RECURSIVE NodesToDelete AS (
        SELECT id
        FROM BrowserData
        WHERE node_type = 'User' AND node_id = OLD.user_id
        UNION ALL
        SELECT ts.id
        FROM BrowserData ts
        INNER JOIN NodesToDelete ntd ON ts.parent_id = ntd.id
    )
    DELETE FROM BrowserData WHERE id IN (SELECT id FROM NodesToDelete);
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION after_experiment_insert_function() 
RETURNS TRIGGER AS $$
DECLARE
    parentNodeId INT;
BEGIN
    SELECT id INTO parentNodeId
    FROM BrowserData
    WHERE node_type = 'User' AND node_id = NEW.user_id
    LIMIT 1;

    IF parentNodeId IS NULL THEN
        RAISE EXCEPTION 'Parent User node not found in BrowserData for user_id %', NEW.user_id;
    END IF;

    INSERT INTO BrowserData (parent_id, node_name, node_type, node_id, depth) 
    VALUES (parentNodeId, NEW.experiment_name, 'Experiment', NEW.experiment_id, 2);

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION after_experiment_update_function()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE BrowserData
    SET 
        node_name = NEW.experiment_name,
        updated_at = NOW()
    WHERE node_type = 'Experiment' AND node_id = NEW.experiment_id;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION after_experiment_delete_function()
RETURNS TRIGGER AS $$
BEGIN
    WITH RECURSIVE NodesToDelete AS (
        SELECT id
        FROM BrowserData
        WHERE node_type = 'Experiment' AND node_id = OLD.experiment_id
        UNION ALL
        SELECT ts.id
        FROM BrowserData ts
        INNER JOIN NodesToDelete ntd ON ts.parent_id = ntd.id
    )
    DELETE FROM BrowserData WHERE id IN (SELECT id FROM NodesToDelete);
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION after_specimen_insert_function()
RETURNS TRIGGER AS $$
DECLARE
    parentNodeId INT;
BEGIN
    SELECT id INTO parentNodeId
    FROM BrowserData
    WHERE node_type = 'Experiment' AND node_id = NEW.experiment_id
    LIMIT 1;

    IF parentNodeId IS NULL THEN
        RAISE EXCEPTION 'Parent Experiment node not found in BrowserData for experiment_id %', NEW.experiment_id;
    END IF;

    INSERT INTO BrowserData (parent_id, node_name, node_type, node_id, depth) 
    VALUES (parentNodeId, NEW.specimen_name, 'Specimen', NEW.specimen_id, 3);

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION after_specimen_update_function()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE BrowserData
    SET 
        node_name = NEW.specimen_name,
        updated_at = NOW()
    WHERE node_type = 'Specimen' AND node_id = NEW.specimen_id;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION after_specimen_delete_function()
RETURNS TRIGGER AS $$
BEGIN
    WITH RECURSIVE NodesToDelete AS (
        SELECT id
        FROM BrowserData
        WHERE node_type = 'Specimen' AND node_id = OLD.specimen_id
        UNION ALL
        SELECT ts.id
        FROM BrowserData ts
        INNER JOIN NodesToDelete ntd ON ts.parent_id = ntd.id
    )
    DELETE FROM BrowserData WHERE id IN (SELECT id FROM NodesToDelete);
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION after_tube_insert_function()
RETURNS TRIGGER AS $$
DECLARE
    parentNodeId INT;
BEGIN
    SELECT id INTO parentNodeId
    FROM BrowserData
    WHERE node_type = 'Specimen' AND node_id = NEW.specimen_id
    LIMIT 1;

    IF parentNodeId IS NULL THEN
        RAISE EXCEPTION 'Parent Specimen node not found in BrowserData for specimen_id %', NEW.specimen_id;
    END IF;

    INSERT INTO BrowserData (parent_id, node_name, node_type, node_id, depth) 
    VALUES (parentNodeId, NEW.tube_name, 'Tube', NEW.tube_id, 4);

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION after_tube_update_function()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE BrowserData
    SET 
        node_name = NEW.tube_name,
        updated_at = NOW()
    WHERE node_type = 'Tube' AND node_id = NEW.tube_id;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION after_tube_delete_function()
RETURNS TRIGGER AS $$
BEGIN
    WITH RECURSIVE NodesToDelete AS (
        SELECT id
        FROM BrowserData
        WHERE node_type = 'Tube' AND node_id = OLD.tube_id
        UNION ALL
        SELECT ts.id
        FROM BrowserData ts
        INNER JOIN NodesToDelete ntd ON ts.parent_id = ntd.id
    )
    DELETE FROM BrowserData WHERE id IN (SELECT id FROM NodesToDelete);
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION after_settings_insert_function()
RETURNS TRIGGER AS $$
DECLARE
    parentNodeId INT;
BEGIN
    SELECT id INTO parentNodeId
    FROM BrowserData
    WHERE 
        (NEW.parent_type = 'Experiment' AND node_type = 'Experiment' AND node_id = NEW.experiment_id) OR
        (NEW.parent_type = 'Specimen' AND node_type = 'Specimen' AND node_id = NEW.specimen_id) OR
        (NEW.parent_type = 'Tube' AND node_type = 'Tube' AND node_id = NEW.tube_id)
    LIMIT 1;

    INSERT INTO BrowserData (parent_id, node_name, node_type, node_id, depth, created_at)
    VALUES (
        parentNodeId,
        NEW.setting_name,
        'Settings',
        NEW.setting_id,
        COALESCE((SELECT depth + 1 FROM BrowserData WHERE id = parentNodeId), 0),
        NOW()
    );

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION after_settings_update_function()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE BrowserData
    SET 
        node_name = NEW.setting_name,
        updated_at = NOW()
    WHERE node_type = 'Settings' AND node_id = NEW.setting_id;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION after_settings_delete_function()
RETURNS TRIGGER AS $$
BEGIN
    WITH RECURSIVE NodesToDelete AS (
        SELECT id
        FROM BrowserData
        WHERE node_type = 'Settings' AND node_id = OLD.setting_id
        UNION ALL
        SELECT ts.id
        FROM BrowserData ts
        INNER JOIN NodesToDelete ntd ON ts.parent_id = ntd.id
    )
    DELETE FROM BrowserData WHERE id IN (SELECT id FROM NodesToDelete);

    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

