use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *, str::FromStr};
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

pub struct Inner_To_Outer_Attribute_Conversion_257;

impl Mutator for Inner_To_Outer_Attribute_Conversion_257 {
    fn name(&self) -> &str {
        "Inner_To_Outer_Attribute_Conversion_257"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(item_macro) = item {
                if item_macro.mac.path.is_ident("proc_macro") {
                    if let Ok(output_tokens) = item_macro.mac.tokens.to_string().parse::<TokenStream>() {
                        let mut output_string = output_tokens.to_string();
                        let inner_attr_pattern = Regex::new(r"#!\[(.*?)\]").unwrap();
                        output_string = inner_attr_pattern.replace_all(&output_string, "#[$1]").to_string();
                        let new_tokens = TokenStream::from_str(&output_string).unwrap();
                        item_macro.mac.tokens = new_tokens;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets procedural macro outputs by converting inner attributes (#![...] to outer attributes (#[...]). This transformation exploits potential compiler weaknesses in attribute context handling within macros, aiming to trigger ICEs or unexpected behavior by altering the attribute semantics in generated code."
    }
}