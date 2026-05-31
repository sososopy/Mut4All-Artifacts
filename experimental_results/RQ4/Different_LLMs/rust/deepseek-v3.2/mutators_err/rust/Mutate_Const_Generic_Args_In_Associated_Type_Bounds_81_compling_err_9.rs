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

pub struct Mutate_Const_Generic_Args_In_Associated_Type_Bounds_81;

impl Mutator for Mutate_Const_Generic_Args_In_Associated_Type_Bounds_81 {
    fn name(&self) -> &str {
        "Mutate_Const_Generic_Args_In_Associated_Type_Bounds_81"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_def) = item {
                for trait_item in &mut trait_def.items {
                    if let syn::TraitItem::Type(assoc_type) = trait_item {
                        for bound in &mut assoc_type.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let mut visitor = ConstGenericVisitor::new();
                                visitor.visit_trait_bound_mut(trait_bound);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait definitions where an associated type has a bound involving another trait with a const generic parameter. It replaces the const expression in the associated type bound with another const expression of the same type, derived from existing constants or simple literals (0, 1, -1). This transformation aims to expose bugs in the compiler's handling of const generics within associated type bounds, particularly in type checking and trait solving, which are likely to trigger internal compiler errors."
    }
}

struct ConstGenericVisitor {
    rng: rand::rngs::ThreadRng,
    constants: Vec<syn::Expr>,
}

impl ConstGenericVisitor {
    fn new() -> Self {
        Self {
            rng: thread_rng(),
            constants: vec![
                parse_quote! { 0 },
                parse_quote! { 1 },
                parse_quote! { -1 },
                parse_quote! { true },
                parse_quote! { false },
                parse_quote! { 'a' },
            ],
        }
    }

    fn visit_trait_bound_mut(&mut self, trait_bound: &mut syn::TraitBound) {
        self.visit_path_mut(&mut trait_bound.path);
    }

    fn visit_path_mut(&mut self, path: &mut syn::Path) {
        for segment in &mut path.segments {
            self.visit_path_segment_mut(segment);
        }
    }

    fn visit_path_segment_mut(&mut self, segment: &mut syn::PathSegment) {
        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
            for arg in &mut args.args {
                if let GenericArgument::Const(const_expr) = arg {
                    let new_expr = self.constants.choose(&mut self.rng).unwrap().clone();
                    *const_expr = new_expr;
                }
            }
        }
    }
}