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

pub struct Expand_Macro_With_Nested_Scopes_22;

impl Mutator for Expand_Macro_With_Nested_Scopes_22 {
    fn name(&self) -> &str {
        "Expand_Macro_With_Nested_Scopes_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(macro_item) = item {
                if let Some(macro_rules) = macro_item.mac.path.get_ident() {
                    if macro_rules == "macro_rules" {
                        if let Ok(macro_content) = syn::parse2::<syn::ItemMacro>(macro_item.mac.tokens.clone()) {
                            let mut new_tokens = TokenStream::new();
                            for token in macro_content.mac.tokens {
                                new_tokens.extend(Some(token.clone()));
                                if let TokenTree::Group(group) = &token {
                                    if group.delimiter() == Delimiter::Brace {
                                        let mut nested_tokens = TokenStream::new();
                                        nested_tokens.extend(quote! {
                                            let x = 10;
                                            if x > 5 {
                                                println!("Inside conditional");
                                                for i in 0..x {
                                                    println!("Iteration: {}", i);
                                                }
                                            }
                                        });
                                        new_tokens.extend(Some(TokenTree::Group(Group::new(Delimiter::Brace, nested_tokens))));
                                    }
                                }
                            }
                            macro_item.mac.tokens = new_tokens;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations and expands them by adding nested scopes including variable declarations, conditionals, and loops within macro definitions. This transformation increases complexity and nesting, potentially exposing issues in scope resolution and non-eager macro expansions within the compiler."
    }
}