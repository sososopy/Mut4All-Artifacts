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

pub struct Modify_Impl_Trait_Return_Type_107;

impl Mutator for Modify_Impl_Trait_Return_Type_107 {
    fn name(&self) -> &str {
        "Modify_Impl_Trait_Return_Type_107"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ImplTraitVisitor<'a> {
            modified: &'a mut bool,
        }

        impl<'a> VisitMut for ImplTraitVisitor<'a> {
            fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
                if let ReturnType::Type(_, ref mut ty) = i.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = **ty {
                        if let Some(first_bound) = type_impl_trait.bounds.first() {
                            let new_type: Type = parse_quote!((fn() -> #first_bound,));

                            *ty = Box::new(new_type);
                            *self.modified = true;
                        }
                    }
                }
                syn::visit_mut::visit_item_fn_mut(self, i);
            }
        }

        let mut modified = false;
        let mut visitor = ImplTraitVisitor { modified: &mut modified };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies functions with `impl Trait` return types, particularly those involved in recursive type definitions or trait interactions. It modifies the return type to a specific type that satisfies the trait bounds, replacing `impl Trait` with a concrete type. This transformation aids in exploring compiler behavior regarding concrete types versus `impl Trait`, especially in recursive contexts."
    }
}