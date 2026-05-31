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

pub struct Modify_Static_Lifetime_Bound_In_Structs_78;

impl Mutator for Modify_Static_Lifetime_Bound_In_Structs_78 {
    fn name(&self) -> &str {
        "Modify_Static_Lifetime_Bound_In_Structs_78"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_static_lifetime = false;
                for field in &mut item_struct.fields {
                    if let Type::Reference(type_reference) = &mut field.ty {
                        if let Some(lifetime) = &type_reference.lifetime {
                            if lifetime.ident == "static" {
                                has_static_lifetime = true;
                                type_reference.lifetime = Some(Lifetime::new("'a", Span::call_site()));
                            }
                        }
                    }
                }
                if has_static_lifetime {
                    item_struct.generics.params.push(GenericParam::Lifetime(LifetimeParam::new(Lifetime::new("'a", Span::call_site()))));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}