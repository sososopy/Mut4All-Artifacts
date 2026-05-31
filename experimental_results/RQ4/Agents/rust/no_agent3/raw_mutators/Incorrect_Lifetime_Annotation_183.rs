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

pub struct Incorrect_Lifetime_Annotation_183;

impl Mutator for Incorrect_Lifetime_Annotation_183 {
    fn name(&self) -> &str {
        "Incorrect_Lifetime_Annotation_183"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics.params.first() {
                    if let GenericParam::Lifetime(lifetime_param) = generics {
                        let incorrect_lifetime = Lifetime::new("'static", Span::call_site());
                        lifetime_param.lifetime = incorrect_lifetime.clone();

                        for field in &mut item_struct.fields {
                            if let Type::Reference(type_ref) = &mut field.ty {
                                if let Some(lifetime) = &mut type_ref.lifetime {
                                    *lifetime = incorrect_lifetime.clone();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions with lifetime annotations. It replaces the correct lifetime with a reserved or incorrect lifetime name, such as 'static. This change is applied consistently throughout the struct definition to maintain syntactic correctness. The mutation exploits Rust's strict lifetime management to potentially reveal compiler bugs."
    }
}