use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    AttrStyle, Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument,
    GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType,
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

pub struct MalfAttrSyntax_237;

impl Mutator for MalfAttrSyntax_237 {
    fn name(&self) -> &str {
        "MalfAttrSyntax_237"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = MalformedAttributeVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator corrupts the parameter syntax of attributes with valid content by replacing them with invalid token sequences like `#[path( = , , )]`. This transformation preserves the attribute's placement and structure but introduces malformed patterns that challenge the parser's error handling and attribute parsing logic, potentially exposing ICEs in the compiler."
    }
}

struct MalformedAttributeVisitor;

impl<'ast> VisitMut for MalformedAttributeVisitor {
    fn visit_attribute_mut(&mut self, attr: &mut Attribute) {
        if let Ok(meta) = attr.parse_meta() {
            if let Meta::List(list) = meta {
                let path = list.path;
                let new_tokens = match attr.style {
                    AttrStyle::Outer => {
                        quote! {
                            #[#path( = , , )]
                        }
                    }
                    AttrStyle::Inner(_) => {
                        quote! {
                            #![#path( = , , )]
                        }
                    }
                    _ => return,
                };
                *attr = Attribute {
                    style: attr.style,
                    tokens: new_tokens,
                };
            }
        }
    }
}