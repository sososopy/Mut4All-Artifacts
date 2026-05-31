use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Invalid_Format_String_In_Error_Attribute_252;

impl Mutator for Invalid_Format_String_In_Error_Attribute_252 {
    fn name(&self) -> &str {
        "Invalid_Format_String_In_Error_Attribute_252"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let has_debug_error = item_enum.attrs.iter().any(|attr| {
                    attr.path().is_ident("derive") && attr.meta.to_token_stream().to_string().contains("Debug, Error")
                });
                if has_debug_error {
                    for variant in &mut item_enum.variants {
                        for attr in &mut variant.attrs {
                            if attr.path().is_ident("error") {
                                let invalid_format = parse_quote! { "{0.invalid} occurred" };
                                attr.meta = syn::Meta::List(syn::MetaList {
                                    path: attr.path.clone(),
                                    paren_token: syn::token::Paren(Span::call_site()),
                                    nested: syn::punctuated::Punctuated::from_iter(vec![syn::NestedMeta::Lit(syn::Lit::Str(invalid_format))]),
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum variants with `#[error(\"...\")]` attributes in enums derived with `Debug, Error`. It modifies the format string to include invalid specifiers, such as `{0.invalid}`, to provoke errors in the proc-macro system, testing its resilience against unexpected input."
    }
}