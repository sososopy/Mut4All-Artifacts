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

pub struct Replace_ImplTrait_Return_With_AsRef_Dyn_161;

impl Mutator for Replace_ImplTrait_Return_With_AsRef_Dyn_161 {
    fn name(&self) -> &str {
        "Replace_ImplTrait_Return_With_AsRef_Dyn_161"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let bounds = &type_impl_trait.bounds;
                        if bounds.is_empty() {
                            continue;
                        }
                        let first_bound = bounds.first().unwrap();
                        if let TypeParamBound::Trait(trait_bound) = first_bound {
                            let trait_path = &trait_bound.path;
                            let trait_name = trait_path.segments.last().unwrap().ident.clone();
                            let new_return_type: Type = parse_quote! {
                                impl AsRef<dyn #trait_name(&())>
                            };
                            item_fn.sig.output = ReturnType::Type(Default::default(), Box::new(new_return_type));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions that have a return type of impl Trait. It changes the return type to impl AsRef<dyn Trait(&())>, where Trait is the original trait from the impl Trait. This transformation introduces a wrapper trait (AsRef) and a trait object with an anonymous lifetime, aiming to trigger internal compiler errors in the lowering phase for impl Trait with nested lifetimes and references, similar to reported bugs. The function body remains unchanged, causing type mismatches and exposing edge cases in compiler's handling of impl Trait and lifetime interactions."
    }
}