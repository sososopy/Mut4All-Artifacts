use proc_macro2::{TokenStream, TokenTree};
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

pub struct Insert_Out_Of_Range_Literal_In_Macros_21;

impl Mutator for Insert_Out_Of_Range_Literal_In_Macros_21 {
    fn name(&self) -> &str {
        "Insert_Out_Of_Range_Literal_In_Macros_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(macro_item) = item {
                let mut new_tokens = TokenStream::new();
                for token in macro_item.mac.tokens.clone() {
                    if let TokenTree::Literal(lit) = token {
                        if let Ok(int_lit) = lit.to_string().parse::<i128>() {
                            let out_of_range_lit = int_lit.saturating_add(1000);
                            new_tokens.extend(quote! { #out_of_range_lit });
                        } else {
                            new_tokens.extend(quote! { #lit });
                        }
                    } else {
                        new_tokens.extend(quote! { #token });
                    }
                }
                macro_item.mac.tokens = new_tokens;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}