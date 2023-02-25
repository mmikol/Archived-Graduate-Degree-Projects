package com.example.artistsearch;

import org.json.JSONException;
import org.json.JSONObject;

public class Gene {
    private String name = "";
    private String image = "";
    private String description = "";

    public Gene(JSONObject data) throws JSONException {
        name = data.getString("name");
        image = data.getString("image");
        description = data.getString("description");
    }

    public String getName() {
        return name;
    }
    public String getImage() {
        return image;
    }
    public String getDescription() { return description; }
}
