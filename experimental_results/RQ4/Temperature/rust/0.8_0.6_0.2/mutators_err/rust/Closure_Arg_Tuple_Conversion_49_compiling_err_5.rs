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

pub struct Closure_Arg_Tuple_Conversion_49;

impl Mutator for Closure_Arg_Tuple_Conversion_49 {
    fn name(&self) -> &str {
        "Closure_Arg_Tuple_Conversion_49"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if segment.ident == "FnMut" || segment.ident == "Fn" || segment.ident == "FnOnce" {
                                    if let PathArguments::Parenthesized(ref mut args) = segment.arguments {
                                        if args.inputs.len() > 1 {
                                            let inputs = std::mem::take(&mut args.inputs);
                                            let tuple_type = Type::Tuple(TypeTuple {
                                                paren_token: token::Paren { span: Span::call_site() },
                                                elems: inputs,
                                            });
                                            args.inputs.push(tuple_type);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Call(ref mut call)) = stmt {
                        if let Expr::Path(ref path) = *call.func {
                            if path.path.segments.len() == 1 {
                                let args = std::mem::take(&mut call.args);
                                if args.len() > 1 {
                                    let tuple_expr = Expr::Tuple(syn::ExprTuple {
                                        attrs: Vec::new(),
                                        paren_token: token::Paren { span: Span::call_site() },
                                        elems: args,
                                    });
                                    call.args.push(tuple_expr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function signatures that accept closures with multiple arguments. It modifies the closure's parameter type to accept a tuple and adjusts the invocation to pass arguments as a tuple. This transformation tests the Rust compiler's handling of tuple conversion in unboxed closures."
    }
}