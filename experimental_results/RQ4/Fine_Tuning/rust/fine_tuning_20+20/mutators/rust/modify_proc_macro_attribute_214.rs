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

pub struct Modify_Proc_Macro_Attribute_214;

impl Mutator for Modify_Proc_Macro_Attribute_214 {
    fn name(&self) -> &str {
        "Modify_Proc_Macro_Attribute_214"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_identity_function = false;
        let mut existing_function_names = HashSet::new();
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                existing_function_names.insert(item_fn.sig.ident.to_string());
                if item_fn.sig.ident == "identity_function" {
                    has_identity_function = true;
                }
            }
        }
        let mut has_modified = false;
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn
                    .attrs
                    .iter()
                    .any(|attr| attr.path().is_ident("proc_macro_attribute"))
                {
                    if has_modified {
                        continue;
                    }
                    let mut replace_expr: Option<Expr> = None;
                    for stmt in &item_fn.block.stmts {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Call(expr_call) = expr {
                                if let Expr::Path(expr_path) = &*expr_call.func {
                                    if expr_path.path.is_ident("panic") {
                                        replace_expr = Some(Expr::Call(ExprCall {
                                            attrs: Vec::new(),
                                            func: Box::new(Expr::Path(ExprPath {
                                                attrs: Vec::new(),
                                                qself: None,
                                                path: SynPath::from(Ident::new(
                                                    "identity_function",
                                                    Span::call_site(),
                                                )),
                                            })),
                                            paren_token: expr_call.paren_token,
                                            args: expr_call.args.clone(),
                                        }));
                                    }
                                }
                            }
                        }
                    }
                    if let Some(replace_expr) = replace_expr {
                        let mut new_block = item_fn.block.clone();
                        new_block.stmts.clear();
                        new_block.stmts.push(Stmt::Expr(replace_expr, None));
                        item_fn.block = new_block;
                        has_modified = true;
                    }
                }
            }
        }
        if !has_identity_function {
            let identity_function: Item = parse_quote! {
                fn identity_function(input: TokenStream) -> TokenStream {
                    input
                }
            };
            file.items.push(identity_function);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions annotated with `#[proc_macro_attribute]` by replacing the `panic!()` call with a call to an existing or newly introduced `identity_function`. If no suitable function exists, it adds a simple identity function that takes and returns a `TokenStream`. This transformation alters the macro's behavior from an immediate panic to a no-op transformation, potentially exposing issues in macro expansion or metadata handling."
    }
}