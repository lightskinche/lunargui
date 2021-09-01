image = lunar.load_image("cool.png")

lunar.register_button({"DisplayText",0,0,250,250},{255,0,255,0},{255,255,255,0},"callback_test",nil)
lunar.register_button({"DisplayText",300,250,250,250},{0,0,255,0},{255,255,255,0},"callback_test2",nil)
lunar.register_button({"DisplayText",300,0,250,250},{0,255,0,0},{255,255,255,0},nil,"callback_test")