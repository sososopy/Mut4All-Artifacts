use crate::mutator::Mutator;
use syn::{Expr, ExprPath, GenericParam, Item, ItemStruct};

pub struct Modify_Const_Generic_Params_162;

impl Mutator for Modify_Const_Generic_Params_162 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Params_162"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, .. }) = item {
                for param in &mut generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default_expr) = &mut const_param.default {
                            if let Expr::Path(ExprPath { path, .. }) = default_expr {
                                let ident = &path.segments.last().unwrap().ident;
                                let new_expr: Expr = syn::parse_quote! { #ident + 1 };
                                *default_expr = new_expr;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets struct definitions with const generic parameters that have default values. It modifies the default value expression by adding 1 to the existing constant, ensuring the mutation remains within valid bounds. This mutation aims to explore potential compiler bugs related to const generic parameters."
    }
}