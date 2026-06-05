
  auto wait_handle = tc::g_postbox->wait(
    "UpdateInputs"sv, [=](const msgpack::object& obj) -> bool {

  });

