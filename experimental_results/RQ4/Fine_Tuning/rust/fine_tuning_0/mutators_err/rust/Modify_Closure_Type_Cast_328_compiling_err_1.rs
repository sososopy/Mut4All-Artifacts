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

pub struct Modify_Closure_Type_Cast_328;

impl Mutator for Modify_Closure_Type_Cast_328 {
    fn name(&self) -> &str {
        "Modify_Closure_Type_Cast_328"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(Expr::Closure(closure)) = stmt {
                        for expr in &mut closure.body {
                            if let Expr::Cast(expr_cast) = expr {
                                if let Type::Reference(type_ref) = &*expr_cast.ty {
                                    if let Type::Path(type_path) = &*type_ref.elem {
                                        if let Some(last_segment) = type_path.path.segments.last() {
                                            if last_segment.ident == "str" {
                                                expr_cast.ty = Box::new(Type::Reference(TypeReference {
                                                    and_token: token::And::default(),
                                                    lifetime: None,
                                                    mutability: None,
                                                    elem: Box::new(Type::Path(TypePath {
                                                        qself: None,
                                                        path: syn::Path::from(Ident::new("SomeOtherType", Span::call_site())),
                                                    })),
                                                }));
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
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closures within functions that involve type casting. It modifies type conversions to cast to an incompatible or non-const trait type, such as changing a cast to `&str` into a cast to `&SomeOtherType`. This mutation aims to explore compiler behavior around trait bounds and type conversions, potentially triggering bugs related to trait resolution and type checking."
    }
}