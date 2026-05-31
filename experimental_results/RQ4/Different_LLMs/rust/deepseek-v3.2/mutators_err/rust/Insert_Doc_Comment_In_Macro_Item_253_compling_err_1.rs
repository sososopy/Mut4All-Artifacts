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

pub struct Insert_Doc_Comment_In_Macro_Item_253;

impl Mutator for Insert_Doc_Comment_In_Macro_Item_253 {
    fn name(&self) -> &str {
        "Insert_Doc_Comment_In_Macro_Item_253"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut macro_invocations = Vec::new();
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                macro_invocations.push(item_macro);
            }
        }
        if macro_invocations.is_empty() {
            return;
        }
        let selected_macro = macro_invocations.choose(&mut rng).unwrap();
        let mut tokens = selected_macro.mac.tokens.clone();
        let token_stream: proc_macro2::TokenStream = tokens.clone();
        let mut items = Vec::new();
        let mut parser = syn::parse::Parser::new(token_stream);
        while !parser.is_empty() {
            if let Ok(item) = parser.parse::<Item>() {
                items.push(item);
            } else {
                parser.advance();
            }
        }
        if items.is_empty() {
            return;
        }
        let selected_item_index = rng.gen_range(0..items.len());
        let mut new_tokens = proc_macro2::TokenStream::new();
        let mut inserted = false;
        let mut parser = syn::parse::Parser::new(tokens.clone());
        let mut current_index = 0;
        while !parser.is_empty() {
            if current_index == selected_item_index && !inserted {
                new_tokens.extend(quote! { /// Doc comment });
                inserted = true;
            }
            if let Ok(item) = parser.parse::<Item>() {
                let item_tokens = quote! { #item };
                new_tokens.extend(item_tokens);
                current_index += 1;
            } else {
                let next = parser.next_token().unwrap();
                new_tokens.extend(proc_macro2::TokenStream::from(next));
            }
        }
        if inserted {
            selected_macro.mac.tokens = new_tokens;
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a doc comment (`/// Doc comment`) before an item inside a declarative macro invocation. It targets macro expansions that process items, aiming to trigger edge cases in the compiler's handling of doc comments in token streams passed between macro expansion phases. This mutation can affect procedural macro parsing and may expose bugs related to doc comment consumption during macro expansion."
    }
}