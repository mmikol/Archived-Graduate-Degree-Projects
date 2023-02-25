package com.example.artistsearch;

import org.json.JSONException;
import org.json.JSONObject;

public class ArtistSearchResult {
    private final String id;
    private final String name;
    private final String image;
    public ArtistSearchResult(JSONObject data) throws JSONException {
        id = data.getString("id");
        name = data.getString("name");
        if (data.getString("image").equals("/assets/shared/missing_image.png")) {
            image = "https://upload.wikimedia.org/wikipedia/commons/thumb/f/fc/Artsy_logo.svg/1024px-Artsy_logo.svg.png";
        } else {
            image = data.getString("image");
        }
    }

    public String getID() {
        return id;
    }
    public String getName() {
        return name;
    }
    public String getImage() {
        return image;
    }
}
