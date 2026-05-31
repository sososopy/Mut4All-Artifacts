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

pub struct Duplicate_Repr_Attribute_183;

impl Mutator for Duplicate_Repr_Attribute_183 {
    fn name(&self) -> &str {
        "Duplicate_Repr_Attribute_183"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            let attrs = match item {
                Item::Struct(s) => &mut s.attrs,
                Item::Enum(e) => &mut e.attrs,
                _ => continue,
            };
            let repr_attrs: Vec<_> = attrs.iter().enumerate().filter(|(_, attr)| {
                attr.path().segments.iter().any(|seg| seg.ident == "repr")
            }).collect();
            if repr_attrs.is_empty() {
                continue;
            }
            let (first_idx, first_attr) = repr_attrs[0];
            let repr_content = first_attr.meta.require_list().ok().map(|list| list.clone());
            if let Some(content) = repr_content {
                let duplicate_attr = syn::Attribute {
                    pound_token: token::Pound::default(),
                    style: first_attr.style,
                    bracket_token: token::Bracket::default(),
                    meta: syn::Meta::List(syn::MetaList {
                        path: first_attr.path().clone(),
                        delimiter: content.delimiter,
                        tokens: content.tokens.clone(),
                    }),
                };
                let insert_pos = if rng.gen_bool(0.5) { first_idx } else { first_idx + 1 };
                attrs.insert(insert_pos, duplicate_attr);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator duplicates the first repr attribute found on structs and enums, inserting the duplicate immediately before or after the original attribute. This creates syntactically valid but semantically conflicting layout directives, potentially triggering compiler errors related to duplicate or conflicting repr specifiers, layout computation, or attribute validation."
    }
}