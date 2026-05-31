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

pub struct Alter_Macro_Definition_Out_Of_Range_21;

impl Mutator for Alter_Macro_Definition_Out_Of_Range_21 {
    fn name(&self) -> &str {
        "Alter_Macro_Definition_Out_Of_Range_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if let Some((_, ref mut tokens)) = mac.mac.tokens.clone().into_iter().next() {
                    let mut new_tokens = TokenStream::new();
                    for token in tokens.clone() {
                        match token {
                            TokenTree::Literal(lit) => {
                                if let Ok(int_val) = lit.to_string().parse::<i128>() {
                                    if int_val < i128::MAX / 10 {
                                        new_tokens.extend(quote! { 999999999999999999999999999999999999 });
                                    } else {
                                        new_tokens.extend(quote! { #lit });
                                    }
                                } else {
                                    new_tokens.extend(quote! { #lit });
                                }
                            }
                            _ => new_tokens.extend(quote! { #token }),
                        }
                    }
                    mac.mac.tokens = new_tokens;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macros with numeric literals, replacing them with excessively large values to test the compiler's handling of out-of-range literals. This transformation will likely trigger warnings or errors, testing the robustness of the compiler's linting and error diagnostics for numeric overflow scenarios."
    }
}