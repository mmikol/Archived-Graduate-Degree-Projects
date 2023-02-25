package com.example.artistsearch;

import org.json.JSONException;
import org.json.JSONObject;

public class Artist {
    private String id = "";
    private String name = "";
    private String birthday = "";
    private String deathday = "";
    private String nationality = "";
    private String biography = "";
    public Artist(JSONObject details) throws JSONException {
        if (details != null) {
            if (details.has("id")) id = details.getString("id");
            if (details.has("name")) name = details.getString("name");
            if (details.has("birthday")) birthday = details.getString("birthday");
            if (details.has("deathday")) deathday = details.getString("deathday");
            if (details.has("nationality")) nationality = details.getString("nationality");
            if (details.has("biography")) biography = details.getString("biography");
        }
    }

    public String getID() { return id; }
    public String getName() { return name; }
    public String getBirthday() { return birthday; }
    public String getDeathday() { return deathday; }
    public String getNationality() { return nationality; }
    public String getBiography() { return biography; }
}
