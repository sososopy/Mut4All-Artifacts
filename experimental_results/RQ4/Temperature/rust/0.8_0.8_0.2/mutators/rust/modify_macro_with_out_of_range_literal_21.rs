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

pub struct Modify_Macro_With_Out_Of_Range_Literal_21;

impl Mutator for Modify_Macro_With_Out_Of_Range_Literal_21 {
    fn name(&self) -> &str {
        "Modify_Macro_With_Out_Of_Range_Literal_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if let Some(macro_rules) = mac.mac.path.segments.last() {
                    if macro_rules.ident == "macro_rules" {
                        if let Ok(syn::ItemMacro { mac: macro_content, .. }) = syn::parse2::<syn::ItemMacro>(mac.mac.tokens.clone()) {
                            for tt in macro_content.tokens {
                                if let TokenTree::Literal(lit) = tt {
                                    if let Ok(literal_value) = lit.to_string().parse::<i64>() {
                                        let out_of_range_literal = "9999999999999999999999999999999999999999";
                                        mac.mac.tokens = quote! { #out_of_range_literal };
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
        "This mutation operator identifies macro rules producing numeric literals and replaces those literals with a very large out-of-range value, pushing the compiler to handle scenarios that could lead to range overflows or related diagnostics."
    }
}