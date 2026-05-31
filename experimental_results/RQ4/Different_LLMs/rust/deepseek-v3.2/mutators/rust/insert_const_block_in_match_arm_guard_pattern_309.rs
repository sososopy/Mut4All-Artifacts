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

pub struct Insert_Const_Block_In_Match_Arm_Guard_Pattern_309;

impl Mutator for Insert_Const_Block_In_Match_Arm_Guard_Pattern_309 {
    fn name(&self) -> &str {
        "Insert_Const_Block_In_Match_Arm_Guard_Pattern_309"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MatchArmVisitor {
            modified: bool,
        }
        impl VisitMut for MatchArmVisitor {
            fn visit_expr_match_mut(&mut self, node: &mut syn::ExprMatch) {
                for arm in &mut node.arms {
                    if let Pat::Tuple(tuple) = &arm.pat {
                        if tuple.elems.is_empty() && arm.guard.is_none() {
                            let const_block: Expr = parse_quote! { const { (|| {})() } };
                            arm.pat = Pat::Verbatim(quote! { #const_block });
                            self.modified = true;
                        }
                    }
                }
                syn::visit_mut::visit_expr_match_mut(self, node);
            }
        }
        struct AsyncTraitImplVisitor<'a> {
            mutator: &'a Insert_Const_Block_In_Match_Arm_Guard_Pattern_309,
        }
        impl<'a> VisitMut for AsyncTraitImplVisitor<'a> {
            fn visit_item_impl_mut(&mut self, node: &mut syn::ItemImpl) {
                if let Some((_, path, _)) = &node.trait_ {
                    let trait_name = path.segments.last().map(|seg| seg.ident.to_string());
                    if trait_name.is_some() {
                        for impl_item in &mut node.items {
                            if let syn::ImplItem::Fn(method) = impl_item {
                                if method.sig.asyncness.is_some() {
                                    let mut visitor = MatchArmVisitor { modified: false };
                                    visitor.visit_block_mut(&mut method.block);
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_item_impl_mut(self, node);
            }
        }
        let mut visitor = AsyncTraitImplVisitor { mutator: self };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match expressions within async trait implementations. It applies when a match expression exists inside an async method of a trait implementation, and the match uses a unit `()` pattern. The operator modifies the match arm by inserting a const block that contains an immediately-invoked closure as the pattern guard expression. Specifically, it changes a match arm of the form `() => { ... }` to `const { (|| {})() } => { ... }`. The operator uses only existing elements: the unit pattern `()`, an empty closure `|| {}`, and the const block syntax. It does not introduce new variables or types. The mutation is performed by locating match arms with a unit pattern and no guard, then replacing the pattern with `const { (|| {})() }`. For example, if the seed code contains `match expr { () => {} }`, it becomes `match expr { const { (|| {})() } => {} }`. This mutation is designed to expose bugs related to the interaction of const evaluation, async functions, and trait implementations, particularly under incomplete features."
    }
}