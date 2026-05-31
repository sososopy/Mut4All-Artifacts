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

pub struct Replace_Tuple_Element_With_NonObjectSafe_Trait_66;

impl Mutator for Replace_Tuple_Element_With_NonObjectSafe_Trait_66 {
    fn name(&self) -> &str {
        "Replace_Tuple_Element_With_NonObjectSafe_Trait_66"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                for field in &mut s.fields {
                    if let syn::Type::Tuple(tuple) = &mut field.ty {
                        for i in 0..tuple.elems.len() - 1 {
                            if let syn::Type::Path(path) = &tuple.elems[i] {
                                let mut bounds = Punctuated::new();
                                bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                    path: path.path.clone(),
                                    modifier: syn::TraitBoundModifier::None,
                                    paren_token: None,
                                }));
                                let new_type = syn::Type::TraitObject(syn::TypeTraitObject {
                                    dyn_token: Some(token::Dyn::default()),
                                    bounds,
                                });
                                tuple.elems[i] = new_type;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}