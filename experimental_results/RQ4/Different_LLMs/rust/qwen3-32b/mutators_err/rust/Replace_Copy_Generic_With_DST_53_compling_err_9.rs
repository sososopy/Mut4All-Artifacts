use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    parse_quote,
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

pub struct Replace_Copy_Generic_With_DST_53;

impl Mutator for Replace_Copy_Generic_With_DST_53 {
    fn name(&self) -> &str {
        "Replace_Copy_Generic_With_DST_53"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut copy_structs = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                for param in &struct_item.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.is_ident("Copy") {
                                    copy_structs.push(struct_item.ident.clone());
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                let mut new_type = None;
                let mut struct_name = None;

                if let Some(ty) = const_item.ty.as_ref().map(|boxed| &**boxed) {
                    if let Type::Path(p) = ty {
                        let path = p;
                        let segments = &path.path.segments;
                        if let Some(first_segment) = segments.first() {
                            if copy_structs.contains(&first_segment.ident) {
                                if let PathArguments::AngleBracketed(args) = &first_segment.arguments {
                                    if !args.args.is_empty() {
                                        let new_type_ast = parse_quote! {
                                            #first_segment< [u8] >
                                        };
                                        new_type = Some(new_type_ast);
                                        struct_name = Some(first_segment.ident.clone());
                                    }
                                }
                            }
                        }
                    }
                }

                if let Some(nt) = new_type {
                    const_item.ty = Some(Box::new(nt));
                    if let Some(expr_box) = const_item.expr.as_mut().map(|boxed_expr| &mut *boxed_expr) {
                        modify_expr(expr_box, &struct_name.unwrap());
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structs with a 'T: Copy' bound and modifies const declarations that instantiate them with concrete types. It replaces the type argument with a DST (e.g., [u8] or str) that does not satisfy Copy, creating a scenario where the type system's constraints are violated during const evaluation. This may trigger ICEs in the compiler's handling of DSTs and Copy traits, particularly in const contexts."
    }
}

fn modify_expr(expr: &mut Expr, struct_name: &Ident) {
    if let Expr::Call(expr_call) = expr {
        if let Expr::Path(func_path) = &*expr_call.func {
            let path = &func_path.path;
            let segments = &path.segments;
            if let Some(first_segment) = segments.first() {
                if first_segment.ident == *struct_name {
                    let new_arg = parse_quote! { [0; 4] };
                    expr_call.args = Punctuated::new();
                    expr_call.args.push(new_arg);
                }
            }
        }
    }
}