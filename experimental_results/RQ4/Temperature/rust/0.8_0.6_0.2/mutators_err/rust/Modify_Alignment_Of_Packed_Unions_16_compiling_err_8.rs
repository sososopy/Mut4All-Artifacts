use proc_macro2::{Span, TokenStream, TokenTree};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Lit, Meta, MetaList, MetaNameValue,
    Pat, PatType, Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier,
    Type, TypeImplTrait, TypeParamBound, TypePath, parse_quote,
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

pub struct Modify_Alignment_Of_Packed_Unions_16;

impl Mutator for Modify_Alignment_Of_Packed_Unions_16 {
    fn name(&self) -> &str {
        "Modify_Alignment_Of_Packed_Unions_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Union(union) = item {
                let mut is_packed = false;
                let mut new_attrs = Vec::new();
                
                for attr in &union.attrs {
                    if attr.path().is_ident("repr") {
                        let mut tokens = Vec::new();
                        attr.parse_nested_meta(|meta| {
                            if let Some(ident) = meta.path.get_ident() {
                                if ident == "packed" {
                                    is_packed = true;
                                }
                            }
                            if let Meta::NameValue(MetaNameValue { lit: Lit::Int(lit_int), .. }) = meta {
                                if let Ok(old_alignment) = lit_int.base10_parse::<u64>() {
                                    let new_alignment = if old_alignment == 1 { 2 } else { old_alignment + 1 };
                                    tokens.push(TokenTree::Literal(proc_macro2::Literal::u64_unsuffixed(new_alignment)));
                                }
                            }
                            Ok(())
                        }).unwrap();

                        new_attrs.push(Attribute {
                            pound_token: attr.pound_token,
                            style: attr.style,
                            bracket_token: attr.bracket_token,
                            meta: Meta::List(MetaList {
                                path: attr.path().clone(),
                                delimiter: syn::MacroDelimiter::Paren(Default::default()),
                                tokens: quote! { (#(#tokens),*) },
                            }),
                        });
                    } else {
                        new_attrs.push(attr.clone());
                    }
                }

                if is_packed {
                    union.attrs = new_attrs;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets unions with the `#[repr(packed)]` attribute, modifying their alignment values. It incrementally increases the alignment to test for ABI and layout alignment mismatches, aiming to expose potential bugs in the handling of packed unions during compilation."
    }
}