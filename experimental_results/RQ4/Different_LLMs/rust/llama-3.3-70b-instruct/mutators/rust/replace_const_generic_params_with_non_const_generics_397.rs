use syn::{GenericParam, TypeParam, punctuated::Punctuated};
use crate::mutator::Mutator;

struct Replace_Const_Generic_Params_With_Non_Const_Generics_397;

impl Mutator for Replace_Const_Generic_Params_With_Non_Const_Generics_397 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Params_With_Non_Const_Generics_397"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for param in &mut item_impl.generics.params {
                    if let GenericParam::Const(param_const) = param {
                        let new_param = GenericParam::Type(TypeParam {
                            attrs: param_const.attrs.clone(),
                            ident: param_const.ident.clone(),
                            colon_token: Some(param_const.colon_token.clone()),
                            bounds: Punctuated::new(),
                            default: None,
                            eq_token: None,
                        });
                        *param = new_param;
                    }
                }
            }

            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(param_const) = param {
                        let new_param = GenericParam::Type(TypeParam {
                            attrs: param_const.attrs.clone(),
                            ident: param_const.ident.clone(),
                            colon_token: Some(param_const.colon_token.clone()),
                            bounds: Punctuated::new(),
                            default: None,
                            eq_token: None,
                        });
                        *param = new_param;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with non-const generic parameters in trait implementations and function definitions. This transformation removes the const constraint, potentially exposing bugs in the Rust compiler related to generic const expressions."
    }
}