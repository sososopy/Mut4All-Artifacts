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

pub struct Alter_Ref_To_Dyn_666;

impl Mutator for Alter_Ref_To_Dyn_666 {
    fn name(&self) -> &str {
        "Alter_Ref_To_Dyn_666"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &mut local.init {
                            if let Expr::Cast(expr_cast) = &mut **expr {
                                if let Type::TraitObject(type_trait_object) = &*expr_cast.ty {
                                    if let Expr::Reference(expr_ref) = &mut *expr_cast.expr {
                                        if expr_ref.mutability.is_some() {
                                            expr_ref.mutability = None;
                                        } else {
                                            expr_ref.mutability = Some(token::Mut {
                                                span: Span::call_site(),
                                            });
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}