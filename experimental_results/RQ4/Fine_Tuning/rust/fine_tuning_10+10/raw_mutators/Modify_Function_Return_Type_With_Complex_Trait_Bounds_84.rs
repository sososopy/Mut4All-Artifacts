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

pub struct Modify_Function_Return_Type_With_Complex_Trait_Bounds_84;

impl Mutator for Modify_Function_Return_Type_With_Complex_Trait_Bounds_84 {
    fn name(&self) -> &str {
        "Modify_Function_Return_Type_With_Complex_Trait_Bounds_84"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **ty {
                        let mut existing_traits = Vec::new();
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                existing_traits.push(trait_bound.clone());
                            }
                        }
                        if existing_traits.len() == 1 {
                            let new_trait: TraitBound = parse_quote!(Debug);
                            type_impl_trait.bounds.push(TypeParamBound::Trait(new_trait));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies functions with a return type of `impl Trait` and modifies the return type to include additional trait bounds. The operator ensures that the new trait bounds are relevant by checking the existing implemented traits and adds a new trait, such as `Debug`, to form a complex trait bound like `impl TraitA + Debug`. This requires minimal changes to the function body to ensure the new traits are satisfied."
    }
}