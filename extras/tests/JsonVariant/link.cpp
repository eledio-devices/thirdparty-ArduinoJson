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

    variant.link(doc2.as<JsonVariant>());

    CHECK(variant.as<std::string>() == "{\"hello\":\"world\"}");
    CHECK(variant.memoryUsage() == 0);

    // altering the linked document should change the result
    doc2["hello"] = "WORLD!";

    CHECK(variant.as<std::string>() == "{\"hello\":\"WORLD!\"}");
  }
}
