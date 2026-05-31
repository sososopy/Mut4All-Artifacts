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

pub struct Replace_Concrete_Type_With_Generic_1;

impl Mutator for Replace_Concrete_Type_With_Generic_1 {
    fn name(&self) -> &str {
        "Replace_Concrete_Type_With_Generic_1"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call) = &*expr {
                            if let Expr::Path(path_expr) = &*call.func {
                                if let Some(last_segment) = path_expr.path.segments.last() {
                                    if last_segment.ident == "new_in" {
                                        let name = Ident::new(&format!("generic_{}", rng.gen_range(0..1000)), Span::call_site());
                                        let generics = parse_quote!(<T, A>);
                                        let data_param = parse_quote!(data: &[T]);
                                        let allocator_param = parse_quote!(allocator: &A);
                                        let mut args = Punctuated::new();
                                        args.push(parse_quote!(data));
                                        args.push(parse_quote!(allocator));
                                        let body_call = ExprCall {
                                            attrs: vec![],
                                            func: call.func.clone(),
                                            paren_token: call.paren_token,
                                            args,
                                        };
                                        let body = parse_quote!({ #body_call });
                                        let new_fn = parse_quote! {
                                            fn #name #generics(#data_param, #allocator_param) #body
                                        };
                                        file.items.push(Item::Fn(new_fn));
                                        let new_call = parse_quote! {
                                            #name::<T, A>(data, allocator)
                                        };
                                        *expr = new_call;
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