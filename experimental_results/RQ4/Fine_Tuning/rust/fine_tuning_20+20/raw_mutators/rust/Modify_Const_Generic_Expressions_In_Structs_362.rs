use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Const_Generic_Expressions_In_Structs_362;

impl Mutator for Modify_Const_Generic_Expressions_In_Structs_362 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Expressions_In_Structs_362"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_const_generic = false;
                for param in &item_struct.generics.params {
                    if matches!(param, GenericParam::Const(_)) {
                        has_const_generic = true;
                        break;
                    }
                }
                if has_const_generic {
                    let struct_ident = &item_struct.ident;
                    for item in &mut file.items {
                        if let Item::Impl(item_impl) = item {
                            if let Some((_, path, _)) = &item_impl.trait_ {
                                if path.is_ident(struct_ident) {
                                    for impl_item in &mut item_impl.items {
                                        if let ImplItem::Type(impl_type) = impl_item {
                                            let mut expr_visitor = ConstExprVisitor {
                                                found: false,
                                            };
                                            expr_visitor.visit_type(&impl_type.ty);
                                            if expr_visitor.found {
                                                impl_type.ty = Box::new(parse_quote! {
                                                    Store<{ O + I + undefined_var }>
                                                });
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
        "This mutator targets structs with const generics, specifically those that involve const expressions in their associated types or implementations. It identifies such expressions and modifies them by introducing an undefined or mismatched arithmetic operation, like adding an undefined variable, to stress-test the compiler's handling of const generics and expression evaluation."
    }
}

struct ConstExprVisitor {
    found: bool,
}

impl<'ast> Visit<'ast> for ConstExprVisitor {
    fn visit_expr(&mut self, node: &'ast Expr) {
        if matches!(node, Expr::Binary(_) | Expr::Unary(_) | Expr::Lit(_)) {
            self.found = true;
        }
        syn::visit::visit_expr(self, node);
    }
}