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

pub struct Replace_Const_Generic_With_ConstBlock_111;

impl Mutator for Replace_Const_Generic_With_ConstBlock_111 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_ConstBlock_111"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Collect all const items without explicit type
        let const_vars: Vec<Ident> = file.items.iter()
            .filter_map(|item| {
                if let Item::Const(item_const) = item {
                    if item_const.ty.is_none() {
                        Some(item_const.ident.clone())
                    } else {
                        None
                    }
                } else {
                    None
                }
            })
            .collect();

        if const_vars.is_empty() {
            return; // No const vars to use
        }

        // Create a visitor to process ExprCall nodes
        struct CallVisitor<'a> {
            const_vars: Vec<Ident>,
        }

        impl<'a> VisitMut for CallVisitor<'a> {
            fn visit_expr_call_mut(&mut self, call: &mut ExprCall) {
                // Check if the func is an ExprPath with generic arguments
                if let Expr::Path(func_path) = &*call.func {
                    // Check if the path has AngleBracketed arguments
                    if let Some(last_segment) = func_path.path.segments.last_mut() {
                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Const(expr) = arg {
                                    // Replace with const block using a const variable
                                    let ident = &self.const_vars[0]; // Use the first const var
                                    let expr_path = Expr::Path(ExprPath {
                                        attrs: vec![],
                                        qself: None,
                                        path: syn::Path::from(ident.clone()),
                                    });
                                    let block = ExprBlock {
                                        attrs: vec![],
                                        block: parse_quote!({ #expr_path }),
                                    };
                                    *arg = GenericArgument::ConstBlock(block);
                                }
                            }
                        }
                    }
                }
                // Visit the func and args
                syn::visit_mut::visit_expr_call_mut(self, call);
            }
        }

        // Apply the visitor to the file
        let mut visitor = CallVisitor { const_vars };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}