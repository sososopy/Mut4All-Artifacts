use syn::{parse_quote, Item, Generics, GenericParam, Expr};
use crate::mutator::Mutator;

struct Replace_Const_Generic_With_Macro_Expansion_110;

impl Mutator for Replace_Const_Generic_With_Macro_Expansion_110 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Macro_Expansion_110"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let Generics::None = struct_item.generics {
                    continue;
                }
                let generics = struct_item.generics.clone().unwrap();
                for param in generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(const_value) = const_param.default {
                            if let Expr::Macro(mac) = const_value {
                                if let Some(mac_ident) = mac.mac.path.get_ident() {
                                    if mac_ident.to_string() == "wrap" {
                                        let tokens: Vec<_> = mac.mac.tokens.into_iter().collect();
                                        if let Some(path) = tokens.get(0) {
                                            if let Expr::Path(path_expr) = path {
                                                if let Some(path_ident) = path_expr.path.get_ident() {
                                                    let new_const_value = parse_quote! { #path_ident };
                                                    const_param.default = Some(new_const_value);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic values with their macro-expanded equivalents. It targets code constructs that use const generics in conjunction with macros, aiming to expose bugs related to the interaction between these two features."
    }
}