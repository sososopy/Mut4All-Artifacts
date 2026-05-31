use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprMacro, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemMacro, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct MacroPathToBlockExpansion_205;

impl Mutator for MacroPathToBlockExpansion_205 {
    fn name(&self) -> &str {
        "MacroPathToBlockExpansion_205"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut identifiers = Vec::new();
        for item in &file.items {
            if let Item::Struct(s) = item {
                identifiers.push(s.ident.clone());
            }
            if let Item::Enum(e) = item {
                identifiers.push(e.ident.clone());
            }
            if let Item::Fn(f) = item {
                identifiers.push(f.sig.ident.clone());
                for input in &f.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Pat::Ident(pat) = &*pat_type.pat {
                            identifiers.push(pat.ident.clone());
                        }
                    }
                }
                for stmt in &f.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(pat) = local.pat.as_ref() {
                            if let Pat::Ident(pat) = pat.as_ref() {
                                identifiers.push(pat.ident.clone());
                            }
                        }
                    }
                }
            }
        }

        struct MacroVisitor {
            identifiers: Vec<Ident>,
        }

        impl<'ast> VisitMut for MacroVisitor {
            fn visit_expr_macro_mut(&mut self, expr: &mut ExprMacro) {
                let block = self.generate_block();
                expr.mac.tokens = block.into();
            }

            fn visit_item_macro_mut(&mut self, item: &mut ItemMacro) {
                let block = self.generate_block();
                item.mac.tokens = block.into();
            }
        }

        impl<'ast> MacroVisitor {
            fn generate_block(&self) -> proc_macro2::TokenStream {
                let mut rng = thread_rng();
                let struct_name = self.identifiers.choose(&mut rng).cloned().unwrap_or_else(|| Ident::new("StructX", Span::call_site()));
                let value_name = self.identifiers.choose(&mut rng).cloned().unwrap_or_else(|| Ident::new("ValueY", Span::call_site()));
                let function_name = self.identifiers.choose(&mut rng).cloned().unwrap_or_else(|| Ident::new("functionZ", Span::call_site()));
                quote! {
                    {
                        let a = #struct_name { field: #value_name };
                        let b = #function_name(a);
                        b
                    }
                }
            }
        }

        let mut visitor = MacroVisitor { identifiers };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations expecting a path argument and replaces them with a block expression containing code that references existing types and variables from the seed program. This disrupts the resolver's expectation of a path, potentially exposing scope resolution errors or ICEs in macro expansion contexts."
    }
}