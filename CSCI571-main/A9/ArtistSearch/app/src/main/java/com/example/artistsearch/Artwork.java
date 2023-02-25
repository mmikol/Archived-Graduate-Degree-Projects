package com.example.artistsearch;

import org.json.JSONException;
import org.json.JSONObject;

public class Artwork {
    private String id = "";
    private String title = "";
    private String image = "";

    public Artwork(JSONObject data) throws JSONException {
        if (data != null) {
            id = data.getString("id");
            title = data.getString("title");
            image = data.getString("image");
        }
    }

    public String getID() {
        return id;
    }

    public String getTitle() {
        return title;
    }

    public String getImage() { return image; }
}
