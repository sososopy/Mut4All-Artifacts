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

pub struct Replace_Array_Length_With_Max_Shift_77;

impl Mutator for Replace_Array_Length_With_Max_Shift_77 {
    fn name(&self) -> &str {
        "Replace_Array_Length_With_Max_Shift_77"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ArrayLengthVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array length expressions in constant contexts (const items, static items, const generic arguments, array type annotations, and array repeat expressions). It replaces the length expression with a large constant arithmetic expression `usize::MAX >> K`, where K is a small integer (0, 1, 2, 4, 8, or 16). This transformation aims to stress the compiler's const evaluation system by introducing extreme array sizes that may cause internal compiler errors due to memory allocation failures, overflow issues, or improper handling of unevaluated constants."
    }
}

struct ArrayLengthVisitor;

impl ArrayLengthVisitor {
    fn new() -> Self {
        Self
    }
    fn generate_replacement(&self) -> Expr {
        let mut rng = thread_rng();
        let shift = *[0, 1, 2, 4, 8, 16].choose(&mut rng).unwrap();
        parse_quote! { usize::MAX >> #shift }
    }
}

impl VisitMut for ArrayLengthVisitor {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Array(arr) = node {
            let replacement = self.generate_replacement();
            arr.len = replacement;
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Repeat(repeat) = node {
            let replacement = self.generate_replacement();
            *repeat.len = replacement;
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
    fn visit_item_mut(&mut self, node: &mut Item) {
        if let Item::Const(const_item) = node {
            let replacement = self.generate_replacement();
            *const_item.expr = replacement;
        }
        if let Item::Static(static_item) = node {
            let replacement = self.generate_replacement();
            *static_item.expr = replacement;
        }
        syn::visit_mut::visit_item_mut(self, node);
    }
    fn visit_generic_argument_mut(&mut self, node: &mut GenericArgument) {
        if let GenericArgument::Const(const_arg) = node {
            let replacement = self.generate_replacement();
            *const_arg = replacement;
        }
        syn::visit_mut::visit_generic_argument_mut(self, node);
    }
}