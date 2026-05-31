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

pub struct Replace_Cfg_Argument_With_Malformed_Tokens_237;

impl Mutator for Replace_Cfg_Argument_With_Malformed_Tokens_237 {
    fn name(&self) -> &str {
        "Replace_Cfg_Argument_With_Malformed_Tokens_237"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = CfgAttributeVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets #[cfg(...)] attributes within the seed program. It replaces the content inside the parentheses with a malformed token sequence: an assignment operator '=' followed by extra commas, then closes the parentheses. This creates a parsing edge case where the parser expects a cfg predicate but encounters syntactically invalid tokens, potentially triggering internal parser state inconsistencies and assertion failures, especially when combined with other attributes or expression contexts."
    }
}

struct CfgAttributeVisitor;

impl VisitMut for CfgAttributeVisitor {
    fn visit_attribute_mut(&mut self, attr: &mut syn::Attribute) {
        if attr.path().is_ident("cfg") {
            if let syn::Meta::List(ref mut meta_list) = attr.meta {
                let span = attr.span();
                let malformed_tokens = quote_spanned! {span=> = , , };
                let new_tokens = proc_macro2::TokenStream::from(malformed_tokens);
                let new_group = proc_macro2::Group::new(proc_macro2::Delimiter::Parenthesis, new_tokens);
                meta_list.tokens = proc_macro2::TokenStream::from_iter(vec![proc_macro2::TokenTree::Group(new_group)]);
            }
        }
        syn::visit_mut::visit_attribute_mut(self, attr);
    }
}