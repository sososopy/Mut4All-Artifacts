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

pub struct Incorrect_Lifetime_Annotation_In_Struct_Definition_382;

impl Mutator for Incorrect_Lifetime_Annotation_In_Struct_Definition_382 {
    fn name(&self) -> &str {
        "Incorrect_Lifetime_Annotation_In_Struct_Definition_382"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = item_struct.generics.params.first() {
                    if let GenericParam::Lifetime(_) = generics {
                        let new_lifetime: Lifetime = parse_quote!('static);
                        let mut new_generics = item_struct.generics.clone();
                        new_generics.params = new_generics
                            .params
                            .into_iter()
                            .map(|param| match param {
                                GenericParam::Lifetime(_) => {
                                    GenericParam::Lifetime(LifetimeParam {
                                        attrs: Vec::new(),
                                        lifetime: new_lifetime.clone(),
                                        colon_token: None,
                                        bounds: Punctuated::new(),
                                    })
                                }
                                _ => param,
                            })
                            .collect();
                        item_struct.generics = new_generics;
                        let mut new_fields = item_struct.fields.clone();
                        for field in &mut new_fields {
                            if let Type::Reference(type_reference) = &mut field.ty {
                                if let Some(lifetime) = &type_reference.lifetime {
                                    if lifetime.ident != "static" {
                                        type_reference.lifetime = Some(new_lifetime.clone());
                                    }
                                }
                            }
                        }
                        item_struct.fields = new_fields;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions with lifetime annotations by replacing them with the reserved lifetime `'static`. It modifies both the struct's generic lifetime parameters and any lifetime annotations within the struct's fields. This transformation is designed to introduce lifetime-related errors, leveraging the compiler's strict lifetime analysis to uncover potential weaknesses in its handling of reserved lifetimes and lifetime resolution within generic contexts."
    }
}