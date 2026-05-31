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

pub struct Replace_For_HRTB_With_Simple_Bound_395;

impl Mutator for Replace_For_HRTB_With_Simple_Bound_395 {
    fn name(&self) -> &str {
        "Replace_For_HRTB_With_Simple_Bound_395"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct HrtbReplacer;
        impl VisitMut for HrtbReplacer {
            fn visit_type_param_bound_mut(&mut self, bound: &mut TypeParamBound) {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    if let Some(ref lifetimes) = trait_bound.lifetimes {
                        if !lifetimes.lifetimes.is_empty() {
                            let mut new_bound = trait_bound.clone();
                            new_bound.lifetimes = None;
                            self.visit_trait_bound_mut(&mut new_bound);
                            *trait_bound = new_bound;
                        }
                    }
                }
                syn::visit_mut::visit_type_param_bound_mut(self, bound);
            }
            fn visit_trait_bound_mut(&mut self, bound: &mut TraitBound) {
                if let Some(ref lifetimes) = bound.lifetimes {
                    if !lifetimes.lifetimes.is_empty() {
                        let mut new_path = bound.path.clone();
                        self.visit_path_mut(&mut new_path);
                        bound.path = new_path;
                        bound.lifetimes = None;
                    }
                }
                syn::visit_mut::visit_trait_bound_mut(self, bound);
            }
            fn visit_path_mut(&mut self, path: &mut SynPath) {
                for segment in &mut path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Type(ty) = arg {
                                self.visit_type_mut(ty);
                            }
                        }
                    }
                }
                syn::visit_mut::visit_path_mut(self, path);
            }
            fn visit_type_mut(&mut self, ty: &mut Type) {
                match ty {
                    Type::Reference(type_ref) => {
                        if let Some(ref lifetime) = type_ref.lifetime {
                            if lifetime.ident != "_" {
                                type_ref.lifetime = Some(Lifetime::new("'_", lifetime.span()));
                            }
                        }
                        self.visit_type_mut(&mut type_ref.elem);
                    }
                    Type::Path(type_path) => {
                        for segment in &mut type_path.path.segments {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Lifetime(lifetime) = arg {
                                        if lifetime.ident != "_" {
                                            *lifetime = Lifetime::new("'_", lifetime.span());
                                        }
                                    }
                                }
                            }
                        }
                    }
                    _ => {}
                }
                syn::visit_mut::visit_type_mut(self, ty);
            }
        }
        let mut replacer = HrtbReplacer;
        replacer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets higher-ranked trait bounds (HRTBs) in const contexts, removing the `for<...>` quantifier and simplifying the bound. It replaces specific lifetime parameters with anonymous lifetimes to maintain syntactic validity. This transformation aims to alter trait resolution behavior, potentially triggering compiler errors related to bound variable resolution in const generic contexts."
    }
}