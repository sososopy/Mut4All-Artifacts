use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, MacroDelimiter, Pat, PatType,
    Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type,
    TypeImplTrait, TypeParamBound, TypePath, parse_quote,
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

pub struct Replace_Doc_Comment_With_Normal_Comment_253;

impl Mutator for Replace_Doc_Comment_With_Normal_Comment_253 {
    fn name(&self) -> &str {
        "Replace_Doc_Comment_With_Normal_Comment_253"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                let macro_content = match &item_macro.mac.delimiter {
                    MacroDelimiter::Paren(paren) => &item_macro.mac.tokens,
                    MacroDelimiter::Brace(brace) => &item_macro.mac.tokens,
                    MacroDelimiter::Bracket(bracket) => &item_macro.mac.tokens,
                };
                let mut new_content = TokenStream::new();
                let mut prev_was_doc_comment = false;
                for token in macro_content.clone() {
                    if let proc_macro2::TokenTree::Punct(punct) = &token {
                        if punct.as_char() == '#' {
                            prev_was_doc_comment = true;
                        } else {
                            prev_was_doc_comment = false;
                        }
                    }
                    if let proc_macro2::TokenTree::Group(group) = &token {
                        let mut new_group_content = TokenStream::new();
                        let mut prev_was_doc_comment_in_group = false;
                        for inner_token in group.stream() {
                            if let proc_macro2::TokenTree::Punct(inner_punct) = &inner_token {
                                if inner_punct.as_char() == '#' {
                                    prev_was_doc_comment_in_group = true;
                                } else {
                                    prev_was_doc_comment_in_group = false;
                                }
                            }
                            if let proc_macro2::TokenTree::Literal(inner_lit) = &inner_token {
                                let inner_lit_str = inner_lit.to_string();
                                if prev_was_doc_comment_in_group
                                    && inner_lit_str.starts_with("\"")
                                {
                                    let new_lit_str = inner_lit_str.replace("///", "//");
                                    let new_lit = proc_macro2::Literal::string(&new_lit_str);
                                    new_group_content.extend(quote! { #new_lit });
                                    continue;
                                }
                            }
                            new_group_content.extend(quote! { #inner_token });
                        }
                        let new_group =
                            proc_macro2::Group::new(group.delimiter(), new_group_content);
                        new_content.extend(quote! { #new_group });
                        continue;
                    }
                    if let proc_macro2::TokenTree::Literal(lit) = &token {
                        let lit_str = lit.to_string();
                        if prev_was_doc_comment && lit_str.starts_with("\"") {
                            let new_lit_str = lit_str.replace("///", "//");
                            let new_lit = proc_macro2::Literal::string(&new_lit_str);
                            new_content.extend(quote! { #new_lit });
                            continue;
                        }
                    }
                    new_content.extend(quote! { #token });
                }
                item_macro.mac.tokens = new_content;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function definitions within macro invocations by replacing doc comments with normal comments. By altering the comment style, it tests the compiler's ability to handle documentation comments during macro expansion and parsing, potentially revealing issues in how comments are processed in macro-generated code."
    }
}