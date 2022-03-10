// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::link()") {
  StaticJsonDocument<1024> doc1, doc2;
  JsonVariant variant = doc1.to<JsonVariant>();

  SECTION("JsonVariant::link(JsonDocument&)") {
    doc2["hello"] = "world";

    variant.link(doc2);

    CHECK(variant.as<std::string>() == "{\"hello\":\"world\"}");
    CHECK(variant.memoryUsage() == 0);

    // altering the linked document should change the result
    doc2["hello"] = "WORLD!";

    CHECK(variant.as<std::string>() == "{\"hello\":\"WORLD!\"}");
  }

  SECTION("JsonVariant::link(MemberProxy)") {
    doc2["obj"]["hello"] = "world";

    variant.link(doc2["obj"]);

    CHECK(variant.as<std::string>() == "{\"hello\":\"world\"}");
    CHECK(variant.memoryUsage() == 0);

    // altering the linked document should change the result
    doc2["obj"]["hello"] = "WORLD!";

    CHECK(variant.as<std::string>() == "{\"hello\":\"WORLD!\"}");
  }

  SECTION("JsonVariant::link(ElementProxy)") {
    doc2[0]["hello"] = "world";

    variant.link(doc2[0]);

    CHECK(variant.as<std::string>() == "{\"hello\":\"world\"}");
    CHECK(variant.memoryUsage() == 0);

    // altering the linked document should change the result
    doc2[0]["hello"] = "WORLD!";

    CHECK(variant.as<std::string>() == "{\"hello\":\"WORLD!\"}");
  }
}

TEST_CASE("Linked document") {
  StaticJsonDocument<1024> doc1, doc2;
  JsonVariant variant = doc1.to<JsonVariant>();
  doc2["hello"] = "world";
  variant.link(doc2);

  SECTION("is<T>()") {
    CHECK(variant.is<JsonArrayConst>() == false);
    CHECK(variant.is<JsonObjectConst>() == true);
    CHECK(variant.is<JsonObject>() == false);
  }

  SECTION("get member") {
    CHECK(variant["hello"].as<std::string>() == "world");
  }

  SECTION("set member") {
    // The link is read-only; the following line should have no side effect
    variant["tutu"] = "toto";

    CHECK(doc1.as<std::string>() == "{\"hello\":\"world\"}");
  }
}
