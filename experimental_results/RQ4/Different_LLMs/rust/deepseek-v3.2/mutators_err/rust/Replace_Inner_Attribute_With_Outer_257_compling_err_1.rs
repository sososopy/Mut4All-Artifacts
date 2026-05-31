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

pub struct Replace_Inner_Attribute_With_Outer_257;

impl Mutator for Replace_Inner_Attribute_With_Outer_257 {
    fn name(&self) -> &str {
        "Replace_Inner_Attribute_With_Outer_257"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = InnerToOuterVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator traverses the entire AST and modifies any inner attribute (`#![...]`) by removing the exclamation mark, turning it into an outer attribute (`#[...]`). This transformation targets the compiler's attribute parsing logic during macro expansion, particularly when inner attributes are placed in contexts where only outer attributes are expected, which can trigger internal compiler errors. The mutation preserves the attribute's path and arguments, only changing its style."
    }
}

struct InnerToOuterVisitor;

impl VisitMut for InnerToOuterVisitor {
    fn visit_attribute_mut(&mut self, attr: &mut syn::Attribute) {
        if attr.style == syn::AttrStyle::Inner {
            attr.style = syn::AttrStyle::Outer;
        }
        syn::visit_mut::visit_attribute_mut(self, attr);
    }
}