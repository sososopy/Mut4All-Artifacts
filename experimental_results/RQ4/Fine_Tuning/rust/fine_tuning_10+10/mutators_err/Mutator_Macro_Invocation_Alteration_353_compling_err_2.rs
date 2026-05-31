use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemMacro, ItemStruct, Lifetime, LifetimeParam, Local, Macro, Pat, PatType, Path as SynPath,
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

pub struct Mutator_Macro_Invocation_Alteration_353;

impl Mutator for Mutator_Macro_Invocation_Alteration_353 {
    fn name(&self) -> &str {
        "Mutator_Macro_Invocation_Alteration_353"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(ref mut item_macro) = item {
                if item_macro.mac.path.is_ident("macro_rules") {
                    if let MacroDelimiter::Brace(ref mut tokens) = item_macro.mac.delimiter {
                        let mut new_tokens = TokenStream::new();
                        for token in tokens.clone().into_iter() {
                            new_tokens.extend(Some(token));
                            if let TokenTree::Group(group) = &token {
                                if group.delimiter() == Delimiter::Brace {
                                    let mut inner_stream = group.stream().into_iter().collect::<Vec<_>>();
                                    if inner_stream.len() > 2 {
                                        inner_stream.insert(2, TokenTree::Ident(Ident::new("cfg", Span::call_site())));
                                        inner_stream.insert(3, TokenTree::Group(Group::new(Delimiter::Parenthesis, TokenStream::new())));
                                        inner_stream.insert(4, TokenTree::Ident(Ident::new("unexpected", Span::call_site())));
                                    }
                                    new_tokens.extend(inner_stream);
                                }
                            }
                        }
                        *tokens = Group::new(Delimiter::Brace, new_tokens).stream();
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations within function bodies. It modifies the macro rules to include an unexpected cfg attribute, potentially conflicting with existing conditions. This explores parser ambiguities or misinterpretations, leveraging existing macro and function constructs."
    }
}