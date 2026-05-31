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

pub struct Modify_Trait_Object_Syntax_In_Type_Alias_256;

impl Mutator for Modify_Trait_Object_Syntax_In_Type_Alias_256 {
    fn name(&self) -> &str {
        "Modify_Trait_Object_Syntax_In_Type_Alias_256"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Type(type_alias) = item {
                let mut has_trait_object = false;
                let mut has_dyn = false;
                type_alias.ty.visit_with(&mut |ty| {
                    if let Type::TraitObject(_) = ty {
                        has_trait_object = true;
                    }
                    if let Type::TraitObject(type_trait_object) = ty {
                        if type_trait_object.dyn_token.is_some() {
                            has_dyn = true;
                        }
                    }
                });
                if has_trait_object && !has_dyn {
                    type_alias.ty = Box::new(Type::TraitObject(TypeTraitObject {
                        dyn_token: Some(token::Dyn::default()),
                        bounds: {
                            let mut bounds = Punctuated::new();
                            type_alias.ty.visit_with(&mut |ty| {
                                if let Type::TraitObject(type_trait_object) = ty {
                                    for bound in &type_trait_object.bounds {
                                        bounds.push(bound.clone());
                                    }
                                }
                            });
                            bounds
                        },
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets type aliases that use trait objects without the `dyn` keyword. It transforms such aliases to explicitly use `dyn`, ensuring that all trait object syntax is modernized to the `dyn Trait` form. This change stresses the compiler's trait resolution and type checking systems, particularly in how they handle legacy trait object syntax and the explicit `dyn` keyword."
    }
}