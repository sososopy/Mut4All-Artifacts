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

pub struct Insert_Thread_Local_In_Const_Generic_152;

impl Mutator for Insert_Thread_Local_In_Const_Generic_152 {
    fn name(&self) -> &str {
        "Insert_Thread_Local_In_Const_Generic_152"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut const_params = Vec::new();
                for param in &item_impl.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_params.push(const_param.ident.clone());
                    }
                }
                if const_params.is_empty() {
                    continue;
                }
                let impl_ty = match &item_impl.self_ty {
                    Type::Path(type_path) => type_path.path.segments.last().map(|seg| seg.ident.clone()),
                    _ => None,
                };
                if impl_ty.is_none() {
                    continue;
                }
                let impl_ty_ident = impl_ty.unwrap();
                let mut rng = thread_rng();
                let target_param = const_params.choose(&mut rng).unwrap();
                for arg in &mut item_impl.generics.args {
                    if let GenericArgument::Const(const_arg) = arg {
                        if let Expr::Block(expr_block) = &mut **const_arg {
                            let static_name = format!("{}_TL", impl_ty_ident);
                            let static_ident = Ident::new(&static_name, Span::call_site());
                            let thread_local_stmt: Stmt = parse_quote! {
                                thread_local! { static #static_ident: #impl_ty_ident = #target_param; }
                            };
                            expr_block.block.stmts.insert(0, thread_local_stmt);
                        } else {
                            let static_name = format!("{}_TL", impl_ty_ident);
                            let static_ident = Ident::new(&static_name, Span::call_site());
                            let original_expr = const_arg.clone();
                            let new_block: Expr = parse_quote! {
                                {
                                    thread_local! { static #static_ident: #impl_ty_ident = #target_param; }
                                    #original_expr
                                }
                            };
                            *const_arg = Box::new(new_block);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets impl blocks with const generic parameters. It modifies const generic arguments by inserting a thread_local macro invocation inside their initialization block. If the argument is already a block, the macro is added as a new statement at the beginning. If not, the argument is wrapped in a block that starts with the thread_local macro. The static variable name is derived from the impl type identifier with '_TL' appended, and its type is the impl type. This creates nested macro expansions inside const generic arguments, potentially triggering HIR ownership mismatch ICEs similar to the bug report scenario."
    }
}