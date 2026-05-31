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

pub struct Lifetime_Parameter_Mutation_383;

impl Mutator for Lifetime_Parameter_Mutation_383 {
    fn name(&self) -> &str {
        "Lifetime_Parameter_Mutation_383"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_lifetime = false;
                let mut rng = thread_rng();

                // Check if the struct has lifetime parameters
                if let Some(generics) = &mut item_struct.generics.params.iter_mut().find_map(|param| {
                    if let GenericParam::Lifetime(lifetime) = param {
                        Some(lifetime)
                    } else {
                        None
                    }
                }) {
                    has_lifetime = true;

                    // Choose an existing lifetime or introduce a new one
                    let lifetime_to_modify = if rng.gen_bool(0.5) {
                        generics.lifetime.clone()
                    } else {
                        Lifetime::new("'t", Span::call_site())
                    };

                    // Modify the chosen lifetime to `'static`
                    generics.lifetime = Lifetime::new("'static", Span::call_site());

                    // Ensure the lifetime is used in at least one field
                    if !has_lifetime {
                        item_struct.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: vec![],
                            lifetime: lifetime_to_modify.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));

                        if let Some(field) = item_struct.fields.iter_mut().next() {
                            if let Type::Reference(type_ref) = &mut field.ty {
                                type_ref.lifetime = Some(lifetime_to_modify.clone());
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct declarations, modifying existing lifetime parameters or introducing a new one with the reserved `'static` lifetime. This transformation aims to provoke lifetime resolution errors and test the compiler's handling of lifetime parameters in structs, potentially revealing bugs in lifetime management and inference."
    }
}