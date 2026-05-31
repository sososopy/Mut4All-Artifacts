use syn::parse_quote;
use rand::thread_rng;
use rand::Rng;
use crate::mutator::Mutator;

struct Replace_Cfg_Attribute_With_Invalid_Argument_237;

impl Mutator for Replace_Cfg_Attribute_With_Invalid_Argument_237 {
    fn name(&self) -> &str {
        "Replace_Cfg_Attribute_With_Invalid_Argument_237"
    }

    fn chain_of_thought(&self) -> &str {
        "Replace cfg attribute with invalid argument to induce internal compiler errors"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for attr in &mut func.attrs {
                    if let syn::Attribute { path, tokens: args, .. } = attr {
                        if path.is_ident("cfg") {
                            let mut rng = thread_rng();
                            let invalid_arg: u8 = rng.gen_range(0..3);
                            match invalid_arg {
                                0 => {
                                    *args = parse_quote!( = );
                                }
                                1 => {
                                    *tokens = parse_quote!( some_identifier );
                                }
                                2 => {
                                    *tokens = parse_quote!( "some_string" );
                                }
                                _ => {}
                            }
                        }
                    }
                }
            }
        }
    }
}