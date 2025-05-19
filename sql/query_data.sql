
\c "SeekCytometer"
SELECT
        p.plot_id,
        p.plot_type,
        p.plot_name,
        p.plot_size,
        p.x_axis_id,
        dx.parameter_name AS x_axis_parameter_name,
        p.x_measure_type,
        p.y_axis_id,
        dy.parameter_name AS y_axis_parameter_name,
        p.y_measure_type
        FROM Plots p
        LEFT JOIN DetectorSettings dx ON p.x_axis_id = dx.detector_setting_id
        LEFT JOIN DetectorSettings dy ON p.y_axis_id = dy.detector_setting_id
        WHERE p.worksheet_id = 1;
-- SELECT * FROM Gates LEFT JOIN PopulationRelation ON Gates.gate_id = PopulationRelation.gate_id LEFT JOIN Populations ON PopulationRelation.population_id = Populations.population_id;

-- SELECT * FROM Populations;