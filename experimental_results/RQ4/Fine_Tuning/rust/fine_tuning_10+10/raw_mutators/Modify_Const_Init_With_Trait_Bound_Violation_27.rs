use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_Const_Init_With_Trait_Bound_Violation_27;

impl Mutator for Modify_Const_Init_With_Trait_Bound_Violation_27 {
    fn name(&self) -> &str {
        "Modify_Const_Init_With_Trait_Bound_Violation_27"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if let Type::Path(type_path) = &*item_const.ty {
                    if let Some(last_segment) = type_path.path.segments.last() {
                        if let PathArguments::AngleBracketed(angle_bracketed) = &last_segment.arguments {
                            for arg in &angle_bracketed.args {
                                if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                    if let Some(inner_segment) = type_path.path.segments.last() {
                                        if let PathArguments::AngleBracketed(inner_angle_bracketed) = &inner_segment.arguments {
                                            for inner_arg in &inner_angle_bracketed.args {
                                                if let GenericArgument::Type(Type::Path(inner_type_path)) = inner_arg {
                                                    if let Some(inner_type_segment) = inner_type_path.path.segments.last() {
                                                        if inner_type_segment.ident == "Trait" {
                                                            if let Expr::Reference(expr_ref) = &*item_const.expr {
                                                                if let Expr::Call(expr_call) = &*expr_ref.expr {
                                                                    if let Expr::Path(expr_path) = &*expr_call.func {
                                                                        if let Some(last_expr_segment) = expr_path.path.segments.last() {
                                                                            if last_expr_segment.ident == last_segment.ident {
                                                                                if let Some(Expr::Lit(expr_lit)) = expr_call.args.first() {
                                                                                    if let syn::Lit::Bool(_) = &expr_lit.lit {
                                                                                        let mutated_expr: Expr = parse_quote! { &#last_expr_segment.ident(true, 42) };
                                                                                        item_const.expr = Box::new(mutated_expr);
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
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "Identify const initializations using a struct or type alias with a trait bound. Check if the structure has a trait bound. Mutate the const initialization by assigning a value that violates the trait bound."
    }
}