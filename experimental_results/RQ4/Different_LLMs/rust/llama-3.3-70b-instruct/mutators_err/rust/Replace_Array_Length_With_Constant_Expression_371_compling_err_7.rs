use syn::{parse_quote, Item, Type, TypeArray, Expr, ExprLet, Pat, PatType};
use crate::mutator::Mutator;

struct Replace_Array_Length_With_Constant_Expression_371;

impl Mutator for Replace_Array_Length_With_Constant_Expression_371 {
    fn name(&self) -> &str {
        "Replace_Array_Length_With_Constant_Expression_371"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                if let Type::Array(TypeArray { elem, len,.. }) = *const_item.ty {
                    let new_len = parse_quote!(10 + 5);
                    *const_item.ty = Type::Array(TypeArray {
                        elem,
                        len: new_len,
                        semi_token: Default::default(),
                        bracket_token: Default::default(),
                    });
                }
            }
            if let Item::Const(const_item) = item {
                if let Expr::Let(ExprLet { ref mut pat,.. }) = *const_item.expr {
                    if let Pat::Type(PatType { ref mut ty,.. }) = **pat {
                        if let Type::Array(TypeArray { elem, len,.. }) = *ty {
                            let new_len = parse_quote!(10 + 5);
                            *ty = Type::Array(TypeArray {
                                elem,
                                len: new_len,
                                semi_token: Default::default(),
                                bracket_token: Default::default(),
                            });
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the length of array declarations with a constant expression, introducing potential bugs related to constant evaluation."
    }
}