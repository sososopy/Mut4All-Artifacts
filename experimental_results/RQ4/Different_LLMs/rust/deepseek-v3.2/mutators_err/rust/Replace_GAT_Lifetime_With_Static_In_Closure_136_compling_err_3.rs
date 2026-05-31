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

pub struct Replace_GAT_Lifetime_With_Static_In_Closure_136;

impl Mutator for Replace_GAT_Lifetime_With_Static_In_Closure_136 {
    fn name(&self) -> &str {
        "Replace_GAT_Lifetime_With_Static_In_Closure_136"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let TraitItem::Fn(trait_method) = trait_item {
                        if trait_method.default.is_some() {
                            let mut visitor = GatLifetimeVisitor::new();
                            visitor.visit_trait_item_fn_mut(trait_method);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait methods with default implementations that contain closure parameters using a GAT with an implicit lifetime. It replaces the GAT's lifetime parameter with 'static in the closure signature, specifically when the closure is typed with a 'static lifetime bound. This transformation aims to expose lifetime checking bugs by forcing a mismatch between the closure's expected lifetime and the GAT's actual lifetime constraints, potentially triggering compiler errors or unexpected behavior in trait method implementations."
    }
}

struct GatLifetimeVisitor {
    found: bool,
}

impl GatLifetimeVisitor {
    fn new() -> Self {
        Self { found: false }
    }
}

impl VisitMut for GatLifetimeVisitor {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            if let Some(last_segment) = type_path.path.segments.last_mut() {
                if last_segment.ident == "Box" {
                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                        if args.args.len() == 1 {
                            if let GenericArgument::Type(inner_type) = &mut args.args[0] {
                                if let Type::TraitObject(trait_object) = inner_type {
                                    if trait_object.dyn_token.is_some() {
                                        for bound in &mut trait_object.bounds {
                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                if trait_bound.path.segments.last().map(|s| s.ident.to_string()) == Some("FnMut".to_string()) {
                                                    if let Some(last_fn_segment) = trait_bound.path.segments.last_mut() {
                                                        if let PathArguments::AngleBracketed(fn_args) = &mut last_fn_segment.arguments {
                                                            if fn_args.args.len() == 1 {
                                                                if let GenericArgument::Type(arg_type) = &mut fn_args.args[0] {
                                                                if let Type::Path(arg_type_path) = arg_type {
                                    if arg_type_path.path.segments.len() >= 2 {
                                        let last_seg = arg_type_path.path.segments.last_mut().unwrap();
                                        if last_seg.ident == "Item" {
                                            let parent_seg = arg_type_path.path.segments.iter().nth(arg_type_path.path.segments.len() - 2).unwrap();
                                            if parent_seg.ident == "Self" {
                                                if let PathArguments::AngleBracketed(gat_args) = &mut last_seg.arguments {
                                                    if gat_args.args.len() == 1 {
                                                        if let GenericArgument::Lifetime(lifetime) = &mut gat_args.args[0] {
                                                            if lifetime.ident == "_" {
                                                                let static_lifetime = Lifetime::new("'static", Span::call_site());
                                                                *lifetime = static_lifetime;
                                                                self.found = true;
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
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}