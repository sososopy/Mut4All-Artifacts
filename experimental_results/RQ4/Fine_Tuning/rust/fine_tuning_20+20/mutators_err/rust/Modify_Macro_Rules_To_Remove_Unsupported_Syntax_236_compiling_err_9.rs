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

pub struct Modify_Macro_Rules_To_Remove_Unsupported_Syntax_236;

impl Mutator for Modify_Macro_Rules_To_Remove_Unsupported_Syntax_236 {
    fn name(&self) -> &str {
        "Modify_Macro_Rules_To_Remove_Unsupported_Syntax_236"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(macro_item) = item {
                if macro_item.mac.path.is_ident("macro_rules") {
                    if let Ok(macro_rules) = syn::parse2::<syn::Macro>(macro_item.mac.tokens.clone()) {
                        let mut transformed = false;
                        let mut rules = macro_rules.tokens.clone();
                        rules = rules
                            .into_iter()
                            .map(|mut transform| {
                                if let TokenTree::Group(group) = &mut transform {
                                    let mut stream = TokenStream::new();
                                    for mut token in group.stream() {
                                        if let TokenTree::Ident(ident) = &mut token {
                                            if ident == "box" {
                                                *ident = Ident::new("Box::new", ident.span());
                                                stream.extend(quote!((#ident)));
                                                transformed = true;
                                                continue;
                                            }
                                        }
                                        stream.extend(quote!(#token));
                                    }
                                    transform = TokenTree::Group(Group::new(group.delimiter(), stream));
                                }
                                transform
                            })
                            .collect();
                        if transformed {
                            macro_item.mac.tokens = quote!(#rules);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Locate macro_rules! definitions and identify usages of the deprecated `box` syntax. Replace `box` with `Box::new()` to ensure compatibility with current Rust syntax, maintaining the macro's functionality while adhering to updated language standards."
    }
}