use proc_macro2::{Span, TokenTree};
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

pub struct Invalid_Format_String_In_Error_Attribute_475;

impl Mutator for Invalid_Format_String_In_Error_Attribute_475 {
    fn name(&self) -> &str {
        "Invalid_Format_String_In_Error_Attribute_475"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                if item_enum.attrs.iter().any(|attr| attr.path().is_ident("derive") && attr.tokens().to_string().contains("Error")) {
                    for variant in &mut item_enum.variants {
                        for attr in &mut variant.attrs {
                            if attr.path().is_ident("error") {
                                if let syn::Meta::NameValue(meta_name_value) = attr.parse_meta().unwrap() {
                                    if let syn::Lit::Str(lit_str) = meta_name_value.lit {
                                        let original = lit_str.value();
                                        let mutated = original.replace("{0}", "{0.invalid}");
                                        attr.tokens = quote! { = #mutated };
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum variants with `#[error(\"...\")]` attributes within enums derived from `Debug, Error`. It modifies the format string in the `#[error(\"...\")]` attribute by introducing an invalid format specifier (`{0.invalid}`). This transformation aims to test how the proc-macro system handles incorrect or unexpected input in error attributes, potentially uncovering bugs in format string parsing or error handling."
    }
}