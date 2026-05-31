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

pub struct Modify_Type_Alias_With_Non_Implementing_Type_476;

impl Mutator for Modify_Type_Alias_With_Non_Implementing_Type_476 {
    fn name(&self) -> &str {
        "Modify_Type_Alias_With_Non_Implementing_Type_476"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_alias_name = None;

        for item in &mut file.items {
            if let Item::Type(item_type) = item {
                if let Type::ImplTrait(type_impl_trait) = &*item_type.ty {
                    type_alias_name = Some(item_type.ident.clone());
                }
            }
        }

        if let Some(type_alias_name) = type_alias_name {
            let mut found_assignment = false;

            for item in &mut file.items {
                if let Item::Static(item_static) = item {
                    if let Type::Path(type_path) = &*item_static.ty {
                        if type_path.path.is_ident(&type_alias_name) {
                            item_static.expr = Box::new(parse_quote! { "string" });
                            found_assignment = true;
                        }
                    }
                }
            }

            if !found_assignment {
                file.items.push(parse_quote! {
                    static FOO1: #type_alias_name = "string";
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}