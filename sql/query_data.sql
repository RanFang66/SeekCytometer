
\c "SeekCytometer"
            SELECT
                u.user_id AS user_id,
                u.user_name AS user_name,
                u.created_at AS user_created_at,
                e.experiment_id AS experiment_id,
                e.experiment_name AS experiment_name,
                e.created_at AS experiment_created_at,
                s.specimen_id AS specimen_id,
                s.specimen_name AS specimen_name,
                s.created_at AS specimen_created_at,
                t.tube_id AS tube_id,
                t.tube_name AS tube_name,
                t.created_at AS tube_created_at,
                st.config_id AS config_id,
                st.parent_type AS config_parent_type,
                st.created_at AS config_created_at
            FROM Users u
            LEFT JOIN Experiments e
                ON u.user_id = e.user_id
            LEFT JOIN Specimens s
                ON e.experiment_id = s.experiment_id
            LEFT JOIN Tubes t
                ON s.specimen_id = t.specimen_id
            LEFT JOIN CytometerSettings st
                ON (
                    (st.parent_type = 'Experiment' AND st.parent_id = e.experiment_id) OR
                    (st.parent_type = 'Specimen' AND st.parent_id = s.specimen_id) OR
                    (st.parent_type = 'Tube' AND st.parent_id = t.tube_id)
                )
            ORDER BY u.user_id, e.experiment_id, s.specimen_id, t.tube_id, st.config_id;
