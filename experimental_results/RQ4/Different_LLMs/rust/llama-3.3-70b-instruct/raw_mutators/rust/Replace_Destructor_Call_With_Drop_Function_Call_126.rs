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

pub struct Replace_Destructor_Call_With_Drop_Function_Call_126;

impl Mutator for Replace_Destructor_Call_With_Drop_Function_Call_126 {
    fn name(&self) -> &str {
        "Replace_Destructor_Call_With_Drop_Function_Call_126"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(ExprCall { expr, args, .. }) = expr {
                            if let Expr::Path(path) = **expr {
                                if path.path.segments.last().unwrap().ident == "drop" {
                                    let drop_expr = parse_quote! {
                                        std::mem::drop(#expr);
                                    };
                                    *stmt = Stmt::Expr(drop_expr, None);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets the usage of destructors in Rust code, specifically focusing on the interaction between smart pointers and the borrow checker. It applies to code constructs involving RefCell, Rc, or other smart pointers where the borrow checker's behavior is crucial. This operator works by identifying points where a value is being dropped or goes out of scope and replacing the implicit drop with an explicit call to the Drop trait's drop function."
    }
}