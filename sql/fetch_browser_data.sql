\c "SeekCytometer"
-- SELECT * FROM BrowserData        WITH RECURSIVE UserHierarchy AS (
WITH RECURSIVE UserHierarchy AS (
    SELECT
        id,
        parent_id,
        node_name,
        node_type,
        node_id,
        depth,
        created_at,
        updated_at
    FROM BrowserData
    WHERE node_type = 'User' AND node_id = 1

    UNION ALL

    SELECT
        bd.id,
        bd.parent_id,
        bd.node_name,
        bd.node_type,
        bd.node_id,
        bd.depth,
        bd.created_at,
        bd.updated_at
    FROM BrowserData bd
    INNER JOIN UserHierarchy uh ON bd.parent_id = uh.id
)
SELECT * FROM UserHierarchy ORDER BY depth, node_type, id;