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

pub struct Modify_Trait_Bound_310;

impl Mutator for Modify_Trait_Bound_310 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_310"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ImplTraitReturnVisitor;

        impl VisitMut for ImplTraitReturnVisitor {
            fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
                if let ReturnType::Type(_, ref mut ty) = i.sig.output {
                    if let Type::ImplTrait(ref mut impl_trait) = **ty {
                        let mut new_bounds = Punctuated::<TypeParamBound, Plus>::new();
                        for bound in &impl_trait.bounds {
                            new_bounds.push(bound.clone());
                        }
                        new_bounds.push(parse_quote!(CoerceUnsized));
                        impl_trait.bounds = new_bounds;
                    }
                }
                syn::visit_mut::visit_item_fn_mut(self, i);
            }
        }

        ImplTraitReturnVisitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with return types using `impl Trait`. It modifies the trait bounds by adding `CoerceUnsized` to the existing bounds. This manipulation aims to explore interactions between `impl Trait` and trait objects, potentially leading to mismatches and compiler errors."
    }
}