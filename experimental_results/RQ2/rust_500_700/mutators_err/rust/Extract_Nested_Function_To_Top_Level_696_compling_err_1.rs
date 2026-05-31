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

pub struct Extract_Nested_Function_To_Top_Level_696;

impl Mutator for Extract_Nested_Function_To_Top_Level_696 {
    fn name(&self) -> &str {
        "Extract_Nested_Function_To_Top_Level_696"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_functions = Vec::new();

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some((_, fields)) = &mut item_struct.fields {
                    for field in fields.iter_mut() {
                        if let syn::Type::Array(type_array) = &mut field.ty {
                            if let syn::Expr::Block(expr_block) = &mut *type_array.len {
                                for stmt in &mut expr_block.block.stmts {
                                    if let Stmt::Item(Item::Fn(nested_fn)) = stmt {
                                        if let ReturnType::Type(_, return_type) = &nested_fn.sig.output {
                                            if let Type::ImplTrait(_) = &**return_type {
                                                let fn_name = Ident::new(
                                                    &format!("{}_extracted", nested_fn.sig.ident),
                                                    Span::call_site(),
                                                );
                                                let mut new_fn = nested_fn.clone();
                                                new_fn.sig.ident = fn_name.clone();
                                                new_functions.push(Item::Fn(new_fn));

                                                *stmt = Stmt::Expr(Expr::Call(ExprCall {
                                                    attrs: Vec::new(),
                                                    func: Box::new(Expr::Path(ExprPath {
                                                        attrs: Vec::new(),
                                                        qself: None,
                                                        path: SynPath::from(fn_name),
                                                    })),
                                                    paren_token: Default::default(),
                                                    args: Punctuated::new(),
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

        file.items.extend(new_functions);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}