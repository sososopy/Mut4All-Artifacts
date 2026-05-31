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

pub struct Replace_Fn_Ptr_With_Higher_Ranked_In_Const_58;

impl Mutator for Replace_Fn_Ptr_With_Higher_Ranked_In_Const_58 {
    fn name(&self) -> &str {
        "Replace_Fn_Ptr_With_Higher_Ranked_In_Const_58"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut found_fn_ptr = false;
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    let mut visitor = FnPtrVisitor {
                        found_fn_ptr: false,
                    };
                    visitor.visit_item_fn_mut(item_fn);
                    found_fn_ptr = visitor.found_fn_ptr;
                }
            }
        }
        if !found_fn_ptr {
            let new_fn: ItemFn = parse_quote! {
                const fn injected_fn_ptr_mutator(param: fn()) -> bool {
                    unsafe { param == param }
                }
            };
            file.items.push(Item::Fn(new_fn));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const functions, scanning for function pointer types (fn() or for<'a> fn()). It swaps between non-higher-ranked and higher-ranked forms, introducing or removing for<'a> bounds. If no function pointer exists, it injects a new const function with a fn() parameter and a comparison operation, then mutates it. This directly creates mismatches that may trigger ICEs in const evaluation when handling higher-ranked trait bounds and function pointer equality."
    }
}

struct FnPtrVisitor {
    found_fn_ptr: bool,
}

impl VisitMut for FnPtrVisitor {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Path(TypePath { qself: None, path }) = node {
            if path.segments.len() == 1 && path.segments[0].ident == "fn" {
                let segment = &mut path.segments[0];
                if segment.arguments.is_none() {
                    let new_arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                        colon2_token: None,
                        lt_token: token::Lt::default(),
                        args: Punctuated::from_iter(vec![GenericArgument::Lifetime(Lifetime::new("'a", Span::call_site()))]),
                        gt_token: token::Gt::default(),
                    });
                    segment.arguments = new_arguments;
                    self.found_fn_ptr = true;
                } else if let PathArguments::AngleBracketed(args) = &segment.arguments {
                    if args.args.len() == 1 {
                        if let GenericArgument::Lifetime(_) = &args.args[0] {
                            segment.arguments = PathArguments::None;
                            self.found_fn_ptr = true;
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
    fn visit_fn_arg_mut(&mut self, node: &mut FnArg) {
        if let FnArg::Typed(PatType { ty, .. }) = node {
            self.visit_type_mut(&mut *ty);
        }
        syn::visit_mut::visit_fn_arg_mut(self, node);
    }
    fn visit_return_type_mut(&mut self, node: &mut ReturnType) {
        if let ReturnType::Type(_, ty) = node {
            self.visit_type_mut(&mut *ty);
        }
        syn::visit_mut::visit_return_type_mut(self, node);
    }
    fn visit_local_mut(&mut self, node: &mut Local) {
        if let Some(init) = &mut node.init {
            self.visit_expr_mut(&mut init.expr);
        }
        syn::visit_mut::visit_local_mut(self, node);
    }
}