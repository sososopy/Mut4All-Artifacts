use proc_macro2::{Span, *};
use quote::*;
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Modify_Const_Expression_In_Associated_Type_417;

impl Mutator for Modify_Const_Expression_In_Associated_Type_417 {
    fn name(&self) -> &str {
        "Modify_Const_Expression_In_Associated_Type_417"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.last().unwrap().ident == "Parent" {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Type(impl_type) = impl_item {
                                if impl_type.ident == "Mapping" {
                                    if let Expr::Block(expr_block) = &*impl_type.ty {
                                        if let Some(Stmt::Expr(Expr::Binary(expr_binary))) =
                                            expr_block.block.stmts.first()
                                        {
                                            if let Expr::Path(left) = &*expr_binary.left {
                                                if let Some(ident) = left.path.get_ident() {
                                                    if ident == "O" {
                                                        let new_expr: Expr = parse_quote! { O * I };
                                                        impl_type.ty = Box::new(Type::Verbatim(
                                                            quote! { Store<{ #new_expr }> },
                                                        ));
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
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct implementations with const generics, specifically modifying associated types that use const expressions. By changing the mathematical operation in the const expression of the associated type, it tests the compiler's handling of const generics, potentially exposing issues in type substitution and const evaluation."
    }
}