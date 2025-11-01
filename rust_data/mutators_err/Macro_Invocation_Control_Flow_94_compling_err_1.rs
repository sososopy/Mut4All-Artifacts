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

pub struct Macro_Invocation_Control_Flow_94;

impl Mutator for Macro_Invocation_Control_Flow_94 {
    fn name(&self) -> &str {
        "Macro_Invocation_Control_Flow_94"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut rng = thread_rng();
                let new_stmt: Stmt = if rng.gen_bool(0.5) {
                    parse_quote! { return; }
                } else {
                    parse_quote! { some_function(); }
                };

                let mut visitor = MacroInvocationVisitor { new_stmt: new_stmt.clone(), rng: &mut rng };
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets macro invocations within function bodies. It identifies macros that take statements as parameters and replaces one of these parameters with a statement that modifies control flow or introduces a function call. This mutation can change the macro's behavior, potentially uncovering unexpected behaviors or compiler issues related to statement handling."
    }
}

struct MacroInvocationVisitor<'a> {
    new_stmt: Stmt,
    rng: &'a mut rand::rngs::ThreadRng,
}

impl<'a> VisitMut for MacroInvocationVisitor<'a> {
    fn visit_macro_mut(&mut self, mac: &mut Macro) {
        if let Some(mac_content) = mac.tokens.clone().into_iter().collect::<Vec<_>>().first_mut() {
            if let TokenTree::Group(group) = mac_content {
                let mut stmts: Vec<Stmt> = syn::parse2(group.stream()).unwrap_or_default();
                if !stmts.is_empty() {
                    let index = self.rng.gen_range(0..stmts.len());
                    stmts[index] = self.new_stmt.clone();
                    *mac_content = TokenTree::Group(Group::new(group.delimiter(), quote! { #(#stmts)* }));
                }
            }
        }
        syn::visit_mut::visit_macro_mut(self, mac);
    }
}