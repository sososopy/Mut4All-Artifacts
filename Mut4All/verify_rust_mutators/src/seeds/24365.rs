use attributes :: * ; pub struct Method { pub access_flags : u16 , pub name_idx : u16 , pub descriptor_idx : u16 , pub attrs_count : u16 , pub attributes : Vec < Attribute > , } impl Method { pub fn to_string (& self) -> String { let mut string_rep = format ! ("Method:\n\
                \t- access_flags=0x{:x}\n\
                \t- name_idx={}\n\
                \t- descriptor_idx={}\n\
                \t- attrs_count={}\n" , self . access_flags , self . name_idx , self . descriptor_idx , self . attrs_count) ; for attr in self . attributes { string_rep = string_rep + format ! ("\tAttributes:\n\
                \t\t- attr_name_idx={}\n\
                \t\t- attr_length={}\n" , attr . attr_name_idx , attr . attr_length) ; } string_rep } }